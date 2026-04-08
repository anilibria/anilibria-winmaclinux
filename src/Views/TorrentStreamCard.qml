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
            id: upperPanel
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
                height: 74
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

        ListView {
            id: torrentFilesListView
            anchors.top: upperPanel.bottom
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            model: torrentNotifierViewModel.cardTorrentFiles
            clip: true
            spacing: 4
            ScrollBar.vertical: ScrollBar {
                active: true
            }
            delegate: Item {
                width: torrentFilesListView.width
                height: 200

                Rectangle {
                    anchors.fill: parent
                    anchors.topMargin: 2
                    anchors.leftMargin: 4
                    anchors.rightMargin: 4
                    radius: 10
                    color: applicationThemeViewModel.panelBackground
                }

                CorneredImage {
                    id: posterFileImage
                    anchors.left: parent.left
                    anchors.leftMargin: 12
                    anchors.verticalCenter: parent.verticalCenter
                    posterSource: modelData.poster
                    preserveAspectCrop: Image.PreserveAspectCrop
                    width: 250
                    height: 180
                }

                Item {
                    width: parent.width
                    height: 60
                    anchors.left: posterFileImage.right
                    anchors.leftMargin: 4
                    anchors.verticalCenter: parent.verticalCenter

                    PlainText {
                        id: fileNameText
                        text: modelData.filename
                        fontPointSize: 10
                    }
                    PlainText {
                        id: fileDownloadPercentText
                        anchors.top: fileNameText.bottom
                        text: "Скачано " + modelData.percent + "%"
                        fontPointSize: 10
                    }
                    PlainText {
                        id: fileDownloadSizeText
                        anchors.top: fileDownloadPercentText.bottom
                        text: "На диске " + modelData.size
                        fontPointSize: 10
                    }
                    PlainText {
                        anchors.top: fileDownloadSizeText.bottom
                        text: modelData.description
                        fontPointSize: 9
                    }
                }
            }
        }

    }
}
