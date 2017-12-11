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

macx: LIBS += -L$$PWD/../ -lboost_filesystem
macx: LIBS += -L$$PWD/../ -lboost_system

DEPENDPATH += $$PWD/../HDF5/1.10.1/lib
macx: LIBS += -L$$PWD/../HDF5/1.10.1/lib -lhdf5
macx: LIBS += -L$$PWD/../HDF5/1.10.1/lib -lhdf5_tools
macx: LIBS += -L$$PWD/../HDF5/1.10.1/lib -lhdf5_hl
macx: LIBS += -L$$PWD/../HDF5/1.10.1/lib -lhdf5_cpp
macx: LIBS += -L$$PWD/../HDF5/1.10.1/lib -lhdf5_hl_cpp

DEPENDPATH += $$PWD/../szip/2.1.1/lib
macx: LIBS += -L$$PWD/../szip/2.1.1/lib -lszip

DEPENDPATH += $$PWD/../zlib/1.2.11/lib
macx: LIBS += -L$$PWD/../zlib/1.2.11/lib -lz

INCLUDEPATH += $$PWD/../HDF5/1.10.1/include
INCLUDEPATH += $$PWD/../szip/2.1.1/include
INCLUDEPATH += $$PWD/../zlib/1.2.11/include

TARGET = pipeline_test_exe
