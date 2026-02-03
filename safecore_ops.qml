import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Window {
    id: opsRoot
    width: 1100
    height: 680
    minimumWidth: 1100
    minimumHeight: 680
    visible: true
    title: "SafeCore"
    color: Theme.bgPrimary

    // Theme aliases for backward compatibility
    property color accent: Theme.accent
    property color accent2: Theme.accent2
    property bool allowClose: false
    property bool showContainerLogs: false
    property var activeDialog: null
    // Dynamic console height based on window size
    // Calculate as percentage of window height minus reserved space
    property int consoleHeight: {
        var reservedSpace = 50 + 100 + 150; // margins + header + buttons/spacing
        var availableHeight = opsRoot.height - reservedSpace;
        return Math.max(320, Math.floor(availableHeight * 0.85));
    }

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

        // If another dialog is already open, blink it instead of opening quit dialog
        if (opsRoot.activeDialog) {
            opsRoot.activeDialog.blinkDialog();
            return;
        }

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
        width: 240
        height: parent.height

        // Enhanced background with gradient
        background: Rectangle {
            radius: 16
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#0A0F1C" }
                GradientStop { position: 0.5; color: "#0E1730" }
                GradientStop { position: 1.0; color: "#0A0F1C" }
            }
            border.color: "#2D3B5F"
            border.width: 1

            // Subtle overlay
            Rectangle {
                anchors.fill: parent
                radius: 16
                color: "#050811"
                opacity: 0.3
            }
        }
        
        contentItem: ColumnLayout {
            anchors.fill: parent
            anchors.margins: 18
            spacing: 16

            // Header with logo/title
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 8

                Text {
                    text: "SafeCore"
                    color: "#FFFFFF"
                    font.pixelSize: 18
                    font.weight: Font.Bold
                    font.letterSpacing: 0.5
                }

                Text {
                    text: "Control Center"
                    color: "#6EE7FF"
                    font.pixelSize: 11
                    font.weight: Font.Medium
                    opacity: 0.8
                }
            }

            // Stylish separator
            Rectangle {
                Layout.fillWidth: true
                height: 2
                radius: 1
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 0.5; color: "#2D3B5F" }
                    GradientStop { position: 1.0; color: "transparent" }
                }
            }

            // Menu items
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 6

                Repeater {
                    model: [
                        { label: "Upgrade", icon: "qrc:/images/drawer/upgrade_icon.png", description: "Update container image" },
                        { label: "Repair", icon: "qrc:/images/drawer/repair_icon.png", description: "Restart container" }
                    ]

                    delegate: Item {
                        Layout.fillWidth: true
                        height: 56

                        Rectangle {
                            id: menuItemBg
                            anchors.fill: parent
                            radius: 10
                            color: hoverArea.containsMouse ? "#050811" : "transparent"
                            border.color: hoverArea.containsMouse ? "#2D3B5F" : "transparent"
                            border.width: 1

                            Behavior on color { ColorAnimation { duration: 200 } }
                            Behavior on border.color { ColorAnimation { duration: 200 } }

                            // Glow effect on hover
                            Rectangle {
                                anchors.fill: parent
                                radius: parent.radius
                                color: "transparent"
                                border.color: opsRoot.accent
                                border.width: hoverArea.containsMouse ? 1 : 0
                                opacity: hoverArea.containsMouse ? 0.3 : 0
                                Behavior on opacity { NumberAnimation { duration: 200 } }
                            }
                        }

                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 10
                            spacing: 2

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 10

                                // Icon with glow
                                Item {
                                    width: 20
                                    height: 20

                                    Image {
                                        id: menuIcon
                                        anchors.centerIn: parent
                                        width: 18
                                        height: 18
                                        source: modelData.icon
                                        fillMode: Image.PreserveAspectFit
                                        smooth: true
                                        sourceSize.width: 18
                                        sourceSize.height: 18
                                        opacity: hoverArea.containsMouse ? 1.0 : 0.8

                                        Behavior on opacity { NumberAnimation { duration: 200 } }
                                    }

                                    // Icon glow on hover
                                    Rectangle {
                                        anchors.centerIn: parent
                                        width: 24
                                        height: 24
                                        radius: 12
                                        color: opsRoot.accent
                                        opacity: hoverArea.containsMouse ? 0.15 : 0
                                        Behavior on opacity { NumberAnimation { duration: 200 } }
                                    }
                                }

                                ColumnLayout {
                                    spacing: 0

                                    Text {
                                        text: modelData.label
                                        color: hoverArea.containsMouse ? "#FFFFFF" : "#E0E7FF"
                                        font.pixelSize: 14
                                        font.weight: Font.Medium
                                        Behavior on color { ColorAnimation { duration: 200 } }
                                    }

                                    Text {
                                        text: modelData.description
                                        color: "#93C5FD"
                                        font.pixelSize: 10
                                        opacity: 0.7
                                    }
                                }

                                Item { Layout.fillWidth: true }

                                // Arrow indicator
                                Text {
                                    text: "›"
                                    color: opsRoot.accent
                                    font.pixelSize: 20
                                    font.bold: true
                                    Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
                                    opacity: hoverArea.containsMouse ? 1.0 : 0.5
                                    Behavior on opacity { NumberAnimation { duration: 200 } }
                                }
                            }
                        }

                        MouseArea {
                            id: hoverArea
                            anchors.fill: parent
                            hoverEnabled: true
                            cursorShape: Qt.PointingHandCursor

                            onClicked: {
                                // If a dialog is already open, blink it instead of opening new one
                                if (opsRoot.activeDialog) {
                                    opsRoot.activeDialog.blinkDialog()
                                    return
                                }

                                if (modelData.label === "Upgrade") {
                                    sideDrawer.close()
                                    if (AppController.dockerOpsRunning) {
                                        upgradeWarningDialog.open()
                                    } else {
                                        upgradeDialog.open()
                                        AppController.startUpgrade()
                                    }
                                } else if (modelData.label === "Repair") {
                                    sideDrawer.close()
                                    if (AppController.dockerOpsRunning) {
                                        repairDialog.open()
                                    } else {
                                        AppController.runDockerOps(false)  // Don't remove volumes on repair
                                    }
                                }
                            }
                        }
                    }
                }
            }

            Item { Layout.fillHeight: true }

            // Stylish separator
            Rectangle {
                Layout.fillWidth: true
                height: 2
                radius: 1
                gradient: Gradient {
                    orientation: Gradient.Horizontal
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 0.5; color: "#2D3B5F" }
                    GradientStop { position: 1.0; color: "transparent" }
                }
            }

            // Footer version text
            ColumnLayout {
                Layout.fillWidth: true
                spacing: 4

                Text {
                    text: "safecore_installer"
                    color: "#64748B"
                    font.pixelSize: 10
                    font.weight: Font.Medium
                    font.letterSpacing: 0.5
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                }

                Text {
                    text: "v1.0"
                    color: "#6EE7FF"
                    font.pixelSize: 9
                    horizontalAlignment: Text.AlignHCenter
                    Layout.fillWidth: true
                    opacity: 0.6
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.leftMargin: 28
        anchors.rightMargin: 28
        anchors.topMargin: 20
        anchors.bottomMargin: 20

        RowLayout {
            Layout.fillWidth: true
            spacing: 10
            Rectangle {
                id: drawerButton
                width: 40
                height: 40
                radius: 8
                color: "#151B2A"
                border.color: "#1F2A4A"
                border.width: 1
                Layout.leftMargin: -20
                Layout.topMargin: -10
                Image {
                    anchors.centerIn: parent
                    source: "qrc:/images/drawer/drawer_icon.png"
                    width: 24
                    height: 24
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

        Item { height: 3 }

        // Stylish separator
        Rectangle {
            Layout.fillWidth: true
            height: 2
            radius: 1
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "transparent" }
                GradientStop { position: 0.5; color: "#2D3B5F" }
                GradientStop { position: 1.0; color: "transparent" }
            }
        }

        Item { height: 3 }

        AppCard {
            Layout.fillWidth: true
            title: "Safecore Manage Program"
            subtitle: "Control the SafeCore container lifecycle on this machine."
            contentItem: ColumnLayout {
                spacing: 10

                // Status indicator row
                RowLayout {
                    Layout.fillWidth: true
                    spacing: 16

                    // Container status indicator with animation
                    Rectangle {
                        width: 12
                        height: 12
                        radius: 6
                        color: AppController.dockerOpsRunning ? "#22C55E" : "#64748B"

                        SequentialAnimation on opacity {
                            running: AppController.dockerOpsRunning
                            loops: Animation.Infinite
                            NumberAnimation { from: 1.0; to: 0.3; duration: 800 }
                            NumberAnimation { from: 0.3; to: 1.0; duration: 800 }
                        }

                        // Glow effect when running
                        Rectangle {
                            anchors.centerIn: parent
                            width: parent.width * 2
                            height: parent.height * 2
                            radius: width / 2
                            color: "transparent"
                            border.color: AppController.dockerOpsRunning ? "#22C55E" : "transparent"
                            border.width: 2
                            opacity: 0.3
                            visible: AppController.dockerOpsRunning
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        color: AppController.dockerOpsRunning ? "#22C55E" : "#93C5FD"
                        font.pixelSize: 13
                        font.weight: Font.Medium
                        text: AppController.dockerOpsRunning
                            ? "Container is running • Real-time monitoring active"
                            : "Start the SafeCore container with the latest settings pulled from your tenant configuration."
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
                    Layout.preferredHeight: (AppController.dockerOpsRunning || AppController.dockerOpsLog.length > 0) ? opsRoot.consoleHeight : 0
                    radius: 12
                    color: "#050811"
                    border.color: "#2D3B5F"
                    border.width: 1
                    visible: !opsRoot.showContainerLogs && (AppController.dockerOpsRunning || AppController.dockerOpsLog.length > 0)

                    property bool autoScroll: true

                    // Subtle gradient overlay
                    Rectangle {
                        anchors.fill: parent
                        radius: parent.radius
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "#0A101800" }
                            GradientStop { position: 1.0; color: "#0A101820" }
                        }
                    }

                    // Header bar with label
                    Rectangle {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 32
                        color: "#0F1623"
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
                                text: "⚙"
                                color: "#6EE7FF"
                                font.pixelSize: 14
                            }

                            Text {
                                text: "Docker Operations Log"
                                color: "#93C5FD"
                                font.pixelSize: 11
                                font.weight: Font.Medium
                            }

                            Item { Layout.fillWidth: true }

                            Rectangle {
                                width: 6
                                height: 6
                                radius: 3
                                color: "#22C55E"
                                opacity: 0.8
                                visible: AppController.dockerOpsRunning

                                SequentialAnimation on opacity {
                                    running: AppController.dockerOpsRunning
                                    loops: Animation.Infinite
                                    NumberAnimation { from: 0.3; to: 1.0; duration: 600 }
                                    NumberAnimation { from: 1.0; to: 0.3; duration: 600 }
                                }
                            }

                            Text {
                                text: AppController.dockerOpsRunning ? "ACTIVE" : "IDLE"
                                color: AppController.dockerOpsRunning ? "#22C55E" : "#64748B"
                                font.pixelSize: 10
                                font.weight: Font.Bold
                                font.letterSpacing: 0.5
                            }
                        }
                    }

                    Flickable {
                        id: opsLogFlickable
                        anchors.fill: parent
                        anchors.topMargin: 42
                        anchors.leftMargin: 12
                        anchors.rightMargin: 12
                        anchors.bottomMargin: 12
                        clip: true
                        contentWidth: opsLog.implicitWidth
                        contentHeight: opsLog.implicitHeight
                        boundsBehavior: Flickable.StopAtBounds
                        ScrollBar.horizontal: ScrollBar { policy: ScrollBar.AsNeeded }
                        ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

                        onContentHeightChanged: {
                            if (parent.autoScroll) {
                                contentY = Math.max(0, contentHeight - height);
                            }
                        }

                        onContentYChanged: {
                            var maxY = Math.max(0, contentHeight - height);
                            parent.autoScroll = (contentY >= maxY - 10);
                        }

                        TextArea {
                            id: opsLog
                            text: AppController.dockerOpsLog.length
                                ? AppController.dockerOpsLog
                                : "Docker output will appear here..."
                            readOnly: true
                            textFormat: TextEdit.PlainText
                            wrapMode: TextEdit.NoWrap
                            font.family: "JetBrains Mono, Consolas, Monaco, Monospace"
                            font.pixelSize: 12
                            color: AppController.dockerOpsLog.length ? "#E0E7FF" : "#64748B"
                            background: null
                            width: implicitWidth
                            height: implicitHeight
                            bottomPadding: 10
                            rightPadding: 10
                        }
                    }
                }
                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: opsRoot.showContainerLogs ? opsRoot.consoleHeight : 0
                    radius: 12
                    color: "#050811"
                    border.color: "#2D3B5F"
                    border.width: 1
                    visible: opsRoot.showContainerLogs

                    property bool autoScrollFollow: true

                    // Subtle gradient overlay
                    Rectangle {
                        anchors.fill: parent
                        radius: parent.radius
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: "#0A101800" }
                            GradientStop { position: 1.0; color: "#0A101820" }
                        }
                    }

                    // Header bar with label
                    Rectangle {
                        anchors.top: parent.top
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: 32
                        color: "#0F1623"
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
                                text: "📡"
                                color: "#6EE7FF"
                                font.pixelSize: 14
                            }

                            Text {
                                text: "Live Container Logs"
                                color: "#93C5FD"
                                font.pixelSize: 11
                                font.weight: Font.Medium
                            }

                            Item { Layout.fillWidth: true }

                            Rectangle {
                                width: 6
                                height: 6
                                radius: 3
                                color: "#22C55E"
                                opacity: 0.8

                                SequentialAnimation on opacity {
                                    running: true
                                    loops: Animation.Infinite
                                    NumberAnimation { from: 0.3; to: 1.0; duration: 600 }
                                    NumberAnimation { from: 1.0; to: 0.3; duration: 600 }
                                }
                            }

                            Text {
                                text: "STREAMING"
                                color: "#22C55E"
                                font.pixelSize: 10
                                font.weight: Font.Bold
                                font.letterSpacing: 0.5
                            }
                        }
                    }

                    Flickable {
                        id: opsLogFollowFlickable
                        anchors.fill: parent
                        anchors.topMargin: 42
                        anchors.leftMargin: 12
                        anchors.rightMargin: 12
                        anchors.bottomMargin: 12
                        clip: true
                        contentWidth: opsLogFollow.implicitWidth
                        contentHeight: opsLogFollow.implicitHeight
                        boundsBehavior: Flickable.StopAtBounds
                        ScrollBar.horizontal: ScrollBar { policy: ScrollBar.AsNeeded }
                        ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

                        onContentHeightChanged: {
                            if (parent.autoScrollFollow) {
                                contentY = Math.max(0, contentHeight - height);
                            }
                        }

                        onContentYChanged: {
                            var maxY = Math.max(0, contentHeight - height);
                            parent.autoScrollFollow = (contentY >= maxY - 10);
                        }

                        TextArea {
                            id: opsLogFollow
                            text: AppController.dockerOpsFollowLog.length
                                ? AppController.dockerOpsFollowLog
                                : "Waiting for container logs..."
                            readOnly: true
                            textFormat: TextEdit.PlainText
                            wrapMode: TextEdit.NoWrap
                            font.family: "JetBrains Mono, Consolas, Monaco, Monospace"
                            font.pixelSize: 12
                            color: AppController.dockerOpsFollowLog.length ? "#E0E7FF" : "#64748B"
                            background: null
                            width: implicitWidth
                            height: implicitHeight
                            bottomPadding: 10
                            rightPadding: 10
                        }
                    }
                }
                Item { height: 3 }
            }
        }

        Item { Layout.fillHeight: true }

        // Stylish separator
        Rectangle {
            Layout.fillWidth: true
            height: 2
            radius: 1
            gradient: Gradient {
                orientation: Gradient.Horizontal
                GradientStop { position: 0.0; color: "transparent" }
                GradientStop { position: 0.5; color: "#2D3B5F" }
                GradientStop { position: 1.0; color: "transparent" }
            }
        }

        Item { height: 3 }

        // Enhanced control panel
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 80
            color: "#0A0F1C"
            border.color: "#1F2A4A"
            border.width: 1
            radius: 16

            RowLayout {
                anchors.fill: parent
                anchors.margins: 16
                spacing: 20

                // Status info section
                RowLayout {
                    spacing: 12

                    Rectangle {
                        width: 40
                        height: 40
                        radius: 8
                        color: AppController.dockerOpsRunning ? "#22C55E15" : "#0A1022"
                        border.color: AppController.dockerOpsRunning ? "#22C55E" : "#64748B"
                        border.width: 1

                        Text {
                            anchors.centerIn: parent
                            text: AppController.dockerOpsRunning ? "▶" : "■"
                            color: AppController.dockerOpsRunning ? "#22C55E" : "#64748B"
                            font.pixelSize: 18
                            font.bold: true
                        }
                    }

                    ColumnLayout {
                        spacing: 2

                        Text {
                            text: "Container Status"
                            color: "#64748B"
                            font.pixelSize: 11
                            font.weight: Font.Medium
                        }

                        Text {
                            text: AppController.dockerOpsStarting ? "Starting..." :
                                  AppController.dockerOpsStopping ? "Stopping..." :
                                  AppController.dockerOpsRunning ? "Running" : "Stopped"
                            color: AppController.dockerOpsRunning ? "#22C55E" : "#93C5FD"
                            font.pixelSize: 14
                            font.weight: Font.Bold
                        }
                    }
                }

                Item { Layout.fillWidth: true }

                // Control buttons
                RowLayout {
                    spacing: 12

                    AppButton {
                        text: "Start"
                        loading: AppController.dockerOpsStarting
                        loadingText: "Starting..."
                        accent: opsRoot.accent
                        enabled: !AppController.dockerOpsRunning && !AppController.dockerOpsStarting && !AppController.dockerOpsStopping
                        implicitWidth: 120
                        implicitHeight: 44
                        onClicked: AppController.restartDockerOps()
                    }

                    AppButton {
                        text: "Stop"
                        loading: AppController.dockerOpsStopping
                        loadingText: "Stopping..."
                        accent: "#F87171"
                        enabled: AppController.dockerOpsRunning && !AppController.dockerOpsStarting && !AppController.dockerOpsStopping
                        implicitWidth: 120
                        implicitHeight: 44
                        onClicked: AppController.stopDockerOps()
                    }
                }
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
        implicitWidth: 650
        implicitHeight: header.height + contentItem.implicitHeight + 24

        onOpened: opsRoot.activeDialog = quitDialog
        onClosed: opsRoot.activeDialog = null

        function blinkDialog() {
            blinkAnimation.start()
        }

        SequentialAnimation {
            id: blinkAnimation
            NumberAnimation {
                target: quitDialog
                property: "opacity"
                from: 1.0
                to: 0.3
                duration: 100
            }
            NumberAnimation {
                target: quitDialog
                property: "opacity"
                from: 0.3
                to: 1.0
                duration: 100
            }
        }

        header: Rectangle {
            height: 44
            color: "#151B2A"
            border.color: "#1F2A4A"
            border.width: 1
            Row {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 18
                spacing: 8
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/images/icons/quit_icon.png"
                    width: 18
                    height: 18
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    sourceSize.width: 18
                    sourceSize.height: 18
                }
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: quitDialog.title
                    color: "#E2E8F0"
                    font.pixelSize: 16
                    font.bold: true
                }
            }
        }
        background: Rectangle {
            radius: 14
            color: "#151B2A"
            border.color: "#1F2A4A"
            border.width: 1
        }

        property string messageText: AppController.dockerOpsRunning
            ? "Do you want to quit SafeCore? The container will keep running."
            : "Do you want to quit the SafeCore application?"
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
                        implicitWidth: 120
                        implicitHeight: 44
                        onClicked: quitDialog.close()
                    }
                    AppButton {
                        text: "Yes"
                        enabled: true
                        accent: opsRoot.accent
                        implicitWidth: 120
                        implicitHeight: 44
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

    Dialog {
        id: repairDialog
        modal: true
        title: "Repair"
        anchors.centerIn: parent
        implicitWidth: 650
        implicitHeight: repairHeader.height + repairContentItem.implicitHeight + 24

        onOpened: opsRoot.activeDialog = repairDialog
        onClosed: opsRoot.activeDialog = null

        function blinkDialog() {
            repairBlinkAnimation.start()
        }

        SequentialAnimation {
            id: repairBlinkAnimation
            NumberAnimation {
                target: repairDialog
                property: "opacity"
                from: 1.0
                to: 0.3
                duration: 100
            }
            NumberAnimation {
                target: repairDialog
                property: "opacity"
                from: 0.3
                to: 1.0
                duration: 100
            }
        }

        header: Rectangle {
            id: repairHeader
            height: 44
            color: "#151B2A"
            border.color: "#1F2A4A"
            border.width: 1
            Row {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 18
                spacing: 8
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/images/drawer/repair_icon.png"
                    width: 18
                    height: 18
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    sourceSize.width: 18
                    sourceSize.height: 18
                }
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: repairDialog.title
                    color: "#E2E8F0"
                    font.pixelSize: 16
                    font.bold: true
                }
            }
        }
        background: Rectangle {
            radius: 14
            color: "#151B2A"
            border.color: "#1F2A4A"
            border.width: 1
        }
        contentItem: Item {
            id: repairContentItem
            implicitHeight: repairBodyLayout.implicitHeight
            implicitWidth: repairBodyLayout.implicitWidth
            ColumnLayout {
                id: repairBodyLayout
                anchors.fill: parent
                anchors.margins: 18
                spacing: 12

                Text {
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    color: "#B8C2E0"
                    text: "Before repairing the SafeCore application, please stop the container first."
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Item { Layout.fillWidth: true }
                    AppButton {
                        text: "OK"
                        enabled: true
                        accent: opsRoot.accent
                        implicitWidth: 120
                        implicitHeight: 44
                        onClicked: repairDialog.close()
                    }
                }
                Item { height: 6 }
            }
        }
    }
    Dialog {
        id: upgradeWarningDialog
        modal: true
        title: "Container Running"
        anchors.centerIn: parent
        implicitWidth: 650
        implicitHeight: upgradeWarningHeader.height + upgradeWarningContentItem.implicitHeight + 24

        onOpened: opsRoot.activeDialog = upgradeWarningDialog
        onClosed: opsRoot.activeDialog = null

        function blinkDialog() {
            upgradeWarningBlinkAnimation.start()
        }

        SequentialAnimation {
            id: upgradeWarningBlinkAnimation
            NumberAnimation {
                target: upgradeWarningDialog
                property: "opacity"
                from: 1.0
                to: 0.3
                duration: 100
            }
            NumberAnimation {
                target: upgradeWarningDialog
                property: "opacity"
                from: 0.3
                to: 1.0
                duration: 100
            }
        }

        header: Rectangle {
            id: upgradeWarningHeader
            height: 44
            color: "#151B2A"
            border.color: "#1F2A4A"
            border.width: 1
            Row {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 18
                spacing: 8
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/images/drawer/upgrade_icon.png"
                    width: 18
                    height: 18
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    sourceSize.width: 18
                    sourceSize.height: 18
                }
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: upgradeWarningDialog.title
                    color: "#E2E8F0"
                    font.pixelSize: 16
                    font.bold: true
                }
            }
        }
        background: Rectangle {
            radius: 14
            color: "#151B2A"
            border.color: "#1F2A4A"
            border.width: 1
        }
        contentItem: Item {
            id: upgradeWarningContentItem
            implicitHeight: upgradeWarningBodyLayout.implicitHeight
            implicitWidth: upgradeWarningBodyLayout.implicitWidth
            ColumnLayout {
                id: upgradeWarningBodyLayout
                anchors.fill: parent
                anchors.margins: 18
                spacing: 12

                Text {
                    Layout.fillWidth: true
                    wrapMode: Text.WordWrap
                    color: "#B8C2E0"
                    font.pixelSize: 14
                    text: "Before upgrading the SafeCore application, stop the container."
                }

                Item { height: 8 }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 12
                    Item { Layout.fillWidth: true }
                    AppButton {
                        text: "OK"
                        enabled: true
                        accent: opsRoot.accent
                        implicitWidth: 120
                        implicitHeight: 44
                        onClicked: upgradeWarningDialog.close()
                    }
                }
                Item { height: 6 }
            }
        }
    }

    Dialog {
        id: upgradeDialog
        modal: true
        title: "Upgrade"
        anchors.centerIn: parent
        implicitWidth: 650
        implicitHeight: upgradeHeader.height + upgradeContentItem.implicitHeight + 24
        closePolicy: AppController.upgradeRunning ? Popup.NoAutoClose : Popup.CloseOnEscape

        function blinkDialog() {
            upgradeBlinkAnimation.start()
        }

        SequentialAnimation {
            id: upgradeBlinkAnimation
            NumberAnimation {
                target: upgradeDialog
                property: "opacity"
                from: 1.0
                to: 0.3
                duration: 100
            }
            NumberAnimation {
                target: upgradeDialog
                property: "opacity"
                from: 0.3
                to: 1.0
                duration: 100
            }
        }

        property bool upgradeAvailable: false
        property bool upgradeComplete: false
        property bool upgradeDownloading: false
        property bool upgradeCanceled: false
        property string statusMessage: ""

        onOpened: {
            opsRoot.activeDialog = upgradeDialog
            upgradeAvailable = false
            upgradeComplete = false
            upgradeDownloading = false
            upgradeCanceled = false
            statusMessage = ""
        }

        onClosed: {
            opsRoot.activeDialog = null
        }

        Connections {
            target: AppController
            function onUpgradeFinished(hasUpdate, message) {
                upgradeDialog.upgradeComplete = true
                upgradeDialog.statusMessage = message
                upgradeDialog.upgradeAvailable = hasUpdate
                upgradeDialog.upgradeCanceled = message.startsWith("Upgrade canceled.")
            }
            function onUpgradeLogChanged() {
                // Detect if download is actually happening (not just "up to date" check)
                // Only show console when we see actual download activity
                if (AppController.upgradeRunning && !upgradeDialog.upgradeDownloading) {
                    const log = AppController.upgradeLog;
                    // Check for actual download indicators, not just "Pulling from"
                    if (log.includes("Pulling fs layer") ||
                        log.includes("Downloading") ||
                        log.includes("Download complete") ||
                        log.includes("Pull complete") ||
                        log.includes("Extracting") ||
                        log.includes("Verifying Checksum") ||
                        log.includes("Waiting")) {
                        upgradeDialog.upgradeDownloading = true;
                    }
                }
            }
        }

        header: Rectangle {
            id: upgradeHeader
            height: 44
            color: "#151B2A"
            border.color: "#1F2A4A"
            border.width: 1
            Row {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.leftMargin: 18
                spacing: 8
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    source: "qrc:/images/drawer/upgrade_icon.png"
                    width: 18
                    height: 18
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    sourceSize.width: 18
                    sourceSize.height: 18
                }
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    text: upgradeDialog.title
                    color: "#E2E8F0"
                    font.pixelSize: 16
                    font.bold: true
                }
            }
        }
        background: Rectangle {
            radius: 14
            color: "#151B2A"
            border.color: "#1F2A4A"
            border.width: 1
        }
        contentItem: Item {
            id: upgradeContentItem
            implicitHeight: upgradeBodyLayout.implicitHeight
            implicitWidth: upgradeBodyLayout.implicitWidth
            ColumnLayout {
                id: upgradeBodyLayout
                anchors.fill: parent
                anchors.margins: 18
                spacing: 12

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    visible: !AppController.upgradeRunning && upgradeDialog.upgradeComplete && !upgradeDialog.upgradeAvailable

                    Rectangle {
                        width: 24
                        height: 24
                        radius: 12
                        color: upgradeDialog.upgradeCanceled ? "#F59E0B" : "#22C55E"
                        Text {
                            anchors.centerIn: parent
                            text: upgradeDialog.upgradeCanceled ? "\u26A0" : "\u2713"
                            font.pixelSize: upgradeDialog.upgradeCanceled ? 14 : 16
                            font.bold: true
                            color: "#FFFFFF"
                        }
                    }

                    Text {
                        Layout.fillWidth: true
                        wrapMode: Text.WordWrap
                        color: "#B8C2E0"
                        text: upgradeDialog.statusMessage
                    }
                }

                ColumnLayout {
                    Layout.fillWidth: true
                    spacing: 8
                    visible: AppController.upgradeRunning || (upgradeDialog.upgradeComplete && upgradeDialog.upgradeAvailable)

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 12

                        // Animated loading spinner (only when checking, not downloading)
                        Item {
                            width: 24
                            height: 24
                            visible: AppController.upgradeRunning && !upgradeDialog.upgradeDownloading

                            Canvas {
                                id: spinnerCanvas
                                anchors.fill: parent
                                antialiasing: true

                                property real rotation: 0

                                onPaint: {
                                    var ctx = getContext("2d");
                                    ctx.clearRect(0, 0, width, height);
                                    ctx.save();
                                    ctx.translate(width / 2, height / 2);
                                    ctx.rotate(rotation * Math.PI / 180);

                                    // Draw circular segments
                                    for (var i = 0; i < 8; i++) {
                                        ctx.save();
                                        ctx.rotate(i * Math.PI / 4);
                                        ctx.beginPath();
                                        var opacity = (i + 1) / 8;
                                        ctx.fillStyle = Qt.rgba(0.38, 0.51, 0.96, opacity);
                                        ctx.arc(7, 0, 2.5, 0, Math.PI * 2);
                                        ctx.fill();
                                        ctx.restore();
                                    }
                                    ctx.restore();
                                }

                                RotationAnimator {
                                    target: spinnerCanvas
                                    from: 0
                                    to: 360
                                    duration: 1200
                                    loops: Animation.Infinite
                                    running: AppController.upgradeRunning && !upgradeDialog.upgradeDownloading
                                    onToChanged: spinnerCanvas.rotation = to
                                }

                                Timer {
                                    interval: 16
                                    running: AppController.upgradeRunning && !upgradeDialog.upgradeDownloading
                                    repeat: true
                                    onTriggered: {
                                        spinnerCanvas.rotation = (spinnerCanvas.rotation + 3) % 360;
                                        spinnerCanvas.requestPaint();
                                    }
                                }
                            }
                        }

                        Text {
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            color: "#B8C2E0"
                            text: AppController.upgradeRunning
                                ? (upgradeDialog.upgradeDownloading ? "Downloading new image..." : "Checking for updates...")
                                : (upgradeDialog.upgradeComplete ? upgradeDialog.statusMessage : "")
                        }
                    }

                    // Progress bar with percentage (shown when downloading)
                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 8
                        visible: upgradeDialog.upgradeDownloading

                        ModernProgressBar {
                            Layout.fillWidth: true
                            value: AppController.upgradeProgress
                            busy: AppController.upgradeRunning
                        }

                        Text {
                            color: "#93C5FD"
                            font.pixelSize: 13
                            text: Math.round(AppController.upgradeProgress * 100) + "%"
                        }

                        Rectangle {
                            visible: upgradeDialog.upgradeComplete && upgradeDialog.upgradeAvailable
                            width: 20
                            height: 20
                            radius: 10
                            color: "#22C55E"
                            Text {
                                anchors.centerIn: parent
                                text: "\u2713"
                                font.pixelSize: 12
                                font.bold: true
                                color: "#FFFFFF"
                            }
                        }
                    }
                }

                Item {
                    id: upgradeSwapArea
                    Layout.fillWidth: true
                    Layout.preferredHeight: 300
                    implicitHeight: 300
                    Layout.bottomMargin: 0
                    visible: !upgradeDialog.upgradeCanceled && (upgradeDialog.upgradeDownloading || (upgradeDialog.upgradeComplete && upgradeDialog.upgradeAvailable))

                    Rectangle {
                        id: upgradeLogPanel
                        anchors.fill: parent
                        radius: 12
                        color: "#050811"
                        border.color: "#2D3B5F"
                        border.width: 1
                        clip: true

                        property bool stickToBottom: true
                        function updateStickState() {
                            if (!upgradeLogFlickable)
                                return;
                            var maxY = Math.max(0, upgradeLogFlickable.contentHeight - upgradeLogFlickable.height);
                            stickToBottom = upgradeLogFlickable.contentY >= maxY - 4;
                        }
                        function scrollToBottom() {
                            if (!upgradeLogFlickable)
                                return;
                            upgradeLogFlickable.contentY = Math.max(0, upgradeLogFlickable.contentHeight - upgradeLogFlickable.height);
                        }

                        // Subtle gradient overlay
                        Rectangle {
                            anchors.fill: parent
                            radius: parent.radius
                            gradient: Gradient {
                                GradientStop { position: 0.0; color: "#0A101800" }
                                GradientStop { position: 1.0; color: "#0A101820" }
                            }
                        }

                        // Header bar with label
                        Rectangle {
                            anchors.top: parent.top
                            anchors.left: parent.left
                            anchors.right: parent.right
                            height: 32
                            color: "#0F1623"
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
                                    text: "⬆"
                                    color: "#6EE7FF"
                                    font.pixelSize: 14
                                }

                                Text {
                                    text: "Upgrade Log"
                                    color: "#93C5FD"
                                    font.pixelSize: 11
                                    font.weight: Font.Medium
                                }

                                Item { Layout.fillWidth: true }

                                Rectangle {
                                    width: 6
                                    height: 6
                                    radius: 3
                                    color: "#22C55E"
                                    opacity: 0.8
                                    visible: AppController.upgradeRunning

                                    SequentialAnimation on opacity {
                                        running: AppController.upgradeRunning
                                        loops: Animation.Infinite
                                        NumberAnimation { from: 0.3; to: 1.0; duration: 600 }
                                        NumberAnimation { from: 1.0; to: 0.3; duration: 600 }
                                    }
                                }

                                Text {
                                    text: AppController.upgradeRunning ? "ACTIVE" : "IDLE"
                                    color: AppController.upgradeRunning ? "#22C55E" : "#64748B"
                                    font.pixelSize: 10
                                    font.weight: Font.Bold
                                    font.letterSpacing: 0.5
                                }
                            }
                        }

                        Flickable {
                            id: upgradeLogFlickable
                            anchors.fill: parent
                            anchors.topMargin: 42
                            anchors.leftMargin: 12
                            anchors.rightMargin: 12
                            anchors.bottomMargin: 12
                            clip: true
                            contentWidth: upgradeLogText.implicitWidth
                            contentHeight: upgradeLogText.implicitHeight
                            boundsBehavior: Flickable.StopAtBounds
                            ScrollBar.horizontal: ScrollBar { policy: ScrollBar.AsNeeded }
                            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
                            onContentYChanged: upgradeLogPanel.updateStickState()
                            onContentHeightChanged: {
                                if (upgradeLogPanel.stickToBottom)
                                    upgradeLogPanel.scrollToBottom();
                            }

                            TextArea {
                                id: upgradeLogText
                                text: AppController.upgradeLog.length ? AppController.upgradeLog : "Waiting for output..."
                                readOnly: true
                                textFormat: TextEdit.PlainText
                                wrapMode: TextEdit.NoWrap
                                font.family: "JetBrains Mono, Consolas, Monaco, Monospace"
                                font.pixelSize: 12
                                color: AppController.upgradeLog.length ? "#E0E7FF" : "#64748B"
                                background: null
                                width: implicitWidth
                                height: implicitHeight
                                bottomPadding: 10
                                rightPadding: 10
                                onTextChanged: {
                                    if (upgradeLogPanel.stickToBottom)
                                        upgradeLogPanel.scrollToBottom();
                                }
                            }
                        }
                        Component.onCompleted: scrollToBottom()
                    }
                }

                Item { Layout.fillHeight: true }

                Rectangle {
                    Layout.fillWidth: true
                    height: 1
                    color: "#1F2A4A"
                    visible: upgradeDialog.upgradeComplete && upgradeDialog.upgradeAvailable
                }

                RowLayout {
                    Layout.fillWidth: true
                    spacing: 10
                    Item { Layout.fillWidth: true }
                    AppButton {
                        text: "Cancel"
                        visible: AppController.upgradeRunning
                        enabled: true
                        accent: "#F87171"
                        implicitWidth: 120
                        implicitHeight: 44
                        onClicked: AppController.cancelUpgrade()
                    }
                    AppButton {
                        text: "OK"
                        visible: upgradeDialog.upgradeComplete && !upgradeDialog.upgradeAvailable
                        enabled: true
                        accent: opsRoot.accent
                        implicitWidth: 120
                        implicitHeight: 44
                        onClicked: upgradeDialog.close()
                    }
                    AppButton {
                        text: "Restart"
                        visible: upgradeDialog.upgradeComplete && upgradeDialog.upgradeAvailable
                        enabled: true
                        accent: opsRoot.accent
                        implicitWidth: 120
                        implicitHeight: 44
                        onClicked: {
                            upgradeDialog.close()
                            AppController.runDockerOps(true)  // Remove volumes after upgrade
                        }
                    }
                }
                Item { height: 10 }
            }
        }
    }
}
