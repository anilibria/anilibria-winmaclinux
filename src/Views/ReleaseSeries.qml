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
                IconButton {
                    id: searchFilterButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/search.svg"
                    iconWidth: 29
                    iconHeight: 29
                    onButtonPressed: {
                        seriesFiltersPopup.open();
                    }

                    Popup {
                        id: seriesFiltersPopup
                        x: 40
                        y: searchFilterButton.height - 45
                        width: 450
                        height: 200
                        modal: true
                        focus: true
                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                        Rectangle {
                            width: parent.width
                            RoundedActionButton {
                                id: startFilterButton
                                anchors.left: parent.left
                                text: "Фильтровать"
                                onClicked: {
                                    releaseLinkedSeries.filterSeries();
                                }
                            }
                            RoundedActionButton {
                                id: clearFiltersButton
                                anchors.right: parent.right
                                text: "Очистить фильтры"
                                onClicked: {
                                    releaseLinkedSeries.clearFilters();
                                }
                            }
                            PlainText {
                                id: labelNameSearchField
                                anchors.top: clearFiltersButton.bottom
                                fontPointSize: 11
                                text: qsTr("Наименование")
                            }
                            TextField {
                                id: nameSearchField
                                width: parent.width - 5
                                anchors.top: labelNameSearchField.bottom
                                anchors.rightMargin: 10
                                placeholderText: "Введите часть или полное наименование релиза"
                                text: releaseLinkedSeries.nameFilter
                                onTextChanged: {
                                    releaseLinkedSeries.nameFilter = text;
                                }
                            }
                        }
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

                /*Switch {
                    id: selectMode
                    anchors.left: parent.left
                    onCheckedChanged: {
                        root.selectedItems = {};
                    }
                    ToolTip.delay: 1000
                    ToolTip.visible: selectMode.hovered
                    ToolTip.text: "Данный переключатель влияет на поведение при клике ЛКМ на связанных релизах в списке\nРежим выбора позволяет выбрать несколько связанных релизов и выполнять действия(например добавление в избранное) сразу в группе"
                }*/

                /*PlainText {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    visible: downloadManager.running
                    fontPointSize: 12
                    text: "Выполняется скачивание, скорость " + downloadManager.displayBytesInSeconds + " скачано " + Math.floor(downloadManager.progress) + "%"
                }*/
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
                                    anchors.topMargin: 6

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
                                        text: thirdName
                                    }

                                    PlainText {
                                        Layout.preferredHeight: 20
                                        fontPointSize: 10
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

                                    IconButton {
                                        height: 30
                                        width: 30
                                        iconColor: ApplicationTheme.filterIconButtonColor
                                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                                        iconPath: "../Assets/Icons/favorite.svg"
                                        iconWidth: 24
                                        iconHeight: 24
                                        ToolTip.delay: 1000
                                        ToolTip.visible: hovered
                                        ToolTip.text: "Открыть меню для операций по добавлению/удалению всей группы в избранное"
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
                                                    const ids = releaseIds.join(`,`);
                                                    synchronizationService.addUserFavorites(applicationSettings.userToken, ids);

                                                    favoriteSeriesMenu.close();
                                                }
                                            }
                                            CommonMenuItem {
                                                text: "Удалить из избранного"
                                                onPressed: {
                                                    const ids = releaseIds.join(`,`);
                                                    synchronizationService.removeUserFavorites(applicationSettings.userToken, ids);

                                                    favoriteSeriesMenu.close();
                                                }
                                            }
                                        }
                                    }
                                    IconButton {
                                        height: 30
                                        width: 30
                                        iconColor: ApplicationTheme.filterIconButtonColor
                                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                                        iconPath: "../Assets/Icons/popcorn.svg"
                                        iconWidth: 24
                                        iconHeight: 24
                                        ToolTip.delay: 1000
                                        ToolTip.visible: hovered
                                        ToolTip.text: "Открыть меню для операций по добавлению/удалению всей группы в кинозал"
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
