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

ApplicationWindow {
    id: appRoot
    visible: true
    width: 1200
    height: 800
//    state: "split"

    //    color: "red"
//    property int imageSrc: -1
    property var currentViewer: image
    toolBar: ToolBar {
        id: toolBar
        Row {
            ToolButton {
                text: "Open"
                iconSource: "qrc:/icons/open.png"
                onClicked: {
                    openDlg.open()
                }
            }
            ToolButton {
                text: "Split"
                iconSource: "qrc:/icons/toggle.png"
                onClicked: {
                    if (text === "Split") {
                        text = "Single"
                        centerItem.state = "split"
                    } else {
                        text = "Split"
                        centerItem.state = "normal"
                    }
                }
            }

        }

    }

    Rectangle {
    id: centerItem
    anchors.fill: parent
    state: "normal"

    Rectangle {
        id: mainRect
        color: "transparent"
        visible: centerItem.state === "normal"
        anchors.fill: parent
        ImageScrollArea {
            id: image
            anchors.fill: parent
            zoomController.visible: true
            zoomScale: 1
            imageSrc: -1
            controlAlignment: Qt.AlignCenter
        }

        onVisibleChanged: {
            if(visible) {
                appRoot.currentViewer = image
            } else {
                appRoot.currentViewer = imageL
            }
        }

    }

    SplitView {
        visible: centerItem.state === "split"
        anchors.fill: parent

        Rectangle {
            id: leftRect
            color: "transparent"
            width: parent.width/2
            Layout.minimumWidth: parent.width/4
            height: parent.height
            ImageScrollArea {
                id: imageL
                anchors.fill: parent
                zoomController.visible: true
                zoomScale: 1
                imageSrc: -1
                controlAlignment: Qt.AlignCenter
            }

            onActiveFocusChanged: {
                appRoot.currentViewer = imageL
            }
        }
        Rectangle {
            id: rightRect
            color: "transparent"
            width: parent.width/2
            Layout.minimumWidth: parent.width/4
            height: parent.height
            ImageScrollArea {
                id: imageR
                anchors.fill: parent
                zoomController.visible: true
                zoomScale: 1
                imageSrc: -1
                controlAlignment: Qt.AlignCenter
            }

            onActiveFocusChanged: {
                appRoot.currentViewer = imageR
            }

        }
    }

    onStateChanged: {
        if(state == "split") {
            appRoot.currentViewer = imageL
        } else {
            appRoot.currentViewer = image
        }
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
            appRoot.currentViewer.imageSrc = currentImageId;
        }
    }


}

