#-------------------------------------------------
#
# Project created by QtCreator 2017-09-19T13:10:22
#
#-------------------------------------------------

QT       -= gui

TARGET = SignalProcessingPipelineStages
TEMPLATE = lib

DEFINES += SIGNALPROCESSINGPIPELINESTAGES_LIBRARY

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    windowmaker.cpp \
    octpipelinestage_cpu.cpp

HEADERS += \
        signalprocessingpipelinestages_global.h \ 
    windowmaker.h \
    octpipelinestage_cpu.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

INCLUDEPATH += $$PWD/../

unix|win32: LIBS += -L$$PWD/../ -lPipeline.1.0.0

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

DESTDIR += $$PWD/../

unix|win32: LIBS += -L$$PWD/../ -lfftw3f

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../
