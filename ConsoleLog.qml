import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: consoleLog

    property string title: "Console Log"
    property string iconText: "⚙"
    property string logText: ""
    property string placeholderText: "Output will appear here..."
    property bool isActive: false
    property bool autoScroll: true

    radius: 12
    color: Theme.bgLog
    border.color: Theme.borderDrawer
    border.width: 1
    clip: true

    // Subtle gradient overlay
    Rectangle {
        anchors.fill: parent
        radius: parent.radius
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#0A101800" }
            GradientStop { position: 1.0; color: "#0A101820" }
        }
    }

    // Header bar
    Rectangle {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 32
        color: Theme.bgLogHeader
        radius: 12

        Rectangle {
            anchors.bottom: parent.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.radius
            color: parent.color
        }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 12
            anchors.rightMargin: 12
            spacing: 8

            Text {
                text: consoleLog.iconText
                color: Theme.accent
                font.pixelSize: Theme.fontSizeLarge
            }

            Text {
                text: consoleLog.title
                color: Theme.textInfo
                font.pixelSize: 11
                font.weight: Font.Medium
            }

            Item { Layout.fillWidth: true }

            Rectangle {
                width: 6
                height: 6
                radius: 3
                color: Theme.accentSuccess
                opacity: 0.8
                visible: consoleLog.isActive

                SequentialAnimation on opacity {
                    running: consoleLog.isActive
                    loops: Animation.Infinite
                    NumberAnimation { from: 0.3; to: 1.0; duration: 600 }
                    NumberAnimation { from: 1.0; to: 0.3; duration: 600 }
                }
            }

            Text {
                text: consoleLog.isActive ? "ACTIVE" : "IDLE"
                color: consoleLog.isActive ? Theme.accentSuccess : Theme.textMuted
                font.pixelSize: Theme.fontSizeSmall
                font.weight: Font.Bold
                font.letterSpacing: 0.5
            }
        }
    }

    Flickable {
        id: flickable
        anchors.fill: parent
        anchors.topMargin: 42
        anchors.leftMargin: 12
        anchors.rightMargin: 12
        anchors.bottomMargin: 12
        clip: true
        contentWidth: logTextArea.implicitWidth
        contentHeight: logTextArea.implicitHeight
        boundsBehavior: Flickable.StopAtBounds
        ScrollBar.horizontal: ScrollBar { policy: ScrollBar.AsNeeded }
        ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

        onContentHeightChanged: {
            if (consoleLog.autoScroll) {
                contentY = Math.max(0, contentHeight - height);
            }
        }

        onContentYChanged: {
            var maxY = Math.max(0, contentHeight - height);
            consoleLog.autoScroll = (contentY >= maxY - 10);
        }

        TextArea {
            id: logTextArea
            text: consoleLog.logText.length ? consoleLog.logText : consoleLog.placeholderText
            readOnly: true
            textFormat: TextEdit.PlainText
            wrapMode: TextEdit.NoWrap
            font.family: "JetBrains Mono, Consolas, Monaco, Monospace"
            font.pixelSize: Theme.fontSizeNormal
            color: consoleLog.logText.length ? Theme.textSecondary : Theme.textMuted
            background: null
            width: implicitWidth
            height: implicitHeight
            bottomPadding: 10
            rightPadding: 10
        }
    }

    function scrollToBottom() {
        flickable.contentY = Math.max(0, flickable.contentHeight - flickable.height);
    }
}
