import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Window {
    id: opsRoot
    width: 1100
    height: 680
    visible: true
    title: "SafeCore"
    color: "#0B1220"

    property color accent: "#6EE7FF"
    property bool allowClose: false
    property bool showContainerLogs: false

    Component.onCompleted: {
        x = (Screen.width - width) / 2
        y = (Screen.height - height) / 2
        if (AppController.dockerOpsAutoRun) {
            Qt.callLater(function() { AppController.runDockerOps(); })
        }
    }
    onClosing: function(close) {
        if (allowClose)
            return;
        close.accepted = false;
        quitDialog.open();
    }
    Connections {
        target: AppController
        function onDockerOpsRunningChanged() {
            if (!AppController.dockerOpsRunning && opsRoot.showContainerLogs) {
                opsRoot.showContainerLogs = false
                AppController.stopDockerOpsLogs()
            }
        }
    }

    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: "#0B1220" }
            GradientStop { position: 0.55; color: "#0F1B33" }
            GradientStop { position: 1.0; color: "#0B1220" }
        }
    }

    Drawer {
        id: sideDrawer
        edge: Qt.LeftEdge
        modal: false
        dim: false
        interactive: true
        width: 220
        height: parent.height
        background: Rectangle {
            color: "#0E1730"
            border.color: "#1F2A4A"
            border.width: 1
        }
        contentItem: ColumnLayout {
            anchors.fill: parent
            anchors.margins: 16
            spacing: 12
            Text {
                text: "SafeCore"
                color: "#E2E8F0"
                font.pixelSize: 16
                font.bold: true
            }
            Rectangle { Layout.fillWidth: true; height: 1; color: "#1F2A4A" }
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8
                Repeater {
                    model: [
                        { label: "Upgrade", icon: "qrc:/images/drawer/upgrade_icon.png" },
                        { label: "Reset", icon: "qrc:/images/drawer/reset_icon.png" },
                        { label: "Repair", icon: "qrc:/images/drawer/repair_icon.png" }
                    ]
                    delegate: ColumnLayout {
                        Layout.fillWidth: true
                        spacing: 8
                        Item {
                            Layout.fillWidth: true
                            height: 40
                            Rectangle {
                                radius: 8
                                anchors.fill: parent
                                color: hoverArea.containsMouse ? "#0A1022" : "transparent"
                            }
                            RowLayout {
                                anchors.fill: parent
                                anchors.margins: 6
                                spacing: 8
                                Image {
                                    width: 14
                                    height: 14
                                    source: modelData.icon
                                    fillMode: Image.PreserveAspectFit
                                    smooth: true
                                    sourceSize.width: 14
                                    sourceSize.height: 14
                                }
                                Text {
                                    text: modelData.label
                                    color: "#E2E8F0"
                                    font.pixelSize: 15
                                }
                                Item { Layout.fillWidth: true }
                            }
                            MouseArea {
                                id: hoverArea
                                anchors.fill: parent
                                hoverEnabled: true
                            }
                        }
                        Rectangle { Layout.fillWidth: true; height: 1; color: "#1F2A4A" }
                    }
                }
            }
            Item { Layout.fillHeight: true }
            Rectangle { Layout.fillWidth: true; height: 1; color: "#1F2A4A" }
            Text {
                text: "safecore_installer_v_0.1"
                color: "#64748B"
                font.pixelSize: 11
                horizontalAlignment: Text.AlignHCenter
                Layout.fillWidth: true
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 28
        spacing: 18

        RowLayout {
            Layout.fillWidth: true
            spacing: 10
            Rectangle {
                id: drawerButton
                width: 32
                height: 32
                radius: 8
                color: "#151B2A"
                border.color: "#1F2A4A"
                border.width: 1
                Layout.leftMargin: -20
                Layout.topMargin: -10
                Image {
                    anchors.centerIn: parent
                    source: "qrc:/images/drawer/drawer_icon.png"
                    width: 18
                    height: 18
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                }
                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        if (sideDrawer.opened)
                            sideDrawer.close()
                        else
                            sideDrawer.open()
                    }
                    onEntered: drawerButton.color = "#1B2440"
                    onExited: drawerButton.color = "#151B2A"
                }
            }
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4
                Text {
                    text: "SafeCore"
                    color: "#E2E8F0"
                    font.pixelSize: 22
                    font.bold: true
                }
                Text {
                    text: "Docker Operations Console"
                    color: "#93C5FD"
                    font.pixelSize: 13
                }
            }
            Item { Layout.fillWidth: true }
            Image {
                source: "qrc:/images/logo/SafeSpace_Global.png"
                fillMode: Image.PreserveAspectFit
                sourceSize.height: 42
                sourceSize.width: 140
                Layout.preferredHeight: 42
                Layout.preferredWidth: 140
                smooth: true
            }
        }

        AppCard {
            Layout.fillWidth: true
            title: "Safecore Manage Program"
            subtitle: "Control the SafeCore container lifecycle on this machine."
            contentItem: ColumnLayout {
                spacing: 10
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 12
                    Text {
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        color: "#93C5FD"
                        font.pixelSize: 13
                        text: "Start the SafeCore container with the latest settings pulled from your tenant configuration."
                    }
                    AppButton {
                        text: opsRoot.showContainerLogs ? "Hide Logs" : "Show Logs"
                        accent: opsRoot.accent
                        enabled: AppController.dockerOpsRunning && !AppController.dockerOpsStopping && !AppController.dockerOpsStarting
                        onClicked: {
                            if (opsRoot.showContainerLogs) {
                                opsRoot.showContainerLogs = false
                            } else {
                                opsRoot.showContainerLogs = true
                                AppController.startDockerOpsLogs()
                            }
                        }
                    }
                }
                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: "#1F2A4A"
                }
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: (AppController.dockerOpsRunning || AppController.dockerOpsLog.length > 0) ? 320 : 0
                    radius: 10
                    color: "#0A1022"
                    border.color: "#1F2A4A"
                    border.width: 1
                    visible: !opsRoot.showContainerLogs && (AppController.dockerOpsRunning || AppController.dockerOpsLog.length > 0)
                    Flickable {
                        id: opsLogFlickable
                        anchors.fill: parent
                        anchors.margins: 10
                        clip: true
                        contentWidth: Math.max(width, opsLog.contentWidth)
                        contentHeight: opsLog.contentHeight
                        boundsBehavior: Flickable.StopAtBounds
                        ScrollBar.horizontal: ScrollBar { policy: ScrollBar.AsNeeded }
                        ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
                        TextArea {
                            id: opsLog
                            text: AppController.dockerOpsLog.length
                                ? AppController.dockerOpsLog
                                : "Docker output will appear here..."
                            readOnly: true
                            textFormat: TextEdit.PlainText
                            wrapMode: TextEdit.NoWrap
                            font.family: "Monospace"
                            font.pixelSize: 12
                            color: "#C7D2FE"
                            background: null
                            width: opsLogFlickable.contentWidth
                        }
                    }
                }
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: opsRoot.showContainerLogs ? 320 : 0
                    radius: 10
                    color: "#0A1022"
                    border.color: "#1F2A4A"
                    border.width: 1
                    visible: opsRoot.showContainerLogs
                    Flickable {
                        id: opsLogFollowFlickable
                        anchors.fill: parent
                        anchors.margins: 10
                        clip: true
                        contentWidth: Math.max(width, opsLogFollow.contentWidth)
                        contentHeight: opsLogFollow.contentHeight
                        boundsBehavior: Flickable.StopAtBounds
                        ScrollBar.horizontal: ScrollBar { policy: ScrollBar.AsNeeded }
                        ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
                        TextArea {
                            id: opsLogFollow
                            text: AppController.dockerOpsFollowLog.length
                                ? AppController.dockerOpsFollowLog
                                : "Docker output will appear here..."
                            readOnly: true
                            textFormat: TextEdit.PlainText
                            wrapMode: TextEdit.NoWrap
                            font.family: "Monospace"
                            font.pixelSize: 12
                            color: "#C7D2FE"
                            background: null
                            width: opsLogFollowFlickable.contentWidth
                        }
                    }
                }
                Item { height: 3}
            }
        }
        Item { Layout.fillHeight: true }

        Rectangle {
            Layout.fillWidth: true
            height: 1
            color: "#1F2A4A"
        }
        RowLayout {
            Layout.fillWidth: true
            spacing: 12
            Item { Layout.fillWidth: true }
            AppButton {
                text: "Run All"
                loading: AppController.dockerOpsStarting
                loadingText: "Starting..."
                accent: opsRoot.accent
                enabled: !AppController.dockerOpsRunning && !AppController.dockerOpsStarting && !AppController.dockerOpsStopping
                onClicked: AppController.runDockerOps()
            }
            AppButton {
                text: "Stop All"
                loading: AppController.dockerOpsStopping
                loadingText: "Stopping..."
                accent: "#F87171"
                enabled: AppController.dockerOpsRunning && !AppController.dockerOpsStarting && !AppController.dockerOpsStopping
                onClicked: AppController.stopDockerOps()
            }
        }
    }

    Dialog {
        id: quitDialog
        modal: true
        parent: opsRoot.contentItem
        closePolicy: Popup.NoAutoClose
        title: "Quit SafeCore"
        anchors.centerIn: parent
        implicitWidth: 380
        implicitHeight: header.height + contentItem.implicitHeight + 24
        header: Rectangle {
            height: 44
            color: "#151B2A"
            border.color: "#1F2A4A"
            border.width: 1
            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 18
                text: quitDialog.title
                color: "#E2E8F0"
                font.pixelSize: 16
                font.bold: true
            }
        }
        background: Rectangle {
            radius: 14
            color: "#151B2A"
            border.color: "#1F2A4A"
            border.width: 1
        }

        property string messageText: "Do you want to quit SafeCore? The container will keep running."
        contentItem: Item {
            implicitHeight: bodyLayout.implicitHeight
            implicitWidth: bodyLayout.implicitWidth
            ColumnLayout {
                id: bodyLayout
                anchors.fill: parent
                anchors.margins: 18
                spacing: 12

                Text {
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    color: "#B8C2E0"
                    text: quitDialog.messageText
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Item { Layout.fillWidth: true }
                    AppButton {
                        text: "No"
                        enabled: true
                        accent: "#374151"
                        onClicked: quitDialog.close()
                    }
                    AppButton {
                        text: "Yes"
                        enabled: true
                        accent: opsRoot.accent
                        onClicked: {
                            quitDialog.close()
                            opsRoot.allowClose = true
                            Qt.quit()
                        }
                    }
                }
                Item { height: 4 }
            }
        }
    }
}
