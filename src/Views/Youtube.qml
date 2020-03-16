import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import "../Controls"

Page {
    id: youtubePage
    anchors.fill: parent
    property int page: 0
    property var allItemsPage

    signal navigateFrom()
    signal navigateTo()

    onNavigateTo: {
        const youtubeVideos = localStorage.getYoutubeItems();
        const items = JSON.parse(youtubeVideos);
        youtubePage.allItemsPage = items;

        youtubeVideosModel.clear();
        for (var i = 0; i < 20; i++) youtubeVideosModel.append({ model: items[i] });
    }

    onWidthChanged: {
        const columnCount = parseInt(youtubePage.width / 520);
        itemGrid.columns = columnCount < 1 ? 1 : columnCount;
    }

    background: Rectangle {
        color: "#D3D3D3"
    }

    Rectangle {
        id: mask
        width: 472
        height: 240
        radius: 10
        visible: false
    }

    ListModel {
        id: youtubeVideosModel
    }

    RowLayout {
        id: panelContainer
        anchors.fill: parent
        spacing: 0
        Rectangle {
            color: "#9e2323"
            width: 40
            Layout.fillHeight: true
            Column {
                IconButton {
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/menu.svg"
                    iconWidth: 29
                    iconHeight: 29
                    onButtonPressed: {
                        drawer.open();
                    }
                }
            }
        }

        ColumnLayout {
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 2

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                height: 45
                color: "#808080"
            }

            Flickable {
                id: scrollview
                Layout.fillHeight: true
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignJustify
                clip: true
                contentWidth: parent.width
                contentHeight: itemGrid.height
                onContentYChanged: {
                    if (scrollview.atYEnd) youtubePage.fillNextPage();
                }
                ScrollBar.vertical: ScrollBar {
                    active: true
                }

                ColumnLayout {
                    width: youtubePage.width
                    height: youtubePage.height
                    Grid {
                        id: itemGrid
                        Layout.alignment: Qt.AlignHCenter
                        columns: 2
                        spacing: 4
                        Repeater {
                            model: youtubeVideosModel
                            Rectangle {
                                width: 480
                                height: 290
                                radius: 10
                                color: "#f2f2f2"
                                layer.enabled: true
                                layer.effect: DropShadow {
                                    transparentBorder: true
                                    horizontalOffset: 2
                                    verticalOffset: 2
                                    radius: 1
                                    samples: 3
                                    color: "gray"
                                }
                                MouseArea {
                                    width: 480
                                    height: 290
                                    onClicked: {
                                        Qt.openUrlExternally("https://www.youtube.com/watch?v=" + modelData.vid);
                                    }
                                }
                                Grid {
                                    columnSpacing: 3
                                    rows: 2
                                    bottomPadding: 4
                                    leftPadding: 4
                                    topPadding: 4
                                    rightPadding: 4

                                    Image {
                                        source: synchronizationService.combineWithWebSiteUrl(modelData.image)
                                        fillMode: Image.PreserveAspectCrop
                                        width: 472
                                        height: 240
                                        layer.enabled: true
                                        layer.effect: OpacityMask {
                                            maskSource: mask
                                        }
                                    }

                                    Text {
                                        Layout.row: 1
                                        width: 460
                                        textFormat: Text.RichText
                                        font.pointSize: 10
                                        leftPadding: 8
                                        topPadding: 4
                                        wrapMode: Text.WordWrap
                                        maximumLineCount: 2
                                        text: qsTr(modelData.title)
                                    }

                                }
                            }
                        }
                    }
                }
            }
        }
    }

    function fillNextPage() {
        youtubePage.page += 1;
        const start = youtubePage.page * 20;
        let end = start + 20;
        if (end < youtubePage.allItemsPage) end = youtubePage.allItemsPage - start;

        for (var i = start; i < end; i++) youtubeVideosModel.append({ model: youtubePage.allItemsPage[i] });
    }

}

