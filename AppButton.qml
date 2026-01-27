import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: btn
    property string text: ""
    property string iconText: ""
    property color accent: "#6EE7FF"
    property bool loading: false
    property string loadingText: "Loading..."
    property bool hovered: false
    property int buttonRadius: 18

    signal clicked()

    radius: buttonRadius
    implicitHeight: 38
    implicitWidth: Math.max(row.implicitWidth + 32, 110)

    color: enabled
        ? (hovered ? Qt.lighter(accent, 1.08) : accent)
        : "#1B2440"
    opacity: enabled ? 1.0 : 0.55

    border.color: enabled
        ? (hovered ? Qt.darker(accent, 1.15) : Qt.darker(accent, 1.3))
        : "#2A3557"
    border.width: 1

    MouseArea {
        anchors.fill: parent
        enabled: btn.enabled && !btn.loading
        hoverEnabled: true
        onClicked: btn.clicked()
        onPressed: btn.scale = 0.98
        onReleased: btn.scale = 1.0
        onCanceled: btn.scale = 1.0
        onEntered: btn.hovered = true
        onExited: btn.hovered = false
        cursorShape: Qt.PointingHandCursor
    }

    Behavior on scale { NumberAnimation { duration: 90 } }
    Behavior on color { ColorAnimation { duration: 120 } }
    Behavior on border.color { ColorAnimation { duration: 120 } }

    RowLayout {
        id: row
        anchors.centerIn: parent
        spacing: 8

        Item {
            id: colorSpinner
            visible: btn.loading
            width: 20
            height: 20
            property var colors: ["#d0f7ffff", "#98eeffff", "#6EE7FF", "#32dbfdff", "#00d3fdff"]

            Repeater {
                model: 8
                delegate: Rectangle {
                    width: 3
                    height: 3
                    radius: width / 2
                    color: colorSpinner.colors[index % colorSpinner.colors.length]
                    x: (colorSpinner.width / 2) + (Math.cos((index / 8) * 2 * Math.PI) * 6) - (width / 2)
                    y: (colorSpinner.height / 2) + (Math.sin((index / 8) * 2 * Math.PI) * 6) - (height / 2)
                    opacity: 0.6 + (index / 8) * 0.4
                }
            }

            RotationAnimator on rotation {
                running: colorSpinner.visible
                from: 0
                to: 360
                duration: 900
                loops: Animation.Infinite
            }
        }

        Text {
            visible: btn.iconText.length > 0
            text: btn.iconText
            color: "#08101D"
            font.pixelSize: 14
            font.bold: true
        }

        Text {
            text: btn.loading ? btn.loadingText : btn.text
            color: "#08101D"
            font.pixelSize: 13
            font.bold: true
        }
    }
}
