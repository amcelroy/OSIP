import QtQuick 2.4
import QtCharts 2.2
import QtQuick.Dialogs 1.2
import edu.utexas.bme.octbackend.ascan 1.0

Dialog {
    property alias chartRawAScan: chartRawAScan

    width: 400
    height: 400

    Column {
        id: column
        x: 0
        y: 0
        width: 200
        height: 400

        ChartView {
            id: chartRawAScan
            title: "Raw A-Scan"
            antialiasing: true

            ValueAxis{
                id: x_axis_raw;
            }

            ValueAxis{
                id: y_axis_raw;
                min: -4
                max: 4
            }

            LineSeries {
                id: lineseriesRawAScan
                name: "Raw A-Scan in Volts"
                axisX: x_axis_raw
                axisY: y_axis_raw
            }
        }
    }
}
