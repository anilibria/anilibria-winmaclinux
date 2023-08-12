import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
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
            width: 42
            height: parent.height
            anchors.right: parent.right
            color: applicationThemeViewModel.currentItems.pageUpperPanel
        }

        Column {
            id: cardButtons
            width: 42
            anchors.right: parent.right
            IconButton {
                height: 40
                width: 40
                hoverColor: applicationThemeViewModel.currentItems.filterIconButtonHoverColor
                iconPath: applicationThemeViewModel.currentItems.iconMainMenuVideoplayer
                iconWidth: 26
                iconHeight: 26
                tooltipMessage: "Смотреть сразу все релизы"
                onButtonPressed: {

                }
            }
        }

        ListView {
            id: releaseSeriesCardListView
            width: parent.width - cardButtons.width
            height: parent.height - 4
            anchors.top: parent.top
            anchors.topMargin: 2
            model: releaseLinkedSeries.cardList
            clip: true
            spacing: 4
            ScrollBar.vertical: ScrollBar {
                active: true
            }
            delegate: Rectangle {
                width: itemContainer.width
                height: 220
                color: "transparent"

                Rectangle {
                    anchors.fill: parent
                    anchors.leftMargin: 4
                    anchors.rightMargin: 47
                    radius: 10
                    color: applicationThemeViewModel.panelBackground

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            if (!releaseLinkedSeries.isCardShowed) releaseLinkedSeries.selectByIndex(identifier);
                        }
                    }

                    RowLayout {
                        anchors.fill: parent

                        Rectangle {
                            color: "transparent"
                            height: parent.height
                            Layout.topMargin: 6
                            Layout.preferredWidth: 150
                            Layout.fillHeight: true
                            Layout.leftMargin: 6

                            CorneredImage {
                                anchors.top: parent.top
                                anchors.left: parent.left
                                anchors.leftMargin: 2
                                posterSource: localStorage.getReleasePosterPath(releaseId, poster)
                                width: 140
                                height: 210
                            }
                        }

                        Rectangle {
                            height: parent.height
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                            color: "transparent"

                            ColumnLayout {
                                spacing: 4
                                anchors.fill: parent
                                anchors.leftMargin: 10

                                Item {
                                    Layout.fillHeight: true
                                }

                                AccentText {
                                    fontPointSize: 10
                                    text: title
                                    Layout.fillWidth: true
                                    maximumLineCount: 2
                                    elide: Text.ElideRight
                                    wrapMode: Text.Wrap
                                }

                                PlainText {
                                    Layout.preferredHeight: inFavorites ? 20 : 0
                                    Layout.fillWidth: true
                                    maximumLineCount: 2
                                    elide: Text.ElideRight
                                    wrapMode: Text.Wrap
                                    fontPointSize: 9
                                    text: inFavorites ? "В избранном" : ""
                                }

                                Item {
                                    Layout.fillHeight: true
                                }
                            }
                        }
                        Rectangle {
                            id: rightBlock
                            Layout.preferredWidth: 200
                            Layout.fillHeight: true
                            Layout.rightMargin: 6
                            height: parent.height
                            color: "transparent"

                            Column {
                                anchors.centerIn: parent

                                FilterPanelIconButton {
                                    iconPath: applicationThemeViewModel.currentItems.iconFavorites
                                    tooltipMessage: "Открыть меню для операций по добавлению/удалению всей группы в избранное"
                                    onButtonPressed: {
                                        favoriteSeriesMenu.open();
                                    }

                                    CommonMenu {
                                        id: favoriteSeriesMenu
                                        y: parent.y
                                        width: 300

                                        CommonMenuItem {
                                            enabled: !!window.userModel.login
                                            text: "Добавить в избранное"
                                            onPressed: {
                                                releasesViewModel.addReleaseToFavorites(releaseId);

                                                favoriteSeriesMenu.close();
                                            }
                                        }
                                        CommonMenuItem {
                                            enabled: !!window.userModel.login
                                            text: "Удалить из избранного"
                                            onPressed: {
                                                releasesViewModel.removeReleaseFromFavorites(releaseId);

                                                favoriteSeriesMenu.close();
                                            }
                                        }
                                    }
                                }
                                FilterPanelIconButton {
                                    iconPath: applicationThemeViewModel.currentItems.iconMainMenuCinemahall
                                    tooltipMessage: "Открыть меню для операций по добавлению/удалению всей группы в кинозал"
                                    onButtonPressed: {
                                        cinemahallSeriesMenu.open();
                                    }

                                    CommonMenu {
                                        id: cinemahallSeriesMenu
                                        y: parent.y
                                        width: 300

                                        CommonMenuItem {
                                            text: "Добавить в кинозал"
                                            onPressed: {
                                                //releasesViewModel.cinemahall.addReleases(releaseIds);

                                                cinemahallSeriesMenu.close();
                                            }
                                        }
                                    }
                                }
                                FilterPanelIconButton {
                                    iconPath: applicationThemeViewModel.currentItems.iconMainMenuVideoplayer
                                    tooltipMessage: "Начать просмотр релиза"
                                    onButtonPressed: {
                                        mainViewModel.selectPage("videoplayer");

                                        onlinePlayerViewModel.quickSetupForSingleRelease(releaseId);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
