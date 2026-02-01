import QtQuick

Item {
    id: progressBar

    property real value: 0.0
    property bool busy: false
    property color accentColor: Theme.accent
    property color accentColor2: Theme.accent2

    implicitHeight: 18
    implicitWidth: 240

    property int _pad: 2

    Rectangle {
        id: track
        anchors.fill: parent
        radius: height / 2
        color: Theme.bgControlPanel
        border.color: Theme.borderPrimary
        border.width: 1
        clip: true

        Rectangle {
            id: fill
            x: progressBar._pad
            y: progressBar._pad
            height: track.height - 2 * progressBar._pad
            radius: 10

            property real v: Math.max(0.0, Math.min(1.0, progressBar.value))
            width: Math.max(height, (track.width - 2 * progressBar._pad) * v)

            gradient: Gradient {
                GradientStop { position: 0.0; color: progressBar.accentColor }
                GradientStop { position: 1.0; color: progressBar.accentColor2 }
            }

            Behavior on width {
                NumberAnimation { duration: 220; easing.type: Easing.OutCubic }
            }
        }

        Rectangle {
            id: dot
            width: 14
            height: 14
            radius: width / 2
            y: (track.height - height) / 2
            color: "#FFFFFF"

            property real innerW: (track.width - 2 * progressBar._pad)
            property real v: Math.max(0.0, Math.min(1.0, progressBar.value))

            x: {
                var left = progressBar._pad;
                var right = track.width - progressBar._pad - width;
                var pos = progressBar._pad + (innerW * v) - (width / 2);
                return Math.max(left, Math.min(right, pos));
            }

            Behavior on x {
                NumberAnimation { duration: 220; easing.type: Easing.OutCubic }
            }
        }

        Rectangle {
            id: shine
            visible: progressBar.busy
            y: progressBar._pad
            height: track.height - 2 * progressBar._pad
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
                running: progressBar.busy
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
