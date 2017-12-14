QT -= gui

CONFIG += c++11 console
CONFIG -= app_bundle

QMAKE_MAC_SDK = macosx10.12

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DESTDIR = ../

SOURCES += main.cpp

unix|win32: LIBS += -L$$PWD/../ -lPipeline.1.0.0

INCLUDEPATH += $$PWD/../Pipeline
DEPENDPATH += $$PWD/../Pipeline

INCLUDEPATH += $$PWD/../boost_1_65

DISTFILES += \
    test.cl \

HEADERS += \
    openclpipeline_test.h \
    saving_test.h

macx {
    LIBS += -L$$PWD/../dependancies/boost_1_65/lib/macOS-xcode_x64 -lboost_filesystem
    LIBS += -L$$PWD/../dependancies/boost_1_65/lib/macOS-xcode_x64 -lboost_system
    LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/macOS-xcode_x64 -lhdf5
    LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/macOS-xcode_x64 -lhdf5_tools
    LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/macOS-xcode_x64 -lhdf5_hl
    LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/macOS-xcode_x64 -lhdf5_cpp
    LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/macOS-xcode_x64 -lhdf5_hl_cpp
    LIBS += -L$$PWD/../dependancies/szip/lib/macOS-xcode_x64 -lszip
    LIBS += -L$$PWD/../dependancies/zlib/lib/macOS-xcode_x64 -lz
}

INCLUDEPATH += $$PWD/../dependancies/HDF5/1.10.1/include
INCLUDEPATH += $$PWD/../dependancies/boost_1_65
INCLUDEPATH += $$PWD/../dependancies/szip/include
INCLUDEPATH += $$PWD/../dependancies/zlib/include

TARGET = pipeline_test_exe
