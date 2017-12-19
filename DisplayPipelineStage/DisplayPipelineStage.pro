#-------------------------------------------------
#
# Project created by QtCreator 2017-10-02T11:19:06
#
#-------------------------------------------------

QT       -= gui

TARGET = DisplayPipelineStage
TEMPLATE = lib

DEFINES += DISPLAYPIPELINESTAGE_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = ../

SOURCES += \
        displaypipelinestage.cpp

HEADERS += \
        displaypipelinestage.h \
        displaypipelinestage_global.h \ 
    displaypipelinestage_impl.h

win32 {
    LIBS += -L$$PWD/../Peripheral.dll
    LIBS += -L$$PWD/../ -lPipeline
}

macx {
    LIBS += -L$$PWD/../ -lPipeline.1.0.0
    LIBS += -L$$PWD/../ -lPeripheral.1.0.0
}

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../
INCLUDEPATH += $$PWD/../dependancies/boost_1_65
