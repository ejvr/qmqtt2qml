#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QLoggingCategory>
#include "qml_qmqtt_client.hpp"
#include "qml_qmqtt_subscription.hpp"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    qmlRegisterType<QmlQmqttClient>("MqttClient", 1, 0, "MqttClient");
    qmlRegisterUncreatableType<QmlQmqttSubscription>("MqttClient", 1, 0, "MqttSubscription", QLatin1String("Subscriptions are read-only"));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
