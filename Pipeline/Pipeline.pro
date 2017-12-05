#-------------------------------------------------
#
# Project created by QtCreator 2017-09-16T20:14:51
#
#-------------------------------------------------

QT       += core network opengl

QT       -= gui

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.12

TARGET = Pipeline
TEMPLATE = lib

DEFINES += PIPELINE_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DESTDIR = ../

INCLUDEPATH += $$PWD/../boost_1_65
INCLUDEPATH += $$PWD/../

SOURCES += \
    inlet.cpp \
    pipelinestage.cpp \
    daqstage.cpp \
    payload.cpp \
    savingstage.cpp \
    openclpipeline.cpp

HEADERS += \
    pipeline_global.h \
    inlet.h \
    pipelinestage.h \
    inlet_impl.h \
    pipelinestage_impl.h \
    daqstage.h \
    daqstage_impl.h \
    payload.h \
    payload_impl.h \
    savingstage.h \
    savingstage_impl.h \
    openclpipeline.h \
    openclpipeline_impl.h \
    pipeline.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

unix|win32: LIBS += -L$$PWD/../ -lPeripheral.1.0.0

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

macx {
    #Add OpenCL framework
    XCODE_FRAMEWORKS = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks
    QMAKE_LFLAGS += -F$${XCODE_FRAMEWORKS}
    LIBS += -framework OpenCL
    INCLUDEPATH += $${XCODE_FRAMEWORKS}/opencl.framework/Headers
}

macx: LIBS += -L$$PWD/../ -lboost_filesystem
macx: LIBS += -L$$PWD/../ -lboost_system
macx: LIBS += -L$$PWD/../HDF5/1.10.1/lib -lhdf5
macx: LIBS += -L$$PWD/../HDF5/1.10.1/lib -lhdf5_tools
macx: LIBS += -L$$PWD/../HDF5/1.10.1/lib -lhdf5_hl
macx: LIBS += -L$$PWD/../HDF5/1.10.1/lib -lhdf5_cpp
macx: LIBS += -L$$PWD/../HDF5/1.10.1/lib -lhdf5_hl_cpp
INCLUDEPATH += $$PWD/../HDF5/1.10.1/include
