#include <QDebug>
#include <QSslConfiguration>
#include "qml_qmqtt_client.hpp"
#include "qml_qmqtt_subscription.hpp"

QmlQmqttClient::QmlQmqttClient(QObject *parent) :
    QObject(parent)
{
}

QmlQmqttClient::~QmlQmqttClient()
{
    for (QmlQmqttSubscription *subscriber: m_subscriptions)
        delete subscriber;
}

void QmlQmqttClient::setUrl(const QUrl &url)
{
    if (m_url == url)
        return;
    m_url = url;
    m_client.reset();
    emit urlChanged();
}

void QmlQmqttClient::setCleanSession(bool c)
{
    if (m_cleanSession == c)
        return;
    m_cleanSession = c;
    emit cleanSessionChanged();
}

void QmlQmqttClient::setReconnectInterval(int v)
{
    if (m_reconnectInterval == v)
        return;
    m_reconnectInterval = v;
    emit reconnectIntervalChanged();
}

QmlQmqttSubscription *QmlQmqttClient::subscribe(const QString &topicFilter)
{
    for (QmlQmqttSubscription *subscriber: m_subscriptions)
    {
        if (subscriber->topicFilter() == topicFilter)
            return subscriber;
    }
    QmlQmqttSubscription *subscriber = new QmlQmqttSubscription(topicFilter, this);
    connect(subscriber, &QmlQmqttSubscription::destroyed, this, &QmlQmqttClient::onDestroyed);
    m_subscriptions.append(subscriber);
    if (m_client != nullptr)
    {
        qInfo() << "Subscribing to" << topicFilter;
        m_client->subscribe(topicFilter);
    }
    return subscriber;
}

void QmlQmqttClient::unsubscribe(const QString &topicFilter)
{
    for (QmlQmqttSubscription *subscription: m_subscriptions)
    {
        if (subscription->topicFilter() == topicFilter)
        {
            // The onDestroyed function will unsubscribe the topic
            delete subscription;
            return;
        }
    }
    qWarning() << "No QmlQmqttSubscription object found with filter" << topicFilter;
}

void QmlQmqttClient::unsubscribe(QmlQmqttSubscription *subscription)
{
    if (!m_subscriptions.contains(subscription))
    {
        qWarning() << "Cannot unsubscribe, because QmlQmqttSubscription object is not stored here";
        return;
    }
    // The onDestroyed function will unsubscribe the topic
    delete subscription;
    Q_ASSERT(!m_subscriptions.contains(subscription));
}

void QmlQmqttClient::publish(const QString &topic, const QString &message)
{
    if (m_client == nullptr || !m_client->isConnectedToHost())
    {
        qWarning() << "Cannot publish because client is not connected";
    }
    m_client->publish(QMQTT::Message(0, topic, message.toUtf8()));
}

void QmlQmqttClient::connectToHost()
{
    if (m_client == nullptr)
    {
        qInfo() << "Creating MQTT connection with" << m_url;
        QString hostname = m_url.host();
        if (hostname.isEmpty())
        {
            qFatal("No hostname specified in url");
            return;
        }
        QString scheme = m_url.scheme();
        if (scheme.isEmpty() || scheme == "mqtt")
        {
            quint16 port = static_cast<quint16>(m_url.port(1883));
            m_client.reset(new QMQTT::Client(hostname, port, false, false));
        }
        else if (scheme == "mqtts")
        {
            quint16 port = static_cast<quint16>(m_url.port(8883));
            auto sslConfig = QSslConfiguration::defaultConfiguration();
            m_client.reset(new QMQTT::Client(hostname, port, sslConfig));
        }
        else if (scheme == "ws")
        {
#ifdef QT_WEBSOCKETS_LIB
            m_client.reset(new QMQTT::Client(
                m_url.toString(),
                "",
                QWebSocketProtocol::VersionLatest));
#else
            qFatal("Websocket support is disabled");
            return;
#endif // QT_WEBSOCKETS_LIB
        }
        else if (scheme == "wss")
        {
#ifdef QT_WEBSOCKETS_LIB
            auto sslConfig = QSslConfiguration::defaultConfiguration();
            m_client.reset(new QMQTT::Client(
                m_url.toString(),
                "",
                QWebSocketProtocol::VersionLatest,
                sslConfig));
#else
            qFatal("Websocket over ssl support is disabled");
            return;
#endif // QT_WEBSOCKETS_LIB
        }
        else
        {
            qFatal("Unknown scheme specified in url");
            return;
        }
        m_client->setUsername(m_url.userName());
        m_client->setPassword(m_url.password().toUtf8());
        m_client->setCleanSession(m_cleanSession);
        m_client->setAutoReconnect(m_reconnectInterval > 0);
        m_client->setAutoReconnectInterval(m_reconnectInterval);
        connect(m_client.data(), &QMQTT::Client::connected, this, &QmlQmqttClient::onConnected);
        connect(m_client.data(), &QMQTT::Client::disconnected, this, &QmlQmqttClient::onDisconnected);
        connect(m_client.data(), &QMQTT::Client::received, this, &QmlQmqttClient::onMessageReceived);
    }
    m_client->connectToHost();
}

void QmlQmqttClient::disconnectFromHost()
{
    if (m_client == nullptr)
        return;
    m_client->disconnectFromHost();
    emit connectedChanged();
}

void QmlQmqttClient::onConnected()
{
    for (QmlQmqttSubscription *subscriber: m_subscriptions)
        m_client->subscribe(subscriber->topicFilter());
    emit connectedChanged();
}

void QmlQmqttClient::onDisconnected()
{
    emit connectedChanged();
}

void QmlQmqttClient::onDestroyed()
{
    QmlQmqttSubscription *subscriber = static_cast<QmlQmqttSubscription *>(sender());
    if (m_client != nullptr)
        m_client->unsubscribe(subscriber->topicFilter());
    m_subscriptions.removeOne(subscriber);
}

void QmlQmqttClient::onMessageReceived(const QMQTT::Message &message)
{
    QList<QmlQmqttSubscription *> matches;
    // This may seem a little paranoia: first we collect all subscriptions which are interested
    // in the message (there may be multiple), and next we emit the signals. We do this, because
    // a slot connected to the signal may cause m_subscriptions to change, which would invalidate
    // its iterators and may crash the loop.
    for (QmlQmqttSubscription *subscriber: m_subscriptions)
    {
        if (subscriber->isMatch(message.topic()))
            matches.append(subscriber);
    }
    if (matches.isEmpty())
    {
        qWarning() << "Message receiver, but no subscriber found. Topic was:" << message.topic();
        return;
    }
    for (QmlQmqttSubscription *subscriber: matches)
    {
        // Check if the subscription is still present in the main list, because it may be removed
        // in a slot connected to QmlQmqttSubscription::messageReceived.
        if (m_subscriptions.contains(subscriber))
            emit subscriber->messageReceived(message.topic(), QString::fromUtf8(message.payload()));
    }
}
