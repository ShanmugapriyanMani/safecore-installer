import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: s
    property string title: ""
    property string subtitle: ""
    property bool active: false
    property bool done: false
    property string indexText: "1"
    property color accent: "#6EE7FF"

    Layout.fillWidth: true
    implicitHeight: 84
    radius: 16

    color: active ? "#0A1022" : "#0B132A"
    border.width: active ? 2 : 1
    border.color: active ? accent : "#1F2A4A"

    RowLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 12

        Rectangle {
            width: 46
            height: 46
            radius: 14
            color: done ? "#A7F3D0" : (active ? s.accent : "#1B2440")
            border.color: "#00000000"

            Text {
                anchors.centerIn: parent
                text: done ? "\u2713" : s.indexText
                color: done ? "#052015" : (active ? "#08101D" : "#B8C2E0")
                font.pixelSize: 16
                font.bold: true
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            Text {
                text: s.title
                color: "white"
                font.pixelSize: 15
                font.bold: true
                elide: Text.ElideRight
                maximumLineCount: 1
            }

            Text {
                text: s.subtitle
                color: "#B8C2E0"
                font.pixelSize: 12
                elide: Text.ElideRight
                maximumLineCount: 1
            }
        }
    }
}
