#ifndef QML_QMQTT_CLIENT_H
#define QML_QMQTT_CLIENT_H

#include <QList>
#include <QObject>
#include <QScopedPointer>
#include <QString>
#include <QUrl>
#include <qmqtt.h>

class QmlQmqttSubscription;

class QmlQmqttClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QUrl url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
public:
    explicit QmlQmqttClient(QObject *parent = nullptr);

    ~QmlQmqttClient() override;

    QUrl url() const
    {
        return m_url;
    }

    void setUrl(const QUrl &url);

    bool connected() const
    {
        return m_client != nullptr && m_client->isConnectedToHost();
    }

    Q_INVOKABLE QmlQmqttSubscription *subscribe(const QString &topicFilter);

    Q_INVOKABLE void unsubscribe(const QString &topicFilter);

    Q_INVOKABLE void unsubscribe(QmlQmqttSubscription *subscription);

    Q_INVOKABLE void publish(const QString& topic, const QString& message);

    Q_INVOKABLE void connectToHost();

    Q_INVOKABLE void disconnectFromHost();

signals:
    void urlChanged();

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
};

#endif // QML_QMQTT_CLIENT_H
