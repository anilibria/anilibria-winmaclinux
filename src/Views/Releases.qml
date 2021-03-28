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
import QtWebView 1.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import Anilibria.Services 1.0
import "../Controls"
import "../Theme"

Page {
    id: page
    property bool selectMode
    property var selectedReleases: []
    property var displayedReleases: []
    property var favoriteReleases: []
    property var scheduledReleases: ({})
    property int pageIndex: 1
    property var openedRelease: null
    property bool runRefreshFavorties: false
    property bool synchronizeEnabled: false
    property int selectedSection: 0
    property var seenMarks: ({})
    property bool fillingReleases: false
    property var sections: [
        "Все релизы",
        "Избранное",
        "Новые релизы",
        "Релизы с новыми сериями",
        "Релизы с новыми торрентами",
        "Расписание",
        "Релизы с обновленными торрентами",
        "История",
        "История просмотра",
        "Просмотренные",
        "Просматриваемые",
        "Не просмотренные"
    ]
    property var sectionSortings: {
        0: { field: 0, direction: 1 },
        1: { field: 0, direction: 1 },
        2: { field: 0, direction: 1 },
        3: { field: 0, direction: 1 },
        4: { field: 0, direction: 1 },
        5: { field: 1, direction: 0 },
        6: { field: 0, direction: 1 },
        7: { field: 7, direction: 1 },
        8: { field: 8, direction: 1 },
        9: { field: 0, direction: 1 },
        10: { field: 0, direction: 1 },
        11: { field: 0, direction: 1 }
    }
    property var changesCounts: []
    property bool showSidePanel: false
    property bool showButtonVisibleChanger: false
    property bool hideCinemahallButton: false
    property bool hideDownloadButton: false
    property bool hideRandomReleaseButton: false
    property bool hideNotificationButton: false
    property bool hideInfoButton: false
    property bool hideSortButton: false
    property bool hideFilterButton: false
    property bool showAlpabeticalCharaters: false

    signal navigateFrom()
    signal watchRelease(int releaseId, string videos, int startSeria)
    signal refreshReleases()
    signal refreshFavorites()
    signal refreshReleaseSchedules()
    signal requestSynchronizeReleases()
    signal navigateTo()
    signal watchCinemahall()

    Keys.onPressed: {
        if (event.key === Qt.Key_Escape) {
            page.openedRelease = null;
            page.showAlpabeticalCharaters = false;
        }
    }

    onWidthChanged: {
        const columnCount = parseInt(page.width / 520);
    }

    onRefreshReleases: {
        refreshAllReleases(false);
    }

    onRefreshReleaseSchedules: {
        refreshSchedule();
    }

    onRefreshFavorites: {
        page.favoriteReleases = localStorage.getFavorites().map(a => a);
    }

    onNavigateTo: {
        refreshSeenMarks();
        refreshAllReleases(true);
    }

    background: Rectangle {
        color: ApplicationTheme.pageBackground
    }

    anchors.fill: parent

    ListModel {
        id: releasesModel
        property int updateCounter: 0
    }

    ListModel {
        id: torrentsModel
    }

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

    MouseArea {
        anchors.fill: parent
        hoverEnabled: true
        onPositionChanged: {
            if (!compactModeSwitch.checked) return;
            if (mouse.x < 80) page.showSidePanel = true;
            if (mouse.x > 100) page.showSidePanel = false;
        }
    }

    RowLayout {
        id: panelContainer
        anchors.fill: parent
        spacing: 0
        enabled: !page.openedRelease
        Rectangle {
            color: ApplicationTheme.pageVerticalPanel
            Layout.preferredWidth: compactModeSwitch.checked && !page.showSidePanel ? 0 : 40
            Layout.fillHeight: true

            Column {
                visible: !compactModeSwitch.checked || page.showSidePanel
                width: compactModeSwitch.checked && !page.showSidePanel ? 0 : 40

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
                    id: refreshMenuButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/refresh.svg"
                    iconWidth: 34
                    iconHeight: 34
                    onButtonPressed: {
                        if (page.synchronizeEnabled) return;

                        page.requestSynchronizeReleases();
                    }

                    ToolTip.delay: 1000
                    ToolTip.visible: refreshMenuButton.hovered
                    ToolTip.text: "Выполнить синхронизацию релизов"
                }
                IconButton {
                    id: favoriteMenuButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/favorite.svg"
                    iconWidth: 29
                    iconHeight: 29
                    onButtonPressed: {
                        if (!page.selectedReleases.length) {
                            favoritePopupHeader.text = "Избранное не доступно";
                            favoritePopupMessage.text = "Выберите релизы в списке путем изменения переключателя выше списка на множественный режим и нажатием ЛКМ на интересующих релизах в списке. Выбранные релизы подсвечиваются красной рамкой.";
                            messagePopup.open();
                            return;
                        }

                        if (!window.userModel.login) {
                            favoritePopupHeader.text = "Избранное не доступно";
                            favoritePopupMessage.text = "Чтобы добавлять в избранное нужно вначале авторизоваться. Для этого перейдите на страницу Войти в меню и войдите под данными своего аккаунта. Если вы не зарегистрированы то необходимо сделать это на сайте, ссылка на сайт будет на странице Войти.";
                            messagePopup.open();
                            return;
                        }

                        favoriteMenu.open();
                    }

                    CommonMenu {
                        id: favoriteMenu
                        y: favoriteMenuButton.height
                        width: 350

                        CommonMenuItem {
                            text: "Добавить в избранное"
                            onPressed: {
                                synchronizationService.addUserFavorites(applicationSettings.userToken, page.selectedReleases.join(','));
                                page.selectedReleases = [];
                            }
                        }
                        CommonMenuItem {
                            text: "Удалить из избранного"
                            onPressed: {
                                synchronizationService.removeUserFavorites(applicationSettings.userToken, page.selectedReleases.join(','));
                                page.selectedReleases = [];
                            }
                        }
                    }

                    Popup {
                        id: messagePopup
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
                                id: favoritePopupHeader
                                width: messagePopup.width - 20
                                fontPointSize: 12
                                font.bold: true
                                elide: Text.ElideRight
                            }

                            Rectangle {
                                color: "transparent"
                                width: messagePopup.width - 20
                                height: messagePopup.height - 50
                                PlainText {
                                    id: favoritePopupMessage
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: parent.width
                                    fontPointSize: 10
                                    wrapMode: Text.WordWrap
                                }
                            }
                        }
                    }

                    ToolTip.delay: 1000
                    ToolTip.visible: favoriteMenuButton.hovered
                    ToolTip.text: "Добавить или удалить релизы из избранного"
                }
                IconButton {
                    id: seenMarkMenuPanelButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/seenmarkpanel.svg"
                    iconWidth: 29
                    iconHeight: 29
                    onButtonPressed: {
                        seenMarkMenuPanel.open();
                    }

                    CommonMenu {
                        id: seenMarkMenuPanel
                        y: seenMarkMenuPanelButton.height
                        width: 300

                        CommonMenuItem {
                            text: "Отметить как просмотренное"
                            enabled: page.selectedReleases.length
                            onPressed: {
                                setSeenStateForRelease(true, page.selectedReleases);
                            }
                        }
                        CommonMenuItem {
                            text: "Отметить как не просмотренное"
                            enabled: page.selectedReleases.length
                            onPressed: {
                                setSeenStateForRelease(false, page.selectedReleases);
                            }
                        }
                        CommonMenuItem {
                            text: "Удалить все отметки о просмотре"
                            onPressed: {
                                seenMarkMenuPanel.close();
                                removeAllSeenMark.open();
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
                                    localStorage.removeAllSeenMark();
                                    refreshSeenMarks();
                                    refreshAllReleases(true);
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

                    ToolTip.delay: 1000
                    ToolTip.visible: seenMarkMenuPanelButton.hovered
                    ToolTip.text: "Отметить релизы как просмотренные или не просмотренные"
                }
                IconButton {
                    id: cinemahallMenuButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/popcorn.svg"
                    iconWidth: 30
                    iconHeight: 30
                    showCrossIcon: page.showButtonVisibleChanger && page.hideCinemahallButton
                    visible: page.showButtonVisibleChanger || !page.hideCinemahallButton
                    onButtonPressed: {
                        if (page.showButtonVisibleChanger) {
                            page.hideCinemahallButton = !page.hideCinemahallButton;
                            localStorage.setHideCinemhallButton(page.hideCinemahallButton);
                        } else {
                            cinemahallMenuPanel.open();
                        }
                    }

                    CommonMenu {
                        id: cinemahallMenuPanel
                        y: cinemahallMenuButton.height
                        width: 300

                        CommonMenuItem {
                            text: "Добавить в кинозал"
                            enabled: page.selectedReleases.length
                            onPressed: {
                                localStorage.addToCinemahall(page.selectedReleases);
                                page.selectedReleases = [];
                                cinemahallMenuPanel.close();
                            }
                        }
                        CommonMenuItem {
                            text: "Смотреть кинозал"
                            onPressed: {
                                if (localStorage.hasCinemahallReleases()) {
                                    watchCinemahall();
                                } else {
                                    notHaveCinemahallReleasesMessagePopup.open();
                                }
                                cinemahallMenuPanel.close();
                            }
                        }
                    }

                    Popup {
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
                                width: messagePopup.width - 20
                                fontPointSize: 12
                                font.bold: true
                                elide: Text.ElideRight
                                text: "Просмотр кинозала"
                            }

                            Rectangle {
                                color: "transparent"
                                width: messagePopup.width - 20
                                height: messagePopup.height - 50
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

                    ToolTip.delay: 1000
                    ToolTip.visible: hovered
                    ToolTip.text: "Управление кинозалом"
                }
                IconButton {
                    id: downloadsMenuButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/downloadcircle.svg"
                    iconWidth: 30
                    iconHeight: 30
                    showCrossIcon: page.showButtonVisibleChanger && page.hideDownloadButton
                    visible: page.showButtonVisibleChanger || !page.hideDownloadButton
                    onButtonPressed: {
                        if (page.showButtonVisibleChanger) {
                            page.hideDownloadButton = !page.hideDownloadButton;
                            localStorage.setHideDownloadButton(page.hideDownloadButton);
                        } else {
                            downloadsMenuPanel.open();
                        }
                    }

                    CommonMenu {
                        id: downloadsMenuPanel
                        y: downloadsMenuButton.height
                        width: 300

                        CommonMenuItem {
                            text: "Скачать все серии в HD"
                            enabled: page.selectedReleases.length
                            onPressed: {
                                for (const releaseId of page.selectedReleases) {
                                    const release = findReleaseById(releaseId);
                                    for (let videoId = 0; videoId < release.countVideos; videoId++) {
                                        localStorage.addDownloadItem(release.id, videoId, 1);
                                    }
                                }

                                page.selectedReleases = [];
                                downloadsMenuPanel.close();
                            }
                        }
                        CommonMenuItem {
                            text: "Скачать все серии в SD"
                            enabled: page.selectedReleases.length
                            onPressed: {
                                for (const releaseId of page.selectedReleases) {
                                    const release = findReleaseById(releaseId);
                                    for (let videoId = 0; videoId < release.countVideos; videoId++) {
                                        localStorage.addDownloadItem(release.id, videoId, 2);
                                    }
                                }

                                page.selectedReleases = [];
                                downloadsMenuPanel.close();
                            }
                        }
                    }

                    ToolTip.delay: 1000
                    ToolTip.visible: hovered
                    ToolTip.text: "Скачивание файлов серий в разных качествах локально"
                }
                IconButton {
                    id: searchPopupButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/search.svg"
                    iconWidth: 29
                    iconHeight: 29
                    showCrossIcon: page.showButtonVisibleChanger && page.hideFilterButton
                    visible: page.showButtonVisibleChanger || !page.hideFilterButton
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
                                 statusesSearchField.text || favoriteMarkSearchField.currentIndex > 0 || seenMarkSearchField.currentIndex > 0
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.rightMargin: 6
                        anchors.topMargin: 10
                        color: "#4ca2c2"
                        width: 16
                        height: 16
                        radius: 12
                    }

                    Popup {
                        id: filtersPopup
                        x: 40
                        y: searchPopupButton.height - 45
                        width: 450
                        height: 440
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
                                    page.refreshAllReleases(false);
                                }
                            }
                            RoundedActionButton {
                                id: clearFiltersButton
                                anchors.right: parent.right
                                text: "Очистить фильтры"
                                onClicked: {
                                    page.clearAdditionalFilters();
                                    page.refreshAllReleases(false);
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
                            }
                            TextField {
                                id: typeSearchField
                                width: parent.width / 2 - 5
                                anchors.top: labelTypeSearchField.bottom
                                anchors.right: parent.right
                                placeholderText: "Тип"
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
                            }
                            TextField {
                                id: seasonesSearchField
                                width: parent.width / 2 - 5
                                anchors.top: labelSeasonsSearchField.bottom
                                anchors.right: parent.right
                                placeholderText: "Вводите через запятую"
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
                                anchors.left: labelFavoriteMarkSearchField.right
                                anchors.rightMargin: 10
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
                            }
                        }
                    }

                    ToolTip.delay: 1000
                    ToolTip.visible: searchPopupButton.hovered
                    ToolTip.text: "Добавить фильтры по дополнительным полям релиза таким как жанры озвучка и т.п."
                }
                IconButton {
                    id: sortingPopupButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/sort.svg"
                    iconWidth: 29
                    iconHeight: 29
                    showCrossIcon: page.showButtonVisibleChanger && page.hideSortButton
                    visible: page.showButtonVisibleChanger || !page.hideSortButton
                    onButtonPressed: {
                        if (page.showButtonVisibleChanger) {
                            page.hideSortButton = !page.hideSortButton;
                            localStorage.setHideSortButton(page.hideSortButton);
                        } else {
                            sortingPopup.open();
                        }
                    }

                    Popup {
                        id: sortingPopup
                        x: 40
                        y: sortingPopupButton.height - 100
                        width: 450
                        height: 200
                        modal: true
                        focus: true
                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                        Rectangle {
                            width: parent.width
                            RoundedActionButton {
                                id: startSortingButton
                                anchors.right: parent.right
                                text: "Сортировать"
                                onClicked: {
                                    page.refreshAllReleases(false);
                                }
                            }

                            PlainText {
                                id: labelSortingField
                                anchors.top: startSortingButton.bottom
                                fontPointSize: 11
                                text: qsTr("Сортировать по")
                            }
                            CommonComboBox {
                                id: sortingComboBox
                                anchors.top: labelSortingField.bottom
                                anchors.right: parent.right
                                anchors.left: parent.left
                                model: ListModel {
                                    ListElement {
                                        text: "Дате последнего обновления"
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
                                }
                            }

                            PlainText {
                                id: labelSortingDirection
                                anchors.top: sortingComboBox.bottom
                                fontPointSize: 11
                                text: qsTr("В порядке")
                            }
                            CommonComboBox {
                                id: sortingDirectionComboBox
                                anchors.top: labelSortingDirection.bottom
                                anchors.right: parent.right
                                anchors.left: parent.left
                                currentIndex: 1
                                model: ListModel {
                                    ListElement { text: "Восходящем" }
                                    ListElement { text: "Нисходящем" }
                                }
                            }
                        }
                    }

                    ToolTip.delay: 1000
                    ToolTip.visible: sortingPopupButton.hovered
                    ToolTip.text: "Указать сортировку списка по одному из полей а также направление сортировки"
                }
                IconButton {
                    id: notificationPopupButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/notification.svg"
                    iconWidth: 29
                    iconHeight: 29
                    showCrossIcon: page.showButtonVisibleChanger && page.hideNotificationButton
                    visible: page.showButtonVisibleChanger || !page.hideNotificationButton
                    onButtonPressed: {
                        if (page.showButtonVisibleChanger) {
                            page.hideNotificationButton = !page.hideNotificationButton;
                            localStorage.setHideNotificationButton(page.hideNotificationButton);
                        } else {
                            page.changesCounts = localStorage.getChangesCounts();
                            notificationPopup.open();
                        }
                    }

                    Rectangle {
                        visible: localStorage.isChangesExists
                        anchors.top: parent.top
                        anchors.right: parent.right
                        anchors.rightMargin: 6
                        anchors.topMargin: 10
                        color: "#4ca2c2"
                        width: 16
                        height: 16
                        radius: 12
                    }

                    Popup {
                        id: notificationPopup
                        x: 40
                        y: sortingPopupButton.height - 100
                        width: 370
                        height: 250
                        modal: true
                        focus: true
                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                        Rectangle {
                            visible: !localStorage.isChangesExists
                            width: parent.width
                            height: parent.height
                            PlainText {
                                anchors.centerIn: parent
                                text: "У Вас нет новых уведомлений"
                                fontPointSize: 16
                            }
                        }

                        Rectangle {
                            visible: localStorage.isChangesExists
                            width: parent.width
                            RoundedActionButton {
                                id: resetNotificationButton
                                anchors.right: parent.right
                                text: "Отметить все как прочитанное"
                                onClicked: {
                                    localStorage.resetAllChanges();
                                    page.changesCounts = [0, 0, 0, 0];
                                }
                            }
                            Column {
                                spacing: 4
                                anchors.top: resetNotificationButton.bottom
                                Rectangle {
                                    visible: page.changesCounts[0] > 0
                                    color: ApplicationTheme.panelBackground
                                    border.width: 3
                                    border.color: ApplicationTheme.selectedItem
                                    width: 340
                                    height: 40

                                    PlainText {
                                        anchors.centerIn: parent
                                        fontPointSize: 11
                                        text: "Новых релизов: " + page.changesCounts[0]
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        onPressed: {
                                            page.changeSection(2);
                                        }
                                    }
                                }
                                Rectangle {
                                    visible: page.changesCounts[1] > 0
                                    color: ApplicationTheme.panelBackground
                                    border.width: 3
                                    border.color: ApplicationTheme.selectedItem
                                    width: 340
                                    height: 40

                                    PlainText {
                                        anchors.centerIn: parent
                                        fontPointSize: 11
                                        text: "Релизов с новыми сериями: " + page.changesCounts[1]
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        onPressed: {
                                            page.changeSection(3);
                                        }
                                    }
                                }
                                Rectangle {
                                    visible: page.changesCounts[2] > 0
                                    color: ApplicationTheme.panelBackground
                                    border.width: 3
                                    border.color: ApplicationTheme.selectedItem
                                    width: 340
                                    height: 40

                                    PlainText {
                                        anchors.centerIn: parent
                                        fontPointSize: 11
                                        text: "Новые торренты: " + page.changesCounts[2]
                                    }
                                    MouseArea {
                                        anchors.fill: parent
                                        onPressed: {
                                            page.changeSection(4);
                                        }
                                    }
                                }
                                Rectangle {
                                    visible: page.changesCounts[3] > 0
                                    color: ApplicationTheme.panelBackground
                                    border.width: 3
                                    border.color: ApplicationTheme.selectedItem
                                    width: 340
                                    height: 40

                                    PlainText {
                                        anchors.centerIn: parent
                                        fontPointSize: 11
                                        text: "Релизы с обновленными торрентами: " + page.changesCounts[3]
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

                    ToolTip.delay: 1000
                    ToolTip.visible: notificationPopupButton.hovered
                    ToolTip.text: "Посмотреть уведомления о непросмотренных изменениях в релизах"
                }
                IconButton {
                    id: randomReleaseMenuButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/dice.svg"
                    iconWidth: 29
                    iconHeight: 29
                    showCrossIcon: page.showButtonVisibleChanger && page.hideRandomReleaseButton
                    visible: page.showButtonVisibleChanger || !page.hideRandomReleaseButton
                    onButtonPressed: {
                        if (page.showButtonVisibleChanger) {
                            page.hideRandomReleaseButton = !page.hideRandomReleaseButton;
                            localStorage.setHideRandomReleaseButton(page.hideRandomReleaseButton);
                        } else {
                            const randomRelease = JSON.parse(localStorage.getRandomRelease());
                            showReleaseCard(randomRelease);
                        }
                    }

                    ToolTip.delay: 1000
                    ToolTip.visible: randomReleaseMenuButton.hovered
                    ToolTip.text: "Открыть карточку релиза выбранного случайным образом"
                }
                IconButton {
                    id: settingsMenuButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/options.svg"
                    iconWidth: 29
                    iconHeight: 29
                    onButtonPressed: {
                        releaseSettingsPopup.open();
                    }

                    Popup {
                        id: releaseSettingsPopup
                        x: 40
                        y: sortingPopupButton.height - 220
                        width: 370
                        height: 390
                        modal: true
                        focus: true
                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                        PlainText {
                            id: downloadTorrentModeLabel
                            fontPointSize: 11
                            text: "Торрент"
                        }
                        CommonComboBox {
                            id: downloadTorrentMode
                            currentIndex: 0
                            anchors.top: downloadTorrentModeLabel.bottom
                            width: 350
                            model: ["Открыть в торрент клиенте", "Сохранить файл"]
                            onCurrentIndexChanged: {
                                localStorage.setTorrentDownloadMode(downloadTorrentMode.currentIndex);
                            }
                        }

                        PlainText {
                            id: notificationForFavoritesLabel
                            anchors.top: downloadTorrentMode.bottom
                            anchors.topMargin: 4
                            fontPointSize: 11
                            text: "Уведомления по избранным"
                        }
                        Switch {
                            id: notificationForFavorites
                            anchors.top: notificationForFavoritesLabel.bottom
                            onCheckedChanged: {
                                localStorage.setNotificationForFavorites(checked);
                            }
                        }

                        PlainText {
                            id: darkModeLabel
                            anchors.top: notificationForFavorites.bottom
                            anchors.topMargin: 4
                            fontPointSize: 11
                            text: "Темная тема"
                        }
                        Switch {
                            id: darkModeSwitch
                            anchors.top: darkModeLabel.bottom
                            onCheckedChanged: {
                                applicationSettings.isDarkTheme = checked;
                                ApplicationTheme.isDarkTheme = checked;
                            }
                        }

                        PlainText {
                            id: clearFilterAfterChangeSectionLabel
                            anchors.top: darkModeSwitch.bottom
                            anchors.topMargin: 4
                            fontPointSize: 11
                            text: "Сбрасывать все фильтры после\nсмены раздела"
                        }
                        Switch {
                            id: clearFilterAfterChangeSectionSwitch
                            anchors.top: clearFilterAfterChangeSectionLabel.bottom
                            onCheckedChanged: {
                                localStorage.setClearFiltersAfterChangeSection(checked);
                            }

                            ToolTip.delay: 1000
                            ToolTip.visible: hovered
                            ToolTip.text: "Разделы это кнопки находящиеся по центру выше списка релизов\nДанная настройка влияет на то будут ли сброшены все фильтры при смене раздела или нет"
                        }

                        PlainText {
                            id: compactModeLabel
                            anchors.top: clearFilterAfterChangeSectionSwitch.bottom
                            anchors.topMargin: 4
                            fontPointSize: 11
                            text: "Компактный режим"
                        }
                        Switch {
                            id: compactModeSwitch
                            anchors.top: compactModeLabel.bottom
                            onCheckedChanged: {
                                localStorage.setCompactMode(checked);
                            }

                            ToolTip.delay: 1000
                            ToolTip.visible: hovered
                            ToolTip.text: "Компактный режим позволяет уменьшить количество элементов на странице"
                        }
                    }

                    ToolTip.delay: 1000
                    ToolTip.visible: settingsMenuButton.hovered
                    ToolTip.text: "Настройки страницы Каталог релизов"
                }

                IconButton {
                    id: informationMenuButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/information.svg"
                    iconWidth: 29
                    iconHeight: 29
                    showCrossIcon: page.showButtonVisibleChanger && page.hideInfoButton
                    visible: page.showButtonVisibleChanger || !page.hideInfoButton
                    onButtonPressed: {
                        if (page.showButtonVisibleChanger) {
                            page.hideInfoButton = !page.hideInfoButton;
                            localStorage.setHideInfoButton(page.hideInfoButton);
                        } else {
                            informationPopup.open();
                        }
                    }

                    Popup {
                        id: informationPopup
                        x: 40
                        y: sortingPopupButton.height - 100
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
                                text: "<a href='https://www.anilibria.tv'>Сайт анилибрии</a>"
                            }
                            LinkedText {
                                fontPointSize: 11
                                text: "<a href='https://t.me/desktopclientanilibria'>Канал о приложении</a>"
                            }

                        }
                    }

                    ToolTip.delay: 1000
                    ToolTip.visible: informationMenuButton.hovered
                    ToolTip.text: "Просмотреть полезные ссылки связанные с приложением"
                }
                IconButton {
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/hidebuttonmenu.svg"
                    iconWidth: 29
                    iconHeight: 29
                    backgroundColor: page.showButtonVisibleChanger ? "#8868b0ab" : "transparent"
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
                color: ApplicationTheme.pageUpperPanel

                Switch {
                    id: multupleMode
                    anchors.left: parent.left
                    onCheckedChanged: {
                        page.selectMode = checked;
                        if (!checked) {
                            page.selectedReleases = [];
                        } else {
                            page.openedRelease = null;
                        }
                    }
                    ToolTip.delay: 1000
                    ToolTip.visible: multupleMode.hovered
                    ToolTip.text: "Данный переключатель влияет на поведение при клике ЛКМ на релизах в списке\nОдиночный выбор позволяет открывать карточку с подробной информацией\nМножественный выбор позволяет выбрать несколько релизов и выполнять действия (добавить в избранное и т.п.)\nЧтобы переключать его можно использовать клик ПКМ в области списка релизов"
                }
                PlainText {
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
                    visible: page.synchronizeEnabled
                    fontPointSize: 12
                    text: "Выполняется синхронизация..."
                }
                PlainText {
                    text: page.sections[page.selectedSection]
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    fontPointSize: 12
                }
            }

            Rectangle {
                id: filtersContainer
                Layout.preferredWidth: 380
                Layout.alignment: Qt.AlignHCenter
                Layout.preferredHeight: 36
                color: "transparent"

                Row {
                    width: filtersContainer.width
                    spacing: 8
                    RoundedTextBox {
                        id: filterByTitle
                        width: 200
                        height: 30
                        fontSize: 10
                        placeholder: "Введите название релиза"
                        onCompleteEditing: {
                            refreshAllReleases(false);
                        }
                    }
                    IconButton {
                        height: 30
                        width: 30
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: "../Assets/Icons/allreleases.svg"
                        iconWidth: 24
                        iconHeight: 24
                        ToolTip.delay: 1000
                        ToolTip.visible: hovered
                        ToolTip.text: "Все релизы"
                        onButtonPressed: {
                            changeSection(0);
                        }
                    }
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
                        ToolTip.text: "Избранное"
                        onButtonPressed: {
                            changeSection(1);
                        }
                    }
                    IconButton {
                        id: notificationMenuButton
                        height: 30
                        width: 30
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: "../Assets/Icons/notification.svg"
                        iconWidth: 24
                        iconHeight: 24
                        ToolTip.delay: 1000
                        ToolTip.visible: hovered
                        ToolTip.text: "Показать меню с фильтрами по уведомлениям"
                        onButtonPressed: {
                            notificationsMenuSections.open();
                        }

                        CommonMenu {
                            id: notificationsMenuSections
                            width: 350
                            y: notificationMenuButton.height

                            CommonMenuItem {
                                text: page.sections[2]
                                onPressed: {
                                    page.changeSection(2);
                                }
                            }
                            CommonMenuItem {
                                text: page.sections[3]
                                onPressed: {
                                    page.changeSection(3);
                                }
                            }
                            CommonMenuItem {
                                text: page.sections[4]
                                onPressed: {
                                    page.changeSection(4);
                                }
                            }
                            CommonMenuItem {
                                text: page.sections[6]
                                onPressed: {
                                    page.changeSection(6);
                                }
                            }
                        }
                    }
                    IconButton {
                        height: 30
                        width: 30
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: "../Assets/Icons/calendar.svg"
                        iconWidth: 26
                        iconHeight: 26
                        ToolTip.delay: 1000
                        ToolTip.visible: hovered
                        ToolTip.text: "Расписание релизов"
                        onButtonPressed: {
                            changeSection(5);
                        }
                    }

                    IconButton {
                        id: historyMenuButton
                        height: 30
                        width: 30
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: "../Assets/Icons/history.svg"
                        iconWidth: 24
                        iconHeight: 24
                        ToolTip.delay: 1000
                        ToolTip.visible: hovered
                        ToolTip.text: "Показать меню с фильтрами по истории и истории просмотра"
                        onButtonPressed: {
                            historyMenuSections.open();
                        }

                        CommonMenu {
                            id: historyMenuSections
                            width: 300
                            y: historyMenuButton.height

                            CommonMenuItem {
                                text: page.sections[7]
                                onPressed: {
                                    page.changeSection(7);
                                }
                            }
                            CommonMenuItem {
                                text: page.sections[8]
                                onPressed: {
                                    page.changeSection(8);
                                }
                            }
                        }
                    }
                    IconButton {
                        id: seenMenuButton
                        height: 30
                        width: 30
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: "../Assets/Icons/seenmarkpanel.svg"
                        iconWidth: 24
                        iconHeight: 24
                        ToolTip.delay: 1000
                        ToolTip.visible: hovered
                        ToolTip.text: "Показать меню с фильтрами по состоянию просмотра"
                        onButtonPressed: {
                            seenMenuSections.open();
                        }

                        CommonMenu {
                            id: seenMenuSections
                            width: 300
                            y: seenMenuButton.height

                            CommonMenuItem {
                                text: page.sections[9]
                                onPressed: {
                                    page.changeSection(9);
                                }
                            }
                            CommonMenuItem {
                                text: page.sections[10]
                                onPressed: {
                                    page.changeSection(10);
                                }
                            }
                            CommonMenuItem {
                                text: page.sections[11]
                                onPressed: {
                                    page.changeSection(11);
                                }
                            }
                        }
                    }
                    IconButton {
                        height: 30
                        width: 30
                        iconColor: ApplicationTheme.filterIconButtonColor
                        hoverColor: ApplicationTheme.filterIconButtonHoverColor
                        iconPath: "../Assets/Icons/alphabet.svg"
                        iconWidth: 24
                        iconHeight: 24
                        ToolTip.delay: 1000
                        ToolTip.visible: hovered
                        ToolTip.text: "Показать фильтр для выбора букв алфавита для поиска по первой букве релиза"
                        onButtonPressed: {
                            page.showAlpabeticalCharaters = true;
                        }
                    }
                }
            }

            Rectangle {
                color: "transparent"
                Layout.fillHeight: true
                Layout.fillWidth: true

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
                    visible: releasesModel.count === 0

                    PlainText {
                        anchors.centerIn: parent
                        fontPointSize: 14
                        horizontalAlignment: Text.AlignHCenter
                        text: localStorage.countReleases > 0 ? "По текущему фильтру ничего не найдено\nПопробуйте указать другие фильтры или раздел и повторить поиск" : "Релизы еще не загружены\nПожалуйста подождите пока они загрузятся"
                    }
                }

                GridView {
                    id: scrollview
                    visible: releasesModel.count > 0
                    anchors.horizontalCenter: parent.horizontalCenter
                    height: parent.height
                    width: parent.width//Math.floor(window.width / 490) * 490
                    cellWidth: parent.width / Math.floor(parent.width / 490)
                    cellHeight: 290
                    delegate: releaseDelegate
                    model: releasesModel
                    clip: true
                    ScrollBar.vertical: ScrollBar {
                        active: true
                    }
                    onContentYChanged: {
                        if (page.fillingReleases) return;

                        if (scrollview.atYEnd) {
                            page.fillingReleases = true;
                            fillNextReleases();
                            page.fillingReleases = false;
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
                                releaseModel: modelData
                                favoriteReleases: page.favoriteReleases
                                isSelected: page.selectedReleases.filter(a => a === releaseModel.id).length

                                onLeftClicked: {
                                    if (page.openedRelease) return;

                                    page.selectItem(modelData);
                                }
                                onRightClicked: {
                                    multupleMode.checked = !multupleMode.checked;
                                }
                                onAddToFavorite: {
                                    synchronizationService.addUserFavorites(applicationSettings.userToken, modelData.id.toString());
                                    page.selectedReleases = [];
                                }
                                onRemoveFromFavorite: {
                                    synchronizationService.removeUserFavorites(applicationSettings.userToken, modelData.id.toString());
                                    page.selectedReleases = [];
                                }
                                onWatchRelease: {
                                    page.watchRelease(id, videos, -1);
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
        width: 50
        height: 50
        anchors.right: parent.right
        anchors.rightMargin: 20
        anchors.bottom: parent.bottom
        IconButton {
            anchors.centerIn: parent
            visible: scrollview.contentY > 100
            height: 30
            width: 30
            iconColor: ApplicationTheme.filterIconButtonColor
            hoverColor: ApplicationTheme.filterIconButtonHoverColor
            iconPath: "../Assets/Icons/arrowup.svg"
            iconWidth: 24
            iconHeight: 24
            ToolTip.delay: 1000
            ToolTip.visible: hovered
            ToolTip.text: "Вернуться в начало списка релизов"
            onButtonPressed: {
                scrollview.contentY = 0;
            }

        }
    }

    ColumnLayout {
        id: cardContainer
        visible: page.openedRelease ? true : false
        anchors.fill: parent
        spacing: 0
        Rectangle {
            color: ApplicationTheme.pageBackground
            Layout.fillWidth: true
            Layout.fillHeight: true
            Column {
                Grid {
                    id: releaseInfo
                    columnSpacing: 3
                    columns: 3
                    bottomPadding: 4
                    leftPadding: 4
                    topPadding: 4
                    rightPadding: 4
                    Image {
                        id: cardPoster
                        source: page.openedRelease ? localStorage.getReleasePosterPath(page.openedRelease.id, page.openedRelease.poster) : '../Assets/Icons/donate.jpg'
                        fillMode: Image.PreserveAspectCrop
                        width: 280
                        height: 390
                        layer.enabled: true
                        layer.effect: OpacityMask {
                            maskSource: cardMask
                        }
                    }
                    Column {
                        width: page.width - cardButtons.width - cardPoster.width
                        enabled: !!page.openedRelease
                        AccentText {
                            textFormat: Text.RichText
                            fontPointSize: 12
                            width: parent.width
                            leftPadding: 8
                            topPadding: 6
                            wrapMode: Text.WordWrap
                            maximumLineCount: 3
                            text: qsTr(page.openedRelease ? page.openedRelease.title : '')
                        }
                        PlainText {
                            textFormat: Text.RichText
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            wrapMode: Text.WordWrap
                            width: parent.width
                            maximumLineCount: 2
                            text: qsTr(page.openedRelease ? page.openedRelease.originalName : '')
                        }
                        PlainText {
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            text: qsTr("<b>Статус:</b> ") + qsTr(page.openedRelease ? `<a href="http://years">${page.openedRelease.status}</a>` : '')
                            onLinkActivated: {
                                statusesSearchField.text = page.openedRelease.status;
                                page.openedRelease = null;
                                page.refreshAllReleases(false);
                            }

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            }
                        }
                        PlainText {
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            text: qsTr("<b>Год:</b> ") + qsTr(page.openedRelease ?  `<a href="http://years">${page.openedRelease.year}</a>` : '')
                            onLinkActivated: {
                                yearsSearchField.text = page.openedRelease.year;
                                page.openedRelease = null;
                                page.refreshAllReleases(false);
                            }

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            }
                        }
                        PlainText {
                            visible: page.openedRelease && page.openedRelease.id && !!page.scheduledReleases[page.openedRelease.id]
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            text: qsTr("<b>В расписании:</b> ") + (page.openedRelease && page.scheduledReleases[page.openedRelease.id] ? getScheduleDay(page.scheduledReleases[page.openedRelease.id]) : '')
                        }

                        PlainText {
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            text: qsTr("<b>Сезон:</b> ") + qsTr(page.openedRelease ? `<a href="http://seasons">${page.openedRelease.season}</a>` : '')
                            onLinkActivated: {
                                seasonesSearchField.text = page.openedRelease.season;
                                page.openedRelease = null;
                                page.refreshAllReleases(false);
                            }

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            }
                        }
                        PlainText {
                            textFormat: Text.RichText
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            width: parent.width
                            wrapMode: Text.WordWrap
                            maximumLineCount: 2
                            text: qsTr("<b>Тип:</b> ") + qsTr(page.openedRelease ? page.openedRelease.type : '')
                        }
                        PlainText {
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            width: parent.width
                            wrapMode: Text.WordWrap
                            maximumLineCount: 2
                            text: qsTr("<b>Жанры:</b> ") + qsTr(page.openedRelease ? getMultipleLinks(page.openedRelease.genres) : '')
                            onLinkActivated: {
                                if (genresSearchField.text.length) {
                                    genresSearchField.text += ", " + link;
                                } else {
                                    genresSearchField.text = link;
                                }
                                page.openedRelease = null;
                                page.refreshAllReleases(false);
                            }

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            }
                        }
                        PlainText {
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            width: parent.width
                            wrapMode: Text.WordWrap
                            maximumLineCount: 2
                            text: qsTr("<b>Озвучка:</b> ") + qsTr(page.openedRelease ? getMultipleLinks(page.openedRelease.voices) : '')
                            onLinkActivated: {
                                if (voicesSearchField.text.length) {
                                    voicesSearchField.text += ", " + link;
                                } else {
                                    voicesSearchField.text = link;
                                }
                                page.openedRelease = null;
                                page.refreshAllReleases(false);
                            }

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            }
                        }
                        PlainText {
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            visible: page.openedRelease ? page.openedRelease.countSeensSeries === page.openedRelease.countVideos : false
                            width: parent.width
                            text: qsTr("<b>Все серии просмотрены</b>")
                        }
                        PlainText {
                            fontPointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            width: parent.width
                            wrapMode: Text.WordWrap
                            text: qsTr("<b>Описание:</b> ") + qsTr(page.openedRelease ? page.openedRelease.description : '')
                            onLinkActivated: {
                                if (link.indexOf("https://www.anilibria.tv/release/") === 0 || link.indexOf("http://www.anilibria.tv/release/") === 0) {
                                    let code = link.replace("https://www.anilibria.tv/release/", "").replace("http://www.anilibria.tv/release/", "").replace(".html", "")
                                    if (code.indexOf(`?`) > -1) code = code.substring( 0, code.indexOf(`?`));
                                    const release = JSON.parse(localStorage.getReleaseByCode(code));
                                    showReleaseCard(release);
                                } else {
                                    Qt.openUrlExternally(link);
                                }
                            }

                            MouseArea {
                                anchors.fill: parent
                                acceptedButtons: Qt.NoButton
                                cursorShape: parent.hoveredLink ? Qt.PointingHandCursor : Qt.ArrowCursor
                            }
                        }
                    }
                    Column {
                        id: cardButtons
                        width: 62
                        IconButton {
                            height: 40
                            width: 40
                            iconColor: ApplicationTheme.filterIconButtonColor
                            hoverColor: ApplicationTheme.filterIconButtonHoverColor
                            iconPath: "../Assets/Icons/close.svg"
                            iconWidth: 28
                            iconHeight: 28
                            onButtonPressed: {
                                page.openedRelease = null;
                            }
                        }
                        IconButton {
                            height: 40
                            width: 40
                            iconColor: ApplicationTheme.filterIconButtonColor
                            hoverColor: ApplicationTheme.filterIconButtonHoverColor
                            iconPath: "../Assets/Icons/copy.svg"
                            iconWidth: 26
                            iconHeight: 26
                            onButtonPressed: {
                                if (Qt.platform.os !== "windows") webView.visible = false;
                                cardCopyMenu.open();
                            }

                            TextEdit {
                                id: hiddenTextField
                                visible: false
                            }

                            CommonMenu {
                                id: cardCopyMenu
                                width: 350
                                onClosed: {
                                    if (Qt.platform.os !== "windows") webView.visible = true;
                                }

                                CommonMenuItem {
                                    text: "Копировать название"
                                    onPressed: {
                                        copyToClipboard(page.openedRelease.title);
                                    }
                                }
                                CommonMenuItem {
                                    text: "Копировать оригинальное название"
                                    onPressed: {
                                        copyToClipboard(page.openedRelease.originalName);
                                    }
                                }
                                CommonMenuItem {
                                    text: "Копировать оба названия"
                                    onPressed: {
                                        copyToClipboard(page.openedRelease.title + ", " + page.openedRelease.originalName);
                                    }
                                }
                                CommonMenuItem {
                                    text: "Копировать описание"
                                    onPressed: {
                                        copyToClipboard(page.openedRelease.description);
                                    }
                                }
                            }
                        }
                        IconButton {
                            height: 40
                            width: 40
                            iconColor: ApplicationTheme.filterIconButtonColor
                            hoverColor: ApplicationTheme.filterIconButtonHoverColor
                            iconPath: "../Assets/Icons/vk.svg"
                            iconWidth: 26
                            iconHeight: 26
                            onButtonPressed: {
                                if (Qt.platform.os !== "windows") webView.visible = false;
                                vkontakteMenu.open();
                            }

                            CommonMenu {
                                id: vkontakteMenu
                                width: 350
                                onClosed: {
                                    if (Qt.platform.os !== "windows") webView.visible = true;
                                }

                                CommonMenuItem {
                                    text: "Авторизоваться для комментариев"
                                    onPressed: {
                                        webView.url = "https://oauth.vk.com/authorize?client_id=-1&display=widget&widget=4&redirect_uri=https://vk.com/";
                                    }
                                }
                                CommonMenuItem {
                                    text: "Переоткрыть комментарии"
                                    onPressed: {
                                        webView.url = getVkontakteCommentPage();
                                    }
                                }
                            }
                        }
                        IconButton {
                            height: 40
                            width: 40
                            iconColor: ApplicationTheme.filterIconButtonColor
                            hoverColor: ApplicationTheme.filterIconButtonHoverColor
                            iconPath: "../Assets/Icons/seenmarkpanel.svg"
                            iconWidth: 26
                            iconHeight: 26
                            onButtonPressed: {
                                if (Qt.platform.os !== "windows") webView.visible = false;
                                seenMarkMenu.open();
                            }

                            CommonMenu {
                                id: seenMarkMenu
                                width: 350
                                onClosed: {
                                    if (Qt.platform.os !== "windows") webView.visible = true;
                                }

                                CommonMenuItem {
                                    text: "Отметить как просмотренное"
                                    onPressed: {
                                        setSeenStateForOpenedRelease(true);
                                    }
                                }
                                CommonMenuItem {
                                    text: "Отметить как не просмотренное"
                                    onPressed: {
                                        setSeenStateForOpenedRelease(false);
                                    }
                                }
                            }
                        }
                        IconButton {
                            height: 40
                            width: 40
                            iconColor: page.openedRelease && page.favoriteReleases.filter(a => a === page.openedRelease.id).length ? ApplicationTheme.selectedFavorite : ApplicationTheme.filterIconButtonColor
                            hoverColor: ApplicationTheme.filterIconButtonHoverColor
                            iconPath: "../Assets/Icons/favorite.svg"
                            iconWidth: 26
                            iconHeight: 26
                            onButtonPressed: {
                                if (!window.userModel.login) {
                                    favoritePopupHeader.text = "Избранное не доступно";
                                    favoritePopupMessage.text = "Чтобы добавлять в избранное нужно вначале авторизоваться. Для этого перейдите на страницу Войти в меню и войдите под данными своего аккаунта. Если вы не зарегистрированы то необходимо сделать это на сайте, ссылка на сайт будет на странице Войти.";
                                    messagePopup.open();
                                    return;
                                }

                                if (Qt.platform.os !== "windows") webView.visible = false;
                                cardFavoritesMenu.open();
                            }

                            CommonMenu {
                                id: cardFavoritesMenu
                                width: 350
                                onClosed: {
                                    if (Qt.platform.os !== "windows") webView.visible = true;
                                }

                                CommonMenuItem {
                                    enabled: page.openedRelease && !page.favoriteReleases.filter(a => a === page.openedRelease.id).length
                                    text: "Добавить в избранное"
                                    onPressed: {
                                        synchronizationService.addUserFavorites(applicationSettings.userToken, page.openedRelease.id.toString());
                                        page.selectedReleases = [];
                                    }
                                }
                                CommonMenuItem {
                                    enabled: page.openedRelease && page.favoriteReleases.filter(a => a === page.openedRelease.id).length
                                    text: "Удалить из избранного"
                                    onPressed: {
                                        synchronizationService.removeUserFavorites(applicationSettings.userToken, page.openedRelease.id.toString());
                                        page.selectedReleases = [];
                                    }
                                }
                            }
                        }
                        IconButton {
                            height: 40
                            width: 40
                            iconColor: ApplicationTheme.filterIconButtonColor
                            hoverColor: ApplicationTheme.filterIconButtonHoverColor
                            iconPath: "../Assets/Icons/external.svg"
                            iconWidth: 26
                            iconHeight: 26
                            onButtonPressed: {
                                if (Qt.platform.os !== "windows") webView.visible = false;
                                externalPlayerMenu.open();
                            }

                            CommonMenu {
                                id: externalPlayerMenu
                                width: 380
                                onClosed: {
                                    if (Qt.platform.os !== "windows") webView.visible = true;
                                }

                                CommonMenuItem {
                                    text: "Открыть во внешнем плеере в HD качестве"
                                    onPressed: {
                                        openInExternalPlayer(localStorage.packAsM3UAndOpen(page.openedRelease.id, "hd"));
                                        externalPlayerMenu.close();
                                    }
                                }
                                CommonMenuItem {
                                    text: "Открыть во внешнем плеере в SD качестве"
                                    onPressed: {
                                        openInExternalPlayer(localStorage.packAsM3UAndOpen(page.openedRelease.id, "sd"));
                                        externalPlayerMenu.close();
                                    }
                                }
                                CommonMenuItem {
                                    text: "Открыть во внешнем плеере в FullHD качестве"
                                    onPressed: {
                                        openInExternalPlayer(localStorage.packAsM3UAndOpen(page.openedRelease.id, "fullhd"));
                                        externalPlayerMenu.close();
                                    }
                                }

                                CommonMenuItem {
                                    notVisible: Qt.platform.os !== "windows"
                                    text: "Открыть в плеере MPC в HD качестве"
                                    onPressed: {
                                        openInExternalPlayer(localStorage.packAsMPCPLAndOpen(page.openedRelease.id, "hd"));
                                        externalPlayerMenu.close();
                                    }
                                }
                                CommonMenuItem {
                                    notVisible: Qt.platform.os !== "windows"
                                    text: "Открыть в плеере MPC в SD качестве"
                                    onPressed: {
                                        openInExternalPlayer(localStorage.packAsMPCPLAndOpen(page.openedRelease.id, "sd"));
                                        externalPlayerMenu.close();
                                    }
                                }
                                CommonMenuItem {
                                    notVisible: Qt.platform.os !== "windows"
                                    text: "Открыть в плеере MPC в FullHD качестве"
                                    onPressed: {
                                        openInExternalPlayer(localStorage.packAsMPCPLAndOpen(page.openedRelease.id, "fullhd"));
                                        externalPlayerMenu.close();
                                    }
                                }
                            }
                        }

                        IconButton {
                            height: 40
                            width: 40
                            iconColor: ApplicationTheme.filterIconButtonColor
                            hoverColor: ApplicationTheme.filterIconButtonHoverColor
                            iconPath: "../Assets/Icons/online.svg"
                            iconWidth: 26
                            iconHeight: 26
                            onButtonPressed: {
                                if (Qt.platform.os !== "windows") webView.visible = false;
                                setSeriesMenu.open();
                            }

                            CommonMenu {
                                id: setSeriesMenu
                                width: 330
                                onClosed: {
                                    if (Qt.platform.os !== "windows") webView.visible = true;
                                }

                                Repeater {
                                    model: page.openedRelease ? page.openedRelease.countVideos : 0

                                    CommonMenuItem {
                                        text: "Серия " + (index + 1)
                                        onPressed: {
                                            watchRelease(page.openedRelease.id, page.openedRelease.videos, index);
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                Rectangle {
                    color: "transparent"
                    width: cardContainer.width
                    height: 60

                    RoundedActionButton {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 10
                        anchors.left: parent.left
                        text: qsTr("Скачать")
                        onClicked: {
                            if (Qt.platform.os !== "windows") webView.visible = false;
                            dowloadTorrent.open();
                        }

                        CommonMenu {
                            id: dowloadTorrent
                            y: parent.height - parent.height - (torrentsModel.count * 40)
                            width: 320
                            onClosed: {
                                if (Qt.platform.os !== "windows") webView.visible = true;
                            }

                            Repeater {
                                model: torrentsModel
                                CommonMenuItem {
                                    text: "Скачать " + modelData.quality + " [" + modelData.series + "]"
                                    onPressed: {
                                        const torrentUri = synchronizationService.combineWithWebSiteUrl(modelData.url);
                                        synchronizationService.downloadTorrent(torrentUri);
                                    }
                                }
                            }
                        }
                    }

                    PlainText {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 100
                        fontPointSize: 11
                        text: "Доступно "+ (page.openedRelease ? page.openedRelease.countTorrents : "0" ) + " торрентов"
                    }

                    PlainText {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: watchButton.left
                        anchors.rightMargin: 10
                        fontPointSize: 11
                        text: "Доступно "+ (page.openedRelease ? page.openedRelease.countVideos : "0" ) + " серий онлайн"
                    }

                    RoundedActionButton {
                        id: watchButton
                        text: qsTr("Смотреть")
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        onClicked: {
                            watchRelease(page.openedRelease.id, page.openedRelease.videos, -1);

                            page.openedRelease = null;
                        }
                    }

                    Row {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                    }

                }
                WebView {
                    id: webView
                    visible: page.openedRelease ? true : false
                    width: cardContainer.width
                    height: cardContainer.height - releaseInfo.height - 60
                }
            }
        }

    }

    ReleaseAlphabeticalCharacters {
        id: releaseAlphabeticalCharacters
        visible: page.showAlpabeticalCharaters
    }

    function setSeenStateForOpenedRelease(newState) {
        localStorage.setSeenMarkAllSeries(page.openedRelease.id, page.openedRelease.countVideos, newState);
        page.openedRelease.countSeensSeries = newState ? page.openedRelease.countVideos : 0;
        const oldRelease = page.openedRelease;
        page.openedRelease = null;
        page.openedRelease = oldRelease;
        refreshSeenMarks();
        refreshAllReleases(true);
    }

    function setSeenStateForRelease(newState, releases) {
        localStorage.setMultipleSeenMarkAllSeries(releases, newState);
        page.selectedReleases = [];
        refreshSeenMarks();
        refreshAllReleases(true);
    }

    function getVkontakteCommentPage() {
        return "https://vk.com/widget_comments.php?app=5315207&width=100%&_ver=1&limit=8&norealtime=0&url=https://www.anilibria.tv/release/" + page.openedRelease.code + ".html";
    }

    function selectItem(item) {
        if (page.selectMode) {
            if (page.openedRelease) page.openedRelease = null;
            if (page.selectedReleases.find(a => a === item.id)) {
                page.selectedReleases = page.selectedReleases.filter(a => a !== item.id);
            } else {
                page.selectedReleases.push(item.id);
            }

            //WORKAROUND: fix refresh list
            const oldSelectedReleases = page.selectedReleases;
            page.selectedReleases = [];
            page.selectedReleases = oldSelectedReleases;
        } else {
            showReleaseCard(item);
        }
    }

    function getReleasesByFilter() {
        return JSON.parse(
            localStorage.getReleasesByFilter(
                page.pageIndex,
                filterByTitle.textContent,
                page.selectedSection,
                descriptionSearchField.text,
                typeSearchField.text,
                genresSearchField.text,
                orAndGenresSearchField.checked,
                voicesSearchField.text,
                orAndVoicesSearchField.checked,
                yearsSearchField.text,
                seasonesSearchField.text,
                statusesSearchField.text,
                sortingComboBox.currentIndex,
                sortingDirectionComboBox.currentIndex == 1 ? true : false,
                favoriteMarkSearchField.currentIndex,
                seenMarkSearchField.currentIndex,
                alphabetListModel.getSelectedCharacters()
            )
        );
    }

    function refreshSeenMarks() {
        page.seenMarks = JSON.parse(localStorage.getSeenMarks());
    }

    function setSeensCounts(releases) {
        for (const release of releases) {
            release.countSeensSeries = 0;
            if (release.id in page.seenMarks) {
                release.countSeensSeries = page.seenMarks[release.id];
            }
        }
    }

    function fillNextReleases() {
        if (releasesModel.count < 12) return;
        if (page.pageIndex === -1) return;

        page.pageIndex += 1;

        const nextPageReleases = getReleasesByFilter();
        setSeensCounts(nextPageReleases);
        for (const displayRelease of nextPageReleases) releasesModel.append({ model: displayRelease });

        if (nextPageReleases.length < 12) page.pageIndex = -1;
    }

    function refreshAllReleases(notResetScroll) {
        if (Object.keys(page.seenMarks).length === 0) refreshSeenMarks();
        page.pageIndex = 1;
        releasesModel.clear();
        const displayReleases = getReleasesByFilter();
        setSeensCounts(displayReleases);
        for (const displayRelease of displayReleases) releasesModel.append({ model: displayRelease });
        if (!notResetScroll) scrollview.contentY = 0;
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
        alphabetListModel.clearCharacters();
    }

    function changeSection(section) {
        if (section === page.selectedSection) return;

        if (clearFilterAfterChangeSectionSwitch.checked) {
            filterByTitle.textContent = "";
            page.clearAdditionalFilters();
        }

        page.selectedSection = section;
        if (section in page.sectionSortings) {
            const defaultSorting = page.sectionSortings[section];
            sortingComboBox.currentIndex = defaultSorting.field;
            sortingDirectionComboBox.currentIndex = defaultSorting.direction;
        }

        refreshAllReleases(false);
    }

    function refreshSchedule() {
        const schedule = localStorage.getSchedule();
        if (schedule) page.scheduledReleases = JSON.parse(schedule);
    }

    function getScheduleDay(dayNumber) {
        const day = parseInt(dayNumber);
        switch (day){
            case 1: return "понедельник";
            case 2: return "вторник";
            case 3: return "среда";
            case 4: return "четверг";
            case 5: return "пятница";
            case 6: return "суббота";
            case 7: return "воскресенье";
        }
        return "---";
    }

    function showReleaseCard(release) {
        if (release.id === -1) return;

        torrentsModel.clear();

        const torrents = JSON.parse(release.torrents);

        for (const torrent of torrents) {
            torrentsModel.append({ model: torrent });
        }

        page.openedRelease = release;
        localStorage.setToReleaseHistory(release.id, 0);
        analyticsService.sendView("releasecard", "show", "%2Freleases");

        localStorage.resetReleaseChanges(release.id);
        page.changesCounts = localStorage.getChangesCounts();

        webView.url = getVkontakteCommentPage();
    }

    function copyToClipboard(text) {
        hiddenTextField.text = text;
        hiddenTextField.selectAll();
        hiddenTextField.copy();
    }

    function openInExternalPlayer(url) {
        if (!url) return;

        Qt.openUrlExternally(url);
    }

    function getMultipleLinks(text) {
        if (!text) return "";
        let result = "";

        const parts = text.split(",");
        let isFirst = true;
        for (const part of parts) {
            const partData = part.trim();
            result += (!isFirst ? ", " : "") + `<a href="${partData}">${partData}</a>`;
            isFirst = false;
        }

        return result;
    }

    function findReleaseById(id) {
        for (let i = 0; i < releasesModel.count; i++) {
            const release = releasesModel.get(i).model;
            if (release.id === id) return release;
        }

        return null;
    }

    Component.onCompleted: {
        refreshAllReleases(false);
        refreshSchedule();

        const userSettings = JSON.parse(localStorage.getUserSettings());
        downloadTorrentMode.currentIndex = userSettings.torrentDownloadMode;
        notificationForFavorites.checked = userSettings.notificationForFavorites;
        darkModeSwitch.checked = applicationSettings.isDarkTheme;
        clearFilterAfterChangeSectionSwitch.checked = userSettings.clearFiltersAfterChangeSection;
        compactModeSwitch.checked = userSettings.compactMode;
        page.hideCinemahallButton = userSettings.hideCinemhallButton;
        page.hideDownloadButton = userSettings.hideDownloadButton;
        page.hideRandomReleaseButton = userSettings.hideRandomReleaseButton;
        page.hideNotificationButton = userSettings.hideNotificationButton;
        page.hideInfoButton = userSettings.hideInfoButton;
        page.hideSortButton = userSettings.hideSortButton;
        page.hideFilterButton = userSettings.hideFilterButton;
    }
}
