#include "qml_qmqtt_subscription.h"
#include "qml_qmqtt_client.h"

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
    re.replace("+", "[a-zA-Z0-9_-]+").replace("#", "[a-zA-Z0-9_\\-/]+").replace("$", "\\$");
    return QRegularExpression(re);
}
