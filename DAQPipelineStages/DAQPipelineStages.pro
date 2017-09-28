#-------------------------------------------------
#
# Project created by QtCreator 2017-09-19T13:50:54
#
#-------------------------------------------------

QT       -= gui

TARGET = DAQPipelineStages
TEMPLATE = lib

DEFINES += DAQPIPELINESTAGES_LIBRARY

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
        alazartechdaqstage.cpp \
    nodaqstage.cpp \
    loaddata.cpp

HEADERS += \
        alazartechdaqstage.h \
        daqpipelinestages_global.h \ 
    nodaqstage.h \
    loaddata.h \
    loaddata_impl.h

INCLUDEPATH += $$PWD/../
INCLUDEPATH += $$PWD/../boost_1_65

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix|win32: LIBS += -L$$PWD/../ -lPipeline.1.0.0

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

DESTDIR = $$PWD/../
