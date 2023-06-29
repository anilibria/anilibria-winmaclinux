import QtQuick 2.12
import QtQuick.Controls 2.12
import Anilibria.ListModels 1.0

Item {
    id: root
    width: parent.width
    height: 330

    property alias releaseId: viewModel.releaseId
    property alias isEmpty: viewModel.isEmpty

    signal openRelease(int releaseId)

    ReleaseSeriesListModel {
        id: viewModel
        linkedSeries: releaseLinkedSeries
    }

    Rectangle {
        id: mask
        width: 180
        height: 260
        radius: 10
        visible: false
    }

    ListView {
        id: seriesListView
        anchors.fill: parent
        orientation: ListView.Horizontal
        boundsBehavior: Flickable.StopAtBounds
        model: viewModel
        clip: true
        ScrollBar.horizontal: ScrollBar {
            active: true
        }
        delegate: Item {
            width: 200
            height: 300

            Item {
                id: posterImage
                anchors.top: parent.top
                anchors.topMargin: 8
                anchors.left: parent.left
                anchors.leftMargin: 8
                width: isSelected ? 184 : 182
                height: isSelected ? 274 : 272

                property bool posterHovered: false
                property bool buttonHovered: false

                CorneredImage {
                    anchors.centerIn: parent
                    posterSource: localStorage.getReleasePosterPath(id, poster)
                    width: 180
                    height: 270
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
                    text: releaseNumber
                    fontPointSize: 10
                    color: applicationThemeViewModel.colorPageIndexText
                }

                Rectangle {
                    id: blockedContainer
                    visible: posterImage.posterHovered && !isSelected
                    anchors.fill: parent
                    color: "black"
                    opacity: 0.4
                }

                Rectangle {
                    visible: isSelected
                    anchors.centerIn: posterImage
                    color: "#adadad"
                    width: 48
                    height: 48
                    radius: 25
                }

                Image {
                    id: starImage
                    visible: isSelected
                    anchors.centerIn: posterImage
                    width: 40
                    height: 40
                    mipmap: true
                    source: assetsLocation.iconsPath + "coloredstar.svg"
                    SequentialAnimation {
                        ScaleAnimator {
                            target: starImage
                            from: 1
                            to: .7
                            duration: 2000
                        }
                        ScaleAnimator {
                            target: starImage
                            from: .7
                            to: 1
                            duration: 2000
                        }
                        loops: Animation.Infinite
                        running: starImage.visible && releasesViewModel.isOpenedCard
                    }
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
                        openRelease(id);
                    }
                }
            }
            AccentText {
                anchors.top: posterImage.bottom
                width: parent.width
                text: title
                fontPointSize: 10
                font.bold: isSelected
                maximumLineCount: 2
                elide: Text.ElideRight
                wrapMode: Text.Wrap
                horizontalAlignment: Qt.AlignHCenter
            }
        }
    }

    NavigationScrollButton {
        visible: seriesListView.contentX > 100
        anchors.left: parent.left
        anchors.leftMargin: 4
        anchors.top: parent.top
        anchors.topMargin: 120
        scrollView: seriesListView
        isLeft: true
    }

    NavigationScrollButton {
        visible: !seriesListView.atXEnd
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.top: parent.top
        anchors.topMargin: 120
        scrollView: seriesListView
        isLeft: false
    }

    EmptyBoxArea {
        visible: viewModel.isEmpty
        anchors.fill: parent
        title: "Нет связанных релизов"
    }
}
