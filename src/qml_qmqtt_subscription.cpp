#include "qml_qmqtt_subscription.hpp"
#include "qml_qmqtt_client.hpp"

QmlQmqttSubscription::QmlQmqttSubscription(const QString &topicFilter, QmlQmqttClient *parent):
    QObject(parent),
    m_topicMatcher(createMatcher(topicFilter)),
    m_topicFilter(topicFilter)
{
}

bool QmlQmqttSubscription::isMatch(const QString &topic) const
{
    return m_topicMatcher.match(topic).hasMatch();
}

QRegularExpression QmlQmqttSubscription::createMatcher(const QString &topic)
{
    QString re = topic;
    re.replace("+", "[^/]+").replace("#", ".+").replace("$", "\\$");
    return QRegularExpression(re);
}
