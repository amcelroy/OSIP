import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import edu.utexas.bme.qmlconfigbackend 1.0
import QtQuick.Dialogs 1.2

import "Support.js" as Support

Dialog {
    id: window;
    visible: true
    width: 480
    height: 480
    title: qsTr("DAQ Config")
    standardButtons: StandardButton.Save | StandardButton.Cancel


    onAccepted: {
        qmldaqconfigbackend.saveDAQConfig(wrapToJSON());
        visible = false;
    }

    onRejected: {
        visible = false;
    }

    Component.onCompleted: {
        var daqconfig = qmldaqconfigbackend.loadDAQConfig();

        var json = JSON.parse(daqconfig);

        pointsPerAScanTextField.text = json.pointsperascan;
        aScansPerBScanTextField.text = json.ascansperbscan
        daqVoltageRange.text = json.daqrange;
        daqBits.text = json.daqbits;
        totalBScanSTextField.text = json.totalbscans;
    }

    //Wraps the controls to a JSON string
    function wrapToJSON(){
        var obj = new Object();
        obj.pointsperascan =  pointsPerAScanTextField.text;
        obj.ascansperbscan = aScansPerBScanTextField.text;
        obj.daqrange = daqVoltageRange.text;
        obj.daqbits = daqBits.text;
        obj.totalbscans = totalBScanSTextField.text;

        return JSON.stringify(obj);
    }

    //Create the C++ object
    QMLDAQConfigBackend {
        id: qmldaqconfigbackend;
    }

    Grid {
        columns: 2
        anchors.fill: parent
        flow: Grid.TopToBottom

        verticalItemAlignment: Grid.AlignVCenter
        horizontalItemAlignment: Grid.AlignHCenter

        Label{
            text: "Points Per A-Scan";
        }

        Label{
            text: "A-Scans Per B-Scan";
        }

        Label{
            text: "Total B-Scans";
        }

        Label{
            text: "DAQ Voltage Range";
        }

        Label{
            text: "DAQ Bits";
        }

        TextField {
            id: pointsPerAScanTextField
            text: "1024";
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 2
                    implicitWidth: 100
                    implicitHeight: 24
                    border.color: "#333"
                    border.width: 1
                }
            }
            onTextChanged: {
                Support.checkTextIsInt(pointsPerAScanTextField.text);
            }
        }

        TextField {
            id: aScansPerBScanTextField
            text: "512";
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 2
                    implicitWidth: 100
                    implicitHeight: 24
                    border.color: "#333"
                    border.width: 1
                }
            }
            onTextChanged: {
                Support.checkTextIsInt(aScansPerBScanTextField.text);
            }
        }

        TextField {
            id: totalBScanSTextField
            text: "512";
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 2
                    implicitWidth: 100
                    implicitHeight: 24
                    border.color: "#333"
                    border.width: 1
                }
            }
            onTextChanged: {
                Support.checkTextIsInt(totalBScanSTextField.text);
            }
        }

        TextField {
            id: daqVoltageRange
            text: "2.0";
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 2
                    implicitWidth: 100
                    implicitHeight: 24
                    border.color: "#333"
                    border.width: 1
                }
            }
            onTextChanged: {
                Support.checkTextIsFloat(daqVoltageRange.text);
            }
        }

        TextField {
            id: daqBits
            text: "12";
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 2
                    implicitWidth: 100
                    implicitHeight: 24
                    border.color: "#333"
                    border.width: 1
                }
            }
            onTextChanged: {
                Support.checkTextIsInt(daqBits.text);
            }
        }
    }
}
