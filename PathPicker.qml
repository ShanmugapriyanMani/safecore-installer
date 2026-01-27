import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Item {
    id: p
    property string installPath: "/opt/ai_box"
    signal copyRequested()

    implicitHeight: col.implicitHeight

    ColumnLayout {
        id: col
        spacing: 10
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: true
            spacing: 10

            Rectangle {
                Layout.fillWidth: true
                implicitHeight: 38
                radius: 12
                color: "#071025"
                border.color: "#1F2A4A"
                border.width: 1
                clip: true

                Text {
                    anchors.fill: parent
                    anchors.margins: 12
                    text: p.installPath
                    color: "white"
                    font.pixelSize: 13
                    elide: Text.ElideMiddle
                    verticalAlignment: Text.AlignVCenter
                }
            }

            AppButton {
                text: "Copy"
                iconText: "CP"
                accent: "#6EE7FF"
                onClicked: p.copyRequested()
            }
        }

        Text {
            Layout.fillWidth: true
            text: "Artifacts will be placed under this folder \n(docker image / env setup / models)."
            color: "#B8C2E0"
            wrapMode: Text.WordWrap
            font.pixelSize: 10
        }
    }
}
