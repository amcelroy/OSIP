QT += quick widgets
QT += quickcontrols2

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += $$PWD/../boost_1_65

SOURCES += main.cpp \
    menubackend.cpp \
    octpipeline.cpp \
    octconfigfile.cpp \
    mainbackend.cpp \
    octdisplaystage.cpp \
    bscanimageprovider.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

#Enable QML debugging
CONFIG+=qml_debug

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    menubackend.h \
    octpipeline.h \
    octconfigfile.h \
    mainbackend.h \
    octdisplaystage.h \
    vertex.h \
    bscanimageprovider.h

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../release/ -lPipeline.1.0.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../debug/ -lPipeline.1.0.0
else:unix: LIBS += -L$$PWD/../ -lPipeline.1.0.0

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../release/ -lProcessingPipelineStages.1.0.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../debug/ -lProcessingPipelineStages.1.0.0
else:unix: LIBS += -L$$PWD/../ -lProcessingPipelineStages.1.0.0

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../release/ -lDAQPipelineStages.1.0.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../debug/ -lDAQPipelineStages.1.0.0
else:unix: LIBS += -L$$PWD/../ -lDAQPipelineStages.1.0.0

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../release/ -lProcessingPipelineStages.1.0.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../debug/ -lProcessingPipelineStages.1.0.0
else:unix: LIBS += -L$$PWD/../ -lProcessingPipelineStages.1.0.0

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../release/ -lDisplayPipelineStage.1.0.0
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../debug/ -lDisplayPipelineStage.1.0.0
else:unix: LIBS += -L$$PWD/../ -lDisplayPipelineStage.1.0.0

INCLUDEPATH += $$PWD/../
DEPENDPATH += $$PWD/../

DISTFILES += \
    bscan_vshader.glsl \
    bscan_fshader.glsl
