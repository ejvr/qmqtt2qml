#ifndef QML_QMQTT_SUBSCRIPTION_H
#define QML_QMQTT_SUBSCRIPTION_H

#include <QObject>
#include <QRegularExpression>

class QmlQmqttClient;

class QmlQmqttSubscription : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString topicFilter READ topicFilter)
public:
    QmlQmqttSubscription(const QString &topicFilter, QmlQmqttClient *parent);

    QString topicFilter() const
    {
        return m_topicFilter;
    }

    bool isMatch(const QString& topic) const;

signals:
     void messageReceived(const QString &topic, const QString &msg);

private:
     static QRegularExpression createMatcher(const QString &topic);

     QRegularExpression m_topicMatcher;
     QString m_topicFilter;
};

#endif // QML_QMQTT_SUBSCRIPTION_H
