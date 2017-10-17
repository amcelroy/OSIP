import QtQuick 2.4
import QtCanvas3D 1.1
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import QtQuick.Controls 1.4
import edu.utexas.bme.menubackend 1.0
import QtCharts 2.2

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

    Timer {
        interval: 5000
        repeat: true
        running: true
        onTriggered: {
            gc();
        }
    }

    Timer {
        interval: 33
        repeat: true
        running: true
        onTriggered: {
            bscanImage.source = "image://bscanProvider/intensity?" + Math.random();
        }
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

            Image {
                id: bscanImage
                source: "image://bscanProvider/x"
                width: 512
                height: 512
                cache: false
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

            Rectangle {
                id: chartAreaPhase
                width: 200
                height: 200
                color: "#000000"

                ChartView{
                    id: chartViewPhase
                    dropShadowEnabled: false
                    plotAreaColor: "#ffffff"
                    titleColor: "#000000"
                    title: "Phase"
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

        Rectangle {
            id: rectangle3
            width: 200
            height: 200
            color: "#ffffff"
        }

    }
}
