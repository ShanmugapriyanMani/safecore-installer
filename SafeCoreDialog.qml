import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Dialog {
    id: dialog

    property string dialogTitle: "Dialog"
    property string iconSource: ""
    property Item dialogContent: null
    property var buttons: []  // Array of {text, accent, visible, enabled, onClicked}
    property bool canClose: true

    modal: true
    closePolicy: canClose ? Popup.CloseOnEscape : Popup.NoAutoClose
    anchors.centerIn: parent
    implicitWidth: Theme.dialogWidth
    implicitHeight: dialogHeader.height + contentItem.implicitHeight + 24

    function blinkDialog() {
        blinkAnimation.start()
    }

    SequentialAnimation {
        id: blinkAnimation
        NumberAnimation {
            target: dialog
            property: "opacity"
            from: 1.0
            to: 0.3
            duration: 100
        }
        NumberAnimation {
            target: dialog
            property: "opacity"
            from: 0.3
            to: 1.0
            duration: 100
        }
    }

    header: Rectangle {
        id: dialogHeader
        height: Theme.dialogHeaderHeight
        color: Theme.bgPopup
        border.color: Theme.borderPrimary
        border.width: 1

        Row {
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            anchors.leftMargin: 18
            spacing: 8

            Image {
                anchors.verticalCenter: parent.verticalCenter
                source: dialog.iconSource
                width: 18
                height: 18
                fillMode: Image.PreserveAspectFit
                smooth: true
                sourceSize.width: 18
                sourceSize.height: 18
                visible: dialog.iconSource.length > 0
            }

            Text {
                anchors.verticalCenter: parent.verticalCenter
                text: dialog.dialogTitle
                color: Theme.textPrimary
                font.pixelSize: Theme.fontSizeTitle
                font.bold: true
            }
        }
    }

    background: Rectangle {
        radius: Theme.dialogRadius
        color: Theme.bgPopup
        border.color: Theme.borderPrimary
        border.width: 1
    }

    contentItem: Item {
        implicitHeight: bodyLayout.implicitHeight
        implicitWidth: bodyLayout.implicitWidth

        ColumnLayout {
            id: bodyLayout
            anchors.fill: parent
            anchors.margins: 18
            spacing: 12

            // Content holder
            Item {
                id: contentHolder
                Layout.fillWidth: true
                visible: dialog.dialogContent !== null
                implicitHeight: dialog.dialogContent ? dialog.dialogContent.implicitHeight : 0

                Component.onCompleted: {
                    if (dialog.dialogContent) {
                        dialog.dialogContent.parent = contentHolder
                        dialog.dialogContent.anchors.fill = contentHolder
                    }
                }
            }

            Item { Layout.fillHeight: true }

            // Buttons row
            RowLayout {
                Layout.fillWidth: true
                spacing: 10
                Item { Layout.fillWidth: true }

                Repeater {
                    model: dialog.buttons

                    AppButton {
                        text: modelData.text || ""
                        accent: modelData.accent || Theme.accent
                        visible: modelData.visible !== undefined ? modelData.visible : true
                        enabled: modelData.enabled !== undefined ? modelData.enabled : true
                        implicitWidth: Theme.buttonWidth
                        implicitHeight: Theme.buttonHeight
                        onClicked: {
                            if (modelData.onClicked)
                                modelData.onClicked()
                        }
                    }
                }
            }

            Item { height: 4 }
        }
    }
}
