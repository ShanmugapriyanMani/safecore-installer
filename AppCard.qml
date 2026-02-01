import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: card

    property string title: ""
    property string subtitle: ""
    property Item contentItem: null
    property bool showSubtitle: true

    // subtitle controls (so Status can be 1-line with "...")
    property bool subtitleWrap: true
    property int subtitleMaxLines: 2
    property int subtitleElide: Text.ElideRight
    property int cardSpacing: 10
    property int titlePixelSize: 16

    radius: 16
    color: "#0A1022"
    border.color: "#1F2A4A"
    border.width: 1

    // IMPORTANT: don't force implicitHeight always; allow Layout.* to control height
    implicitHeight: col.implicitHeight + 24

    ColumnLayout {
        id: col
        anchors.fill: parent
        anchors.margins: 16
        spacing: cardSpacing

        Text {
            Layout.fillWidth: true
            text: card.title
            color: "white"
            font.pixelSize: titlePixelSize
            font.bold: true
            elide: Text.ElideRight
            maximumLineCount: 1
        }

        Text {
            Layout.fillWidth: true
            text: card.subtitle
            color: "#B8C2E0"
            font.pixelSize: 12

            wrapMode: card.subtitleWrap ? Text.WordWrap : Text.NoWrap
            maximumLineCount: card.subtitleMaxLines
            elide: card.subtitleElide
            visible: card.showSubtitle && card.subtitle.length > 0
        }

        // Content holder: height = content implicitHeight (NO EXTRA GAP)
        Item {
            id: contentHolder
            Layout.fillWidth: true
            visible: card.contentItem !== null
            implicitHeight: (card.contentItem ? card.contentItem.implicitHeight : 0)

            Component.onCompleted: {
                if (card.contentItem) {
                    card.contentItem.parent = contentHolder
                    card.contentItem.anchors.fill = contentHolder
                }
            }
        }
    }
}
