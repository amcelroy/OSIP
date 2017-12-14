#-------------------------------------------------
#
# Project created by QtCreator 2017-10-19T08:49:51
#
#-------------------------------------------------

QT       += core

macx {
    #Add Labview DAQmx framework
    QMAKE_LFLAGS += -F/Library/Frameworks/
    LIBS += -framework nidaqmxbase
    LIBS += -framework nidaqmxbaselv
    INCLUDEPATH += /Library/Frameworks/nidaqmxbase.framework/Headers
}

TARGET = Peripheral
TEMPLATE = lib

CONFIG += c++11

DEFINES += PERIPHERAL_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = $$PWD/../

SOURCES += \
    galvos.cpp \
    nidaqmx.cpp \
    nidaqmxgalvos.cpp

HEADERS += \
        peripheral_global.h \  
    galvos.h \
    nidaqmx.h \
    nidaqmxgalvos.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

macx: LIBS += -L$$PWD/../ -lPipeline.1.0.0

INCLUDEPATH += $$PWD/../Pipeline
DEPENDPATH += $$PWD/../Pipeline

INCLUDEPATH += $$PWD/../dependancies/boost_1_65
INCLUDEPATH += $$PWD/../dependancies/fftw/include
