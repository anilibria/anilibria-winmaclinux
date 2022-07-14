import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import Anilibria.ListModels 1.0
import "../Theme"

Item {
    width: parent.width
    height: 330

    property alias releaseId: viewModel.releaseId

    signal openVideo(int videoId)

    ReleaseOnlineSeriesListModel {
        id: viewModel
        releases: releasesViewModel
        onlinePlayer: onlinePlayerViewModel
    }

    Rectangle {
        id: mask
        width: 180
        height: 260
        radius: 8
        visible: false
    }

    ListView {
        anchors.fill: parent
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        model: viewModel
        clip: true
        ScrollBar.horizontal: ScrollBar {
            active: true
        }
        delegate: Item {
            width: 410
            height: 330

            Rectangle {
                id: posterImage
                anchors.top: parent.top
                anchors.topMargin: 8
                anchors.left: parent.left
                anchors.leftMargin: 8
                width: isCurrentVideo ? 404 : 400
                height: isCurrentVideo ? 274 : 272
                border.color: isCurrentVideo ? ApplicationTheme.selectedItem : "#adadad"
                border.width: isCurrentVideo ? 2 : 1
                radius: 14

                property bool posterHovered: false

                Image {
                    anchors.centerIn: parent
                    source: poster
                    sourceSize: Qt.size(350, 500)
                    fillMode: Image.PreserveAspectCrop
                    width: 400
                    height: 270
                    layer.enabled: true
                    layer.effect: OpacityMask {
                        maskSource: mask
                    }
                }

                Image {
                    id: bookmarkImage
                    anchors.left: posterImage.left
                    anchors.leftMargin: -7
                    width: 50
                    height: 50
                    source: assetsLocation.iconsPath + "numberbookmark.svg"
                }

                PlainText {
                    anchors.left: bookmarkImage.left
                    anchors.top: bookmarkImage.top
                    anchors.topMargin: 22
                    width: 40
                    horizontalAlignment: Qt.AlignHCenter
                    text: id
                    fontPointSize: 10
                    color: "white"
                }

                Rectangle {
                    id: blockedContainer
                    visible: posterImage.posterHovered && !isCurrentVideo
                    anchors.fill: parent
                    color: "black"
                    opacity: 0.4
                    radius: 12
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        posterImage.posterHovered = true;
                    }
                    onExited: {
                        posterImage.posterHovered = false;
                    }
                    onPressed: {
                        openVideo(index);
                    }
                }
            }
            AccentText {
                anchors.top: posterImage.bottom
                width: parent.width
                text: isSeens ? "Просмотрено" : currentTimeVideo
                fontPointSize: 10
                maximumLineCount: 1
                elide: Text.ElideRight
                wrapMode: Text.NoWrap
                horizontalAlignment: Qt.AlignHCenter
            }
        }
    }

    EmptyBoxArea {
        visible: viewModel.isEmpty
        anchors.fill: parent
        title: "Нет онлайн серий"
    }
}
