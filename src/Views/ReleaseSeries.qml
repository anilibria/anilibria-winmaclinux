/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2021 Roman Vladimirov

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
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0
import Anilibria.Services 1.0
import "../Theme"
import "../Controls"

Page {
    id: root
    anchors.fill: parent
    background: Rectangle {
        color: ApplicationTheme.pageBackground
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
        Rectangle {
            color: ApplicationTheme.pageVerticalPanel
            width: 40
            Layout.fillHeight: true
            Column {
                LeftPanelIconButton {
                    iconPath: assetsLocation.iconsPath + "menu.svg"
                    tooltipMessage: "Открыть меню приложения"
                    iconWidth: 29
                    iconHeight: 29
                    onButtonPressed: {
                        drawer.open();
                    }
                }
            }
        }

        ColumnLayout {
            id: itemContainer
            Layout.fillHeight: true
            Layout.fillWidth: true
            spacing: 2

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 45
                height: 45
                color: ApplicationTheme.pageUpperPanel

                RoundedTextBox {
                    id: filterByTitle
                    anchors.centerIn: parent
                    width: 310
                    height: 40
                    placeholder: "Введите название релиза"
                    onCompleteEditing: {
                        releaseLinkedSeries.nameFilter = textContent;
                        releaseLinkedSeries.filterSeries();
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
                        color: ApplicationTheme.panelBackground

                        RowLayout {
                            anchors.fill: parent

                            Rectangle {
                                color: "transparent"
                                height: parent.height
                                Layout.topMargin: 6
                                Layout.preferredWidth: 300
                                Layout.fillHeight: true
                                Layout.leftMargin: 6

                                Image {
                                    visible: !!firstPoster
                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    anchors.leftMargin: 0
                                    source: localStorage.getReleasePosterPath(releaseIds[0], firstPoster)
                                    sourceSize: Qt.size(180, 270)
                                    fillMode: Image.PreserveAspectCrop
                                    width: 140
                                    height: 210
                                    layer.enabled: true
                                    layer.effect: OpacityMask {
                                        maskSource: mask
                                    }
                                }
                                Image {
                                    visible: !!secondPoster
                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    anchors.leftMargin: thirdPoster ? 80 : 160
                                    source: localStorage.getReleasePosterPath(releaseIds[1], secondPoster)
                                    sourceSize: Qt.size(180, 270)
                                    fillMode: Image.PreserveAspectCrop
                                    width: 140
                                    height: 210
                                    layer.enabled: true
                                    layer.effect: OpacityMask {
                                        maskSource: mask
                                    }
                                }
                                Image {
                                    visible: !!thirdPoster
                                    anchors.top: parent.top
                                    anchors.left: parent.left
                                    anchors.leftMargin: 160
                                    source: localStorage.getReleasePosterPath(releaseIds[2], thirdPoster)
                                    sourceSize: Qt.size(180, 270)
                                    fillMode: Image.PreserveAspectCrop
                                    width: 140
                                    height: 210
                                    layer.enabled: true
                                    layer.effect: OpacityMask {
                                        maskSource: mask
                                    }
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

                                    PlainText {
                                        Layout.preferredHeight: 20
                                        fontPointSize: 10
                                        text: firstName
                                    }

                                    PlainText {
                                        Layout.preferredHeight: 20
                                        fontPointSize: 10
                                        text: secondName
                                    }

                                    PlainText {
                                        Layout.preferredHeight: 20
                                        fontPointSize: 10
                                        visible: !!thirdName
                                        text: thirdName
                                    }

                                    PlainText {
                                        Layout.preferredHeight: 20
                                        fontPointSize: 10
                                        visible: !!otherReleases
                                        text: otherReleases
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
                                        iconPath: "../Assets/Icons/favorite.svg"
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
                                                    for (const releaseId of releaseIds) {
                                                        releasesViewModel.addReleaseToFavorites(releaseId);
                                                    }

                                                    favoriteSeriesMenu.close();
                                                }
                                            }
                                            CommonMenuItem {
                                                enabled: !!window.userModel.login
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
                                        iconPath: "../Assets/Icons/popcorn.svg"
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
                                                    localStorage.addToCinemahall(releaseIds);

                                                    cinemahallSeriesMenu.close();
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
        }
    }
}
