import QtQuick 2.14
import QtQuick.Window 2.14
import QtQuick.Controls 1.2
import QtQuick.Controls 2.12
import QtQuick.Controls 2.14
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.3
import BioImages 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.0
import QtQuick.Controls 1.4

Rectangle {
    id: appRoot
    visible: true
    width: 1200
    height: 800
    //    color: "red"
    property int imageSrc: -1
    ToolBar {
        id: toolBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        height: 50

        RowLayout {
            anchors.verticalCenter: parent.verticalCenter
            height: 55

            ToolButton {
                implicitHeight: parent.height
                text: "Open"
                iconSource: "qrc:/icons/open.png"
                onClicked: {
                    openDlg.open()
                }
            }

        }
    }

    Rectangle{
        y: (parent.height / 2) - (height/2)
        x: 0
        id: imageScroller
        width: 100
        height: parent.height * 0.8
        color: "lightGray"
        ListView  {
            anchors.fill: parent
            anchors.margins: 2
            clip: true
            spacing: 2
            model: Manager
            delegate: Rectangle {
                color: "gray"
                width: imageScroller.width
                height: 100
                Image {
                    id: img
                    source: `image://Bi/${index}/0/nan/0/nan/nan/Grays`
                    width: parent.width
                    height: 85
                    sourceSize.width: parent.width
                    sourceSize.height: 85
                    anchors.top: parent.top
                }
                Text {
                    id: title
                    text: bioImgName
                    anchors.top: img.bottom
                    color: "white"
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        appRoot.imageSrc = index
                    }
                }
            }

        }
    }
    Rectangle {
        id: leftRect
        color: "transparent"

        anchors.top:toolBar.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: imageScroller.right


        ImageScrollArea {
            id: image
            anchors.fill: parent
            zoomController.visible: true
            zoomScale: 1
            imageSrc: appRoot.imageSrc
            controlAlignment: Qt.AlignCenter
        }

    }

    FileDialog {
        id: openDlg
        title: "Input Dialog box"
        selectExisting: true
        selectMultiple: false
        selectFolder: false
        onAccepted: {
            var currentImageId = Manager.addImage(fileUrl);
            console.log("Current Image id ", currentImageId)
            appRoot.imageSrc = currentImageId;
        }
    }
}

