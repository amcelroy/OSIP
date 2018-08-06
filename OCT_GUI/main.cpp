#include <QApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>
#include "menubackend.h"
#include "OCTBackend.h"
#include "octconfigfile.h"
#include "loadoctpipeline.hpp"
#include "bscanimageprovider.h"
#include "qmldaqconfigbackend.h"
#include "qmlgalvobackend.h"
#include "enfaceimageprovider.h"
#include "ascanbackend.h"

#define DEBUG_QML 0

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QQmlApplicationEngine engine;

    qmlRegisterType<MenuBackend>("edu.utexas.bme.menubackend", 1, 0, "MenuBackend");
    qmlRegisterType<QMLDAQConfigBackend>("edu.utexas.bme.qmlconfigbackend", 1, 0, "QMLDAQConfigBackend");
    qmlRegisterType<QMLGalvoBackend>("edu.utexas.bme.qmlgalvobackend", 1, 0, "QMLGalvoBackend");
    qmlRegisterType<OCTBackend>("edu.utexas.bme.octbackend", 1, 0, "OCTBackend");
    qmlRegisterType<AScanBackend>("edu.utexas.bme.octbackend.ascan", 1, 0, "AScanBackend");

    //Register the Image provider BEFORE loading the qml!!!
    BScanImageProvider* _bscanImageProvider = new BScanImageProvider();
    engine.addImageProvider(QLatin1String("bscanProvider"), _bscanImageProvider);

    EnFaceImageProvider* _enfaceImageProvider = new EnFaceImageProvider();
    engine.addImageProvider(QLatin1String("enFaceProvider"), _enfaceImageProvider);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    QObject* rootObject = engine.rootObjects().first();
    QObject* qml_minMaxSlider = rootObject->findChild<QObject*>("qml_minMaxSlider");
    QObject* qml_bscanSlider = rootObject->findChild<QObject*>("qml_bscanSlider");
    QObject* qml_BScanImage = rootObject->findChild<QObject*>("qml_BScanImage");
    AScanBackend* qml_AScanBackend = rootObject->findChild<AScanBackend*>("qml_AScanBackend");
    OCTBackend* octBackend = rootObject->findChild<OCTBackend*>("qml_OCTBackend");

    //Connect QML Signals to C++ Slots
    QObject::connect(qml_minMaxSlider, SIGNAL(firstChanged(QVariant)),
                     octBackend->getOCTPipeline(), SLOT(slotMinScaleChanged(QVariant)));
    QObject::connect(qml_minMaxSlider, SIGNAL(secondChanged(QVariant)),
                     octBackend->getOCTPipeline(), SLOT(slotMaxScaleChanged(QVariant)));
    QObject::connect(qml_BScanImage, SIGNAL(enfaceChanged(QVariant, QVariant)),
                     octBackend, SLOT(enFaceSliderChanged(QVariant,QVariant)));

    //Connect C++ signals to QML Slots

    //Updates the DAQ parameters
    QObject::connect(octBackend->getOCTPipeline(), SIGNAL(DAQChanged(QVariant,QVariant,QVariant)),
                     qml_bscanSlider, SLOT(slotDAQChanged(QVariant,QVariant,QVariant)));

    //Updates the current frame displayed
    QObject::connect(octBackend->getOCTPipeline(), SIGNAL(BScanUpdated(QVariant)),
                     qml_bscanSlider, SLOT(slotBScanUpdated(QVariant)));

    //Update and draw a processed frame
    QObject::connect(octBackend->getOCTPipeline(), SIGNAL(FrameProcessed()),
                     qml_BScanImage, SLOT(slotUpdateBScanImage()));

//    //Updates the current frame displayed
//    QObject::connect(octBackend.getOCTPipeline(), SIGNAL(BScanUpdated(QVariant)),
//                     qml_bscanSlider, SLOT(slotBScanUpdated(QVariant)));

    QObject::connect(qml_bscanSlider, SIGNAL(signalSliderChanged(QVariant)),
                     octBackend->getOCTPipeline(), SLOT(slotBScanSliderChanged(QVariant)));

#if(DEBUG_QML == 0)
    //Run a normal loading procedure
    octBackend->setMode(OCTBackend::MODE_REVIEW);
    octBackend->getOCTPipeline()->init();
    octBackend->getOCTPipeline()->setBScanSlider(qml_bscanSlider);
    octBackend->getOCTPipeline()->getLoader()->setBufferData(true);
    octBackend->getOCTPipeline()->getDisplay()->setBScanImageProvider(_bscanImageProvider);
    octBackend->getOCTPipeline()->getDisplay()->setEnFaceImageProvider(_enfaceImageProvider);
    octBackend->getOCTPipeline()->getDisplay()->setAScanBackend(qml_AScanBackend);
    octBackend->getOCTPipeline()->getDisplay()->setMax(40);
    octBackend->getOCTPipeline()->getDisplay()->setMin(-20);
    octBackend->loadOCT("C:\\Users\\Austin\\Desktop\\OSIP\\test_data\\");
    octBackend->getOCTPipeline()->getLoader()->setLoop(false);
    octBackend->enFaceSliderChanged(QVariant(100), QVariant(250));
#endif

    return app.exec();
}
