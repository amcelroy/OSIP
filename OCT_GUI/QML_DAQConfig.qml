import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import edu.utexas.bme.qmlconfigbackend 1.0

Window {
    id: window;
    visible: true
    width: 480
    height: 480
    title: qsTr("DAQ Config")

    function wrapToJSON(){
        var obj = new Object();
        obj.pointsperascan =  pointsPerAScanTextField.text;

        return JSON.stringify(obj);
    }

    function checkTextIsInt(text){
        var check = parseInt(text);
        if(Number.isNaN(check)) {
            continueButton.enabled = false;
        }else{
            continueButton.enabled = true;
        }
    }

    QMLDAQConfigBackend {
        id: qmldaqconfigbackend;
    }

    Column{
        spacing: 2;
        anchors { horizontalCenter: parent.horizontalCenter; }

        Row {
            spacing: 10;

            Label{
                text: "Points Per A-Scan";
                anchors { verticalCenter: pointsPerAScanTextField.verticalCenter; }
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
                    checkTextIsInt(pointsPerAScanTextField.text);
                }
            }
        }

        Row {
            spacing: 10;
            anchors { horizontalCenter: parent.horizontalCenter; }

            Label{
                text: "A-Scans Per B-Scan";
                anchors { verticalCenter: aScansPerBScanTextField.verticalCenter; }
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
            }
        }

        Row {
            spacing: 10;
            anchors { horizontalCenter: parent.horizontalCenter; }

            Label{
                text: "Total B-Scans";
                anchors { verticalCenter: totalBScanSTextField.verticalCenter; }
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
            }
        }

        Row {
            spacing: 10;
            anchors { horizontalCenter: parent.horizontalCenter; }

            Button{
                text: "Cancel";
                onClicked: {
                    window.close();
                }
            }

            Button {
                id: continueButton;
                text: "Continue";
                enabled: true;
                onClicked: {
                    qmldaqconfigbackend.saveDAQConfig(wrapToJSON());
                }
            }
        }
    }
}
