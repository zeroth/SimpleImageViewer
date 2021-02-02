import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

Item {
    id: rootImageScrollArea

    property alias zoomController: zoomSlider
    property string imageSrc: ""
    property real zoomScale: 1.0
    property var controlAlignment: Qt.AlignLeft
    property var stackIndex: 0


    clip: true
    Rectangle {
        id: imageContainer
        Image {
            id: image
            //            source: imageSrc
        }

        width: image.width
        height: image.height
        color: "transparent"
        smooth: false
        antialiasing: false
        MouseArea {
            anchors.fill: parent
            drag.target: parent
            hoverEnabled: true
            onPositionChanged: {
                let x = Math.round(mouse.x);
                let y = Math.round(mouse.y);
                let val = Manager.intensity(imageSrc, x, y, stackSlider.value);
                //                let posInfo = ` X: ${x}, Y: ${y},  Value: ${val}`;
                localtionInfo.xPos = x;
                localtionInfo.yPos = y;
                localtionInfo.zPos = stackSlider.value;
                localtionInfo.intensity = val;

            }
            onWheel: {
                if (wheel.modifiers & Qt.ControlModifier) {
                    if(wheel.angleDelta.y > 0)
                        zoomSlider.increase();
                    else
                        zoomSlider.decrease();
                } else {
                    if(wheel.angleDelta.y > 0)
                        stackSlider.increase();
                    else
                        stackSlider.decrease();
                }
            }
        }

        scale: zoomScale

        Component.onCompleted: updateImageLocation()
        function screenshot(filename) {
            console.log(filename);
            imageContainer.grabToImage(function(result) {
                console.log("File name ", filename)
                var check = result.saveToFile(filename);
                console.log("File saved ", check)
            }, Qt.size(imageContainer.width*rootImageScrollArea.zoomScale,
                       imageContainer.height*rootImageScrollArea.zoomScale));
        }
    }

    FileDialog {
        id: scrSaveDlg
        title: "Save Screenshot"
        selectExisting: false

        onAccepted: function save(){
            console.log(Manager.toLocalPath(fileUrl))
            imageContainer.screenshot(Manager.toLocalPath(fileUrl))
        }
    }

    onWidthChanged: {
        updateImageLocation()
    }
    onHeightChanged: {
        updateImageLocation()
    }

    onImageSrcChanged: {
        thresholdSlider.from = Manager.imgMin(imageSrc)
        thresholdSlider.first.value = Manager.imgMin(imageSrc)
        thresholdSlider.to = Manager.imgMax(imageSrc)
        thresholdSlider.second.value = Manager.imgMax(imageSrc)

        let imageLength = Manager.imgLength(imageSrc)
        stackSlider.visible =  imageLength > 1
        stackSlider.from = 0
        stackSlider.to = imageLength-1
        stackSlider.value = 0

        updateImage();
        updateImageLocation();
    }

    function updateImage() {
        var imageUrl = `image://Bi/img/${imageSrc}/cmap/${colorMapCmb.currentText}/min/${thresholdSlider.first.value}/max/${thresholdSlider.second.value}/t/1/s/${stackSlider.value}`;
        image.source = ""
        image.source = imageUrl;
    }

    function updateImageLocation() {
        imageContainer.x =  (rootImageScrollArea.width/2) - (image.width / 2)
        imageContainer.y = (rootImageScrollArea.height/2) - (image.height / 2)
    }


    ColumnLayout {
        x: parent.width - 50
        y: parent.height/2 - parent.height/4

        ControlSlider {
            id: stackSlider
            visible: false
            stepSize: 1
            from: 0
            value: 0
            to: 100
            title: "Stack Slider"
            orientation: Qt.Vertical
            onMoved: updateImage()/* {
                stackIndex = value
            }*/
            onValueChanged: updateImage()/*{
                stackIndex = value
            }*/

        }

    }

    RowLayout {
        x: controlAlignment === Qt.AlignLeft ? 50 : (parent.width/2  - childrenRect.width/2)
        y: parent.height - 60
        spacing: 10
        opacity: 1

        Rectangle {
            id:localtionInfo
            implicitWidth: localtionInfoTxt.implicitWidth + 10
            implicitHeight: localtionInfoTxt.implicitHeight + 10
            radius: 5

            property real xPos: 0
            property real yPos: 0
            property real zPos: 0
            property real intensity: 0
            //            color: "transparent"
            color: "lightGray"
            Text {
                id:localtionInfoTxt
                anchors.centerIn: parent
                text: `X : ${localtionInfo.xPos}, Y: ${localtionInfo.yPos}, Z: ${localtionInfo.zPos}\nValue: ${localtionInfo.intensity.toFixed(3)}`

            }
        }

        ControlSlider {
            id: zoomSlider
            from: 0.7
            value: 1.0
            to: 15.0

            title:"Zoom"
            onMoved: {
                zoomScale = value
            }
            onValueChanged: {
                zoomScale = value
            }
        }

        ThresholdSlider {
            id: thresholdSlider
            from:0
            to: 255

            first.value: 0
            first.onMoved: updateImage();

            second.value: 255
            second.onMoved: updateImage();

        }

        ComboBox {
            id: colorMapCmb
            model: Manager.availableColorMaps();
            onActivated: updateImage()

            background: Rectangle {
                implicitWidth: 120
                implicitHeight: 40
                radius: 2
            }

            function updateDefaultSelection() {
                colorMapCmb.currentIndex = colorMapCmb.find("Greys", Qt.MatchExactly)
            }

            Component.onCompleted:  updateDefaultSelection()
            onModelChanged: updateDefaultSelection()

        }

        Rectangle {
            color: "transparent"
            implicitWidth: 32
            implicitHeight: 32
            Image {
                anchors.fill: parent
                source: "qrc:/icons/zoom_reset.png"
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        zoomSlider.value = 1.0
                    }
                }
            }
        }

        Rectangle{
            color: "transparent"
            implicitWidth: 32
            implicitHeight: 32
            Image {
                source: "qrc:/icons/reset.png"
                anchors.fill: parent
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        rootImageScrollArea.updateImageLocation()
                    }
                }
            }
        }

        Rectangle{
            color: "transparent"
            implicitWidth: 32
            implicitHeight: 32
            Image {
                source: "qrc:/icons/screenshot.png"
                anchors.fill: parent
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        scrSaveDlg.open()
                    }
                }
            }
        }



        /*MouseArea {
            hoverEnabled: true
            anchors.fill: parent
            onEntered: {
                parent.opacity = 1
            }
            onExited: {
                parent.opacity = 0.3
            }
        }*/

    }

}
