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

    function updateGUI(){
        if(octBackend.Mode == octBackend.MODE_REVIEW){
            reprocessOrGalvo.text = "Update En Face";
        }else if(octBackend.Mode == octBackend.MODE_LIVE){

        }
    }

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
        objectName: "qml_OCTBackend";
        onModeChanged: {
            window.updateGUI();
        }
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

                    MouseArea {
                        property int startX: 0
                        property int startY: 0
                        property bool dragStarted: false
                        property bool rectSelected: false


                        id: enfaceMouseArea;
                        anchors.fill: parent;
                        drag.maximumX: parent.width;
                        drag.minimumX: 0;
                        drag.minimumY: 0;
                        drag.maximumY: parent.height;

                        Rectangle{
                            id: selectionRectangle;
                            color: "transparent";
                            border.color: "yellow";

                            function bounds(point){
                                selectionRectangle.x = point.x - width/2;
                                selectionRectangle.y = point.y - height/2;

                                if(x + width > parent.width){
                                    //Don't move x, allow y move
                                    width = (parent.width - point.x)*2;
                                    x = point.x - width/2;
                                }

                                if(x < 0){
                                    //Don't move x, allow y move
                                    width = (point.x)*2;
                                    x = 0;
                                }

                                if(y < 0){
                                    //Don't move x, allow y move
                                    height = (point.y)*2;
                                    y = 0;
                                }

                                if(y + height > parent.height){
                                    //Don't move x, allow y move
                                    height = (parent.height - point.y)*2;
                                    y = point.y - height/2;
                                }
                            }
                        }

                        onPressed: {
                            var p = selectionRectangle.mapFromItem(enfaceMouseArea,
                                            enfaceMouseArea.mouseX,
                                            enfaceMouseArea.mouseY);
                            if(selectionRectangle.contains(p)){
                                //Box was clicked on, move the box
                                selectionRectangle.bounds(Qt.point(enfaceMouseArea.mouseX, enfaceMouseArea.mouseY));
                                rectSelected = true;
                            }else{
                                //Point is outside of box, create a new box
                                startX = enfaceMouseArea.mouseX;
                                startY = enfaceMouseArea.mouseY;

                                selectionRectangle.x = -1;
                                selectionRectangle.y = -1;
                                selectionRectangle.width = 0;
                                selectionRectangle.height = 0;

                                selectionRectangle.x = startX;
                                selectionRectangle.y = startY;
                                dragStarted = true;
                                rectSelected = false;
                            }
                        }

                        onPositionChanged: {
                            if(dragStarted){
                                selectionRectangle.width = enfaceMouseArea.mouseX - startX;
                                selectionRectangle.height = enfaceMouseArea.mouseY - startY;
                            }

                            if(rectSelected){
                                selectionRectangle.bounds(Qt.point(enfaceMouseArea.mouseX, enfaceMouseArea.mouseY));
                            }
                        }

                        onReleased: {
                            dragStarted = false;
                            rectSelected = false;
                            octBackend.enFaceSelectionBoundsChanged(selectionRectangle.x,
                                                                    selectionRectangle.y,
                                                                    selectionRectangle.width,
                                                                    selectionRectangle.height,
                                                                    enfaceMouseArea.width,
                                                                    enfaceMouseArea.height);
                        }


                    }
                }

                Row {
                    anchors.bottom: enFaceRectangle.bottom;
                    anchors.horizontalCenter: enFaceRectangle.horizontalCenter;

                    Button {
                        id: reprocessOrGalvo
                        text: "Update En Face"

                        onClicked: {
                            octBackend.reprocessEnFace();
                        }
                    }

                    Button {
                        id: fullFieldButton
                        text: "Full Field"

                        onClicked: {
                            //Add zoom out feature
                            octBackend.onFullField();

                            if(octBackend.Mode == OCTBackend.MODE_REVIEW){
                                octBackend.reprocessEnFace();
                            }
                        }
                    }
                }
            }
        }
    }
}
