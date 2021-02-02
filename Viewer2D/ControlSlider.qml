import QtQuick 2.0
import QtQuick.Controls 2.12

Slider {
    id:mainSlider
    property string title: "slider"
    function isHorizontal() {
        return mainSlider.orientation == Qt.Horizontal
    }

    handle: Rectangle {
        property var hX : mainSlider.leftPadding + mainSlider.visualPosition * (mainSlider.availableWidth - width);
        property var hY : mainSlider.topPadding + mainSlider.availableHeight / 2 - height / 2;
        property var vX : ((mainSlider.availableWidth / 2) - (width / 2)) - 5
        property var vY : mainSlider.topPadding + mainSlider.visualPosition * (mainSlider.availableHeight - height)
        x: mainSlider.isHorizontal() ? hX : vX
        y: mainSlider.isHorizontal() ? hY : vY
        implicitWidth: 26
        implicitHeight: 26
        radius: 13
        color: mainSlider.pressed ? "#f0f0f0" : "#f6f6f6"
        border.color: "#bdbebf"
    }
    background: Rectangle {
        x: mainSlider.leftPadding
        y: mainSlider.topPadding + mainSlider.availableHeight / 2 - height / 2
        implicitWidth: mainSlider.isHorizontal() ? 200 :4
        implicitHeight: mainSlider.isHorizontal() ? 4 :200
        width: mainSlider.isHorizontal() ? mainSlider.availableWidth : implicitWidth
        height: mainSlider.isHorizontal() ? implicitHeight : mainSlider.availableHeight
        radius: 2
        color: "#bdbebf"

        Rectangle{
            id: valueContainer
            x: mainSlider.isHorizontal() ? mainSlider.width/2 - (width/2) : -(width)
            y: mainSlider.isHorizontal() ? -(mainSlider.height) : mainSlider.height/2 + (valueLabel.implicitHeight/2)
            width: valueLabel.implicitWidth + 10
            height: valueLabel.implicitHeight + 10
            color: "lightGray"
            radius: 5
            transform: Rotation { angle: mainSlider.isHorizontal() ? 0 : -90}
            Text {
                id: valueLabel
                text: mainSlider.value.toFixed(2)

                anchors.centerIn: parent
            }
        }

        Rectangle {
            width: mainSlider.isHorizontal() ? mainSlider.visualPosition * parent.width : parent.width
            height: mainSlider.isHorizontal() ? parent.height : mainSlider.visualPosition * parent.height
            color: "#21be2b"
            radius: 2
        }
        Rectangle {
            id: titleContainer
            x: mainSlider.isHorizontal() ? mainSlider.width/2 - (width/2) : width/4
            y: mainSlider.isHorizontal() ? 15 : mainSlider.height/2 + (titleLabel.implicitHeight/2)
            width: titleLabel.implicitWidth + 10
            height: titleLabel.implicitHeight + 10
            color: "lightGray"
            radius: 5
             transform: Rotation { angle: mainSlider.isHorizontal() ? 0 : -90}
            Text {
                id: titleLabel
                text: qsTr(title)

                anchors.centerIn: parent
            }
        }


    }
    /*handle: Rectangle {
        x: mainSlider.leftPadding + mainSlider.visualPosition * (mainSlider.availableWidth - width)
        y: mainSlider.topPadding + mainSlider.availableHeight / 2 - height / 2
        implicitWidth: 26
        implicitHeight: 26
        radius: 13
        color: mainSlider.pressed ? "#f0f0f0" : "#f6f6f6"
        border.color: "#bdbebf"
    }
    background: Rectangle {
        x: mainSlider.leftPadding
        y: mainSlider.topPadding + mainSlider.availableHeight / 2 - height / 2
        implicitWidth: 200
        implicitHeight: 4
        width: mainSlider.availableWidth
        height: implicitHeight
        radius: 2
        color: "#bdbebf"

        Rectangle {
            width: mainSlider.visualPosition * parent.width
            height: parent.height
            color: "#21be2b"
            radius: 2
        }
        Text {

            text: qsTr(title)
            x: mainSlider.width/2 - (width/2)
            y: 15
        }
    }*/
}
