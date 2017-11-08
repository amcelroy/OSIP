#-------------------------------------------------
#
# Project created by QtCreator 2017-11-06T12:58:42
#
#-------------------------------------------------

QT       -= gui
QT += quick widgets
QT += quickcontrols2

TARGET = OCTLibrary
TEMPLATE = lib

DEFINES += OCTLIBRARY_LIBRARY

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

INCLUDEPATH += $$PWD/../boost_1_65

SOURCES += \
        octlibrary.cpp \
    loadoctpipeline.cpp \
    octconfigfile.cpp \
    octdisplaystage.cpp \
    octpipeline.cpp \
    octpipelinestage_cpu.cpp \
    windowmaker.cpp \
    alazartechdaqstage.cpp \
    nodaqstage.cpp \
    bscanimageprovider.cpp \
    enfaceimageprovider.cpp

HEADERS += \
        octlibrary.h \
        octlibrary_global.h \ 
    loadoctpipeline_impl.h \
    loadoctpipeline.h \
    octconfigfile.h \
    octdisplaystage.h \
    octpipeline.h \
    octpipelinestage_cpu.h \
    windowmaker.h \
    alazartechdaqstage.h \
    nodaqstage.h \
    bscanimageprovider.h \
    enfaceimageprovider.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix|win32: LIBS += -L$$PWD/../ -lPipeline.1.0.0

INCLUDEPATH += $$PWD/../Pipeline
DEPENDPATH += $$PWD/../

unix|win32: LIBS += -L$$PWD/../ -lPeripheral.1.0.0

INCLUDEPATH += $$PWD/../Peripheral
DEPENDPATH += $$PWD/../

unix|win32: LIBS += -L$$PWD/../ -lfftw3f.3

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

unix|win32: LIBS += -L$$PWD/../ -lDisplayPipelineStage.1.0.0

INCLUDEPATH += $$PWD/../DisplayPipelineStage
DEPENDPATH += $$PWD/../
