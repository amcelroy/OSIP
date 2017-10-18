import QtQuick 2.4
import QtCanvas3D 1.1
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import edu.utexas.bme.menubackend 1.0
import QtCharts 2.2
import QtQuick.Controls.Styles 1.1

import "BScan.js" as GLBScan;

Window {
    id: window
    title: qsTr("OCT_GUI")
    width: 1280
    height: 768
    visible: true

    onActiveChanged: {
        chartViewIntensity.legend.visible = false;
        chartViewPhase.legend.visible = false;
    }

    MenuBackend{
        id: menuBackend;
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
                onTriggered: menuBackend.selectedItem = text
            }
            MenuItem {
                text: "Galvos"
                onTriggered: menuBackend.selectedItem = text
            }
        }
    }

    SplitView {
        id: splitView
        orientation: Qt.Vertical
        anchors.fill: parent
        width: 256
        height: 256

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
                    id: bscanImage
                    objectName: "qml_BScanImage";
                    source: "image://bscanProvider/x";
                    width: parent.width - minMaxSlider.width;
                    height: parent.height - bscanSlider.height;
                    cache: false;

                    function slotUpdateBScanImage(){
                        bscanImage.source = "image://bscanProvider/intensity?" + Math.random();
                    }

                    Rectangle {
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
                    }
                }

                RangeSlider{
                    signal firstChanged(var f);
                    signal secondChanged(var f);

                    id: minMaxSlider;
                    objectName: "qml_minMaxSlider";
                    orientation: Qt.Vertical;
                    width: 32;
                    height: 512;
                    anchors.right: parent.right;
                    anchors.verticalCenter: parent.verticalCenter;
                    first.onValueChanged: firstChanged(first.value)
                    second.onValueChanged: secondChanged(second.value)
                    from: -40;
                    to: 60;
                    first.value: -20;
                    second.value: 40;
                }

                Slider{
                    signal signalSliderChanged(var i);

                    id: bscanSlider;
                    objectName: "qml_bscanSlider";
                    height: minMaxSlider.width;
                    width: parent.width - 50;
                    orientation: Qt.Horizontal;
                    anchors.bottom: parent.bottom;
                    anchors.horizontalCenter: parent.horizontalCenter;
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

            Rectangle {
                id: rectangle1
                width: 200
                height: 512
                color: "#000000"
            }

        }

        SplitView {
            id: splitView3
            width: 100
            height: 100
            orientation: Qt.Horizontal

            Rectangle {
                id: chartAreaIntensity
                width: 200
                height: 200
                color: "#000000"

                ChartView{
                    id: chartViewIntensity
                    plotAreaColor: "#ffffff"
                    titleColor: "#000000"
                    title: "Intensity"
                    anchors.fill: parent
                    antialiasing: true
                    LineSeries {
                        name: "LineSeries"
                        XYPoint { x: 0; y: 0 }
                        XYPoint { x: 1.1; y: 2.1 }
                        XYPoint { x: 1.9; y: 3.3 }
                        XYPoint { x: 2.1; y: 2.1 }
                        XYPoint { x: 2.9; y: 4.9 }
                        XYPoint { x: 3.4; y: 3.0 }
                        XYPoint { x: 4.1; y: 3.3 }
                    }

                }
            }
        }
    }
}
