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
    property string imageSrc: ""
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

    Rectangle {
        id: leftRect
        color: "transparent"

        anchors.top:toolBar.bottom
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.left: parent.left


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
            appRoot.imageSrc = currentImageId;
        }
    }
}

