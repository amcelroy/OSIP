#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "qmldaqconfigbackend.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<QMLDAQConfigBackend>("edu.utexas.bme.qmlconfigbackend", 1, 0, "QMLDAQConfigBackend");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/QML_DAQConfig.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
