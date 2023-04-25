/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2020 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

import QtQuick 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.12
import QtGraphicalEffects 1.0
import "../Controls"

Rectangle {
    id: releaseItem
    width: mainViewModel.isSmallSizeMode ? 280 : 480
    height: 280
    radius: 10
    border.color: applicationThemeViewModel.selectedItem
    border.width: inSelected ? 3 : 0
    color: !isCompactReleaseMode ? applicationThemeViewModel.panelBackground : "transparent"
    layer.enabled: !isCompactReleaseMode
    layer.effect: DropShadow {
        transparentBorder: true
        horizontalOffset: 2
        verticalOffset: 2
        radius: 1
        samples: 3
        color: applicationThemeViewModel.panelBackgroundShadow
    }

    property bool isCompactReleaseMode: compactModeSwitch.checked || mainViewModel.isSmallSizeMode

    property string posterPath: localStorage.getReleasePosterPath(id, poster)

    signal leftClicked()
    signal rightClicked()
    signal addToFavorite(int id)
    signal removeFromFavorite(int id)
    signal watchRelease(int id, string videos, string poster)

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

        Rectangle {
            visible: id > -1 && !isCompactReleaseMode
            width: 182
            height: 272
            border.color: applicationThemeViewModel.posterBorder
            border.width: 1
            radius: 12

            Image {
                anchors.centerIn: parent
                source: releaseItem.posterPath
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
                anchors.centerIn: parent
                width: 180
                height: 270
                color: applicationThemeViewModel.currentItems.colorPosterFilter
                radius: 12
            }
        }

        Rectangle {
            visible: isCompactReleaseMode
            width: mainViewModel.isSmallSizeMode ? 280 : 460
            height: 236
            color: "transparent"

            Rectangle {
                visible: id > -1
                anchors.horizontalCenter: parent.horizontalCenter
                width: 152
                height: 232
                border.color: applicationThemeViewModel.posterBorder
                border.width: 1
                radius: 12

                Image {
                    anchors.centerIn: parent
                    source: releaseItem.posterPath
                    sourceSize: Qt.size(350, 500)
                    fillMode: Image.PreserveAspectCrop
                    width: 150
                    height: 230
                    layer.enabled: true
                    layer.effect: OpacityMask {
                        maskSource: mask
                    }
                }

                Rectangle {
                    anchors.centerIn: parent
                    width: 150
                    height: 230
                    color: applicationThemeViewModel.currentItems.colorPosterFilter
                    radius: 12
                }

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
                    text: qsTr("<b>Озвучка:</b> ") + voices
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
                                    releaseItem.watchRelease(id, videos, poster);
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
                                    width: 320

                                    Repeater {
                                        model: releasesViewModel.itemTorrents
                                        delegate: CommonMenuItem {
                                            text: "Скачать " + quality + " [" + series + "] " + size
                                            onPressed: {
                                                releasesViewModel.itemTorrents.downloadTorrent(currentIndex);
                                                torrentsMenu.close();

                                                userActivityViewModel.addDownloadedTorrentToCounter();
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
                    Image {
                        visible: inSchedule
                        source: applicationThemeViewModel.currentItems.iconReleaseCatalogSchedule
                        width: 22
                        height: 22
                        mipmap: true
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
