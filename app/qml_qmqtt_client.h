#ifndef QML_QMQTT_CLIENT_H
#define QML_QMQTT_CLIENT_H

#include <QObject>
#include <QString>
#include <QVector>
#include <qmqtt.h>

class QmlQmqttSubscription;

class QmlQmqttClient : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString hostname READ hostname WRITE setHostname NOTIFY hostnameChanged)
    Q_PROPERTY(quint16 port READ port WRITE setPort NOTIFY portChanged)
    Q_PROPERTY(bool connected READ connected NOTIFY connectedChanged)
public:
    explicit QmlQmqttClient(QObject *parent = nullptr);

    ~QmlQmqttClient();

    QString hostname() const
    {
        return m_hostname;
    }

    void setHostname(const QString &h);

    quint16 port() const
    {
        return m_port;
    }

    void setPort(quint16 port);

    bool connected() const
    {
        return m_client != nullptr && m_client->isConnectedToHost();
    }

    Q_INVOKABLE QmlQmqttSubscription *subscribe(const QString &topic);

    Q_INVOKABLE void connectToHost();

    Q_INVOKABLE void disconnectFromHost();

signals:
    void hostnameChanged();

    void portChanged();

    void connectedChanged();

private slots:
    void onConnected();

    void onDisconnected();

    void onDestroyed();

    void onMessageReceived(const QMQTT::Message &message);

private:
    void updateClient();

    QVector<QmlQmqttSubscription *> m_subscriptions;
    QString m_hostname;
    QMQTT::Client *m_client;
    quint16 m_port;
};

class QmlQmqttSubscription : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString topic READ topic)
public:
    QmlQmqttSubscription(const QString &topic, QmlQmqttClient *parent):
        QObject(parent),
        m_topic(topic)
    {}

    QString topic() const
    {
        return m_topic;
    }

signals:
     void messageReceived(const QString &msg);

private:
     QString m_topic;
};

#endif // QML_QMQTT_CLIENT_H
