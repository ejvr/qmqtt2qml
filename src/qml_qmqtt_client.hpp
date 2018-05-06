#ifndef QML_QMQTT_CLIENT_H
#define QML_QMQTT_CLIENT_H

#include <QList>
#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QUrl>
#include <qmqtt.h>
#include "qmqtt2qml_defines.hpp"

class QmlQmqttSubscription;

class QMQTT2QML_EXPORT QmlQmqttClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(bool cleanSession READ cleanSession WRITE setCleanSession NOTIFY cleanSessionChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
    Q_PROPERTY(int reconnectInterval READ reconnectInterval WRITE setReconnectInterval
        NOTIFY reconnectIntervalChanged)
public:
    explicit QmlQmqttClient(QObject *parent = nullptr);

    ~QmlQmqttClient() override;

    QUrl url() const
    {
        return m_url;
    }

    void setUrl(const QUrl &url);

    bool cleanSession() const
    {
        return m_cleanSession;
    }

    void setCleanSession(bool c);

    int reconnectInterval() const
    {
        return m_reconnectInterval;
    }

    void setReconnectInterval(int v);

    bool connected() const
    {
        return m_client != nullptr && m_client->isConnectedToHost();
    }

    Q_INVOKABLE QmlQmqttSubscription *subscribe(const QString &topicFilter);

    Q_INVOKABLE void unsubscribe(const QString &topicFilter);

    Q_INVOKABLE void unsubscribe(QmlQmqttSubscription *subscription);

    Q_INVOKABLE void publish(const QString& topic, const QString& message, bool retain = false);

    Q_INVOKABLE void connectToHost();

    Q_INVOKABLE void disconnectFromHost();

signals:
    void urlChanged();

    void cleanSessionChanged();

    void reconnectIntervalChanged();

    void connectedChanged();

private slots:
    void onConnected();

    void onDisconnected();

    void onDestroyed();

    void onMessageReceived(const QMQTT::Message &message);

private:
    QList<QmlQmqttSubscription *> m_subscriptions;
    QScopedPointer<QMQTT::Client> m_client;
    QUrl m_url;
    bool m_cleanSession = true;
    int m_reconnectInterval = 5000;
};

#endif // QML_QMQTT_CLIENT_H
