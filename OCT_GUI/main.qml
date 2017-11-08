import QtQuick 2.4
import QtCanvas3D 1.1
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import edu.utexas.bme.menubackend 1.0
import QtCharts 2.2
import QtQuick.Controls.Styles 1.1
import QtQuick.Dialogs 1.2
import QtQuick 2.7
import edu.utexas.bme.octbackend 1.0

ApplicationWindow {
    id: window
    title: qsTr("OCT_GUI")
    width: 1280
    height: 768
    visible: true

    property var daqWindow;
    property var galvoWindow;

    onActiveChanged: {
        chartViewIntensity.legend.visible = false;
        chartViewPhase.legend.visible = false;
    }

    Component.onCompleted: {
        var daqcomponent = Qt.createComponent("QML_DAQConfig.qml")
        daqWindow = daqcomponent.createObject(daqWindow)

        var galvocomponent = Qt.createComponent("QML_GalvoConfig.qml")
        galvoWindow = galvocomponent.createObject(galvoWindow)
    }

    MenuBackend{
        id: menuBackend;
    }

    OCTBackend{
        id: octBackend;
    }

    MenuBar{
        id: menuBar

        Menu{
            title: "File"
            MenuItem {
                text: "Open"
                onTriggered: menuBackend.selectedItem = text
            }
            MenuItem {
                text: "Options"
                onTriggered: menuBackend.selectedItem = text
            }
        }
        Menu{
            title: "Configure"
            MenuItem {
                text: "Pipeline"
                onTriggered: menuBackend.selectedItem = text
            }
            MenuItem {
                text: "DAQ"
                onTriggered: {
                    daqWindow.visible = true;
                }

            }
            MenuItem {
                text: "Galvos"
                onTriggered: {
                    galvoWindow.visible = true;
                }
            }
        }
    }

    SplitView {
        id: splitView
        orientation: Qt.Vertical
        anchors.fill: parent
        width: windows.width / 2
        height: window.width / 2

        SplitView {
            id: splitView1
            width: window.width / 2;
            height: window.width / 2;
            orientation: Qt.Horizontal
            visible: true

            Rectangle{
                id: rectImageView;
                width: window.width / 2;
                height: bscanImage.height;
                color: "#000000";

                Image {
                    signal enfaceChanged(var x, var y);

                    id: bscanImage
                    objectName: "qml_BScanImage";
                    source: "image://bscanProvider/x";
                    width: parent.width - minMaxSlider.width;
                    height: parent.height - bscanSlider.height;
                    cache: false;

                    function slotUpdateBScanImage(){
                        bscanImage.source = "image://bscanProvider/intensity?" + Math.random();
                        enFaceImage.source = "image://enFaceProvider/intensity?" + Math.random();
                    }

                    Component.onCompleted: {
                        bscanImage.enfaceChanged(enface1MouseArea.y, enface2MouseArea.y);
                    }

                    /*Rectangle {
                        signal aScanSelectedChanged(var x);

                        id: aScanSelector;
                        objectName: "qml_aScanSelector";
                        width: 3;
                        height: parent.height;
                        x: parent.width/2;

                        MouseArea {
                            anchors.fill: parent;
                            drag.target: parent;
                            drag.axis: Drag.XAxis;
                            drag.maximumX: bscanImage.width*.98;
                            drag.minimumX: bscanImage.width*.02;
                            onReleased: {
                                aScanSelector.aScanSelectedChanged(x);
                            }
                        }
                    }*/

                    Rectangle {

                        id: enFace1;
                        objectName: "qml_enFace1";
                        width: parent.width;
                        height: 3;
                        x: 0;
                        y: parent.height*.45;

                        MouseArea {
                            id: enface1MouseArea;
                            anchors.fill: parent;
                            drag.target: parent;
                            drag.axis: Drag.YAxis;
                            drag.maximumX: bscanImage.height*.98;
                            drag.minimumX: bscanImage.height*.02;
                            onReleased: {
                                bscanImage.enfaceChanged(enface1MouseArea.y, enface2MouseArea.y);
                            }
                        }
                    }

                    Rectangle {
                        signal enface2changed(var x);

                        id: enFace2;
                        objectName: "qml_enFace2";
                        width: parent.width;
                        height: 3;
                        x: 0;
                        y: parent.height*.5;

                        MouseArea {
                            id: enface2MouseArea;
                            anchors.fill: parent;
                            drag.target: parent;
                            drag.axis: Drag.YAxis;
                            drag.maximumX: bscanImage.height*.98;
                            drag.minimumX: bscanImage.height*.02;
                            onReleased: {
                                bscanImage.enfaceChanged(enface1MouseArea.y, enface2MouseArea.y);
                            }
                        }
                    }
                }

                RangeSlider{
                    signal firstChanged(var f);
                    signal secondChanged(var f);

                    id: minMaxSlider;
                    objectName: "qml_minMaxSlider";
                    orientation: Qt.Vertical;
                    width: 32;
                    height: parent.height*.9;
                    anchors.right: parent.right;
                    anchors.verticalCenter: parent.verticalCenter;
                    first.onValueChanged: firstChanged(first.value)
                    second.onValueChanged: secondChanged(second.value)
                    from: -40;
                    to: 60;
                    first.value: -20;
                    second.value: 40;
                }

                Grid {
                    columns: 3
                    anchors.bottom: parent.bottom;
                    anchors.horizontalCenter: parent.horizontalCenter;
                    width: parent.width*.8;

                    Button {
                        text: "Record"
                        onClicked: {
                            octBackend.record();
                        }
                    }

                    Slider{
                        signal signalSliderChanged(var i);

                        id: bscanSlider;
                        objectName: "qml_bscanSlider";
                        height: minMaxSlider.width;
                        width: parent.width*.8;
                        orientation: Qt.Horizontal;
                        stepSize: 1;
                        value: 0;
                        minimumValue: 0;
                        maximumValue: 100;
                        onValueChanged: {
                            signalSliderChanged(bscanSlider.value);
                        }
                        style: SliderStyle {
                            groove: Rectangle{
                                color: "#CC5500";
                                implicitHeight: 6;
                                radius: 6;
                            }
                        }

                        function slotDAQChanged(pointsPerA, AperB, NumB){
                            maximumValue = NumB;
                        }

                        function slotBScanUpdated(frame){
                            value = frame;
                        }
                    }
                }
            }

            Rectangle {
                id: enFaceRectangle
                width: window.width / 2;
                height: 512;
                color: "#000000"

                Image {
                    id: enFaceImage
                    objectName: "qml_EnFaceImage";
                    source: "image://enFaceProvider/x";
                    width: parent.width - minMaxSlider.width;
                    height: parent.height - bscanSlider.height;
                    cache: false;
                }
            }
        }
    }
}
