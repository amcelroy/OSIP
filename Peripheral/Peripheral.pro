#-------------------------------------------------
#
# Project created by QtCreator 2017-10-19T08:49:51
#
#-------------------------------------------------

#QT       += core

TARGET = Peripheral
TEMPLATE = lib

CONFIG += c++11

DEFINES += PERIPHERAL_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
#DEFINES += QT_DEPRECATED_WARNINGS

DESTDIR = ../

SOURCES += \
    galvos.cpp \
    nidaqmxgalvos.cpp

HEADERS += \
    galvos.h \
    nidaqmxgalvos.h


win32 {
    INCLUDEPATH += "C:\Program Files (x86)\National Instruments\Shared\ExternalCompilerSupport\C\include"
    LIBS += -L"C:\Program Files (x86)\National Instruments\Shared\ExternalCompilerSupport\C\lib64\msvc" -lNIDAQmx
}

macx {
    QMAKE_MAC_SDK = macosx10.13

    #Add Labview DAQmx framework
    QMAKE_LFLAGS += -F/Library/Frameworks/
    LIBS += -framework nidaqmxbase
    LIBS += -framework nidaqmxbaselv
    INCLUDEPATH += /Library/Frameworks/nidaqmxbase.framework/Headers
}

INCLUDEPATH += $$PWD/../dependancies/boost_1_65
INCLUDEPATH += $$PWD/../Pipeline
