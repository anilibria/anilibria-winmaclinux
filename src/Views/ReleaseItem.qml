import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import "../Controls"

Item {
    id: releaseItem
    width: mainViewModel.isSmallSizeMode ? 280 : 480
    height: 280

    property bool isCompactReleaseMode: compactModeSwitch.checked || mainViewModel.isSmallSizeMode

    property string posterPath: localStorage.getReleasePosterPath(id, poster)

    signal leftClicked()
    signal rightClicked()
    signal addToFavorite(int id)
    signal removeFromFavorite(int id)
    signal watchRelease(int id)

    Rectangle {
        visible: !releaseItem.isCompactReleaseMode
        anchors.left: parent.left
        anchors.leftMargin: 2
        anchors.top: parent.top
        anchors.topMargin: 2
        width: releaseItem.width
        height: releaseItem.height
        color: "transparent"
        border.color: applicationThemeViewModel.currentItems.panelBackgroundShadow
        border.width: 2
        radius: 10
    }

    Rectangle {
        anchors.fill: parent
        radius: 10
        border.color: applicationThemeViewModel.currentItems.selectedItem
        border.width: inSelected ? 3 : 0
        color: !releaseItem.isCompactReleaseMode ? applicationThemeViewModel.panelBackground : "transparent"

        Rectangle {
            id: mask
            width: 180
            height: 260
            radius: 10
            visible: false
        }

        MouseArea {
            width: 480
            height: 280
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            hoverEnabled: true
            onClicked: {
                if(mouse.button & Qt.RightButton) {
                    releaseItem.rightClicked();
                    return;
                } else {
                    releaseItem.leftClicked();
                }
            }
            onEntered: {
                let processedDescription = description.replace(`\n`,` `).replace(`\r`,``).replace(/\&quot\;/g,``);
                const tagIndex = processedDescription.indexOf(`<`);
                if (tagIndex > -1) {
                    processedDescription = processedDescription.substring(0, tagIndex);
                }

                if (processedDescription.length > 350) {
                    page.releaseDescription = processedDescription.substring(0, 340) + "...";
                } else {
                    page.releaseDescription = processedDescription;
                }
            }
            onExited: {
                page.releaseDescription = "";
            }
        }
        Grid {
            anchors.topMargin: 10
            columnSpacing: 3
            rowSpacing: 3
            columns: !isCompactReleaseMode ? 2 : 1
            rows: isCompactReleaseMode ? 2 : 1
            bottomPadding: 4
            leftPadding: 4
            topPadding: 4
            rightPadding: 4

            CorneredImage {
                id: itemPosterImage
                visible: !isCompactReleaseMode
                width: 182
                height: 272
                posterSource: releaseItem.posterPath
                emptyBorderBackground: applicationThemeViewModel.currentItems.panelBackground

                Item {
                    visible: startInGroup
                    anchors.right: itemPosterImage.left
                    anchors.top: itemPosterImage.top
                    width: groupHeaderText.width + 12
                    height: 20
                    rotation: 270
                    transformOrigin: Item.Right

                    Rectangle {
                        anchors.fill: parent
                        color: applicationThemeViewModel.pageUpperPanel
                        radius: 8
                        border.color: applicationThemeViewModel.posterBorder
                        border.width: 1
                    }

                    PlainText {
                        id: groupHeaderText
                        anchors.left: parent.left
                        anchors.leftMargin: 6
                        anchors.bottom: parent.bottom
                        anchors.verticalCenter: parent.verticalCenter
                        text: groupValue
                        fontPointSize: 10
                    }
                }
            }

            Item {
                visible: isCompactReleaseMode
                width: mainViewModel.isSmallSizeMode ? 280 : 460
                height: 236

                CorneredImage {
                    width: 152
                    height: 232
                    anchors.centerIn: parent
                    posterSource: releaseItem.posterPath
                    emptyBorderBackground: applicationThemeViewModel.currentItems.pageBackground
                }
            }

            Item {
                visible: isCompactReleaseMode
                width: mainViewModel.isSmallSizeMode ? 280 : 460
                height: 38

                AccentText {
                    fontPointSize: 10
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    width: parent.width
                    leftPadding: 8
                    topPadding: 6
                    wrapMode: Text.WordWrap
                    maximumLineCount: 2
                    text: title
                }
            }

            Grid {
                visible: !isCompactReleaseMode
                height: 280
                Layout.row: 1
                Layout.column: 1
                rows: 2
                columns: 1
                Column {
                    id: gridItemtextContainer
                    AccentText {
                        enabled: false
                        textFormat: Text.RichText
                        fontPointSize: 12
                        width: 280
                        leftPadding: 8
                        topPadding: 6
                        wrapMode: Text.WordWrap
                        maximumLineCount: 3
                        text: qsTr(title)
                    }
                    PlainText {
                        enabled: false
                        visible: id > -1
                        textFormat: Text.RichText
                        fontPointSize: 10
                        leftPadding: 8
                        topPadding: 4
                        text: qsTr(status) + ' - ' + season + " " + year
                    }
                    PlainText {
                        enabled: false
                        visible: id > -1
                        textFormat: Text.RichText
                        fontPointSize: 10
                        leftPadding: 8
                        topPadding: 4
                        width: 280
                        wrapMode: Text.WordWrap
                        maximumLineCount: 2
                        text: qsTr("<b>Тип:</b> ") + type
                    }
                    PlainText {
                        enabled: false
                        visible: id > -1
                        fontPointSize: 10
                        leftPadding: 8
                        topPadding: 4
                        width: 280
                        wrapMode: Text.WordWrap
                        maximumLineCount: 2
                        text: qsTr("<b>Жанры:</b> ") + genres
                    }
                    PlainText {
                        enabled: false
                        visible: id > -1
                        fontPointSize: 10
                        leftPadding: 8
                        topPadding: 4
                        width: 280
                        wrapMode: Text.WordWrap
                        maximumLineCount: 2
                        text: qsTr("<b>Команда:</b> ") + voices
                    }
                    Row {
                        visible: id > -1
                        leftPadding: 8
                        topPadding: 4
                        Item {
                            width: 22
                            height: 22

                            Rectangle {
                                id: onlineRectangle
                                anchors.fill: parent
                                color: "transparent"
                                opacity: .5
                            }

                            TooltipedImage {
                                source: applicationThemeViewModel.currentItems.iconReleaseCatalogOnline
                                width: 22
                                height: 22

                                property bool imageHovered: false

                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onPressed: {
                                        releaseItem.watchRelease(id);
                                    }
                                    onEntered: {
                                        onlineRectangle.color = applicationThemeViewModel.panelBackgroundShadow;
                                        parent.imageHovered = true;
                                    }
                                    onExited: {
                                        onlineRectangle.color = "transparent";
                                        parent.imageHovered = false;
                                    }
                                }

                                tooltipMessage: `Начать просмотр этого релиза`
                                tooltipMessageVisible: imageHovered
                            }
                        }
                        PlainText {
                            leftPadding: 4
                            rightPadding: 8
                            fontPointSize: 12
                            enabled: false
                            text: '' + countVideos + (countSeensSeries > 0 ? "<font color='" + applicationThemeViewModel.currentItems.colorSeenReleaseItemText + "'>/" + countSeensSeries + "</font>  " : "")
                        }

                        Item {
                            id: torrentMenuContainer
                            width: 22
                            height: 22

                            property int torrentReleaseId: 0

                            Rectangle {
                                id: torrentsRectangle
                                anchors.fill: parent
                                color: "transparent"
                                opacity: .5
                                radius: 8
                            }

                            TooltipedImage {
                                anchors.fill: parent
                                source: applicationThemeViewModel.currentItems.iconReleaseCatalogTorrent

                                property bool torrentImageHovered: false

                                MouseArea {
                                    anchors.fill: parent
                                    hoverEnabled: true
                                    onPressed: {
                                        releasesViewModel.prepareTorrentsForListItem(id);
                                        torrentMenuContainer.torrentReleaseId = id;
                                        torrentMenuLoader.sourceComponent = torrentMenuComponent;
                                    }
                                    onEntered: {
                                        torrentsRectangle.color = applicationThemeViewModel.panelBackgroundShadow;
                                        parent.torrentImageHovered = true;
                                    }
                                    onExited: {
                                        torrentsRectangle.color = "transparent";
                                        parent.torrentImageHovered = false;
                                    }
                                }

                                tooltipMessage: `Показать все торренты для этого релиза`
                                tooltipMessageVisible: torrentImageHovered

                                Loader {
                                    id: torrentMenuLoader
                                    onLoaded: {
                                        item.open();
                                    }
                                }

                                Component {
                                    id: torrentMenuComponent

                                    CommonMenu {
                                        id: torrentsMenu
                                        autoWidth: true

                                        Repeater {
                                            model: releasesViewModel.itemTorrents
                                            delegate: CommonMenuItem {
                                                text: "Скачать " + quality + " [" + series + "] " + size + " " + timecreation
                                                onPressed: {
                                                    synchronizationServicev2.downloadTorrent(url, torrentMenuContainer.torrentReleaseId);
                                                    torrentsMenu.close();

                                                    if (userConfigurationViewModel.markAsReadAfterDownload) {
                                                        releasesViewModel.setSeenMarkForSingleRelease(torrentMenuContainer.torrentReleaseId, true);
                                                    }
                                                }
                                            }
                                        }

                                        onClosed: {
                                            torrentMenuLoader.sourceComponent = null;
                                        }
                                    }
                                }
                            }
                        }
                        PlainText {
                            leftPadding: 4
                            rightPadding: 8
                            fontPointSize: 12
                            text: '' + countTorrents
                        }
                        TooltipedImage {
                            visible: inSchedule
                            source: applicationThemeViewModel.currentItems.iconReleaseCatalogSchedule
                            width: 22
                            height: 22
                        }
                        PlainText {
                            visible: inSchedule
                            leftPadding: 8
                            topPadding: 1
                            fontPointSize: 11
                            text: scheduledDay
                        }
                    }
                }
                Rectangle {
                    visible: id > -1
                    color: "transparent"
                    height: 272 - gridItemtextContainer.height
                    width: 280

                    Item {
                        id: favoritesItem
                        anchors.bottom: parent.bottom
                        anchors.left: parent.left
                        anchors.leftMargin: 8
                        width: 90
                        height: 20

                        property bool favoritesImageHovered

                        Rectangle {
                            anchors.left: parent.left
                            width: 18 + countFavoritesLabel.width
                            height: 20
                            color: favoritesItem.favoritesImageHovered ? applicationThemeViewModel.panelBackgroundShadow : "transparent"
                            opacity: .5
                            radius: 8
                        }

                        Row {
                            anchors.left: parent.left

                            TooltipedImage {
                                source: applicationThemeViewModel.currentItems.iconFavorites
                                width: 18
                                height: 20

                                tooltipMessage: inFavorites ? `Удалить из избранного` : `Добавить в избранное`
                                tooltipMessageVisible: favoritesItem.favoritesImageHovered
                            }
                            PlainText {
                                id: countFavoritesLabel
                                leftPadding: 4
                                rightPadding: 4
                                fontPointSize: 12
                                text: rating
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            onEntered: {
                                parent.favoritesImageHovered = true;
                            }
                            onExited: {
                                parent.favoritesImageHovered = false;
                            }
                            onPressed: {
                                if (inFavorites) {
                                    releaseItem.removeFromFavorite(id);
                                } else {
                                    releaseItem.addToFavorite(id);
                                }
                            }
                        }
                    }

                    PlainText {
                        visible: inFavorites
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.rightMargin: 4
                        color: applicationThemeViewModel.headerTextColor
                        fontPointSize: 12
                        text: "В избранном"
                    }
                }
            }
        }
    }
}

