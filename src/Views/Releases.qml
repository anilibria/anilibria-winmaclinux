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
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import Anilibria.ListModels 1.0
import "../Controls"

Page {
    id: page
    property int selectedSection: 0
    property int startedSection: 0
    property string releaseDescription: ""
    property bool showButtonVisibleChanger: false
    property bool hideCinemahallButton: false
    property bool hideDownloadButton: false
    property bool hideRandomReleaseButton: false
    property bool hideNotificationButton: false
    property bool hideInfoButton: false
    property bool hideFilterButton: false
    property bool showAlpabeticalCharaters: false
    property bool toggler: false
    property alias backgroundImageWidth: itemsContainer.width
    property alias backgroundImageHeight: itemsContainer.height

    signal navigateFrom()
    signal watchSingleRelease(int releaseId, string videos, int startSeria, string poster)
    signal refreshReleases()
    signal refreshFavorites()
    signal refreshReleaseSchedules()
    signal navigateTo()
    signal watchCinemahall()
    signal watchMultipleReleases()
    signal setWebViewUrl()

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            if (releasePosterPreview.isVisible) {
                releasePosterPreview.isVisible = false;
            } else {
                releasesViewModel.hideReleaseCard();
                page.showAlpabeticalCharaters = false;
            }
        }
        if (event.key === Qt.Key_Home) {
            if (compactModeSwitch.checked && !releasesViewModel.synchronizationEnabled) showPanelInCompactModeButton.clicked();
        }
    }

    onWidthChanged: {
        const columnCount = parseInt(page.width / 520);
    }

    onRefreshReleases: {
        releasesViewModel.items.refresh();
    }

    onRefreshReleaseSchedules: {
    }

    onSetWebViewUrl: {
        //REMOVE!!!!
    }

    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
    }

    onNavigateTo: {
        page.forceActiveFocus();
    }

    anchors.fill: parent

    Rectangle {
        id: mask
        width: 180
        height: 260
        radius: 10
        visible: false
    }

    Rectangle {
        id: cardMask
        width: 180
        height: 260
        radius: 6
        visible: false
    }

    RowLayout {
        id: panelContainer
        anchors.fill: parent
        spacing: 0
        enabled: !releasesViewModel.isOpenedCard
        Rectangle {
            color: applicationThemeViewModel.pageVerticalPanel
            Layout.preferredWidth: compactModeSwitch.checked && !releasesViewModel.showSidePanel ? 0 : 40
            Layout.fillHeight: true

            Column {
                visible: !compactModeSwitch.checked || releasesViewModel.showSidePanel
                width: compactModeSwitch.checked && !releasesViewModel.showSidePanel ? 0 : 40

                LeftPanelIconButton {
                    iconPath: applicationThemeViewModel.iconMainMenu
                    iconWidth: 28
                    iconHeight: 28
                    overlayVisible: false
                    tooltipMessage: "Открыть меню приложения"
                    onButtonPressed: {
                        drawer.open();
                    }
                }

                LeftPanelIconButton {
                    iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogSynchronization
                    iconWidth: 34
                    iconHeight: 34
                    overlayVisible: false
                    tooltipMessage: "Выполнить синхронизацию релизов"
                    onButtonPressed: {
                        if (releasesViewModel.synchronizationEnabled) return;

                        releasesViewModel.synchronizationEnabled = true;
                        synchronizationService.synchronizeReleases(1);
                    }
                }

                LeftPanelIconButton {
                    iconPath: applicationThemeViewModel.currentItems.iconFavorites
                    iconWidth: 29
                    iconHeight: 29
                    overlayVisible: false
                    tooltipMessage: "Добавить или удалить релизы из избранного"
                    onButtonPressed: {
                        favoriteMenu.open();
                    }

                    CommonMenu {
                        id: favoriteMenu
                        y: parent.height
                        width: 350

                        CommonMenuItem {
                            text: "Добавить в избранное"
                            enabled: releasesViewModel.items.isHasSelectRelease
                            onPressed: {
                                releasesViewModel.addSelectedReleaseToFavorites();
                                favoriteMenu.close();
                            }
                        }
                        CommonMenuItem {
                            text: "Удалить из избранного"
                            enabled: releasesViewModel.items.isHasSelectRelease
                            onPressed: {
                                releasesViewModel.removeSelectedReleaseFromFavorites();
                                favoriteMenu.close();
                            }
                        }
                    }
                }
                LeftPanelIconButton {
                    iconPath: applicationThemeViewModel.currentItems.iconSeen
                    overlayVisible: false
                    iconWidth: 29
                    iconHeight: 29
                    tooltipMessage: "Отметить релизы как просмотренные или не просмотренные"
                    onButtonPressed: {
                        seenMarkMenuPanel.open();
                    }

                    CommonMenu {
                        id: seenMarkMenuPanel
                        y: parent.height
                        width: 350

                        CommonMenuItem {
                            text: "Отметить как просмотренное"
                            enabled: releasesViewModel.items.isHasSelectRelease
                            onPressed: {
                                setSeenStateForRelease(true);
                            }
                        }
                        CommonMenuItem {
                            text: "Отметить как не просмотренное"
                            enabled: releasesViewModel.items.isHasSelectRelease
                            onPressed: {
                                setSeenStateForRelease(false);
                            }
                        }
                        CommonMenuItem {
                            text: "Удалить все отметки о просмотре"
                            onPressed: {
                                seenMarkMenuPanel.close();
                                removeAllSeenMark.open();
                            }
                        }
                        CommonMenuItem {
                            text: "Скрыть выбранные релизы"
                            enabled: releasesViewModel.items.isHasSelectRelease
                            onPressed: {
                                seenMarkMenuPanel.close();
                                addToHidedReleasesConfirm.open();
                            }
                        }
                        CommonMenuItem {
                            enabled: releasesViewModel.items.isHasSelectRelease
                            text: "Убрать из скрытых выбранные релизы"
                            onPressed: {
                                seenMarkMenuPanel.close();
                                releasesViewModel.removeFromHidedSelectedReleases();
                                releasesViewModel.clearSelectedReleases();
                            }
                        }
                        CommonMenuItem {
                            text: "Сделать все скрытые релизы видимыми"
                            onPressed: {
                                seenMarkMenuPanel.close();
                                removeAllHidedReleasesConfirm.open();
                            }
                        }
                    }
                }
                LeftPanelIconButton {
                    iconPath: applicationThemeViewModel.iconMainMenuCinemahall
                    overlayVisible: false
                    showCrossIcon: page.showButtonVisibleChanger && page.hideCinemahallButton
                    visible: page.showButtonVisibleChanger || !page.hideCinemahallButton
                    tooltipMessage: "Управление кинозалом"
                    onButtonPressed: {
                        if (page.showButtonVisibleChanger) {
                            page.hideCinemahallButton = !page.hideCinemahallButton;
                            localStorage.setHideCinemhallButton(page.hideCinemahallButton);
                        } else {
                            cinemahallMenuPanel.open();
                        }
                    }

                    transform: [
                        Scale {
                            id: scaleCinemahall
                            origin.x: notificationsButton.width / 2
                            origin.y: notificationsButton.height / 2
                            xScale: 1
                            yScale: 1
                        }
                    ]

                    SequentialAnimation {
                        running: window.active && releasesViewModel.hasCinemahallNotSeenVideos
                        loops: Animation.Infinite
                        ParallelAnimation {
                            NumberAnimation { target: scaleCinemahall; property: "xScale"; to: 1.2; duration: 300 }
                            NumberAnimation { target: scaleCinemahall; property: "yScale"; to: 1.2; duration: 300 }
                        }
                        ParallelAnimation {
                            NumberAnimation { target: scaleCinemahall; property: "xScale"; to: 1; duration: 300 }
                            NumberAnimation { target: scaleCinemahall; property: "yScale"; to: 1; duration: 300 }
                        }
                        ParallelAnimation {
                            NumberAnimation { target: scaleCinemahall; property: "xScale"; to: 1; duration: 1200 }
                            NumberAnimation { target: scaleCinemahall; property: "yScale"; to: 1; duration: 1200 }
                        }
                    }

                    CommonMenu {
                        id: cinemahallMenuPanel
                        y: parent.height
                        width: 300

                        CommonMenuItem {
                            text: "Добавить в кинозал"
                            enabled: releasesViewModel.items.isHasSelectRelease
                            onPressed: {
                                releasesViewModel.addToCinemahallSelectedReleases();
                                releasesViewModel.clearSelectedReleases();
                                cinemahallMenuPanel.close();
                                analyticsService.sendView("addtocinemahall", "open", "%2Frelease");
                            }
                        }
                        CommonMenuItem {
                            text: "Смотреть кинозал"
                            onPressed: {
                                if (releasesViewModel.cinemahall.hasItems) {
                                    watchCinemahall();
                                } else {
                                    notHaveCinemahallReleasesMessagePopup.open();
                                }
                                cinemahallMenuPanel.close();
                                analyticsService.sendView("cinemahallwatch", "open", "%2Frelease");
                            }
                        }
                        CommonMenuItem {
                            text: "Смотреть кинозал по избранному"
                            enabled: releasesViewModel.countFavorites
                            onPressed: {
                                onlinePlayerViewModel.quickSetupForFavoritesCinemahall();
                                mainViewModel.selectPage("videoplayer");

                                cinemahallMenuPanel.close();
                                analyticsService.sendView("cinemahallfav", "open", "%2Frelease");
                            }
                        }
                    }

                    DefaultPopup {
                        id: notHaveCinemahallReleasesMessagePopup
                        x: window.width / 2 - 225
                        y: window.height / 2 - 100
                        width: 450
                        height: 150
                        modal: true
                        focus: true
                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                        Column {
                            width: parent.width
                            spacing: 10
                            AccentText {
                                id: notHaveCinemahallReleasesHeader
                                width: notHaveCinemahallReleasesMessagePopup.width - 20
                                fontPointSize: 12
                                font.bold: true
                                elide: Text.ElideRight
                                text: "Просмотр кинозала"
                            }

                            Rectangle {
                                color: "transparent"
                                width: notHaveCinemahallReleasesMessagePopup.width - 20
                                height: notHaveCinemahallReleasesMessagePopup.height - 50
                                PlainText {
                                    id: notHaveCinemahallReleasesMessage
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: parent.width
                                    fontPointSize: 10
                                    wrapMode: Text.WordWrap
                                    text: "У Вас нет релизов в кинозале, чтобы добавить их переведите режим выбора в множественный режим,\n выберите релизы и используйте пункт Добавить в кинозал"
                                }
                            }
                        }
                    }
                }
                LeftPanelIconButton {
                    iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogSearch
                    overlayVisible: false
                    iconWidth: 29
                    iconHeight: 29
                    showCrossIcon: page.showButtonVisibleChanger && page.hideFilterButton
                    visible: page.showButtonVisibleChanger || !page.hideFilterButton
                    tooltipMessage: "Добавить фильтры по дополнительным полям релиза таким как жанры озвучка и т.п."
                    onButtonPressed: {
                        if (page.showButtonVisibleChanger) {
                            page.hideFilterButton = !page.hideFilterButton;
                            localStorage.setHideFilterButton(page.hideFilterButton);
                        } else {
                            filtersPopup.open();
                        }
                    }

                    Rectangle {
                        id: filtersExistsMark
                        visible: descriptionSearchField.text || typeSearchField.text || genresSearchField.text ||
                                 voicesSearchField.text || yearsSearchField.text || seasonesSearchField.text ||
                                 statusesSearchField.text || favoriteMarkSearchField.currentIndex > 0 || seenMarkSearchField.currentIndex > 0 ||
                                 releaseSeriesFilterField.checked || scheduleDayFilterField.text
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.rightMargin: 6
                        anchors.topMargin: 10
                        color: "#4ca2c2"
                        width: 16
                        height: 16
                        radius: 12
                    }

                    DefaultPopup {
                        id: filtersPopup
                        x: 40
                        y: -220
                        width: 450
                        height: 490
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
                                    releasesViewModel.items.refresh();
                                }
                            }
                            RoundedActionButton {
                                id: clearFiltersButton
                                anchors.right: parent.right
                                text: "Очистить фильтры"
                                onClicked: {
                                    page.clearAdditionalFilters();
                                    releasesViewModel.items.refresh();
                                }
                            }
                            PlainText {
                                id: labelDescriptionSearchField
                                anchors.top: clearFiltersButton.bottom
                                fontPointSize: 11
                                text: qsTr("Описание")
                            }
                            PlainText {
                                id: labelTypeSearchField
                                anchors.top: clearFiltersButton.bottom
                                anchors.left: typeSearchField.left
                                fontPointSize: 11
                                text: qsTr("Тип")
                            }
                            TextField {
                                id: descriptionSearchField
                                width: parent.width / 2 - 5
                                anchors.top: labelDescriptionSearchField.bottom
                                anchors.rightMargin: 10
                                placeholderText: "Описание"
                                onTextChanged: {
                                    releasesViewModel.items.descriptionFilter = text;
                                }
                            }
                            TextField {
                                id: typeSearchField
                                width: parent.width / 2 - 5
                                anchors.top: labelTypeSearchField.bottom
                                anchors.right: parent.right
                                placeholderText: "Тип"
                                onTextChanged: {
                                    releasesViewModel.items.typeFilter = text;
                                }

                            }

                            PlainText {
                                id: labelGenresSearchField
                                anchors.top: descriptionSearchField.bottom
                                anchors.rightMargin: 10
                                fontPointSize: 11
                                text: qsTr("Жанры")
                            }
                            TextField {
                                id: genresSearchField
                                width: parent.width * 0.7
                                anchors.top: labelGenresSearchField.bottom
                                placeholderText: "Вводите жанры через запятую"
                                onTextChanged: {
                                    releasesViewModel.items.genresFilter = text;
                                }
                            }
                            PlainText {
                                id: labelOrAndGenresSearchField
                                anchors.top: labelGenresSearchField.bottom
                                anchors.left: genresSearchField.right
                                topPadding: 16
                                leftPadding: 4
                                fontPointSize: 11
                                text: qsTr("ИЛИ/И")
                            }
                            Switch {
                                id: orAndGenresSearchField
                                anchors.top: labelGenresSearchField.bottom
                                anchors.left: labelOrAndGenresSearchField.right
                                onCheckedChanged: {
                                    releasesViewModel.items.genresFilterOr = checked;
                                }
                            }

                            PlainText {
                                id: labelVoicesSearchField
                                anchors.top: genresSearchField.bottom
                                anchors.rightMargin: 10
                                fontPointSize: 11
                                text: qsTr("Озвучка")
                            }
                            TextField {
                                id: voicesSearchField
                                width: parent.width * 0.7
                                anchors.top: labelVoicesSearchField.bottom
                                placeholderText: "Вводите войсеров через запятую"
                                onTextChanged: {
                                    releasesViewModel.items.voicesFilter = text;
                                }
                            }
                            PlainText {
                                id: labelOrAndVoicesSearchField
                                anchors.top: labelVoicesSearchField.bottom
                                anchors.left: voicesSearchField.right
                                topPadding: 16
                                leftPadding: 4
                                fontPointSize: 11
                                text: qsTr("ИЛИ/И")
                            }
                            Switch {
                                id: orAndVoicesSearchField
                                anchors.top: labelVoicesSearchField.bottom
                                anchors.left: labelOrAndVoicesSearchField.right
                                onCheckedChanged: {
                                    releasesViewModel.items.voicesFilterOr = checked;
                                }
                            }

                            PlainText {
                                id: labelYearsSearchField
                                anchors.top: voicesSearchField.bottom
                                fontPointSize: 11
                                text: qsTr("Года")                                
                            }
                            PlainText {
                                id: labelSeasonsSearchField
                                anchors.top: voicesSearchField.bottom
                                anchors.left: typeSearchField.left
                                fontPointSize: 11
                                text: qsTr("Сезоны")
                            }
                            TextField {
                                id: yearsSearchField
                                width: parent.width / 2 - 5
                                anchors.top: labelYearsSearchField.bottom
                                anchors.rightMargin: 10
                                placeholderText: "Вводите через запятую"
                                onTextChanged: {
                                    releasesViewModel.items.yearsFilter = text;
                                }
                            }
                            TextField {
                                id: seasonesSearchField
                                width: parent.width / 2 - 5
                                anchors.top: labelSeasonsSearchField.bottom
                                anchors.right: parent.right
                                placeholderText: "Вводите через запятую"
                                onTextChanged: {
                                    releasesViewModel.items.seasonesFilter = text;
                                }
                            }
                            PlainText {
                                id: labelStatusesSearchField
                                anchors.top: yearsSearchField.bottom
                                anchors.rightMargin: 10
                                fontPointSize: 11
                                text: qsTr("Статусы")
                            }
                            TextField {
                                id: statusesSearchField
                                anchors.top: labelStatusesSearchField.bottom
                                anchors.right: parent.right
                                anchors.left: parent.left
                                placeholderText: "Вводите статусы через запятую"
                                onTextChanged: {
                                    releasesViewModel.items.statusesFilter = text;
                                }
                            }

                            PlainText {
                                id: labelFavoriteMarkSearchField
                                width: parent.width / 2 - 5
                                anchors.top: statusesSearchField.bottom
                                anchors.rightMargin: 10
                                fontPointSize: 11
                                text: qsTr("Признак избранности")
                            }
                            PlainText {
                                id: labelSeenMarkSearchField
                                anchors.top: statusesSearchField.bottom
                                width: parent.width / 2 - 5
                                anchors.right: parent.right
                                fontPointSize: 11
                                text: qsTr("Признак просмотра")
                            }
                            CommonComboBox {
                                id: favoriteMarkSearchField
                                width: parent.width / 2 - 5
                                anchors.top: labelFavoriteMarkSearchField.bottom
                                anchors.rightMargin: 10
                                model: ListModel {
                                    ListElement {
                                        text: "Не используется"
                                    }
                                    ListElement {
                                        text: "В избранном"
                                    }
                                    ListElement {
                                        text: "Не в избранном"
                                    }
                                }
                                onCurrentIndexChanged: {
                                    releasesViewModel.items.favoriteMarkFilter = currentIndex;
                                }
                            }
                            CommonComboBox {
                                id: seenMarkSearchField
                                width: parent.width / 2 - 5
                                anchors.top: labelFavoriteMarkSearchField.bottom
                                anchors.right: parent.right
                                model: ListModel {
                                    ListElement {
                                        text: "Не используется"
                                    }
                                    ListElement {
                                        text: "Просмотренные"
                                    }
                                    ListElement {
                                        text: "Просматриваемые"
                                    }
                                    ListElement {
                                        text: "Не просмотренные"
                                    }
                                }
                                onCurrentIndexChanged: {
                                    releasesViewModel.items.seenMarkFilter = currentIndex;
                                }
                            }

                            PlainText {
                                id: labelReleaseSeriesFilterField
                                anchors.top: favoriteMarkSearchField.bottom
                                anchors.rightMargin: 10
                                fontPointSize: 11
                                text: "Часть связанного релиза"
                            }

                            Switch {
                                id: releaseSeriesFilterField
                                anchors.top: labelReleaseSeriesFilterField.bottom
                                onCheckedChanged: {
                                    releasesViewModel.items.hasReleaseSeriesFilter = checked;
                                }
                            }

                            PlainText {
                                id: labelScheduleDayFilterField
                                width: parent.width / 2 - 5
                                anchors.top: favoriteMarkSearchField.bottom
                                anchors.right: parent.right
                                fontPointSize: 11
                                text: "День недели"
                            }
                            TextField {
                                id: scheduleDayFilterField
                                width: parent.width / 2 - 5
                                anchors.top: labelReleaseSeriesFilterField.bottom
                                anchors.right: parent.right
                                placeholderText: "Вводите через запятую"
                                onTextChanged: {
                                    releasesViewModel.items.scheduleDayFilter = text;
                                }
                            }
                        }
                    }
                }
                LeftPanelIconButton {
                    id: notificationsButton
                    iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogNotification
                    iconWidth: 29
                    iconHeight: 29
                    overlayVisible: false
                    showCrossIcon: page.showButtonVisibleChanger && page.hideNotificationButton
                    tooltipMessage: "Посмотреть уведомления о непросмотренных изменениях в релизах"
                    visible: page.showButtonVisibleChanger || !page.hideNotificationButton
                    onButtonPressed: {
                        if (page.showButtonVisibleChanger) {
                            page.hideNotificationButton = !page.hideNotificationButton;
                            localStorage.setHideNotificationButton(page.hideNotificationButton);
                        } else {
                            notificationPopup.open();
                        }
                    }

                    transform: [
                        Rotation {
                            id: notificationRotation
                            origin.x: notificationsButton.width / 2
                            origin.y: 0
                            angle: 0
                        }
                    ]

                    SequentialAnimation {
                        running: window.active && releasesViewModel.isChangesExists
                        loops: Animation.Infinite
                        NumberAnimation { target: notificationRotation; property: "angle"; to: 10; duration: 300 }
                        NumberAnimation { target: notificationRotation; property: "angle"; to: -10; duration: 300 }
                        NumberAnimation { target: notificationRotation; property: "angle"; to: 0; duration: 300 }
                        NumberAnimation { target: notificationRotation; property: "angle"; to: 0; duration: 1200 }
                    }

                    DefaultPopup {
                        id: notificationPopup
                        x: 40
                        y: parent.height - 100
                        width: 370
                        height: 250
                        modal: true
                        focus: true
                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                        Rectangle {
                            color: "transparent"
                            visible: !releasesViewModel.isChangesExists
                            width: parent.width
                            height: parent.height
                            PlainText {
                                anchors.centerIn: parent
                                text: "У Вас нет новых уведомлений"
                                fontPointSize: 16
                            }
                        }

                        Rectangle {
                            visible: releasesViewModel.isChangesExists
                            width: parent.width
                            RoundedActionButton {
                                id: resetNotificationButton
                                anchors.right: parent.right
                                text: "Отметить все как прочитанное"
                                onClicked: {
                                    releasesViewModel.resetAllChanges();
                                }
                            }
                            Column {
                                spacing: 4
                                anchors.top: resetNotificationButton.bottom
                                Rectangle {
                                    visible: releasesViewModel.newReleasesCount > 0
                                    color: applicationThemeViewModel.panelBackground
                                    border.width: 3
                                    border.color: applicationThemeViewModel.selectedItem
                                    width: 340
                                    height: 40

                                    PlainText {
                                        anchors.centerIn: parent
                                        fontPointSize: 11
                                        text: "Новых релизов: " + releasesViewModel.newReleasesCount
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        onPressed: {
                                            page.changeSection(2);
                                        }
                                    }
                                }
                                Rectangle {
                                    visible: releasesViewModel.newOnlineSeriesCount > 0
                                    color: applicationThemeViewModel.panelBackground
                                    border.width: 3
                                    border.color: applicationThemeViewModel.selectedItem
                                    width: 340
                                    height: 40

                                    PlainText {
                                        anchors.centerIn: parent
                                        fontPointSize: 11
                                        text: "Релизов с новыми сериями: " + releasesViewModel.newOnlineSeriesCount
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        onPressed: {
                                            page.changeSection(3);
                                        }
                                    }
                                }
                                Rectangle {
                                    visible: releasesViewModel.newTorrentsCount > 0
                                    color: applicationThemeViewModel.panelBackground
                                    border.width: 3
                                    border.color: applicationThemeViewModel.selectedItem
                                    width: 340
                                    height: 40

                                    PlainText {
                                        anchors.centerIn: parent
                                        fontPointSize: 11
                                        text: "Новые торренты: " + releasesViewModel.newTorrentsCount
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        onPressed: {
                                            page.changeSection(4);
                                        }
                                    }
                                }
                                Rectangle {
                                    visible: releasesViewModel.newTorrentSeriesCount > 0
                                    color: applicationThemeViewModel.panelBackground
                                    border.width: 3
                                    border.color: applicationThemeViewModel.selectedItem
                                    width: 340
                                    height: 40

                                    PlainText {
                                        anchors.centerIn: parent
                                        fontPointSize: 11
                                        text: "Релизы с обновленными торрентами: " + releasesViewModel.newTorrentSeriesCount
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        onPressed: {
                                            page.changeSection(6);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                LeftPanelIconButton {
                    iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogRandom
                    iconWidth: 32
                    iconHeight: 32
                    overlayVisible: false
                    showCrossIcon: page.showButtonVisibleChanger && page.hideRandomReleaseButton
                    visible: page.showButtonVisibleChanger || !page.hideRandomReleaseButton
                    tooltipMessage: "Открыть карточку релиза выбранного случайным образом"
                    onButtonPressed: {
                        if (page.showButtonVisibleChanger) {
                            page.hideRandomReleaseButton = !page.hideRandomReleaseButton;
                            localStorage.setHideRandomReleaseButton(page.hideRandomReleaseButton);
                        } else {
                            releasesViewModel.showRandomRelease();
                        }
                    }
                    onRightButtonPressed: {
                        randomMenuPanel.open();
                    }

                    CommonMenu {
                        id: randomMenuPanel
                        y: parent.height
                        width: 380

                        CommonMenuItem {
                            text: "Случайный релиз по избранному"
                            onPressed: {
                                releasesViewModel.showRandomReleaseFromFavorites();
                            }
                        }
                        CommonMenuItem {
                            text: "Случайный релиз по текущему фильтру"
                            onPressed: {
                                releasesViewModel.showRandomReleaseInFiltered();
                            }
                        }
                        CommonMenuItem {
                            text: "Случайный релиз по просматриваемому"
                            enabled: releasesViewModel.countFavorites
                            onPressed: {
                                releasesViewModel.showRandomReleaseInSeen();
                            }
                        }
                    }

                }
                LeftPanelIconButton {
                    iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogSettings
                    iconWidth: 29
                    iconHeight: 29
                    overlayVisible: false
                    tooltipMessage: "Настройки страницы Каталог релизов"
                    onButtonPressed: {
                        releaseSettingsPopup.open();
                    }

                    DefaultPopup {
                        id: releaseSettingsPopup
                        x: 40
                        y: -480
                        width: 720
                        height: 520
                        modal: true
                        focus: true
                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                        Column {
                            height: releaseSettingsPopup.height
                            width: parent.width / 2
                            spacing: 4

                            PlainText {
                                id: notificationForFavoritesLabel
                                fontPointSize: 11
                                text: "Уведомления по избранным"
                            }
                            Switch {
                                id: notificationForFavorites
                                onCheckedChanged: {
                                    releasesViewModel.items.filterByFavorites = checked;
                                    localStorage.setNotificationForFavorites(checked);
                                    releasesViewModel.notificationForFavorites = checked;
                                }
                            }

                            PlainText {
                                id: darkModeLabel
                                fontPointSize: 11
                                text: "Тема"
                            }
                            CommonComboBox {
                                id: themes
                                width: 320
                                model: applicationThemeViewModel.themes

                                onActivated: {
                                    applicationThemeViewModel.selectedTheme = applicationThemeViewModel.themes[currentIndex]
                                }
                                onModelChanged: {
                                    currentIndex = applicationThemeViewModel.themes.indexOf(applicationThemeViewModel.selectedTheme);
                                }
                                Component.onCompleted: {
                                    currentIndex = applicationThemeViewModel.themes.indexOf(applicationThemeViewModel.selectedTheme);
                                }
                            }

                            PlainText {
                                id: compactModeLabel
                                fontPointSize: 11
                                text: "Компактный режим"
                            }
                            CommonSwitch {
                                id: compactModeSwitch
                                tooltipMessage: "Компактный режим позволяет уменьшить количество элементов на странице"
                                onCheckedChanged: {
                                    localStorage.setCompactMode(checked);
                                }
                            }

                            PlainText {
                                id: useCustomToolbarLabel
                                fontPointSize: 11
                                text: "Использовать кастомный тулбар"
                            }
                            CommonSwitch {
                                id: useCustomToolbarSwitch
                                tooltipMessage: "Если настройка включена будет использоваться кастомный тулбар окна с дополнительным функционалом"
                                onCheckedChanged: {
                                    applicationSettings.useCustomToolbar = checked;

                                    if (applicationSettings.useCustomToolbar) {
                                        window.flags = Qt.FramelessWindowHint | Qt.Window | Qt.WindowMinimizeButtonHint;
                                        toolBar.visible = true;
                                    } else {
                                        window.flags = 1;
                                        toolBar.visible = false;
                                    }
                                }
                            }

                            PlainText {
                                fontPointSize: 11
                                text: "Включить акселерацию при прокручивании"
                            }
                            CommonSwitch {
                                tooltipMessage: "Если настройка включена то при прокручивании будет включена акселерация которая будет добавляет эффект затухания"
                                checked: userConfigurationViewModel.usingScrollAcceleration
                                onCheckedChanged: {
                                    userConfigurationViewModel.usingScrollAcceleration = checked;
                                }
                            }
                            PlainText {
                                fontPointSize: 11
                                text: "После скачивания торрента отметить\nкак просмотренное"
                            }
                            CommonSwitch {
                                tooltipMessage: "Если настройка включена то при прокручивании будет включена акселерация которая будет добавляет эффект затухания"
                                checked: userConfigurationViewModel.markAsReadAfterDownload
                                onCheckedChanged: {
                                    userConfigurationViewModel.markAsReadAfterDownload = checked;
                                }
                            }
                        }

                        Column {
                            height: releaseSettingsPopup.height
                            width: parent.width / 2
                            anchors.right: parent.right
                            spacing: 4

                            PlainText {
                                id: downloadTorrentModeLabel
                                fontPointSize: 11
                                text: "Торрент"
                            }
                            CommonComboBox {
                                id: downloadTorrentMode
                                currentIndex: 0
                                width: 350
                                model: ["Открыть в торрент клиенте", "Сохранить файл", "Использовать TorrentStream"]
                                onCurrentIndexChanged: {
                                    localStorage.setTorrentDownloadMode(downloadTorrentMode.currentIndex);
                                }
                            }

                            PlainText {
                                id: clearFilterAfterChangeSectionLabel
                                fontPointSize: 11
                                text: "Сбрасывать все фильтры после\nсмены раздела"
                            }
                            CommonSwitch {
                                id: clearFilterAfterChangeSectionSwitch
                                onCheckedChanged: {
                                    localStorage.setClearFiltersAfterChangeSection(checked);
                                }

                                tooltipMessage: "Разделы это кнопки находящиеся по центру выше списка релизов\nДанная настройка влияет на то будут ли сброшены все фильтры при смене раздела или нет"
                            }

                            PlainText {
                                id: showReleaseDescriptionLabel
                                fontPointSize: 11
                                text: "Показывать описание в списке"
                            }
                            CommonSwitch {
                                id: showReleaseDescriptionSwitch
                                onCheckedChanged: {
                                    localStorage.setShowReleaseDescription(checked);
                                }
                                tooltipMessage: "Если настройка включена при наведении на релизы будет показываться описание в виде небольшой плашки в нижней части окна"
                            }

                            PlainText {
                                id: notCloseReleaseCardAfterWatchLabel
                                fontPointSize: 11
                                text: "Не закрывать карточку релиза после просмотра"
                            }
                            CommonSwitch {
                                id: notCloseReleaseCardAfterWatchSwitch
                                checked: userConfigurationViewModel.notCloseReleaseCardAfterWatch
                                onCheckedChanged: {
                                    userConfigurationViewModel.notCloseReleaseCardAfterWatch = checked;
                                }
                                tooltipMessage: "Если настройка включена при нажатии на кнопку Смотреть из карточки сама карточка релизов не будет закрыта"
                            }

                            PlainText {
                                id: notSaveWindowPositionLabel
                                fontPointSize: 11
                                text: "Не сохранять позицию окна"
                            }
                            CommonSwitch {
                                id: notSaveWindowPositionSwitch
                                checked: userConfigurationViewModel.notSaveWindowPosition
                                onCheckedChanged: {
                                    userConfigurationViewModel.notSaveWindowPosition = checked;
                                }
                                tooltipMessage: "Если настройка включена то при закрытии окна не сохраняется позиция и размер окна"
                            }

                            RoundedActionButton {
                                text: "Настроить фон"
                                onClicked: {
                                    releaseSettingsPopup.close();
                                    backgroundImagePopup.open();
                                }
                            }

                            RoundedActionButton {
                                buttonEnabled: userConfigurationViewModel.startPage !==  "release"
                                text: "Сделать страницу стартовой"
                                onClicked: {
                                    userConfigurationViewModel.startPage = "release";
                                }
                            }
                        }
                    }

                    BackgroundImagePopup {
                        id: backgroundImagePopup
                        x: 40
                        y: -390
                    }
                }

                LeftPanelIconButton {
                    iconPath: applicationThemeViewModel.currentItems.iconInfo
                    overlayVisible: false
                    iconWidth: 29
                    iconHeight: 29
                    showCrossIcon: page.showButtonVisibleChanger && page.hideInfoButton
                    tooltipMessage: "Просмотреть полезные ссылки связанные с приложением"
                    visible: page.showButtonVisibleChanger || !page.hideInfoButton
                    onButtonPressed: {
                        if (page.showButtonVisibleChanger) {
                            page.hideInfoButton = !page.hideInfoButton;
                            localStorage.setHideInfoButton(page.hideInfoButton);
                        } else {
                            informationPopup.open();
                        }
                    }

                    DefaultPopup {
                        id: informationPopup
                        x: 40
                        y: parent.height - 100
                        width: 320
                        height: 96
                        modal: true
                        focus: true
                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                        Column {
                            LinkedText {
                                fontPointSize: 11
                                text: "<a href='http://anilibriadesktop.reformal.ru/'>Написать идею, ошибку, вопрос?</a>"
                            }
                            LinkedText {
                                fontPointSize: 11
                                text: "<a href='https://t.me/Libria911Bot'>Техподдержка Анилибрии</a>"
                            }
                            LinkedText {
                                fontPointSize: 11
                                text: "<a href='https://t.me/desktopclientanilibria'>Telegram канал о приложении</a>"
                            }
                            LinkedText {
                                fontPointSize: 11
                                text: "<a href='https://t.me/desktopclientanilibria/259'>Видео для новых пользователей</a>"
                            }
                        }
                    }
                }
                LeftPanelIconButton {
                    iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogHidedButtons
                    overlayVisible: false
                    iconWidth: 29
                    iconHeight: 29
                    backgroundColor: page.showButtonVisibleChanger ? "#8868b0ab" : "transparent"
                    tooltipMessage: "Переключение между режимом добавления/удаления кнопок и обычным меню"
                    onButtonPressed: {
                        page.showButtonVisibleChanger = !page.showButtonVisibleChanger;
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
                color: applicationThemeViewModel.pageUpperPanel

                CommonSwitch {
                    id: multupleMode
                    anchors.left: parent.left
                    onCheckedChanged: {
                        releasesViewModel.selectMode = checked;
                        if (!checked) {
                            releasesViewModel.clearSelectedReleases();
                        } else {
                            releasesViewModel.hideReleaseCard();
                        }
                    }
                    tooltipMessage: "Данный переключатель влияет на поведение при клике ЛКМ на релизах в списке\nОдиночный выбор позволяет открывать карточку с подробной информацией\nМножественный выбор позволяет выбрать несколько релизов и выполнять действия (добавить в избранное и т.п.)\nЧтобы переключать его можно использовать клик ПКМ в области списка релизов"
                }
                PlainText {
                    visible: !mainViewModel.isSmallSizeMode
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.leftMargin: 4
                    anchors.left: multupleMode.right
                    fontPointSize: 12
                    text: multupleMode.checked ? "Множественный выбор" : "Одиночный выбор"

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            multupleMode.checked = !multupleMode.checked;
                        }
                    }
                }

                PlainText {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    visible: releasesViewModel.synchronizationEnabled
                    fontPointSize: mainViewModel.isSmallSizeMode ? 10 : 12
                    text: "Выполняется синхронизация..."
                }

                RoundedActionButton {
                    id: showPanelInCompactModeButton
                    visible: compactModeSwitch.checked && !releasesViewModel.synchronizationEnabled
                    text: releasesViewModel.showSidePanel ? "Скрыть панель" : "Показать панель"
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    onClicked: {
                        releasesViewModel.showSidePanel = !releasesViewModel.showSidePanel;
                    }
                }


                RoundedActionButton {
                    id: setToStartedSectionButton
                    visible: page.startedSection !== releasesViewModel.items.section
                    text: "Сделать стартовым"
                    textSize: !mainViewModel.isSmallSizeMode ? 11 : 10
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: displaySection.left
                    anchors.rightMargin: 8
                    onClicked: {
                        localStorage.setStartedSection(releasesViewModel.items.section);
                        page.startedSection = releasesViewModel.items.section;
                    }
                }

                PlainText {
                    id: displaySection
                    text: releasesViewModel.sectionNames[releasesViewModel.items.section]
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    fontPointSize: mainViewModel.isSmallSizeMode ? 10 : 12
                }
            }

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 36
                color: "transparent"

                PlainText {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 14
                    visible: releasesViewModel.items.isHasReleases && window.width > 970
                    text: "Найдено: " + releasesViewModel.items.countFilteredReleases
                    fontPointSize: 10
                }

                Rectangle {
                    id: filtersContainer
                    anchors.centerIn: parent
                    width: mainViewModel.isSmallSizeMode ? 230 : 520
                    height: parent.height
                    color: "transparent"

                    Row {
                        width: filtersContainer.width
                        spacing: 8
                        RoundedTextBox {
                            id: filterByTitle
                            width: mainViewModel.isSmallSizeMode ? 180 : 210
                            height: 40
                            placeholder: "Введите название релиза"
                            onCompleteEditing: {
                                releasesViewModel.items.titleFilter = textContent;
                                releasesViewModel.items.refresh();
                            }

                            FilterPanelIconButton {
                                visible: filterByTitle.textContent.length
                                anchors.right: parent.right
                                anchors.top: parent.top
                                anchors.topMargin: 6
                                width: 20
                                height: 20
                                iconPath: applicationThemeViewModel.currentItems.iconClearTextControl
                                overlayVisible: false
                                iconWidth: 16
                                iconHeight: 16
                                tooltipMessage: "Очистить фильтр по названию"
                                onButtonPressed: {
                                    filterByTitle.textContent = "";
                                    releasesViewModel.items.titleFilter = "";
                                    releasesViewModel.items.refresh();
                                }
                            }
                        }

                        FilterPanelIconButton {
                            visible: mainViewModel.isSmallSizeMode
                            iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogAllReleases
                            overlayVisible: false
                            tooltipMessage: "Выберите раздел"
                            onButtonPressed: {
                                allSectionsMenu.open();
                            }

                            CommonMenu {
                                id: allSectionsMenu
                                width: 350
                                y: parent.height

                                Repeater {
                                    model: releasesViewModel.countSections.map(a => a)
                                    delegate: CommonMenuItem {
                                        text: releasesViewModel.sectionNames[modelData]
                                        onPressed: {
                                            page.changeSection(modelData);
                                        }
                                    }

                                }
                            }
                        }

                        FilterPanelIconButton {
                            visible: !mainViewModel.isSmallSizeMode
                            iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogAllReleases
                            overlayVisible: false
                            tooltipMessage: "Все релизы"
                            onButtonPressed: {
                                changeSection(0);
                            }
                        }
                        FilterPanelIconButton {
                            visible: !mainViewModel.isSmallSizeMode
                            iconPath: applicationThemeViewModel.currentItems.iconFavorites
                            overlayVisible: false
                            tooltipMessage: "Избранное"
                            onButtonPressed: {
                                changeSection(1);
                            }
                        }
                        FilterPanelIconButton {
                            visible: !mainViewModel.isSmallSizeMode
                            iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogNotification
                            overlayVisible: false
                            tooltipMessage: "Показать меню с фильтрами по уведомлениям"
                            onButtonPressed: {
                                notificationsMenuSections.open();
                            }

                            CommonMenu {
                                id: notificationsMenuSections
                                width: 350
                                y: parent.height

                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[2]
                                    onPressed: {
                                        page.changeSection(2);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[3]
                                    onPressed: {
                                        page.changeSection(3);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[4]
                                    onPressed: {
                                        page.changeSection(4);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[6]
                                    onPressed: {
                                        page.changeSection(6);
                                    }
                                }
                            }
                        }
                        FilterPanelIconButton {
                            visible: !mainViewModel.isSmallSizeMode
                            iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogSchedule
                            overlayVisible: false
                            iconWidth: 26
                            iconHeight: 26
                            tooltipMessage: "Расписание релизов"
                            onButtonPressed: {
                                changeSection(5);
                            }
                        }
                        FilterPanelIconButton {
                            visible: !mainViewModel.isSmallSizeMode
                            iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogHistory
                            overlayVisible: false
                            tooltipMessage: "Показать меню с фильтрами по истории и истории просмотра"
                            onButtonPressed: {
                                historyMenuSections.open();
                            }

                            CommonMenu {
                                id: historyMenuSections
                                width: 300
                                y: parent.height

                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[7]
                                    onPressed: {
                                        page.changeSection(7);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[8]
                                    onPressed: {
                                        page.changeSection(8);
                                    }
                                }
                            }
                        }
                        FilterPanelIconButton {
                            id: seenMenuButton
                            visible: !mainViewModel.isSmallSizeMode
                            iconPath: applicationThemeViewModel.currentItems.iconSeen
                            overlayVisible: false
                            tooltipMessage: "Показать меню с фильтрами по состоянию просмотра"
                            onButtonPressed: {
                                seenMenuSections.open();
                            }

                            CommonMenu {
                                id: seenMenuSections
                                width: 300
                                y: parent.height

                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[9]
                                    onPressed: {
                                        page.changeSection(9);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[10]
                                    onPressed: {
                                        page.changeSection(10);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[11]
                                    onPressed: {
                                        page.changeSection(11);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[12]
                                    onPressed: {
                                        page.changeSection(12);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[18]
                                    onPressed: {
                                        page.changeSection(18);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[19]
                                    onPressed: {
                                        page.changeSection(19);
                                    }
                                }
                            }
                        }
                        FilterPanelIconButton {
                            visible: !mainViewModel.isSmallSizeMode
                            iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogAlphabet
                            iconWidth: 24
                            iconHeight: 24
                            overlayVisible: false
                            tooltipMessage: "Показать фильтр для выбора букв алфавита для поиска по первой букве релиза"
                            onButtonPressed: {
                                page.showAlpabeticalCharaters = true;
                            }
                        }
                        FilterPanelIconButton {
                            visible: !mainViewModel.isSmallSizeMode
                            iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogCompilation
                            overlayVisible: false
                            iconWidth: 24
                            iconHeight: 24
                            tooltipMessage: "Показать тематические фильтры"
                            onButtonPressed: {
                                customListMenuSections.open();
                            }

                            CommonMenu {
                                id: customListMenuSections
                                width: 350
                                y: parent.height

                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[14]
                                    onPressed: {
                                        page.changeSection(14);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[15]
                                    onPressed: {
                                        page.changeSection(15);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[16]
                                    onPressed: {
                                        page.changeSection(16);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[17]
                                    onPressed: {
                                        page.changeSection(17);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[20]
                                    onPressed: {
                                        page.changeSection(20);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[21]
                                    onPressed: {
                                        page.changeSection(21);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[22]
                                    onPressed: {
                                        page.changeSection(22);
                                    }
                                }
                                CommonMenuItem {
                                    text: releasesViewModel.sectionNames[23]
                                    onPressed: {
                                        page.changeSection(23);
                                    }
                                }
                            }
                        }
                    }
                }

                CommonComboBox {
                    id: sortingComboBox
                    visible: window.width > 970
                    width: 160
                    height: parent.height + 2
                    fontPointSize: 9
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: sortingDirectionButton.left
                    anchors.rightMargin: 2
                    model: ListModel {
                        ListElement {
                            text: "Дате обновления"
                        }
                        ListElement {
                            text: "Дню в расписании"
                        }
                        ListElement {
                            text: "Имени"
                        }
                        ListElement {
                            text: "Году"
                        }
                        ListElement {
                            text: "Рейтингу"
                        }
                        ListElement {
                            text: "Статусу"
                        }
                        ListElement {
                            text: "Оригинальному имени"
                        }
                        ListElement {
                            text: "История"
                        }
                        ListElement {
                            text: "История просмотра"
                        }
                        ListElement {
                            text: "Сезону"
                        }
                        ListElement {
                            text: "Признак избранности"
                        }
                        ListElement {
                            text: "Признак просмотра"
                        }
                        ListElement {
                            text: "Связанным релизам"
                        }
                    }
                    onActivated: {
                        releasesViewModel.items.refresh();
                    }

                    onCurrentIndexChanged: {
                        releasesViewModel.items.sortingField = currentIndex;
                    }
                }

                FilterPanelIconButton {
                    id: sortingDirectionButton
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 14
                    iconPath: releasesViewModel.items.sortingDescending ? applicationThemeViewModel.currentItems.iconReleaseCatalogSortDesc : applicationThemeViewModel.currentItems.iconReleaseCatalogSortAsc
                    tooltipMessage: "Направление сортировки списка"
                    onButtonPressed: {
                        releasesViewModel.items.sortingDescending = !releasesViewModel.items.sortingDescending;
                        releasesViewModel.items.refresh();
                    }
                }
            }

            Rectangle {
                id: itemsContainer
                color: "transparent"
                Layout.fillHeight: true
                Layout.fillWidth: true

                Image {
                    id: backgroundFile
                    asynchronous: true
                    visible: releasesViewModel.imageBackgroundViewModel.isHasImage
                    fillMode: releasesViewModel.imageBackgroundViewModel.imageMode
                    source: releasesViewModel.imageBackgroundViewModel.processedImagePath
                    opacity: releasesViewModel.imageBackgroundViewModel.opacity / 100
                    width: releasesViewModel.imageBackgroundViewModel.imageWidth
                    height: releasesViewModel.imageBackgroundViewModel.imageHeight
                    x: releasesViewModel.imageBackgroundViewModel.imageX
                    y: releasesViewModel.imageBackgroundViewModel.imageY
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton | Qt.MiddleButton
                    onWheel: {
                        if (wheel.angleDelta.y < 0) scrollview.flick(0, -800);
                        if (wheel.angleDelta.y > 0) scrollview.flick(0, 800);
                    }
                    onPressed: {
                        multupleMode.checked = !multupleMode.checked;
                    }
                }

                Rectangle {
                    color: "transparent"
                    anchors.fill: parent
                    visible: releasesViewModel.countReleases === 0 || !releasesViewModel.items.isHasReleases

                    PlainText {
                        anchors.centerIn: parent
                        fontPointSize: 14
                        horizontalAlignment: Text.AlignHCenter
                        text: releasesViewModel.countReleases > 0 ? "По текущему фильтру ничего не найдено\nПопробуйте указать другие фильтры или раздел и повторить поиск" : "Релизы еще не загружены\nПожалуйста подождите пока они загрузятся"
                    }
                }

                GridView {
                    id: scrollview
                    visible: releasesViewModel.items.isHasReleases
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height
                    width: parent.width
                    cellWidth: parent.width / Math.floor(parent.width / (mainViewModel.isSmallSizeMode ? 250 : 490))
                    cellHeight: 290
                    delegate: releaseDelegate
                    model: releasesViewModel.items
                    flickDeceleration: userConfigurationViewModel.usingScrollAcceleration ? 1000 : 10000
                    clip: true
                    ScrollBar.vertical: ScrollBar {
                        active: true
                        onPositionChanged: {
                            if (position < -0.0008 && !releasesViewModel.synchronizationEnabled) {
                                releasesViewModel.synchronizationEnabled = true;
                                synchronizationService.synchronizeReleases(1);
                            }
                        }
                    }

                    Component {
                        id: releaseDelegate
                        Rectangle {
                            color: "transparent"
                            width: scrollview.cellWidth
                            height: scrollview.cellHeight

                            ReleaseItem {
                                anchors.centerIn: parent

                                onLeftClicked: {
                                    if (releasesViewModel.isOpenedCard) return;

                                    releasesViewModel.selectRelease(id);
                                }
                                onRightClicked: {
                                    multupleMode.checked = !multupleMode.checked;
                                }
                                onAddToFavorite: {
                                    releasesViewModel.addReleaseToFavorites(id);
                                    releasesViewModel.clearSelectedReleases();
                                }
                                onRemoveFromFavorite: {
                                    releasesViewModel.removeReleaseFromFavorites(id);
                                    releasesViewModel.clearSelectedReleases();
                                }
                                onWatchRelease: {
                                    page.watchSingleRelease(id, videos, -1, poster);
                                }
                            }
                        }
                    }

                    Component.onCompleted: {
                        scrollview.maximumFlickVelocity = scrollview.maximumFlickVelocity - 1050;
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

        RoundedActionButton {
            id: watchMultipleButton
            visible: releasesViewModel.items.isHasSelectRelease
            text: qsTr("Смотреть выбранное")
            anchors.verticalCenter: parent.verticalCenter
            anchors.left: parent.left
            opacity: 0.8
            onClicked: {
                watchMultipleReleases();
            }
        }
        IconButton {
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 8
            visible: scrollview.contentY > 100
            height: 30
            width: 30
            hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
            iconPath: applicationThemeViewModel.currentItems.iconBackToTop
            overlayVisible: false
            iconWidth: 24
            iconHeight: 24
            tooltipMessage: "Вернуться в начало списка релизов"
            onButtonPressed: {
                scrollview.contentY = 0;
            }

        }
    }

    ReleaseCard {
        id: cardContainer
    }

    Rectangle {
        color: applicationThemeViewModel.pageBackground
        opacity: 0.8
        visible: page.releaseDescription && page.releaseDescription !== "" && showReleaseDescriptionSwitch.checked
        enabled: false
        anchors.left: parent.left
        anchors.leftMargin: 42
        anchors.bottom: parent.bottom
        anchors.rightMargin: 2
        height: 105
        width: parent.width / 2

        PlainText {
            anchors.fill: parent
            fontPointSize: 11
            text: page.releaseDescription
            wrapMode: Text.WordWrap
            maximumLineCount: 5
            verticalAlignment: Text.AlignVCenter
        }
    }

    ReleaseAlphabeticalCharacters {
        id: releaseAlphabeticalCharacters
        visible: page.showAlpabeticalCharaters
    }

    ReleasePosterPreview {
        id: releasePosterPreview
    }

    InfoForNewcomers {
        id: infoForNewcomers
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.leftMargin: 40
    }

    MessageModal {
        id: addToHidedReleasesConfirm
        header: "Добавить релизы в скрытые?"
        message: "Вы уверены что хотите добавить релизы в скрытые?\nЭти релизы будут скрыты везде кроме раздела Скрытые релизы."
        content: Row {
            spacing: 6
            anchors.right: parent.right

            RoundedActionButton {
                text: "Ок"
                width: 100
                onClicked: {
                    releasesViewModel.addToHidedSelectedReleases();
                    addToHidedReleasesConfirm.close();
                    releasesViewModel.clearSelectedReleases();
                }
            }
            RoundedActionButton {
                text: "Отмена"
                width: 100
                onClicked: {
                    addToHidedReleasesConfirm.close();
                }
            }
        }
    }

    MessageModal {
        id: removeAllHidedReleasesConfirm
        header: "Сделать все релизы видимыми?"
        message: "Вы уверены что хотите удалить все скрытые релизы?\nЭти релизы будут доступны во всех разделах."
        content: Row {
            spacing: 6
            anchors.right: parent.right

            RoundedActionButton {
                text: "Ок"
                width: 100
                onClicked: {
                    releasesViewModel.removeFromHidedSelectedReleases();
                    removeAllHidedReleasesConfirm.close();
                    releasesViewModel.clearSelectedReleases();
                }
            }
            RoundedActionButton {
                text: "Отмена"
                width: 100
                onClicked: {
                    removeAllHidedReleasesConfirm.close();
                }
            }
        }
    }

    MessageModal {
        id: removeAllSeenMark
        header: "Удаление признаков просмотра"
        message: "Вы уверены что хотите удалить все признаки просмотра у всех релизов?"
        content: Row {
            spacing: 6
            anchors.right: parent.right

            RoundedActionButton {
                text: "Ок"
                width: 100
                onClicked: {
                    releasesViewModel.removeAllSeenMark();
                    releasesViewModel.items.refresh();
                    removeAllSeenMark.close();
                }
            }
            RoundedActionButton {
                text: "Отмена"
                width: 100
                onClicked: {
                    removeAllSeenMark.close();
                }
            }
        }
    }

    function setSeenStateForOpenedRelease(newState) {
        releasesViewModel.setSeenMarkAllSeries(releasesViewModel.openedReleaseId, releasesViewModel.openedReleaseCountVideos, newState);
        releasesViewModel.refreshOpenedReleaseCard();
        releasesViewModel.items.refreshSingleItem(releasesViewModel.openedReleaseId);
    }

    function setSeenStateForRelease(newState, releases) {
        releasesViewModel.setSeenMarkAllSeriesSelectedReleases(newState);
        releasesViewModel.items.refreshSelectedItems();
        releasesViewModel.clearSelectedReleases();
        seenMarkMenuPanel.close();
    }

    function clearAdditionalFilters() {
        descriptionSearchField.text = "";
        typeSearchField.text = "";
        genresSearchField.text = "";
        orAndGenresSearchField.checked = false;
        voicesSearchField.text = "";
        orAndVoicesSearchField.checked = false;
        yearsSearchField.text = "";
        seasonesSearchField.text = "";
        statusesSearchField.text = "";
        favoriteMarkSearchField.currentIndex = 0;
        seenMarkSearchField.currentIndex = 0;
        releaseSeriesFilterField.checked = false;
        scheduleDayFilterField.text = "";

        alphabetListModel.clearCharacters();
    }

    function changeSection(section) {
        if (releasesViewModel.items.section === section) return;

        if (clearFilterAfterChangeSectionSwitch.checked) {
            filterByTitle.textContent = "";
            page.clearAdditionalFilters();
        }

        releasesViewModel.items.section = section;
        releasesViewModel.setupSortingForSection();
        sortingComboBox.currentIndex = releasesViewModel.items.sortingField;

        releasesViewModel.items.refresh();
    }

    Component.onCompleted: {
        const userSettings = JSON.parse(localStorage.getUserSettings());
        downloadTorrentMode.currentIndex = userSettings.torrentDownloadMode;
        notificationForFavorites.checked = userSettings.notificationForFavorites;
        releasesViewModel.items.filterByFavorites = notificationForFavorites.checked;
        clearFilterAfterChangeSectionSwitch.checked = userSettings.clearFiltersAfterChangeSection;
        compactModeSwitch.checked = userSettings.compactMode;
        page.hideCinemahallButton = userSettings.hideCinemhallButton;
        page.hideDownloadButton = userSettings.hideDownloadButton;
        page.hideRandomReleaseButton = userSettings.hideRandomReleaseButton;
        page.hideNotificationButton = userSettings.hideNotificationButton;
        page.hideInfoButton = userSettings.hideInfoButton;
        page.hideFilterButton = userSettings.hideFilterButton;
        showReleaseDescriptionSwitch.checked = userSettings.showReleaseDescription;
        useCustomToolbarSwitch.checked = applicationSettings.useCustomToolbar;

        const startedSection = userSettings.startedSection;
        if (startedSection) changeSection(startedSection);
        page.startedSection = startedSection;

        releasesViewModel.items.refresh();
    }
}
