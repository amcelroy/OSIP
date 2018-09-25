#-------------------------------------------------
#
# Project created by QtCreator 2017-11-06T12:58:42
#
#-------------------------------------------------

QT       -= gui

TARGET = OCTLibrary
TEMPLATE = app

CONFIG += c++17

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

SOURCES += \
    octdisplaystage.cpp \
    octpipelinestage_cpu.cpp \
    octconfigfile.cpp \
    main.cpp \
    websocketserver.cpp

HEADERS += \
    octdisplaystage.h \
    octpipeline.h \
    octpipelinestage_cpu.h \
    nodaqstage.h \
    loadoctpipeline.hpp \
    octconfigfile.h \
    websocketserver.h

macx {
    QMAKE_MAC_SDK = macosx10.13
    #Add OpenCL framework
    XCODE_FRAMEWORKS = /Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX.sdk/System/Library/Frameworks
    QMAKE_LFLAGS += -F$${XCODE_FRAMEWORKS}
    LIBS += -framework OpenCL
    INCLUDEPATH += $${XCODE_FRAMEWORKS}/opencl.framework/Headers

    LIBS += -L$$PWD/../dependancies/boost_1_65/lib/macOS-xcode_x64 -lboost_filesystem
    LIBS += -L$$PWD/../dependancies/boost_1_65/lib/macOS-xcode_x64 -lboost_system
    LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/macOS-xcode_x64 -lhdf5
    LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/macOS-xcode_x64 -lhdf5_tools
    LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/macOS-xcode_x64 -lhdf5_hl
    LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/macOS-xcode_x64 -lhdf5_cpp
    LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/macOS-xcode_x64 -lhdf5_hl_cpp
    LIBS += -L$$PWD/../dependancies/fftw/lib/macOS-xcode_x64 -lfftw3f
    LIBS += -L$$PWD/../dependancies/szip/lib/macOS-xcode_x64 -lszip
    LIBS += -L$$PWD/../dependancies/zlib/lib/macOS-xcode_x64 -lz
}

win32 {
    QMAKE_CXXFLAGS_WARN_ON -= -wd4661

    ###OPENCL###
    LIBS += -L"C:\Program Files (x86)\AMD APP SDK\3.0\lib\x86_64" -lopencl
    LIBS += -L"C:\Intel\OpenCL\sdk\lib\x64" -lopencl
    INCLUDEPATH += "C:\Program Files (x86)\AMD APP SDK\3.0\include"
    INCLUDEPATH += "C:\Intel\OpenCL\sdk\include"

    Release {
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_tools
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_hl
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_cpp
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_hl_cpp
    }

    Debug {
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_D
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_tools_D
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_hl_D
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_cpp_D
        LIBS += -L$$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64 -llibhdf5_hl_cpp_D
    }

    DEPENDPATH += $$PWD/../dependancies/HDF5/1.10.1/lib/win10-vs17_x64
    LIBS += -L$$PWD/../dependancies/szip/lib/win-vs17_x64 -llibszip
    LIBS += -L$$PWD/../dependancies/zlib/lib/win-vs17_x64 -llibzlib
    LIBS += -L$$PWD/../dependancies/fftw/lib/win-vs17_x64 -llibfftw3f-3
}

INCLUDEPATH += $$PWD/../Peripheral
INCLUDEPATH += $$PWD/../Pipeline
INCLUDEPATH += $$PWD/../dependancies/HDF5/1.10.1/include
INCLUDEPATH += $$PWD/../dependancies/boost_1_65
INCLUDEPATH += $$PWD/../dependancies/fftw/include
INCLUDEPATH += $$PWD/../dependancies/szip/include
INCLUDEPATH += $$PWD/../dependancies/zlib/include
INCLUDEPATH += $$PWD/../dependancies
DEPENDPATH += $$PWD/..
