#include <QApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>
#include "menubackend.h"
#include "mainbackend.h"
#include "octconfigfile.h"
#include "DAQPipelineStages/loadoctpipeline.h"
#include "bscanimageprovider.h"
#include "qmldaqconfigbackend.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;
    mainbackend _mainBackend;

    qmlRegisterType<MenuBackend>("edu.utexas.bme.menubackend", 1, 0, "MenuBackend");
    qmlRegisterType<QMLDAQConfigBackend>("edu.utexas.bme.qmlconfigbackend", 1, 0, "QMLDAQConfigBackend");

    //Register the Image provider BEFORE loading the qml!!!
    BScanImageProvider* _bscanImageProvider = new BScanImageProvider();
    engine.addImageProvider(QLatin1String("bscanProvider"), _bscanImageProvider);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QObject* rootObject = engine.rootObjects().first();
    QObject* qml_minMaxSlider = rootObject->findChild<QObject*>("qml_minMaxSlider");
    QObject* qml_bscanSlider = rootObject->findChild<QObject*>("qml_bscanSlider");
    QObject* qml_BScanImage = rootObject->findChild<QObject*>("qml_BScanImage");

    //Connect QML Signals to C++ Slots
    QObject::connect(qml_minMaxSlider, SIGNAL(firstChanged(QVariant)),
                     _mainBackend.getOCTPipeline(), SLOT(slotMinScaleChanged(QVariant)));
    QObject::connect(qml_minMaxSlider, SIGNAL(secondChanged(QVariant)),
                     _mainBackend.getOCTPipeline(), SLOT(slotMaxScaleChanged(QVariant)));

    //Connect C++ signals to QML Slots

    //Updates the DAQ parameters
    QObject::connect(_mainBackend.getOCTPipeline(), SIGNAL(DAQChanged(QVariant,QVariant,QVariant)),
                     qml_bscanSlider, SLOT(slotDAQChanged(QVariant,QVariant,QVariant)));

    //Updates the current frame displayed
    QObject::connect(_mainBackend.getOCTPipeline(), SIGNAL(BScanUpdated(QVariant)),
                     qml_bscanSlider, SLOT(slotBScanUpdated(QVariant)));

    //Update and draw a processed frame
    QObject::connect(_mainBackend.getOCTPipeline(), SIGNAL(FrameProcessed()),
                     qml_BScanImage, SLOT(slotUpdateBScanImage()));

    //Updates the current frame displayed
    QObject::connect(_mainBackend.getOCTPipeline(), SIGNAL(BScanUpdated(QVariant)),
                     qml_bscanSlider, SLOT(slotBScanUpdated(QVariant)));

    QObject::connect(qml_bscanSlider, SIGNAL(signalSliderChanged(QVariant)),
                     _mainBackend.getOCTPipeline(), SLOT(slotBScanSliderChanged(QVariant)));

//    _mainBackend.getOCTPipeline()->init();
//    _mainBackend.getOCTPipeline()->setBScanSlider(qml_bscanSlider);
//    _mainBackend.getOCTPipeline()->getDisplay()->setImageProvider(_bscanImageProvider);
//    _mainBackend.getOCTPipeline()->getDisplay()->setMax(40);
//    _mainBackend.getOCTPipeline()->getDisplay()->setMin(-20);
//    _mainBackend.loadOCT("/Users/amcelroy/Code/OSIP/test_data/");
//    _mainBackend.getOCTPipeline()->getLoader()->setLoop(false);

    return app.exec();
}
