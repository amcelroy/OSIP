#include <QApplication>
#include <QQmlApplicationEngine>

#include "menubackend.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qmlRegisterType<MenuBackend>("edu.utexas.bme.menubackend", 1, 0, "MenuBackend");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;



    return app.exec();
}
