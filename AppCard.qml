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
    property int titlePixelSize: Theme.fontSizeTitle

    radius: Theme.cardRadius
    color: Theme.bgLog
    border.color: Theme.borderPrimary
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
            color: Theme.textPrimary
            font.pixelSize: titlePixelSize
            font.bold: true
            elide: Text.ElideRight
            maximumLineCount: 1
        }

        Text {
            Layout.fillWidth: true
            text: card.subtitle
            color: Theme.textSecondary
            font.pixelSize: Theme.fontSizeNormal

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
