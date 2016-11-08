import QtQuick 2.1
import QtQuick.Controls 1.0
import QtQuick.Layouts 1.0

Item {
    id: editbox

    property alias label: editLabel.text
    property alias text: editField.text

    implicitWidth: editLabel.implicitWidth + editField.implicitWidth
    implicitHeight: editField.implicitHeight

    RowLayout {
        Text {
            id: editLabel
            Layout.alignment: Qt.AlignLeft | Qt.AlignVCenter
            text: "Label:"
        }

        TextField {
            id: editField
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
            placeholderText: "not initialized"
        }
    }
}
