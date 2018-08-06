#-------------------------------------------------
#
# Project created by QtCreator 2018-07-14T10:07:54
#
#-------------------------------------------------

QT       -= core gui

TARGET = AlazarTechLibrary
TEMPLATE = lib

DEFINES += ALAZARTECHLIBRARY_LIBRARY

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
    alazartechdaqstage.cpp

HEADERS += \
        alazartechlibrary.h \
    alazartechdaqstage.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}

win32 {
    QMAKE_CXXFLAGS_WARN_ON -= -wd4661

    LIBS += -L$$PWD/../dependancies/Alazar/lib/x64 -lAtsApi
}

INCLUDEPATH += $$PWD/../Pipeline
INCLUDEPATH += $$PWD/../dependancies/boost_1_65
INCLUDEPATH += $$PWD/../dependancies/Alazar/include
DESTDIR = $$PWD/../
