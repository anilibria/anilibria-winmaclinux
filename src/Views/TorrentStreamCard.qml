import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Controls"

ColumnLayout {
    id: cardContainer
    anchors.fill: parent
    spacing: 0
    Rectangle {
        color: applicationThemeViewModel.currentItems.pageBackground
        Layout.fillWidth: true
        Layout.fillHeight: true

        Rectangle {
            width: parent.width
            height: 220
            anchors.right: parent.right
            color: applicationThemeViewModel.currentItems.pageUpperPanel

            CorneredImage {
                id: posterImage
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.leftMargin: 4
                anchors.topMargin: 4
                posterSource: torrentNotifierViewModel.cardReleaseId > 0 ? releasesViewModel.getReleasePoster(torrentNotifierViewModel.cardReleaseId) : ""
                width: 140
                height: 210
            }

            Item {
                width: parent.width
                height: 50
                anchors.left: posterImage.right
                anchors.verticalCenter: parent.verticalCenter

                AccentText {
                    id: releaseNameText
                    anchors.left: parent.left
                    anchors.leftMargin: 4
                    anchors.top: parent.top
                    text: torrentNotifierViewModel.cardReleaseName
                    fontPointSize: 11
                }

                PlainText {
                    id: downloadStatusText
                    anchors.left: parent.left
                    anchors.top: releaseNameText.bottom
                    anchors.leftMargin: 4
                    anchors.topMargin: 2
                    text: torrentNotifierViewModel.cardDownloadStatus
                    fontPointSize: 10
                }
                PlainText {
                    id: downloadSizeText
                    anchors.left: parent.left
                    anchors.top: downloadStatusText.bottom
                    anchors.leftMargin: 4
                    anchors.topMargin: 2
                    text: "На диске " + torrentNotifierViewModel.cardDownloadSize
                    fontPointSize: 10
                }
                PlainText {
                    anchors.left: parent.left
                    anchors.top: downloadSizeText.bottom
                    anchors.leftMargin: 4
                    anchors.topMargin: 2
                    text: torrentNotifierViewModel.cardDownloadFileStatus
                    fontPointSize: 10
                }
            }
        }

    }
}
