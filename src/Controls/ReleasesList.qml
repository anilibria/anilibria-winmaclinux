import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import Anilibria.ListModels 1.0

Item {
    width: parent.width
    height: 330

    property alias filterMode: viewModel.filterMode
    property alias hasItems: viewModel.hasItems
    property alias countItems: viewModel.countItems

    ReleaseSimpleListModel {
        id: viewModel
        releases: releasesViewModel
    }

    Rectangle {
        id: mask
        width: 180
        height: 260
        radius: 10
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
            width: 200
            height: 300

            Rectangle {
                id: posterImage
                anchors.top: parent.top
                anchors.topMargin: 8
                anchors.left: parent.left
                anchors.leftMargin: 8
                width: 182
                height: 272
                border.color: "#adadad"
                border.width: 1
                radius: 12

                property bool posterHovered: false
                property bool buttonHovered: false

                Image {
                    anchors.centerIn: parent
                    source: localStorage.getReleasePosterPath(id, poster)
                    sourceSize: Qt.size(350, 500)
                    fillMode: Image.PreserveAspectCrop
                    width: 180
                    height: 270
                    layer.enabled: true
                    layer.effect: OpacityMask {
                        maskSource: mask
                    }
                }

                Rectangle {
                    id: blockedContainer
                    visible: posterImage.posterHovered
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
                }

                RoundedActionButton {
                    visible: posterImage.posterHovered
                    width: 150
                    anchors.centerIn: parent
                    text: "Смотреть"
                    onClicked: {
                        mainViewModel.selectPage("videoplayer");
                        onlinePlayerViewModel.quickSetupForSingleRelease(id);
                    }
                    onButtonHoveredChanged: {
                        posterImage.posterHovered = buttonHovered;
                    }
                }
            }
            AccentText {
                anchors.top: posterImage.bottom
                width: parent.width
                text: title
                fontPointSize: 10
                maximumLineCount: 2
                elide: Text.ElideRight
                wrapMode: Text.Wrap
                horizontalAlignment: Qt.AlignHCenter
            }
        }
    }
}
