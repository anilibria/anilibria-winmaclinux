import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Anilibria.Services 1.0
import "../Controls"

Page {
    id: root
    anchors.fill: parent
    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
    }

    signal navigateFrom()
    signal navigateTo()

    Rectangle {
        id: mask
        width: 180
        height: 260
        radius: 10
        visible: false
    }

    RowLayout {
        id: panelContainer
        anchors.fill: parent
        spacing: 0

        ColumnLayout {
            id: itemContainer
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 2

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                height: 45
                color: applicationThemeViewModel.pageUpperPanel

                PlainText {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 14
                    visible: releaseLinkedSeries.nameFilter && !mainViewModel.isSmallSizeMode
                    text: "Найдено: " + releaseLinkedSeries.countGroups
                    fontPointSize: 10
                }

                RoundedTextBox {
                    id: filterByTitle
                    anchors.centerIn: parent
                    width: 310
                    height: 40
                    placeholder: "Введите название релиза или жанра"
                    onCompleteEditing: {
                        releaseLinkedSeries.nameFilter = textContent;
                        releaseLinkedSeries.filterSeries();
                    }
                }

                CommonComboBox {
                    id: sortingComboBox
                    visible: !mainViewModel.isSmallSizeMode
                    width: 160
                    height: parent.height
                    fontPointSize: 9
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: sortingDirectionButton.left
                    anchors.rightMargin: 2
                    model: ListModel {
                        ListElement {
                            text: "Количеству релизов"
                        }
                        ListElement {
                            text: "Названию"
                        }
                        ListElement {
                            text: "Жанрам"
                        }
                        ListElement {
                            text: "Рейтингу"
                        }
                        ListElement {
                            text: "Количество сидов"
                        }
                        ListElement {
                            text: "Сезонам"
                        }
                        ListElement {
                            text: "Годам"
                        }
                    }
                    onCurrentIndexChanged: {
                        releaseLinkedSeries.sortingField = currentIndex;
                    }
                }

                FilterPanelIconButton {
                    id: sortingDirectionButton
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 14
                    iconPath: releaseLinkedSeries.sortingDirection ? applicationThemeViewModel.currentItems.iconReleaseCatalogSortDesc : applicationThemeViewModel.currentItems.iconReleaseCatalogSortAsc
                    tooltipMessage: "Направление сортировки списка"
                    onButtonPressed: {
                        releaseLinkedSeries.sortingDirection = !releaseLinkedSeries.sortingDirection;
                    }
                }
            }

            ListView {
                Layout.fillHeight: true
                Layout.fillWidth: true
                model: releaseLinkedSeries
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
                        anchors.rightMargin: 4
                        radius: 10
                        color: applicationThemeViewModel.panelBackground

                        MouseArea {
                            anchors.fill: parent
                            onPressed: {
                                releaseLinkedSeries.selectByIndex(identifier);
                                mainViewModel.selectPage("releaseseries:" + identifier);
                            }
                        }

                        RowLayout {
                            anchors.fill: parent

                            Rectangle {
                                color: "transparent"
                                height: parent.height
                                Layout.topMargin: 6
                                Layout.preferredWidth: 300
                                Layout.fillHeight: true
                                Layout.leftMargin: 6

                                CorneredImage {
                                    visible: !!firstPoster
                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    anchors.leftMargin: 0
                                    posterSource: localStorage.getReleasePosterPath(releaseIds[0], firstPoster)
                                    width: 140
                                    height: 210
                                }
                                CorneredImage {
                                    visible: !!secondPoster
                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    anchors.leftMargin: thirdPoster ? 80 : 160
                                    posterSource: localStorage.getReleasePosterPath(releaseIds[1], secondPoster)
                                    width: 140
                                    height: 210
                                }
                                CorneredImage {
                                    visible: !!thirdPoster
                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    anchors.leftMargin: 160
                                    posterSource: localStorage.getReleasePosterPath(releaseIds[2], thirdPoster)
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
                                        text: firstThreeNamesRole
                                        Layout.fillWidth: true
                                        maximumLineCount: 2
                                        elide: Text.ElideRight
                                        wrapMode: Text.Wrap
                                    }

                                    PlainText {
                                        Layout.preferredHeight: 20
                                        fontPointSize: 10
                                        visible: !!otherReleases
                                        text: otherReleases
                                    }

                                    PlainText {
                                        Layout.preferredHeight: 20
                                        Layout.fillWidth: true
                                        maximumLineCount: 2
                                        elide: Text.ElideRight
                                        wrapMode: Text.Wrap
                                        fontPointSize: 10
                                        text: genres
                                    }

                                    PlainText {
                                        Layout.preferredHeight: 20
                                        Layout.fillWidth: true
                                        maximumLineCount: 2
                                        elide: Text.ElideRight
                                        wrapMode: Text.Wrap
                                        fontPointSize: 9
                                        text: countInFavorites
                                    }

                                    PlainText {
                                        Layout.preferredHeight: 20
                                        Layout.fillWidth: true
                                        fontPointSize: 9
                                        text: totalTime + ", эпизодов " + totalEpisode + ", рейтинг " + totalRating
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
                                                text: "Добавить в избранное"
                                                onPressed: {
                                                    for (const releaseId of releaseIds) {
                                                        releasesViewModel.addReleaseToFavorites(releaseId);
                                                    }

                                                    favoriteSeriesMenu.close();
                                                }
                                            }
                                            CommonMenuItem {
                                                text: "Удалить из избранного"
                                                onPressed: {
                                                    for (const releaseId of releaseIds) {
                                                        releasesViewModel.removeReleaseFromFavorites(releaseId);
                                                    }

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
                                                    releasesViewModel.cinemahall.addReleases(releaseIds);

                                                    cinemahallSeriesMenu.close();
                                                }
                                            }
                                        }
                                    }
                                    FilterPanelIconButton {
                                        iconPath: applicationThemeViewModel.currentItems.iconMainMenuVideoplayer
                                        tooltipMessage: "Начать просмотр всей группы в видеоплеере"
                                        onButtonPressed: {
                                            mainViewModel.selectPage("videoplayer");

                                            onlinePlayerViewModel.quickSetupForMultipleRelease(releaseIds);
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

    ReleaseSeriesCard {
        id: releaseSeriesCard
        visible: releaseLinkedSeries.isCardShowed
    }
}
