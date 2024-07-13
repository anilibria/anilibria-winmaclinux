import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Controls"

Page {
    id: youtubePage
    anchors.fill: parent
    property int page: 0
    property var allItemsPage

    signal navigateFrom()
    signal navigateTo()

    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
    }

    Loader {
        anchors.fill: parent
        sourceComponent: youtubePage.visible ? youtubePageContent : null
    }

    Component {
        id: youtubePageContent

        Item {
            anchors.fill: parent

            Rectangle {
                id: mask
                width: 472
                height: 240
                radius: 10
                visible: false
            }

            RowLayout {
                id: panelContainer
                anchors.fill: parent
                spacing: 0

                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    spacing: 2

                    Rectangle {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 45
                        height: 45
                        color: applicationThemeViewModel.pageUpperPanel
                    }

                    GridView {
                        id: scrollView
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        cellWidth: parent.width / Math.floor(parent.width / 480)
                        cellHeight: 298
                        model: youtubeViewModel
                        clip: true
                        ScrollBar.vertical: ScrollBar {
                            active: true
                        }
                        delegate: Rectangle {
                            color: "transparent"
                            width: scrollView.cellWidth
                            height: scrollView.cellHeight

                            Rectangle {
                                anchors.left: imageItemContainer.left
                                anchors.top: imageItemContainer.top
                                anchors.leftMargin: 2
                                anchors.topMargin: 2
                                width: 480
                                height: 290
                                radius: 10
                                color: applicationThemeViewModel.currentItems.panelBackgroundShadow
                            }

                            Rectangle {
                                id: imageItemContainer
                                anchors.centerIn: parent
                                width: 480
                                height: 290
                                radius: 10
                                color: applicationThemeViewModel.panelBackground

                                MouseArea {
                                    width: 480
                                    height: 290
                                    onClicked: {
                                        Qt.openUrlExternally(videoUrl);
                                    }
                                }
                                Grid {
                                    columnSpacing: 3
                                    rows: 2
                                    bottomPadding: 4
                                    leftPadding: 4
                                    topPadding: 4
                                    rightPadding: 4

                                    CorneredImage {
                                        width: 472
                                        height: 240
                                        posterSource: image
                                        emptyBorderBackground: applicationThemeViewModel.currentItems.panelBackground
                                    }

                                    PlainText {
                                        Layout.row: 1
                                        width: 460
                                        textFormat: Text.RichText
                                        fontPointSize: 10
                                        leftPadding: 8
                                        topPadding: 4
                                        wrapMode: Text.WordWrap
                                        maximumLineCount: 2
                                        text: title
                                    }

                                }
                            }
                        }
                    }
                }
            }

            Rectangle {
                color: "transparent"
                width: 190
                height: 50
                anchors.right: parent.right
                anchors.rightMargin: 20
                anchors.bottom: parent.bottom

                LeftPanelIconButton {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.topMargin: 8
                    hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                    tooltipMessage: "Вернуться в начало списка релизов"
                    visible: scrollView.contentY > 100
                    iconPath: applicationThemeViewModel.currentItems.iconBackToTop
                    iconWidth: 24
                    iconHeight: 24
                    onButtonPressed: {
                        scrollView.contentY = 0;
                    }

                }
            }
        }
    }
}

