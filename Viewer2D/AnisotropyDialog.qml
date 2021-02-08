import QtQuick 2.0
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.2

Dialog {
    id: dlgRoot
    title: "New Anisotropy"

    property url parlBgFile : ""
    property url perpBgFile : ""
    property url parlFile : ""
    property url perpFile : ""
    property double subtractValue: 0

    standardButtons: Dialog.Ok | Dialog.Cancel

    GridLayout{
        id: gridLayout
        anchors.centerIn: parent
        rows: 6
        columns: 6

        Label {
            text: "Anisotropy"
            Layout.columnSpan: 6
            font.pointSize: 20
        }

        Label {
            id: parlBgTxt
            text: "Parallel Background"
            Layout.columnSpan: 3
        }
        ComboBox {
            id:parlBgButton
            model: Manager;
            textRole: "bioImgName"
            Layout.columnSpan: 3
            background: Rectangle {
                implicitWidth: 120
                implicitHeight: 40
                radius: 2
            }
            onCurrentIndexChanged: {
               dlgRoot.parlBgFile=    Manager.imagePathAt(currentIndex);
            }
        }

        Label {
            id: perpBgTxt
            text: "Perpendicular Background "
            Layout.columnSpan: 3
        }

        ComboBox {
            id:perpBgButton
            model: Manager;
            textRole: "bioImgName"
            Layout.columnSpan: 3
            background: Rectangle {
                implicitWidth: 120
                implicitHeight: 40
                radius: 2
            }

            onCurrentIndexChanged: {
               dlgRoot.perpBgFile =  Manager.imagePathAt(currentIndex);
            }
        }

        Label {
            id: parlTxt
            text: "Parallel"
            Layout.columnSpan: 3
        }

        ComboBox {
            id:parlButton
            model: Manager;
            textRole: "bioImgName"
            Layout.columnSpan: 3
            background: Rectangle {
                implicitWidth: 120
                implicitHeight: 40
                radius: 2
            }
            onCurrentIndexChanged: {
               dlgRoot.parlFile =  Manager.imagePathAt(currentIndex);
            }
        }

        Label {
            id: perpTxt
            text: "Perpendicular "
            Layout.columnSpan: 3
        }
        ComboBox {
            id:perpButton
            model: Manager;
            textRole: "bioImgName"
            Layout.columnSpan: 3
            background: Rectangle {
                implicitWidth: 120
                implicitHeight: 40
                radius: 2
            }
            onCurrentIndexChanged: {
               dlgRoot.perpFile =  Manager.imagePathAt(currentIndex);
            }
        }
        TextField {
            id: subtractTxt
            placeholderText: "Subtract Value"
            Layout.columnSpan: 6
            text: subtractValue
            validator: DoubleValidator{bottom: 0; top: Math.pow(2, 32) -1;}
        }

    }

}
