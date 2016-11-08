import QtQuick 2.3
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtDataVisualization 1.2

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: qsTr("Radar Object Visualizer")

    menuBar: MenuBar {
        Menu {
            title: qsTr("File")
            MenuItem {
                text: qsTr("Load settings")
                onTriggered: settingsFileDialog.open();
            }
            MenuItem {
                text: qsTr("Load signal")
                onTriggered: signalsFileDialog.open();
            }
            MenuItem {
                text: qsTr("Exit")
                onTriggered: Qt.quit();
            }
        }
    }

    FileDialog {
            id: settingsFileDialog
            objectName: "settingsFileDialog"
            modality: Qt.WindowModal
            nameFilters: ["Settings File (*.settings)"]

            onRejected:
            {
                console.log("Canceled")
            }
            onAccepted:
            {
                console.log("Settings file selected: " + fileUrl);
            }
    }

    FileDialog {
            id: signalsFileDialog
            objectName: "signalsFileDialog"
            modality: Qt.WindowModal
            nameFilters: ["Signals File (*.signals)"]

            onRejected:
            {
                console.log("Canceled")
            }
            onAccepted:
            {
                console.log("Signals file selected: " + fileUrl);
            }
    }

    Rectangle {
        width: parent.width
        height: settingsLayout.height
        anchors.left: parent.left
        anchors.top: parent.top

        GridLayout {
            columns: 3
            id: settingsLayout
            columnSpacing: 20

            Column {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 5

                EditBox {
                    id: rangeCellCountInput
                    label: "Range cell count:"
                    text: "121"
                    anchors.right: parent.right
                }

                EditBox {
                    id: signalsCountInput
                    label: "Signals count:"
                    text: "512"
                    anchors.right: parent.right
                }
            }

            Column {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 5

                EditBox {
                    id: distanceStepInput
                    label: "Distance step:"
                    text: "1.0"
                    anchors.right: parent.right
                }

                EditBox {
                    id: distanceOffsetInput
                    label: "Distance offset:"
                    text: "0.0"
                    anchors.right: parent.right
                }
            }

            Column {
                Layout.fillWidth: true
                Layout.fillHeight: true
                GroupBox {
                    id: signalBox
                    enabled: true
                    checked: false
                    clip: false
                    flat: false
                    visible: true
                    title: qsTr("Signal")
                    z: 0
                }

                Rectangle {
                    id: signalsStatusRect
                    objectName: "signalsStatusRect"
                    color: "#9e9e9e"
                    border.width: 0
                }

                Text {
                    id: signalsStatusText
                    objectName: "signalsStatusText"
                    text: qsTr("Status is not defined")
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    z: 1
                }

                Button {
                    id: analyzeBtn
                    objectName: "analyzeBtn"
                    width: 100
                    height: 100
                    text: qsTr("Analyze")
                    isDefault: true
                    transformOrigin: Item.Center
                }
            }
        }
    }
}
