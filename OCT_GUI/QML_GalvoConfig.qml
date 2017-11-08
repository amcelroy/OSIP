import QtQuick 2.4
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2
import QtCharts 2.2
import edu.utexas.bme.qmlgalvobackend 1.0
import QtQuick.Dialogs 1.2

Dialog {
    id: window
    width: 400
    height: 400
    standardButtons: StandardButton.Save | StandardButton.Cancel
    visible: false

    QMLGalvoBackend {
        id: qmlgalvobackend
    }

    onAccepted: {
        visible = false;
    }

    onRejected: {
        visible = false;
    }

    Grid {
        flow: Grid.TopToBottom
        anchors.fill: parent
        rows: 2

        verticalItemAlignment: Grid.AlignVCenter
        horizontalItemAlignment: Grid.AlignHCenter

        ChartView {
            id: scanPatternDisplay
            title: "Scan Pattern"
            anchors.topMargin: 0
            animationOptions: ChartView.NoAnimation
            theme: ChartView.ChartThemeDark

            width: 200
            height: 200
            backgroundColor: "#d71111"

            PieSeries {
                id: pieSeries
                PieSlice {
                    label: "eaten"
                    value: 94.9
                }
                PieSlice {
                    label: "not yet eaten"
                    value: 5.1
                }
            }
        }

        Grid {
            id: grid
            columns: 2
            rows: 4
            horizontalItemAlignment: Grid.AlignHCenter
            verticalItemAlignment: Grid.AlignVCenter

            Label {
                id: fastAxisAmplitudeLabel
                text: qsTr("Fast Axis Amplitude")
            }

            TextField {
                id: fastAxisAmplitudeTextField
                text: qsTr("Text Field")
                scale: 0.8
            }

            Label {
                id: fastAxisOffsetLabel
                text: qsTr("Fast Axis Offset")
            }

            TextField {
                id: fastAxisOffsetTextField
                text: qsTr("Text Field")
                scale: 0.8
            }

            Label {
                id: slowAxisAmplitudeLabel
                text: qsTr("Slow Axis Amplitude")
            }

            TextField {
                id: slowAxisAmplitudeTextField
                text: qsTr("Text Field")
                scale: 0.8
            }

            Label {
                id: slowAxisOffsetLabel
                text: qsTr("Slow Axis Offset")
            }

            TextField {
                id: slowAxisOffsetTextField
                text: qsTr("Text Field")
                scale: 0.8
            }
        }
    }
}
