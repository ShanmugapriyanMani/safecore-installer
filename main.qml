import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window

Window {
    id: root
    width: 1100
    height: 680
    minimumWidth: 980
    minimumHeight: 620
    visible: true
    title: "SafeCore"
    flags: Qt.Window | Qt.CustomizeWindowHint | Qt.WindowTitleHint | Qt.WindowCloseButtonHint

    // Theme aliases for backward compatibility
    readonly property color bgPrimary: Theme.bgPrimary
    readonly property color bgSecondary: Theme.bgSecondary
    readonly property color bgCard: Theme.bgCard
    readonly property color bgInput: Theme.bgInput
    readonly property color bgInputDisabled: Theme.bgInputDisabled
    readonly property color bgPopup: Theme.bgPopup
    readonly property color bgLog: Theme.bgLog
    readonly property color borderPrimary: Theme.borderPrimary
    readonly property color borderSecondary: Theme.borderSecondary
    readonly property color borderProgress: Theme.borderProgress
    readonly property color accent: Theme.accent
    readonly property color accent2: Theme.accent2
    readonly property color accentError: Theme.accentError
    readonly property color accentSuccess: Theme.accentSuccess
    readonly property color accentSuccessDark: Theme.accentSuccessDark
    readonly property color accentWarning: Theme.accentWarning
    readonly property color accentWarningDark: Theme.accentWarningDark
    readonly property color textPrimary: Theme.textPrimary
    readonly property color textSecondary: Theme.textSecondary
    readonly property color textMuted: Theme.textMuted
    readonly property color textPlaceholder: Theme.textPlaceholder
    readonly property color textInfo: Theme.textInfo
    readonly property color textSuccess: Theme.textSuccess
    readonly property color textError: Theme.textError
    readonly property color textSuccessDark: Theme.textSuccessDark
    readonly property color textErrorDark: Theme.textErrorDark
    readonly property color textLog: Theme.textLog
    readonly property color stepInactive: Theme.stepInactive
    readonly property color buttonSecondary: Theme.buttonSecondary
    readonly property color shimmerLight: Theme.shimmerLight

    color: Theme.bgPrimary
    property bool allowClose: false
    property bool installationComplete: false
    property bool devInstallStart: false
    property bool devTenantStart: false
    property bool devSyncSuccess: false
    property bool devFinishStart: false
    property bool runDockerAndQuit: false
    property string tipText: {
        if (AppController.currentStep === 0)
            return "Review the steps and click Next when you're ready.";
        if (AppController.currentStep === 1)
            return "Use valid MAC/Tenant IDs, then sync the relay URL.";
        if (AppController.currentStep === 2)
            return "Run Docker install, then fetch env variables and models.";
        if (AppController.currentStep === 3)
            return "Verify everything and finish the setup.";
        return "Keep your MAC ID and Tenant ID ready before registration.";
    }

    Component.onCompleted: {
        x = (Screen.width - width) / 2
        y = (Screen.height - height) / 2
        if (devInstallStart)
            AppController.forceStep(2)
        if (devSyncSuccess || devTenantStart) {
            AppController.forceStep(1)
            rightStack.syncSuccess = true
            rightStack.syncMessage = "Sync API registration completed successfully."
        }
        if (devFinishStart)
            AppController.forceStep(3)
    }
    onClosing: function(close) {
        if (allowClose)
            return;
        close.accepted = false;
        quitDialog.open();
    }

    // background gradient
    Rectangle {
        anchors.fill: parent
        gradient: Gradient {
            GradientStop { position: 0.0; color: root.bgPrimary }
            GradientStop { position: 1.0; color: root.bgSecondary }
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 16
        spacing: 12

        // Main content row
        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 18

            // Left panel
            Rectangle {
                Layout.preferredWidth: 360
                Layout.fillHeight: true
                radius: 18
                color: root.bgCard
                border.color: root.borderPrimary
                border.width: 1
                clip: true

                    ColumnLayout {
                        anchors.fill: parent
                        anchors.margins: 20
                        spacing: 14

                        Image {
                            Layout.preferredWidth: 200
                            Layout.preferredHeight: 40
                            Layout.maximumWidth: 200
                            Layout.maximumHeight: 40
                            fillMode: Image.PreserveAspectFit
                            smooth: true
                            mipmap: true
                            source: "qrc:/images/logo/SafeSpace_Global.png"
                            sourceSize.width: Math.round(width * Screen.devicePixelRatio)
                            sourceSize.height: Math.round(height * Screen.devicePixelRatio)
                        }

                        Text {
                            text: "SafeCore Installer"
                            font.pixelSize: 14
                            font.weight: Font.DemiBold
                            color: "white"
                        }

                    Text {
                        text: "Guided setup for registration and installation\nwith Docker, env variables, and AI models."
                        wrapMode: Text.WordWrap
                        color: root.textSecondary
                        font.pixelSize: 13
                    }

                    Rectangle { Layout.fillWidth: true; height: 1; color: root.borderPrimary }

                    StepRow {
                        title: "Welcome"
                        subtitle: "Get ready to start installation"
                        active: AppController.currentStep === 0
                        done: AppController.currentStep > 0
                        indexText: "1"
                        accent: root.accent
                    }

                    StepRow {
                        title: "Registration"
                        subtitle: "Generate registration key"
                        active: AppController.currentStep === 1
                        done: AppController.keyValid
                        indexText: "2"
                        accent: root.accent
                    }

                    StepRow {
                        title: "Installation"
                        subtitle: "Install & Pull docker image"
                        active: AppController.currentStep === 2
                        done: AppController.currentStep > 2 || root.installationComplete
                        indexText: "3"
                        accent: root.accent
                    }

                    StepRow {
                        title: "Run"
                        subtitle: "Run the SafeCore application"
                        active: AppController.currentStep === 3
                        done: AppController.currentStep === 3 && !AppController.busy
                        indexText: "4"
                        accent: root.accent
                    }

                }
            }

            // Right panel
            Rectangle {
                Layout.fillWidth: true
                Layout.fillHeight: true
                radius: 18
                color: root.bgCard
                border.color: root.borderPrimary
                border.width: 1
                clip: true

            Item {
                anchors.fill: parent
                anchors.margins: 22

                Item {
                    id: stageIndicator
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.top: parent.top
                    height: 46

                    property int stageCount: 4
                    property real progressRatio: stageCount <= 1
                        ? 1.0
                        : Math.max(0.0, Math.min(1.0, AppController.currentStep / (stageCount - 1)))
                    property int circleSize: 14

                    Rectangle {
                        id: baseLine
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: stageIndicator.circleSize / 2
                        anchors.rightMargin: stageIndicator.circleSize / 2
                        anchors.verticalCenter: parent.verticalCenter
                        height: 6
                        radius: height / 2
                        color: root.borderProgress
                    }

                    Rectangle {
                        id: progressLine
                        anchors.left: baseLine.left
                        anchors.verticalCenter: baseLine.verticalCenter
                        height: baseLine.height
                        radius: height / 2
                        width: baseLine.width * stageIndicator.progressRatio
                        gradient: Gradient {
                            GradientStop { position: 0.0; color: root.accent }
                            GradientStop { position: 1.0; color: root.accent2 }
                        }
                        Behavior on width { NumberAnimation { duration: 220; easing.type: Easing.OutCubic } }
                    }

                    Repeater {
                        model: stageIndicator.stageCount
                        delegate: Rectangle {
                            width: stageIndicator.circleSize
                            height: stageIndicator.circleSize
                            radius: width / 2
                            y: baseLine.y + (baseLine.height - height) / 2
                            x: stageIndicator.stageCount <= 1
                                ? baseLine.x
                                : baseLine.x + (baseLine.width * (index / (stageIndicator.stageCount - 1.0))) - (width / 2)
                            color: index < AppController.currentStep
                                ? root.accent
                                : (index === AppController.currentStep ? root.accent2 : root.stepInactive)
                            border.color: index <= AppController.currentStep ? root.bgPrimary : root.borderSecondary
                            border.width: 1
                        }
                    }
                }

                Item {
                    id: footerBar
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: parent.bottom
                    height: 86

                    // Stylish separator
                    Rectangle {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.top: parent.top
                        height: 2
                        radius: 1
                        gradient: Gradient {
                            orientation: Gradient.Horizontal
                            GradientStop { position: 0.0; color: "transparent" }
                            GradientStop { position: 0.5; color: "#2D3B5F" }
                            GradientStop { position: 1.0; color: "transparent" }
                        }
                    }

                    // Enhanced control panel
                    Rectangle {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        anchors.top: parent.top
                        anchors.topMargin: 14
                        color: "#0A1022"
                        border.color: "#1F2A4A"
                        border.width: 1
                        radius: 16

                        RowLayout {
                            anchors.fill: parent
                            anchors.margins: 16
                            spacing: 12

                            // Control buttons
                            RowLayout {
                                spacing: 12
                                visible: AppController.currentStep === 1 || AppController.currentStep === 2

                                AppButton {
                                    text: "Register"
                                    visible: AppController.currentStep === 1
                                    enabled: !AppController.busy && !AppController.keyValid
                                    loading: AppController.busy && AppController.currentStep === 1 && !AppController.keyValid
                                    loadingText: "Registering..."
                                    onClicked: AppController.validateKey()
                                    accent: root.accent
                                    implicitHeight: 44
                                }
                                AppButton {
                                    text: "Sync Device"
                                    visible: AppController.currentStep === 1
                                    enabled: AppController.keyValid && !AppController.syncBusy && !rightStack.syncSuccess
                                    loading: AppController.syncBusy
                                    loadingText: "Syncing..."
                                    onClicked: AppController.syncRelay()
                                    accent: root.accent
                                    implicitHeight: 44
                                }
                                AppButton {
                                    text: "Setup Tenant"
                                    visible: AppController.currentStep === 1
                                    enabled: rightStack.syncSuccess && !AppController.tenantBusy && !AppController.tenantSuccess
                                    loading: AppController.tenantBusy
                                    loadingText: "Setting up..."
                                    onClicked: AppController.fetchTenantData(tenantSelect.currentText)
                                    accent: root.accent
                                    implicitHeight: 44
                                }
                                AppButton {
                                    text: "Install"
                                    visible: AppController.currentStep === 2
                                    loading: AppController.installPrereqsRunning
                                    loadingText: "Installing..."
                                    enabled: !AppController.installPrereqsRunning && !AppController.installPrereqsDone
                                    onClicked: {
                                        installState.showLogs = true
                                        installState.started = false
                                        installState.awaitingAuth = false
                                        installState.statusText = "Installing Docker and NVIDIA Container Toolkit..."
                                        AppController.startInstallPrereqs()
                                    }
                                    accent: root.accent
                                    implicitHeight: 44
                                }
                                AppButton {
                                    text: installState.installDone ? "Pulled" : "Pull"
                                    visible: AppController.currentStep === 2
                                    loading: installState.awaitingAuth
                                    loadingText: "Connecting..."
                                    enabled: AppController.installPrereqsDone && !installTimer.running && !installState.installDone && !AppController.dockerPullActive && !installState.awaitingAuth
                                    onClicked: {
                                        installState.reset()
                                        installState.statusText = "Authenticating with Docker registry..."
                                        installState.started = false
                                        installState.awaitingAuth = true
                                        AppController.pullDockerImage()
                                    }
                                    accent: root.accent
                                    implicitHeight: 44
                                }
                                AppButton {
                                    text: "Cancel"
                                    visible: AppController.currentStep === 2
                                    enabled: AppController.dockerPullActive
                                    onClicked: cancelDialog.open()
                                    accent: root.accentError
                                    implicitHeight: 44
                                }
                            }

                            Item { Layout.fillWidth: true }

                            AppButton {
                                text: AppController.currentStep === 3 ? "Run" : "Next"
                                enabled: {
                                    if (AppController.currentStep === 0)
                                        return true;
                                    if (AppController.currentStep === 1)
                                        return AppController.tenantSuccess;
                                    if (AppController.currentStep === 2)
                                        return installState.installDone;
                                    if (AppController.currentStep === 3)
                                        return launchCheck.checked;
                                    return true;
                                }
                                onClicked: {
                                    if (AppController.currentStep === 3) {
                                        const serviceOk = AppController.installDockerService(false)
                                        if (!serviceOk)
                                            return
                                        if (launchCheck.checked) {
                                            const ok = AppController.launchDockerOpsApp(true)
                                            if (ok) {
                                                root.allowClose = true
                                                Qt.quit()
                                            }
                                            return
                                        }
                                        root.allowClose = true
                                        Qt.quit()
                                    } else if (AppController.currentStep === 0) {
                                        AppController.goToStep(1)
                                    } else if (AppController.currentStep === 1) {
                                        AppController.goToStep(2)
                                    } else if (AppController.currentStep === 2) {
                                        AppController.goToStep(3)
                                    }
                                }
                                accent: root.accent
                                implicitWidth: 120
                                implicitHeight: 44
                            }
                        }
                    }
                }

                StackLayout {
                    id: rightStack
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.bottom: footerBar.top
                    anchors.top: stageIndicator.bottom
                    anchors.topMargin: 10
                    anchors.bottomMargin: 6
                    currentIndex: AppController.currentStep

                        property bool syncSuccess: false
                        property string syncMessage: ""

                        // Welcome
                        ColumnLayout {
                            spacing: 14
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            AppCard {
                                Layout.fillWidth: true
                                Layout.preferredHeight: 220
                                title: "Welcome to the SafeCore Online Installer"
                                subtitle: "Get ready to install Safecore on this device."
                                contentItem: ColumnLayout {
                                    spacing: 12

                                    Text {
                                        Layout.fillWidth: true
                                        wrapMode: Text.WordWrap
                                        color: root.textSecondary
                                        text: "This setup will guide you through registration and installation."
                                    }
                                    Text {
                                        Layout.fillWidth: true
                                        wrapMode: Text.WordWrap
                                        color: root.textInfo
                                        text: "What to expect:\n• Validate MAC/Tenant and generate a key\n• Install Docker and fetch env variables\n• Download required AI models"
                                    }
                                    
                                }
                            }

                        }

                        // Registration
                        Flickable {
                            id: registrationScroll
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            clip: true
                            contentWidth: width
                            contentHeight: registrationContent.implicitHeight
                            boundsBehavior: Flickable.StopAtBounds

                            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
                            property bool syncScrollPending: false
                            function scheduleScrollToBottom() {
                                syncScrollPending = true
                                syncScrollTimer.restart()
                            }
                            onContentHeightChanged: {
                                if (syncScrollPending) {
                                    syncScrollPending = false
                                    registrationScroll.contentY = Math.max(0, registrationScroll.contentHeight - registrationScroll.height)
                                }
                            }
                            Timer {
                                id: syncScrollTimer
                                interval: 30
                                repeat: false
                                onTriggered: {
                                    registrationScroll.contentY = Math.max(0, registrationScroll.contentHeight - registrationScroll.height)
                                    Qt.callLater(function() {
                                        registrationScroll.contentY = Math.max(0, registrationScroll.contentHeight - registrationScroll.height)
                                    })
                                }
                            }

                            ColumnLayout {
                                id: registrationContent
                                width: parent.width
                                spacing: 14

                                AppCard {
                                    Layout.fillWidth: true
                                    title: "Registration"
                                    subtitle: "Provide MAC ID and Tenant ID to generate a key."
                                    contentItem: ColumnLayout {
                                        spacing: 12

                                        RowLayout {
                                            Layout.fillWidth: true
                                            spacing: 8

                                            TextField {
                                                id: macField
                                                Layout.fillWidth: true
                                                leftPadding: 12
                                                rightPadding: 12
                                                topPadding: 8
                                                bottomPadding: 8
                                            placeholderText: "MAC ID (ex: 00:14:22:01:23:45)"
                                                font.pixelSize: 13
                                                color: macField.enabled ? root.textPrimary : root.textMuted
                                                placeholderTextColor: root.textPlaceholder
                                                text: AppController.macId
                                                onTextEdited: {
                                                    var hex = text.replace(/[^0-9a-fA-F]/g, "").toUpperCase()
                                                    if (hex.length > 12)
                                                        hex = hex.slice(0, 12)
                                                    var parts = []
                                                    for (var i = 0; i < hex.length; i += 2)
                                                        parts.push(hex.slice(i, i + 2))
                                                var formatted = parts.join(":")
                                                AppController.macId = formatted
                                                cursorPosition = formatted.length
                                                }
                                                inputMethodHints: Qt.ImhPreferUppercase | Qt.ImhNoPredictiveText
                                                enabled: !AppController.busy && !AppController.keyValid
                                                background: Rectangle {
                                                    radius: 4
                                                    color: macField.enabled ? root.bgInput : root.bgInputDisabled
                                                    border.color: macField.activeFocus ? root.accent : root.borderPrimary
                                                    border.width: 1
                                                }
                                            }

                                            AppButton {
                                                text: "Get"
                                                enabled: !AppController.busy && !AppController.keyValid && AppController.macId.length === 0
                                                onClicked: AppController.populateMacId()
                                                accent: root.accent
                                                implicitWidth: 70
                                                implicitHeight: 36
                                            }
                                        }

                                        TextField {
                                            id: tenantField
                                            Layout.fillWidth: true
                                            leftPadding: 12
                                            rightPadding: 12
                                            topPadding: 8
                                            bottomPadding: 8
                                            placeholderText: "Tenant ID (ex: 28C30B1F-FF4B-48DB-804F-4A1CB57990E6)"
                                            font.pixelSize: 13
                                            color: tenantField.enabled ? root.textPrimary : root.textMuted
                                            placeholderTextColor: root.textPlaceholder
                                            text: AppController.tenantId
                                            onTextEdited: {
                                                var hex = text.replace(/[^0-9a-fA-F]/g, "").toUpperCase()
                                                if (hex.length > 32)
                                                    hex = hex.slice(0, 32)
                                                var parts = []
                                                var sizes = [8, 4, 4, 4, 12]
                                                var index = 0
                                                for (var i = 0; i < sizes.length; i++) {
                                                    var size = sizes[i]
                                                    if (index >= hex.length)
                                                        break
                                                    parts.push(hex.slice(index, index + size))
                                                    index += size
                                                }
                                                var formatted = parts.join("-")
                                                AppController.tenantId = formatted
                                                cursorPosition = formatted.length
                                            }
                                            inputMethodHints: Qt.ImhPreferUppercase | Qt.ImhNoPredictiveText
                                            enabled: !AppController.busy && !AppController.keyValid
                                            background: Rectangle {
                                                radius: 4
                                                color: tenantField.enabled ? root.bgInput : root.bgInputDisabled
                                                border.color: tenantField.activeFocus ? root.accent : root.borderPrimary
                                                border.width: 1
                                            }
                                        }

                                    RowLayout {
                                        Layout.fillWidth: true
                                        visible: AppController.registrationMessage.length > 0
                                        spacing: 8
                                        Rectangle {
                                            width: 18
                                            height: 18
                                            radius: width / 2
                                            color: AppController.keyValid ? root.accentSuccess : root.accentWarning
                                            border.color: AppController.keyValid ? root.accentSuccessDark : root.accentWarningDark
                                            border.width: 1
                                            Text {
                                                anchors.centerIn: parent
                                                text: AppController.keyValid ? "\u2713" : "\u2715"
                                                color: AppController.keyValid ? root.textSuccessDark : root.textErrorDark
                                                font.pixelSize: 12
                                                font.bold: true
                                            }
                                        }
                                        Text {
                                            Layout.fillWidth: true
                                            wrapMode: Text.WordWrap
                                            color: AppController.keyValid ? root.textSuccess : root.textError
                                            font.pixelSize: 12
                                            text: AppController.registrationMessage
                                        }
                                    }

                                    Item { height: 3 }
                                }
                            }

                                AppCard {
                                    Layout.fillWidth: true
                                    visible: AppController.keyValid || devSyncSuccess
                                title: "Sync"
                                subtitle: "Relay URL"
                                contentItem: ColumnLayout {
                                    spacing: 12

                                        TextField {
                                            id: relayField
                                            Layout.fillWidth: true
                                            leftPadding: 12
                                            rightPadding: 12
                                            topPadding: 8
                                            bottomPadding: 8
                                            placeholderText: "ex: https://relay.example.com/api/v1/relay"
                                            font.pixelSize: 13
                                            color: relayField.enabled ? root.textPrimary : root.textMuted
                                            placeholderTextColor: root.textPlaceholder
                                            text: AppController.relayUrl
                                            onTextChanged: AppController.relayUrl = text
                                            background: Rectangle {
                                                radius: 4
                                                color: relayField.enabled ? root.bgInput : root.bgInputDisabled
                                                border.color: relayField.activeFocus ? root.accent : root.borderPrimary
                                                border.width: 1
                                            }
                                        }

                                        Text {
                                            Layout.fillWidth: true
                                            visible: AppController.relayError.length > 0
                                            wrapMode: Text.WordWrap
                                            color: root.textError
                                            font.pixelSize: 12
                                            text: AppController.relayError
                                        }

                                        RowLayout {
                                            Layout.fillWidth: true
                                            Layout.topMargin: 6
                                            spacing: 10
                                            Rectangle {
                                                visible: rightStack.syncSuccess
                                                width: 18
                                                height: 18
                                                radius: width / 2
                                                color: root.accentSuccess
                                                border.color: root.accentSuccessDark
                                                border.width: 1
                                                Text {
                                                    anchors.centerIn: parent
                                                    text: "\u2713"
                                                    color: root.textSuccessDark
                                                    font.pixelSize: 12
                                                    font.bold: true
                                                }
                                            }
                                            Text {
                                                wrapMode: Text.WordWrap
                                                color: rightStack.syncSuccess ? root.textSuccess : root.textError
                                                text: rightStack.syncMessage.length
                                                    ? rightStack.syncMessage
                                                    : (rightStack.syncSuccess ? "Sync API registration completed successfully." : "")
                                                visible: rightStack.syncMessage.length > 0
                                                horizontalAlignment: Text.AlignRight
                                                font.pixelSize: 12
                                            }
                                        }

                                        Item { height: 3 }
                                    }
                                }

                                AppCard {
                                    Layout.fillWidth: true
                                    visible: rightStack.syncSuccess
                                    title: "Tenant"
                                    subtitle: "Setup the tenant data"
                                    contentItem: ColumnLayout {
                                        spacing: 10
                                        Text {
                                            Layout.fillWidth: true
                                            color: "white"
                                            font.pixelSize: 12
                                            text: "Vertical"
                                        }
                                        ComboBox {
                                            id: tenantSelect
                                            Layout.fillWidth: true
                                            implicitHeight: 30
                                            model: ["SNL", "Schools", "Prison"]
                                            font.pixelSize: 13
                                            contentItem: Text {
                                                text: tenantSelect.displayText
                                                color: root.textPrimary
                                                verticalAlignment: Text.AlignVCenter
                                                leftPadding: 10
                                            }
                                            indicator: Text {
                                                text: "\u25BC"
                                                color: root.textMuted
                                                anchors.verticalCenter: parent.verticalCenter
                                                anchors.right: parent.right
                                                anchors.rightMargin: 10
                                                font.pixelSize: 10
                                            }
                                            background: Rectangle {
                                                radius: 8
                                                color: root.bgInput
                                                border.color: tenantSelect.activeFocus ? root.accent : root.borderPrimary
                                                border.width: 1
                                            }
                                            popup: Popup {
                                                y: tenantSelect.height + 4
                                                width: tenantSelect.width
                                                implicitHeight: contentItem.implicitHeight
                                                padding: 0
                                                background: Rectangle {
                                                    radius: 5
                                                    color: root.bgLog
                                                    border.color: root.borderPrimary
                                                    border.width: 1
                                                }
                                                contentItem: ListView {
                                                    implicitHeight: contentHeight
                                                    model: tenantSelect.popup.visible ? tenantSelect.delegateModel : null
                                                    clip: true
                                                }
                                            }
                                            delegate: ItemDelegate {
                                                width: tenantSelect.width
                                                height: 36
                                                text: modelData
                                                font.pixelSize: 13
                                                hoverEnabled: true
                                                readonly property bool isFirst: index === 0
                                                readonly property bool isLast: index === (tenantSelect.model.length - 1)
                                                contentItem: Text {
                                                    text: modelData
                                                    color: root.textPrimary
                                                    verticalAlignment: Text.AlignVCenter
                                                    leftPadding: 10
                                                }
                                                background: Rectangle {
                                                    color: highlighted || hovered ? root.bgSecondary : root.bgLog
                                                    radius: isLast ? 8 : 0
                                                }
                                            }
                                        }
                                        RowLayout {
                                            Layout.fillWidth: true
                                            visible: AppController.tenantMessage.length > 0
                                            spacing: 8
                                            Rectangle {
                                                width: 18
                                                height: 18
                                                radius: width / 2
                                                color: AppController.tenantSuccess ? root.accentSuccess : root.accentWarning
                                                border.color: AppController.tenantSuccess ? root.accentSuccessDark : root.accentWarningDark
                                                border.width: 1
                                                Text {
                                                    anchors.centerIn: parent
                                                    text: AppController.tenantSuccess ? "\u2713" : "\u2715"
                                                    color: AppController.tenantSuccess ? root.textSuccessDark : root.textErrorDark
                                                    font.pixelSize: 12
                                                    font.bold: true
                                                }
                                            }
                                            Text {
                                                Layout.fillWidth: true
                                                wrapMode: Text.WordWrap
                                                font.pixelSize: 12
                                                color: AppController.tenantSuccess ? root.textSuccess : root.textError
                                                text: AppController.tenantMessage
                                            }
                                        }
                                        Item { height: 3 }
                                    }
                                }

                                Item { Layout.fillHeight: true }
                            }
                        }

                        // Installation
                        ColumnLayout {
                            id: installPage
                            spacing: 14
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            QtObject {
                                id: installState
                                property bool started: false
                                property bool installDone: false
                                property bool awaitingAuth: false
                                property bool showLogs: false
                                property bool useLogProgress: false
                                property bool cancelPending: false
                                property bool installFailed: false
                                property int elapsedMs: 0
                                property int progressWindowMs: 180000
                                property real maxProgress: 0.95
                                property real progress: 0.0
                                property string statusText: "Ready to Install."
                                property string pullErrorMessage: ""

                                function reset() {
                                    elapsedMs = 0;
                                    progress = 0.0;
                                    statusText = "Ready to Install.";
                                    installDone = false;
                                    started = false;
                                    awaitingAuth = false;
                                    installFailed = false;
                                    showLogs = false;
                                    useLogProgress = false;
                                    cancelPending = false;
                                    pullErrorMessage = "";
                                    root.installationComplete = false;
                                }
                            }

                            Timer {
                                id: installTimer
                                interval: 100
                                repeat: true
                                onTriggered: {
                                    if (installState.useLogProgress)
                                        return;
                                    installState.elapsedMs = Math.min(installState.progressWindowMs, installState.elapsedMs + interval);
                                    var t = installState.elapsedMs / 45000.0;
                                    var eased = installState.maxProgress * (1 - Math.exp(-t));
                                    installState.progress = Math.min(installState.maxProgress, eased);
                                    installState.statusText = "Pulling Docker image...";
                                }
                            }

                            Connections {
                                target: AppController
                                function onCurrentStepChanged() {
                                    if (AppController.currentStep !== 2) {
                                        installTimer.stop();
                                    }
                                }
                                function onDockerPullStarted() {
                                    if (installState.awaitingAuth) {
                                        installState.awaitingAuth = false
                                        installState.started = true
                                        installState.statusText = "Pulling Docker image..."
                                        installState.useLogProgress = true
                                        installState.cancelPending = false
                                        installState.pullErrorMessage = ""
                                        installTimer.start()
                                    }
                                }
                                function onDockerPullProgressChanged() {
                                    if (AppController.dockerPullProgress > 0) {
                                        installState.useLogProgress = true
                                        installState.progress = AppController.dockerPullProgress
                                    }
                                }
                                function onDockerPullFinished(ok, message) {
                                    if (installState.cancelPending) {
                                        installTimer.stop()
                                        AppController.clearDockerPullLog()
                                        installState.reset()
                                        installState.statusText = "Ready to Pull."
                                        return
                                    }
                                    installTimer.stop()
                                    installState.awaitingAuth = false
                                    installState.started = false
                                    installState.progress = ok ? 1.0 : installState.progress
                                    installState.statusText = ok ? message : "Failed to pull docker image."
                                    installState.installDone = ok
                                    installState.pullErrorMessage = ok ? "" : message
                                    root.installationComplete = ok
                                }
                                function onInstallPrereqsRunningChanged() {
                                    if (AppController.installPrereqsRunning) {
                                        installState.showLogs = true
                                        installState.installFailed = false
                                        installState.statusText = "Installing Docker and NVIDIA Container Toolkit..."
                                    } else if (!AppController.installPrereqsDone && !installState.started) {
                                        // Install finished but not done = failed
                                        installState.installFailed = true
                                        installState.showLogs = true  // Keep logs visible to show error
                                        installState.statusText = "Installation failed. Check the log for details."
                                    }
                                }
                                function onInstallPrereqsDoneChanged() {
                                    if (AppController.installPrereqsDone) {
                                        installState.installFailed = false
                                        installState.statusText = "Ready to Pull."
                                    }
                                }
                            }

                            AppCard {
                                Layout.fillWidth: true
                                title: "Installation"
                                subtitle: AppController.installPrereqsDone
                                    ? (installState.started || installState.awaitingAuth
                                        ? "Pulling docker image for installation."
                                        : "Docker and NVIDIA Container Toolkit installed.")
                                    : (installState.installFailed
                                        ? "Installation failed. Check the log and try again."
                                        : "Install Docker and NVIDIA Container Toolkit")
                                contentItem: ColumnLayout {
                                    spacing: 12

                                    RowLayout {
                                        Layout.fillWidth: true
                                        Text {
                                            Layout.fillWidth: true
                                            wrapMode: Text.WordWrap
                                            maximumLineCount: 2
                                            elide: Text.ElideRight
                                            color: root.textSecondary
                                            text: installState.statusText
                                        }
                                        RowLayout {
                                            spacing: 6
                                            Text {
                                                color: root.textInfo
                                                text: Math.round(installState.progress * 100) + "%"
                                            }
                                            Rectangle {
                                                visible: installState.installDone
                                                width: 18
                                                height: 18
                                                radius: width / 2
                                                color: root.accentSuccess
                                                border.color: root.accentSuccessDark
                                                border.width: 1
                                                Text {
                                                    anchors.centerIn: parent
                                                    text: "\u2713"
                                                    color: root.textSuccessDark
                                                    font.pixelSize: 12
                                                    font.bold: true
                                                }
                                            }
                                        }
                                    }

                                    RowLayout {
                                        Layout.fillWidth: true
                                        spacing: 8
                                        ModernProgressBar {
                                            Layout.fillWidth: true
                                            value: installState.progress
                                            busy: installTimer.running
                                        }
                                    }

                                    Text {
                                        Layout.fillWidth: true
                                        visible: installState.pullErrorMessage.length > 0
                                        wrapMode: Text.WordWrap
                                        color: root.accentError
                                        text: installState.pullErrorMessage
                                    }

                                    Item {
                                        id: pullSwap
                                        Layout.fillWidth: true
                                        Layout.preferredHeight: 300
                                        implicitHeight: 300
                                        Layout.bottomMargin: 8
                                        visible: installState.pullErrorMessage.length === 0
                                            && (installState.started
                                                || installState.awaitingAuth
                                                || AppController.installPrereqsRunning
                                                || AppController.installPrereqsDone
                                                || installState.installFailed)

                                        Rectangle {
                                            id: dockerLogPanel
                                            anchors.fill: parent
                                            radius: 12
                                            color: "#050811"
                                            border.color: "#2D3B5F"
                                            border.width: 1
                                            clip: true
                                            property bool stickToBottom: true
                                            function updateStickState() {
                                                if (!dockerLogFlickable)
                                                    return;
                                                var maxY = Math.max(0, dockerLogFlickable.contentHeight - dockerLogFlickable.height);
                                                stickToBottom = dockerLogFlickable.contentY >= maxY - 4;
                                            }
                                            function scrollToBottom() {
                                                if (!dockerLogFlickable)
                                                    return;
                                                dockerLogFlickable.contentY = Math.max(0, dockerLogFlickable.contentHeight - dockerLogFlickable.height);
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
                                                        text: (installState.started || installState.awaitingAuth) ? "🐳" : "⚙"
                                                        color: "#6EE7FF"
                                                        font.pixelSize: 14
                                                    }

                                                    Text {
                                                        text: (installState.started || installState.awaitingAuth) ? "Docker Pull Log" : "Installation Log"
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
                                                        visible: installTimer.running || AppController.installPrereqsRunning

                                                        SequentialAnimation on opacity {
                                                            running: installTimer.running || AppController.installPrereqsRunning
                                                            loops: Animation.Infinite
                                                            NumberAnimation { from: 0.3; to: 1.0; duration: 600 }
                                                            NumberAnimation { from: 1.0; to: 0.3; duration: 600 }
                                                        }
                                                    }

                                                    Text {
                                                        text: (installTimer.running || AppController.installPrereqsRunning) ? "ACTIVE" : "IDLE"
                                                        color: (installTimer.running || AppController.installPrereqsRunning) ? "#22C55E" : "#64748B"
                                                        font.pixelSize: 10
                                                        font.weight: Font.Bold
                                                        font.letterSpacing: 0.5
                                                    }
                                                }
                                            }

                                            Flickable {
                                                id: dockerLogFlickable
                                                anchors.fill: parent
                                                anchors.topMargin: 42
                                                anchors.leftMargin: 12
                                                anchors.rightMargin: 12
                                                anchors.bottomMargin: 12
                                                clip: true
                                                contentWidth: dockerLogText.implicitWidth
                                                contentHeight: dockerLogText.implicitHeight
                                                boundsBehavior: Flickable.StopAtBounds
                                                ScrollBar.horizontal: ScrollBar { policy: ScrollBar.AsNeeded }
                                                ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
                                                onContentYChanged: dockerLogPanel.updateStickState()
                                                onContentHeightChanged: {
                                                    if (dockerLogPanel.stickToBottom)
                                                        dockerLogPanel.scrollToBottom();
                                                }

                                                TextArea {
                                                    id: dockerLogText
                                                    text: (installState.started || installState.awaitingAuth)
                                                        ? (AppController.dockerPullLog.length ? AppController.dockerPullLog : "Waiting for docker output...")
                                                        : (AppController.installPrereqsLog.length
                                                            ? AppController.installPrereqsLog
                                                            : (AppController.installPrereqsRunning || AppController.installPrereqsDone
                                                                ? ""
                                                                : "Waiting for install output..."))
                                                    readOnly: true
                                                    textFormat: TextEdit.PlainText
                                                    wrapMode: TextEdit.NoWrap
                                                    font.family: "JetBrains Mono, Consolas, Monaco, Monospace"
                                                    font.pixelSize: 12
                                                    color: (AppController.dockerPullLog.length || AppController.installPrereqsLog.length) ? "#E0E7FF" : "#64748B"
                                                    background: null
                                                    width: implicitWidth
                                                    height: implicitHeight
                                                    bottomPadding: 10
                                                    rightPadding: 10
                                                    onTextChanged: {
                                                        if (dockerLogPanel.stickToBottom)
                                                            dockerLogPanel.scrollToBottom();
                                                    }
                                                }
                                            }
                                            Component.onCompleted: scrollToBottom()
                                        }
                                    }
                                }
                            }

                        }

                    // Finish
                    ColumnLayout {
                        spacing: 14
                        Layout.fillWidth: true
                        Layout.fillHeight: true

                            AppCard {
                                Layout.fillWidth: true
                                title: "Run"
                                subtitle: "Installation completed"
                                cardSpacing: 6
                                contentItem: ColumnLayout {
                                    spacing: 10
                                    Text {
                                        Layout.fillWidth: true
                                        Layout.topMargin: 1
                                        wrapMode: Text.WordWrap
                                        color: root.textInfo
                                        text: "Safecore has been installed successfully on your device."
                                    }
                                    Item { height: 50 }
                                    CheckBox {
                                        id: launchCheck
                                        text: "Run Safecore"
                                        checked: true
                                        spacing: 10
                                        indicator: null
                                        contentItem: Row {
                                            spacing: 10
                                            Rectangle {
                                                width: 20
                                                height: 20
                                                radius: 4
                                                color: launchCheck.checked ? root.accent : root.bgLog
                                                border.color: launchCheck.checked ? root.textInfo : root.borderPrimary
                                                border.width: 1
                                                Text {
                                                    anchors.centerIn: parent
                                                    text: launchCheck.checked ? "\u2713" : ""
                                                    color: root.bgPrimary
                                                    font.pixelSize: 13
                                                    font.bold: true
                                                }
                                            }
                                            Text {
                                                text: launchCheck.text
                                                color: root.textPrimary
                                                font.pixelSize: 15
                                                verticalAlignment: Text.AlignVCenter
                                            }
                                        }
                                    }
                                    Item { Layout.fillHeight: true }
                                    Text {
                                        Layout.fillWidth: true
                                        Layout.topMargin: 6
                                        wrapMode: Text.WordWrap
                                        color: root.textSecondary
                                        text: "Click Run to Start the SafeCore."
                                        font.pixelSize: 13
                                    }
                                    Item { height: 6 }
                                }
                            }

                            Item { Layout.fillHeight: true }
                }

                Dialog {
                    id: quitDialog
                    modal: true
                    parent: root.contentItem
                    closePolicy: Popup.NoAutoClose
                    title: "Quit Installer"
                    anchors.centerIn: parent
                    implicitWidth: 650
                    implicitHeight: header.height + contentItem.implicitHeight + 24
                    header: Rectangle {
                        height: 44
                        color: root.bgPopup
                        border.color: root.borderPrimary
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
                                color: root.textPrimary
                                font.pixelSize: 16
                                font.bold: true
                            }
                        }
                    }
                    background: Rectangle {
                        radius: 14
                        color: root.bgPopup
                        border.color: root.borderPrimary
                        border.width: 1
                    }

                    property string messageText: "Do you want to quit the installer application?"
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
                            color: root.textSecondary
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
                                    accent: root.buttonSecondary
                                    implicitWidth: 120
                                    implicitHeight: 44
                                    onClicked: quitDialog.close()
                                }
                                AppButton {
                                    text: "Yes"
                                    enabled: true
                                    accent: root.accent
                                    implicitWidth: 120
                                    implicitHeight: 44
                                    onClicked: {
                                        quitDialog.close()
                                        root.allowClose = true
                                        root.close()
                                    }
                                }
                            }
                            Item { height: 4 }
                        }
                    }
                }

                Dialog {
                    id: cancelDialog
                    modal: true
                    parent: root.contentItem
                    closePolicy: Popup.NoAutoClose
                    title: "Cancel Operation"
                    anchors.centerIn: parent
                    implicitWidth: 650
                    implicitHeight: header.height + contentItem.implicitHeight + 24
                    header: Rectangle {
                        height: 44
                        color: root.bgPopup
                        border.color: root.borderPrimary
                        border.width: 1
                        Text {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left
                            anchors.leftMargin: 18
                            text: cancelDialog.title
                            color: root.textPrimary
                            font.pixelSize: 16
                            font.bold: true
                        }
                    }
                    background: Rectangle {
                        radius: 14
                        color: root.bgPopup
                        border.color: root.borderPrimary
                        border.width: 1
                    }
                    contentItem: Item {
                        implicitHeight: cancelBody.implicitHeight
                        implicitWidth: cancelBody.implicitWidth
                        ColumnLayout {
                            id: cancelBody
                            anchors.fill: parent
                            anchors.margins: 18
                            spacing: 12

                            Text {
                                Layout.fillWidth: true
                                wrapMode: Text.WordWrap
                                color: root.textSecondary
                                text: "Do you want to cancel pulling the docker image?"
                            }

                            Item { Layout.fillHeight: true }

                            RowLayout {
                                Layout.fillWidth: true
                                spacing: 10
                                Item { Layout.fillWidth: true }
                                AppButton {
                                    text: "No"
                                    enabled: true
                                    accent: root.buttonSecondary
                                    implicitWidth: 120
                                    implicitHeight: 44
                                    onClicked: cancelDialog.close()
                                }
                                    AppButton {
                                        text: "Yes"
                                        enabled: true
                                        accent: root.accentError
                                        implicitWidth: 120
                                        implicitHeight: 44
                                        onClicked: {
                                            cancelDialog.close()
                                            if (AppController.installPrereqsRunning) {
                                                AppController.cancelInstallPrereqs()
                                                AppController.clearInstallPrereqsLog()
                                                installState.showLogs = false
                                                installState.statusText = "Ready to Install."
                                                installState.pullErrorMessage = "Docker installation canceled."
                                                return
                                            }
                                            // Set cancelPending first, then cancel - let onDockerPullFinished handle the reset
                                            installState.cancelPending = true
                                            installState.statusText = "Canceling..."
                                            AppController.cancelDockerPull()
                                    }
                                }
                            }
                            Item { height: 4 }
                        }
                    }
                }

                Connections {
                    target: AppController
                    function onSyncResult(ok, message) {
                        rightStack.syncSuccess = ok
                        rightStack.syncMessage = message.length
                            ? message
                            : (ok ? "Sync API registration completed successfully." : "Sync API registration failed.")
                        if (ok) {
                            registrationScroll.scheduleScrollToBottom()
                        }
                    }
                    function onDockerRunFinished(ok) {
                        if (root.runDockerAndQuit) {
                            root.allowClose = true
                            Qt.quit()
                        }
                    }
                    function onDockerOpsFinished(ok, message) {
                        if (root.runDockerAndQuit) {
                            root.allowClose = true
                            Qt.quit()
                        }
                    }
                }
            }
        }
    }

    }

    // ---------- Modern ProgressBar ----------
    component ModernProgressBar : Item {
        id: mp
        property real value: 0.0
        property bool busy: false

        implicitHeight: 18
        implicitWidth: 240
        property int pad: 2

        Rectangle {
            id: track
            anchors.fill: parent
            radius: height / 2
            color: root.bgLog
            border.color: root.borderPrimary
            border.width: 1
            clip: true

            Rectangle {
                id: fill
                x: mp.pad
                y: mp.pad
                height: track.height - 2 * mp.pad
                radius: 10
                property real v: Math.max(0.0, Math.min(1.0, mp.value))
                width: Math.max(height, (track.width - 2 * mp.pad) * v)
                gradient: Gradient {
                    GradientStop { position: 0.0; color: root.accent }
                    GradientStop { position: 1.0; color: root.accent2 }
                }
                Behavior on width { NumberAnimation { duration: 220; easing.type: Easing.OutCubic } }
            }

            Rectangle {
                id: dot
                width: 14
                height: 14
                radius: width / 2
                y: (track.height - height) / 2
                color: root.shimmerLight
                property real innerW: (track.width - 2 * mp.pad)
                property real v: Math.max(0.0, Math.min(1.0, mp.value))
                x: {
                    var left = mp.pad;
                    var right = track.width - mp.pad - width;
                    var pos = mp.pad + (innerW * v) - (width / 2);
                    return Math.max(left, Math.min(right, pos));
                }
                Behavior on x { NumberAnimation { duration: 220; easing.type: Easing.OutCubic } }
            }

            Rectangle {
                id: shine
                visible: mp.busy
                y: mp.pad
                height: track.height - 2 * mp.pad
                width: 90
                radius: 10
                opacity: 0.22
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#FFFFFF00" }
                    GradientStop { position: 0.5; color: "#FFFFFFAA" }
                    GradientStop { position: 1.0; color: "#FFFFFF00" }
                }
                x: -width
                SequentialAnimation on x {
                    running: mp.busy
                    loops: Animation.Infinite
                    NumberAnimation {
                        from: -shine.width + 100
                        to: track.width
                        duration: 1100
                        easing.type: Easing.InOutSine
                    }
                }
            }
        }
    }
}
}
