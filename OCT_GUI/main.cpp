#include <QApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>
#include "menubackend.h"
#include "mainbackend.h"
#include "octconfigfile.h"
#include "DAQPipelineStages/loadoctpipeline.h"
#include "bscanimageprovider.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    mainbackend _mainBackend;

    qmlRegisterType<MenuBackend>("edu.utexas.bme.menubackend", 1, 0, "MenuBackend");

    //Register the Image provider BEFORE loading the qml!!!
    BScanImageProvider* _bscanImageProvider = new BScanImageProvider();
    engine.addImageProvider(QLatin1String("bscanProvider"), _bscanImageProvider);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QObject *rootObject = engine.rootObjects().first();

    _mainBackend.getOCTPipeline()->getDisplay()->setImageProvider(_bscanImageProvider);
    _mainBackend.loadOCT("/Users/amcelroy/Code/OSIP/test_data/");

    return app.exec();
}
