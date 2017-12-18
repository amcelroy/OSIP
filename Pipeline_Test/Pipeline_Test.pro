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

unix|macx: LIBS += -L$$PWD/../ -lPipeline.1.0.0

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

win32 {
    QMAKE_CXXFLAGS_WARN_ON -= -wd4661

    LIBS += -L"C:\Program Files (x86)\AMD APP SDK\3.0\lib\x86_64" -lopencl
    INCLUDEPATH += "C:\Program Files (x86)\AMD APP SDK\3.0\include"

    Release {
        LIBS += -L$$PWD/../dependancies/boost_1_65/lib/win-vs17_x64 -llibboost_filesystem-vc141-mt-1_65_1
        LIBS += -L$$PWD/../dependancies/boost_1_65/lib/win-vs17_x64 -llibboost_system-vc141-mt-1_65_1
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_tools
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_hl
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_cpp
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_hl_cpp
    }

    Debug {
        LIBS += -L$$PWD/../dependancies/boost_1_65/lib/win-vs17_x64 -llibboost_filesystem-vc141-mt-gd-1_65_1
        LIBS += -L$$PWD/../dependancies/boost_1_65/lib/win-vs17_x64 -llibboost_system-vc141-mt-gd-1_65_1
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_D
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_tools_D
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_hl_D
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_cpp_D
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_hl_cpp_D
    }

    DEPENDPATH += $$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64
    LIBS += -L$$PWD/../dependancies/szip/lib/win-vs17_x64 -llibszip
    LIBS += -L$$PWD/../dependancies/zlib/lib/win-vs17_x64 -llibzlib

    #OSIP Libraries
    LIBS += -L$$PWD/../ -lPipeline
}

INCLUDEPATH += $$PWD/../dependancies/HDF5/1.10.1/include
INCLUDEPATH += $$PWD/../dependancies/boost_1_65
INCLUDEPATH += $$PWD/../dependancies/szip/include
INCLUDEPATH += $$PWD/../dependancies/zlib/include

TARGET = pipeline_test_exe
