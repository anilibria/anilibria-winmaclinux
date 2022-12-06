import QtQuick 2.12
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import Anilibria.ListModels 1.0

Item {
    id: root
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
        id: releasesListView
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
                border.color: applicationThemeViewModel.posterBorder
                border.width: 1
                radius: 12

                property bool posterHovered: false

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

                Item {
                    anchors.centerIn: parent
                    visible: posterImage.posterHovered
                    width: 150
                    height: 28

                    Rectangle {
                        width: 150
                        height: parent.height
                        color: applicationThemeViewModel.roundedButtonBackground
                        border.color: "transparent"
                        border.width: 1
                        radius: 18

                        PlainText {
                            id: buttonTitle
                            fontPointSize: 11
                            anchors.centerIn: parent
                            color: applicationThemeViewModel.roundedButtonForeground
                            text: "Смотреть"
                        }
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    hoverEnabled: true
                    onEntered: {
                        posterImage.posterHovered = true;
                        myAnilibriaViewModel.hoveredDescription = description
                    }
                    onExited: {
                        posterImage.posterHovered = false;
                        myAnilibriaViewModel.hoveredDescription = ""
                    }
                    onPressed: {
                        mainViewModel.selectPage("videoplayer");
                        onlinePlayerViewModel.quickSetupForSingleRelease(id);
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

    NavigationScrollButton {
        visible: root.height > 0 && releasesListView.contentX > 100
        anchors.left: parent.left
        anchors.leftMargin: 4
        anchors.top: parent.top
        anchors.topMargin: 120
        scrollView: releasesListView
        isLeft: true
    }

    NavigationScrollButton {
        visible: root.height > 0 && !releasesListView.atXEnd
        anchors.right: parent.right
        anchors.rightMargin: 4
        anchors.top: parent.top
        anchors.topMargin: 120
        scrollView: releasesListView
        isLeft: false
    }
}
