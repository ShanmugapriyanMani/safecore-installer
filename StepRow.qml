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
    property color accent: Theme.accent

    Layout.fillWidth: true
    implicitHeight: 84
    radius: Theme.cardRadius

    color: active ? Theme.bgLog : "#0B132A"
    border.width: active ? 2 : 1
    border.color: active ? accent : Theme.borderPrimary

    RowLayout {
        anchors.fill: parent
        anchors.margins: 14
        spacing: 12

        Rectangle {
            width: 46
            height: 46
            radius: 14
            color: done ? Theme.stepDone : (active ? s.accent : Theme.buttonDisabled)
            border.color: "#00000000"

            Text {
                anchors.centerIn: parent
                text: done ? "\u2713" : s.indexText
                color: done ? Theme.textSuccessDark : (active ? "#08101D" : Theme.textSecondary)
                font.pixelSize: Theme.fontSizeTitle
                font.bold: true
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            spacing: 4

            Text {
                text: s.title
                color: Theme.textPrimary
                font.pixelSize: 15
                font.bold: true
                elide: Text.ElideRight
                maximumLineCount: 1
            }

            Text {
                text: s.subtitle
                color: Theme.textSecondary
                font.pixelSize: Theme.fontSizeNormal
                elide: Text.ElideRight
                maximumLineCount: 1
            }
        }
    }
}
