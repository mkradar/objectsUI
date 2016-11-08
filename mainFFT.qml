import QtQuick 2.3
import QtQuick.Controls 1.1
import QtQuick.Dialogs 1.2
import QtQuick.Controls 1.2
import QtQuick.Layouts 1.0
import QtDataVisualization 1.2

// import Graph 1.0

ApplicationWindow {
    visible: true
    width: 1200
    height: 800
    title: qsTr("Radar Visualizer")

    Data {
            id: data
        }

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

    Text {
        id: rangeCountLabel
        x: 63
        y: 28
        text: qsTr("Range count:")
        font.pixelSize: 12
    }

    TextField {
        id: rangeCountValue
        objectName: "rangeCountValue"
        x: 137
        y: 25
        width: 120
        placeholderText: "not initialized"
        readOnly: true
    }

    Text {
        id: modulationFrequencyLabel
        x: 17
        y: 54
        text: qsTr("Modulation frequency:")
        font.pixelSize: 12
    }

    TextField {
        id: modulationFrequencyValue
        x: 137
        y: 51
        width: 120
        z: 1
        objectName: "modulationFrequencyValue"
        readOnly: true
        placeholderText: qsTr("not initialized")
    }

    GroupBox {
        id: settingsBox
        x: 5
        y: 7
        width: 259
        height: 100
        clip: false
        visible: true
        enabled: true
        z: 0
        flat: false
        checked: false
        title: qsTr("Settings")
    }

    Text {
        id: frequencyDeviationLabel
        x: 17
        y: 82
        text: qsTr("Frequency deviation:")
        font.pixelSize: 12
    }

    TextField {
        id: frequencyDeviationValue
        x: 137
        y: 79
        width: 120
        objectName: "frequencyDeviationValue"
        z: 1
        readOnly: true
        placeholderText: qsTr("not initialized")
    }

    Text {
        id: signalsCountLabel
        x: 328
        y: 28
        text: "Signal count:"
        font.pixelSize: 12
    }

    TextField {
        id: signalsCountValue
        x: 402
        y: 25
        width: 120
        objectName: "signalsCountValue"
        readOnly: true
        placeholderText: "not initialized"
    }

    GroupBox {
        id: signalBox
        x: 270
        y: 7
        width: 258
        height: 100
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
        x: 276
        y: 65
        width: 246
        height: 34
        color: "#9e9e9e"
        border.width: 0
    }

    Text {
        id: signalsStatusText
        objectName: "signalsStatusText"
        x: 310
        y: 73
        width: 179
        height: 18
        text: qsTr("Status is not defined")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        z: 1
        font.pixelSize: 12
    }

    Button {
        id: analyzeBtn
        objectName: "analyzeBtn"
        x: 534
        y: 7
        width: 100
        height: 100
        text: qsTr("Analyze")
        isDefault: true
        transformOrigin: Item.Center
    }

    /* Item {
        width: 1184
        height: 677
        x: 5
        y: 113
        Graph {
                id: graph
                objectName: "graph"
                anchors.fill: parent
                anchors.margins: 30

                function graphAddSample(newValue)
                {
                    appendSample(newValue);
                }

                function newSample(i) {
                    return (Math.sin(i / 100.0 * Math.PI * 2) + 1) * 0.4 + Math.random() * 0.05;
                }

                Component.onCompleted: {
                    for (var i=0; i<100; ++i)
                        appendSample(newSample(i));
                }

                property int offset: 100;
            }

    } */

    GridLayout {
        id: gridLayout
        columns: 2
        Layout.fillHeight: true
        Layout.fillWidth: true
        anchors.top: parent.top
        anchors.topMargin: 113
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            border.color: surfaceGraph.theme.gridLineColor
            border.width: 2

            Surface3D {
                id: surfaceGraph
                anchors.fill: parent
                anchors.margins: parent.border.width
                theme: Theme3D {
                    type: Theme3D.ThemePrimaryColors
                    font.pointSize: 60
                }
                scene.activeCamera.cameraPreset: Camera3D.CameraPresetIsometricLeftHigh

                Surface3DSeries {
                    itemLabelFormat: "Pop density at (@xLabel N, @zLabel E): @yLabel"
                    ItemModelSurfaceDataProxy {
                        itemModel: data.sharedData
                        // The surface data points are not neatly lined up in rows and columns,
                        // so we define explicit row and column roles.
                        rowRole: "row"
                        columnRole: "col"
                        xPosRole: "latitude"
                        zPosRole: "longitude"
                        yPosRole: "pop_density"
                    }
                }
            }
        }

        // We'll use one grid cell for buttons
        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            border.color: scatterGraph.theme.gridLineColor
            border.width: 2

            Scatter3D {
                id: scatterGraph
                anchors.fill: parent
                anchors.margins: parent.border.width
                theme: Theme3D {
                    type: Theme3D.ThemeDigia
                    font.pointSize: 60
                }
                scene.activeCamera.cameraPreset: Camera3D.CameraPresetIsometricLeftHigh

                Scatter3DSeries {
                    itemLabelFormat: "Pop density at (@xLabel N, @zLabel E): @yLabel"
                    ItemModelScatterDataProxy {
                        itemModel: data.sharedData
                        // Mapping model roles to scatter series item coordinates.
                        xPosRole: "latitude"
                        zPosRole: "longitude"
                        yPosRole: "pop_density"
                    }
                }
            }
        }

        Rectangle {
            Layout.fillHeight: true
            Layout.fillWidth: true
            border.color: barGraph.theme.gridLineColor
            border.width: 2

            Bars3D {
                id: barGraph
                anchors.fill: parent
                anchors.margins: parent.border.width
                theme: Theme3D {
                    type: Theme3D.ThemeQt
                    font.pointSize: 60
                }
                selectionMode: AbstractGraph3D.SelectionItemAndRow | AbstractGraph3D.SelectionSlice
                scene.activeCamera.cameraPreset: Camera3D.CameraPresetIsometricLeftHigh

                Bar3DSeries {
                    itemLabelFormat: "@seriesName: @valueLabel"
                    name: "Population density"

                    ItemModelBarDataProxy {
                        itemModel: data.sharedData
                        // Mapping model roles to bar series rows, columns, and values.
                        rowRole: "row"
                        columnRole: "col"
                        valueRole: "pop_density"
                    }
                }
            }
        }
    }
}
