import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtWebView 1.1
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2
import QtGraphicalEffects 1.0
import Anilibria.Services 1.0
import "../Controls"

Page {
    id: page
    property bool selectMode
    property var selectedReleases: []
    property var displayedReleases: []
    property var favoriteReleases: []
    property var scheduledReleases: ({})
    property int pageIndex: 1
    property bool isBusy: false
    property var openedRelease: null
    property bool runRefreshFavorties: false
    property bool synchronizeEnabled: false
    property int selectedSection: 0
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

    signal navigateFrom()
    signal watchRelease(int releaseId, string videos)
    signal refreshReleases()
    signal refreshFavorites()
    signal refreshReleaseSchedules()
    signal requestSynchronizeReleases()
    signal navigateTo()

    onWidthChanged: {
        const columnCount = parseInt(page.width / 520);
        itemGrid.columns = columnCount < 1 ? 1 : columnCount;
    }

    onRefreshReleases: {
        refreshAllReleases();
    }

    onRefreshReleaseSchedules: {
        refreshSchedule();
    }

    onRefreshFavorites: {
        page.favoriteReleases = localStorage.getFavorites().map(a => a);

        if (page.runRefreshFavorties && page.selectedReleases.length) {
            page.runRefreshFavorties = false;
            page.selectedReleases = [];
        }
    }

    background: Rectangle {
        color: "#D3D3D3"
    }

    anchors.fill: parent

    ListModel {
        id: releasesModel
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
                IconButton {
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

                    Menu {
                        id: favoriteMenu
                        y: favoriteMenuButton.height

                        MenuItem {
                            font.pixelSize: 14
                            text: "Добавить в избранное"
                            onPressed: {
                                page.runRefreshFavorties = true;
                                synchronizationService.addUserFavorites(applicationSettings.userToken, page.selectedReleases.join(','));
                            }
                        }
                        MenuItem {
                            font.pixelSize: 14
                            text: "Удалить из избранного"
                            onPressed: {
                                page.runRefreshFavorties = true;
                                synchronizationService.removeUserFavorites(applicationSettings.userToken, page.selectedReleases.join(','));
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
                            Text {
                                id: favoritePopupHeader
                                width: messagePopup.width - 20
                                font.pixelSize: 14
                                font.bold: true
                                elide: Text.ElideRight
                            }

                            Rectangle {
                                width: messagePopup.width - 20
                                height: messagePopup.height - 50
                                Text {
                                    id: favoritePopupMessage
                                    anchors.verticalCenter: parent.verticalCenter
                                    width: parent.width
                                    font.pixelSize: 12
                                    wrapMode: Text.WordWrap
                                }
                            }
                        }
                    }
                }
                IconButton {
                    id: searchPopupButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/search.svg"
                    iconWidth: 29
                    iconHeight: 29
                    onButtonPressed: {
                        filtersPopup.open();
                    }

                    Popup {
                        id: filtersPopup
                        x: 40
                        y: searchPopupButton.height - 45
                        width: 450
                        height: 380
                        modal: true
                        focus: true
                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                        Rectangle {
                            width: parent.width
                            Button {
                                id: startFilterButton
                                anchors.left: parent.left
                                text: "Фильтровать"
                                onClicked: {
                                    page.refreshAllReleases();
                                }
                            }
                            Button {
                                id: clearFiltersButton
                                anchors.right: parent.right
                                text: "Очистить фильтры"
                                onClicked: {
                                    descriptionSearchField.text = "";
                                    typeSearchField.text = "";

                                    page.refreshAllReleases();
                                }
                            }
                            Text {
                                id: labelDescriptionSearchField
                                anchors.top: clearFiltersButton.bottom
                                font.pixelSize: 14
                                text: qsTr("Описание")
                            }
                            Text {
                                id: labelTypeSearchField
                                anchors.top: clearFiltersButton.bottom
                                anchors.left: typeSearchField.left
                                font.pixelSize: 14
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

                            Text {
                                id: labelGenresSearchField
                                anchors.top: descriptionSearchField.bottom
                                anchors.rightMargin: 10
                                font.pixelSize: 14
                                text: qsTr("Жанры")
                            }
                            TextField {
                                id: genresSearchField
                                width: parent.width * 0.7
                                anchors.top: labelGenresSearchField.bottom
                                placeholderText: "Вводите жанры через запятую"
                            }
                            Text {
                                id: labelOrAndGenresSearchField
                                anchors.top: labelGenresSearchField.bottom
                                anchors.left: genresSearchField.right
                                topPadding: 16
                                leftPadding: 4
                                font.pixelSize: 14
                                text: qsTr("ИЛИ/И")
                            }
                            Switch {
                                id: orAndGenresSearchField
                                anchors.top: labelGenresSearchField.bottom
                                anchors.left: labelOrAndGenresSearchField.right
                            }

                            Text {
                                id: labelVoicesSearchField
                                anchors.top: genresSearchField.bottom
                                anchors.rightMargin: 10
                                font.pixelSize: 14
                                text: qsTr("Озвучка")
                            }
                            TextField {
                                id: voicesSearchField
                                width: parent.width * 0.7
                                anchors.top: labelVoicesSearchField.bottom
                                placeholderText: "Вводите войсеров через запятую"
                            }
                            Text {
                                id: labelOrAndVoicesSearchField
                                anchors.top: labelVoicesSearchField.bottom
                                anchors.left: voicesSearchField.right
                                topPadding: 16
                                leftPadding: 4
                                font.pixelSize: 14
                                text: qsTr("ИЛИ/И")
                            }
                            Switch {
                                id: orAndVoicesSearchField
                                anchors.top: labelVoicesSearchField.bottom
                                anchors.left: labelOrAndVoicesSearchField.right
                            }

                            Text {
                                id: labelYearsSearchField
                                anchors.top: voicesSearchField.bottom
                                font.pixelSize: 14
                                text: qsTr("Года")
                            }
                            Text {
                                id: labelSeasonsSearchField
                                anchors.top: voicesSearchField.bottom
                                anchors.left: typeSearchField.left
                                font.pixelSize: 14
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
                            Text {
                                id: labelStatusesSearchField
                                anchors.top: yearsSearchField.bottom
                                anchors.rightMargin: 10
                                font.pixelSize: 14
                                text: qsTr("Статусы")
                            }
                            TextField {
                                id: statusesSearchField
                                anchors.top: labelStatusesSearchField.bottom
                                anchors.right: parent.right
                                anchors.left: parent.left
                                placeholderText: "Вводите статусы через запятую"
                            }

                        }
                    }
                }
                IconButton {
                    id: sortingPopupButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/sort.svg"
                    iconWidth: 29
                    iconHeight: 29
                    onButtonPressed: {
                        sortingPopup.open();
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
                            Button {
                                id: startSortingButton
                                anchors.right: parent.right
                                text: "Сортировать"
                                onClicked: {
                                    page.refreshAllReleases();
                                }
                            }

                            Text {
                                id: labelSortingField
                                anchors.top: startSortingButton.bottom
                                font.pixelSize: 14
                                text: qsTr("Сортировать по")
                            }
                            ComboBox {
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
                                }
                            }

                            Text {
                                id: labelSortingDirection
                                anchors.top: sortingComboBox.bottom
                                font.pixelSize: 14
                                text: qsTr("В порядке")
                            }
                            ComboBox {
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
                }
                IconButton {
                    id: notificationPopupButton
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/notification.svg"
                    iconWidth: 29
                    iconHeight: 29
                    onButtonPressed: {
                        page.changesCounts = localStorage.getChangesCounts();
                        notificationPopup.open();
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
                            Text {
                                anchors.centerIn: parent
                                text: "У Вас нет новых уведомлений"
                                font.pixelSize: 20
                            }
                        }

                        Rectangle {
                            visible: localStorage.isChangesExists
                            width: parent.width
                            Button {
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
                                    border.width: 3
                                    border.color: "red"
                                    width: 340
                                    height: 40
                                    Text {
                                        anchors.centerIn: parent
                                        font.pixelSize: 14
                                        text: "Новых релизов: " + page.changesCounts[0]
                                    }
                                }
                                Rectangle {
                                    visible: page.changesCounts[1] > 0
                                    border.width: 3
                                    border.color: "red"
                                    width: 340
                                    height: 40
                                    Text {
                                        anchors.centerIn: parent
                                        font.pixelSize: 14
                                        text: "Релизов с новыми сериями: " + page.changesCounts[1]
                                    }
                                }
                                Rectangle {
                                    visible: page.changesCounts[2] > 0
                                    border.width: 3
                                    border.color: "red"
                                    width: 340
                                    height: 40
                                    Text {
                                        anchors.centerIn: parent
                                        font.pixelSize: 14
                                        text: "Новые торренты: " + page.changesCounts[2]
                                    }
                                }
                                Rectangle {
                                    visible: page.changesCounts[3] > 0
                                    border.width: 3
                                    border.color: "red"
                                    width: 340
                                    height: 40
                                    Text {
                                        anchors.centerIn: parent
                                        font.pixelSize: 14
                                        text: "Релизы с обновленными торрентами: " + page.changesCounts[3]
                                    }
                                }
                            }
                        }
                    }
                }
                IconButton {
                    height: 45
                    width: 40
                    iconColor: "white"
                    iconPath: "../Assets/Icons/dice.svg"
                    iconWidth: 29
                    iconHeight: 29
                    onButtonPressed: {
                        const randomRelease = JSON.parse(localStorage.getRandomRelease());
                        showReleaseCard(randomRelease);
                    }
                }
                IconButton {
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
                        y: sortingPopupButton.height - 100
                        width: 370
                        height: 160
                        modal: true
                        focus: true
                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                        Text {
                            id: downloadTorrentModeLabel
                            font.pixelSize: 14
                            text: "Торрент"
                        }
                        ComboBox {
                            id: downloadTorrentMode
                            currentIndex: 0
                            anchors.top: downloadTorrentModeLabel.bottom
                            width: 350
                            model: ["Открыть в торрент клиенте", "Сохранить файл"]
                            onCurrentIndexChanged: {
                                localStorage.setTorrentDownloadMode(downloadTorrentMode.currentIndex);
                            }
                        }

                        Text {
                            id: notificationForFavoritesLabel
                            anchors.top: downloadTorrentMode.bottom
                            anchors.topMargin: 4
                            font.pixelSize: 14
                            text: "Уведомления по избранным (пока не работает)"
                        }

                        Switch {
                            id: notificationForFavorites
                            anchors.top: notificationForFavoritesLabel.bottom
                            onCheckedChanged: {
                                localStorage.setNotificationForFavorites(checked);
                            }
                        }

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
                Row {
                    Switch {
                        id: multupleMode
                        onCheckedChanged: {
                            page.selectMode = checked;
                            if (!checked) {
                                page.selectedReleases = [];
                            } else {
                                page.openedRelease = null;
                            }
                        }
                    }
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        visible: page.synchronizeEnabled
                        font.pointSize: /*Qt.platform.os === `osx` ? 18 :*/ 12
                        text: "Выполняется синхронизация..."
                    }
                }
                Text {
                    text: page.sections[page.selectedSection]
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    font.pointSize: 12
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
                        fontSize: 12
                        placeholder: "Введите название релиза"
                        onCompleteEditing: {
                            refreshAllReleases();
                        }
                    }
                    IconButton {
                        height: 30
                        width: 30
                        iconColor: "black"
                        hoverColor: "white"
                        iconPath: "../Assets/Icons/allreleases.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            changeSection(0);
                        }
                    }
                    IconButton {
                        height: 30
                        width: 30
                        iconColor: "black"
                        hoverColor: "white"
                        iconPath: "../Assets/Icons/favorite.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            changeSection(1);
                        }
                    }
                    IconButton {
                        id: notificationMenuButton
                        height: 30
                        width: 30
                        iconColor: "black"
                        hoverColor: "white"
                        iconPath: "../Assets/Icons/notification.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            notificationsMenuSections.open();
                        }

                        Menu {
                            id: notificationsMenuSections
                            width: 300
                            y: notificationMenuButton.height

                            MenuItem {
                                font.pixelSize: 14
                                text: page.sections[2]
                                onPressed: {
                                    page.changeSection(2);
                                }
                            }
                            MenuItem {
                                font.pixelSize: 14
                                text: page.sections[3]
                                onPressed: {
                                    page.changeSection(3);
                                }
                            }
                            MenuItem {
                                font.pixelSize: 14
                                text: page.sections[4]
                                onPressed: {
                                    page.changeSection(4);
                                }
                            }
                            MenuItem {
                                font.pixelSize: 14
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
                        iconColor: "black"
                        hoverColor: "white"
                        iconPath: "../Assets/Icons/calendar.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            changeSection(5);
                        }
                    }

                    IconButton {
                        id: historyMenuButton
                        height: 30
                        width: 30
                        iconColor: "black"
                        hoverColor: "white"
                        iconPath: "../Assets/Icons/history.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            historyMenuSections.open();
                        }

                        Menu {
                            id: historyMenuSections
                            width: 300
                            y: historyMenuButton.height

                            MenuItem {
                                font.pixelSize: 14
                                text: page.sections[7]
                                onPressed: {
                                    page.changeSection(7);
                                }
                            }
                            MenuItem {
                                font.pixelSize: 14
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
                        iconColor: "black"
                        hoverColor: "white"
                        iconPath: "../Assets/Icons/seenmark.svg"
                        iconWidth: 24
                        iconHeight: 24
                        onButtonPressed: {
                            seenMenuSections.open();
                        }

                        Menu {
                            id: seenMenuSections
                            width: 300
                            y: seenMenuButton.height

                            MenuItem {
                                font.pixelSize: 14
                                text: page.sections[9]
                                onPressed: {
                                    page.changeSection(9);
                                }
                            }
                            MenuItem {
                                font.pixelSize: 14
                                text: page.sections[10]
                                onPressed: {
                                    page.changeSection(10);
                                }
                            }
                            MenuItem {
                                font.pixelSize: 14
                                text: page.sections[11]
                                onPressed: {
                                    page.changeSection(11);
                                }
                            }
                        }
                    }
                }
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
                    if (scrollview.atYEnd && !page.isBusy) {
                        page.isBusy = true;
                        fillNextReleases();
                        page.isBusy = false;
                    }
                }
                ScrollBar.vertical: ScrollBar {
                    active: true
                }

                ColumnLayout {
                    width: page.width
                    height: page.height
                    Grid {
                        id: itemGrid
                        Layout.alignment: Qt.AlignHCenter
                        columns: 2
                        spacing: 4
                        Repeater {
                            model: releasesModel
                            Rectangle {
                                width: 480
                                height: 260
                                radius: 10
                                border.color: "red"
                                border.width: page.selectedReleases.filter(a => a === modelData.id).length ? 3 : 0
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
                                    height: 260
                                    acceptedButtons: Qt.LeftButton | Qt.RightButton
                                    onClicked: {
                                        if(mouse.button & Qt.RightButton) {
                                            multupleMode.checked = !multupleMode.checked;
                                            return;
                                        }

                                        if (page.openedRelease) return;

                                        page.selectItem(modelData);
                                    }                                    
                                }
                                Grid {
                                    columnSpacing: 3
                                    columns: 2
                                    bottomPadding: 4
                                    leftPadding: 4
                                    topPadding: 4
                                    rightPadding: 4
                                    Image {
                                        source: modelData.poster
                                        fillMode: Image.PreserveAspectCrop
                                        width: 180
                                        height: 252
                                        layer.enabled: true
                                        layer.effect: OpacityMask {
                                            maskSource: mask
                                        }
                                    }
                                    Grid {
                                        height: 260
                                        Layout.row: 1
                                        Layout.column: 1
                                        rows: 2
                                        columns: 1
                                        Column {
                                            id: gridItemtextContainer
                                            Text {
                                                textFormat: Text.RichText
                                                color: "#a32727"
                                                font.pointSize: 12
                                                width: 280
                                                leftPadding: 8
                                                topPadding: 6
                                                wrapMode: Text.WordWrap
                                                maximumLineCount: 3
                                                text: qsTr(modelData.title)
                                            }
                                            Text {
                                                textFormat: Text.RichText
                                                font.pointSize: 10
                                                leftPadding: 8
                                                topPadding: 4
                                                text: qsTr("<b>Статус:</b> ") + qsTr(modelData.status)
                                            }
                                            Text {
                                                font.pointSize: 10
                                                leftPadding: 8
                                                topPadding: 4
                                                text: qsTr("<b>" + modelData.season + " " + modelData.year + "</b>")
                                            }
                                            Text {
                                                textFormat: Text.RichText
                                                font.pointSize: 10
                                                leftPadding: 8
                                                topPadding: 4
                                                width: 280
                                                wrapMode: Text.WordWrap
                                                maximumLineCount: 2
                                                text: qsTr("<b>Тип:</b> ") + qsTr(modelData.type)
                                            }
                                            Text {
                                                font.pointSize: 10
                                                leftPadding: 8
                                                topPadding: 4
                                                width: 280
                                                wrapMode: Text.WordWrap
                                                maximumLineCount: 2
                                                text: qsTr("<b>Жанры:</b> ") + qsTr(modelData.genres)
                                            }
                                            Text {
                                                font.pointSize: 10
                                                leftPadding: 8
                                                topPadding: 4
                                                width: 280
                                                wrapMode: Text.WordWrap
                                                maximumLineCount: 2
                                                text: qsTr("<b>Озвучка:</b> ") + qsTr(modelData.voices)
                                            }
                                            Row {
                                                leftPadding: 8
                                                topPadding: 4
                                                Image {
                                                    mipmap: true
                                                    source: '../Assets/Icons/online.svg'
                                                    width: 22
                                                    height: 22
                                                }
                                                Text {
                                                    leftPadding: 4
                                                    rightPadding: 4
                                                    font.pixelSize: 18
                                                    text: '' + modelData.countVideos
                                                }
                                                Image {
                                                    mipmap: true
                                                    source: '../Assets/Icons/utorrent.svg'
                                                    width: 22
                                                    height: 22
                                                }
                                                Text {
                                                    leftPadding: 4
                                                    rightPadding: 4
                                                    font.pixelSize: 18
                                                    text: '' + modelData.countTorrents
                                                }
                                            }
                                        }
                                        Rectangle {
                                            color: "transparent"
                                            height: 252 - gridItemtextContainer.height
                                            width: 280
                                            Row {
                                                anchors.bottom: parent.bottom
                                                leftPadding: 8
                                                topPadding: 8
                                                Image {
                                                    mipmap: true
                                                    source: '../Assets/Icons/rating.svg'
                                                    width: 20
                                                    height: 20
                                                }
                                                Text {
                                                    leftPadding: 4
                                                    rightPadding: 4
                                                    font.pixelSize: 18
                                                    text: modelData.rating
                                                }
                                                ColoredIcon {
                                                    visible: page.favoriteReleases.filter(a => a === modelData.id).length
                                                    iconSource: '../Assets/Icons/star.svg'
                                                    iconWidth: 20
                                                    iconHeight: 20
                                                    iconColor: "#a32727"
                                                }
                                                Text {
                                                    visible: page.favoriteReleases.filter(a => a === modelData.id).length
                                                    leftPadding: 4
                                                    color: "#a32727"
                                                    font.pixelSize: 18
                                                    text: "В избранном"
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

    ColumnLayout {
        id: cardContainer
        visible: page.openedRelease ? true : false
        anchors.fill: parent
        spacing: 0
        Rectangle {
            color: "#D3D3D3"
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
                        source: page.openedRelease ? page.openedRelease.poster : '../Assets/Icons/donate.jpg'
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
                        Text {
                            textFormat: Text.RichText
                            color: "#a32727"
                            font.pointSize: 12
                            width: parent.width
                            leftPadding: 8
                            topPadding: 6
                            wrapMode: Text.WordWrap
                            maximumLineCount: 3
                            text: qsTr(page.openedRelease ? page.openedRelease.title : '')
                        }
                        Text {
                            textFormat: Text.RichText
                            font.pointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            wrapMode: Text.WordWrap
                            width: parent.width
                            maximumLineCount: 2
                            text: qsTr(page.openedRelease ? page.openedRelease.originalName : '')
                        }
                        Text {
                            textFormat: Text.RichText
                            font.pointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            text: qsTr("<b>Статус:</b> ") + qsTr(page.openedRelease ? page.openedRelease.status : '')
                        }
                        Text {
                            font.pointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            text: qsTr("<b>Год:</b> ") + qsTr(page.openedRelease ? page.openedRelease.year : '')
                        }
                        Text {
                            visible: page.openedRelease && page.openedRelease.id && !!page.scheduledReleases[page.openedRelease.id]
                            font.pointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            text: qsTr("<b>В расписании:</b> ") + (page.openedRelease && page.scheduledReleases[page.openedRelease.id] ? getScheduleDay(page.scheduledReleases[page.openedRelease.id]) : '')
                        }

                        Text {
                            font.pointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            text: qsTr("<b>Сезон:</b> ") + qsTr(page.openedRelease ? page.openedRelease.season : '')
                        }
                        Text {
                            textFormat: Text.RichText
                            font.pointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            width: parent.width
                            wrapMode: Text.WordWrap
                            maximumLineCount: 2
                            text: qsTr("<b>Тип:</b> ") + qsTr(page.openedRelease ? page.openedRelease.type : '')
                        }
                        Text {
                            font.pointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            width: parent.width
                            wrapMode: Text.WordWrap
                            maximumLineCount: 2
                            text: qsTr("<b>Жанры:</b> ") + qsTr(page.openedRelease ? page.openedRelease.genres : '')
                        }
                        Text {
                            font.pointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            width: parent.width
                            wrapMode: Text.WordWrap
                            maximumLineCount: 2
                            text: qsTr("<b>Озвучка:</b> ") + qsTr(page.openedRelease ? page.openedRelease.voices : '')
                        }
                        Text {
                            textFormat: Text.RichText
                            font.pointSize: 10
                            leftPadding: 8
                            topPadding: 4
                            width: parent.width
                            wrapMode: Text.WordWrap
                            text: qsTr("<b>Описание:</b> ") + qsTr(page.openedRelease ? page.openedRelease.description : '')
                        }
                    }
                    Column {
                        id: cardButtons
                        width: 62
                        IconButton {
                            height: 40
                            width: 40
                            iconColor: "black"
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
                            iconColor: "black"
                            iconPath: "../Assets/Icons/copy.svg"
                            iconWidth: 26
                            iconHeight: 26
                            onButtonPressed: {
                                cardCopyMenu.open();
                            }

                            TextEdit {
                                id: hiddenTextField
                                visible: false
                            }

                            Menu {
                                id: cardCopyMenu
                                width: 300

                                MenuItem {
                                    width: parent.width
                                    font.pixelSize: 14
                                    text: "Копировать название"
                                    onPressed: {
                                        copyToClipboard(page.openedRelease.title);
                                    }
                                }
                                MenuItem {
                                    width: parent.width
                                    font.pixelSize: 14
                                    text: "Копировать оригинальное название"
                                    onPressed: {
                                        copyToClipboard(page.openedRelease.originalName);
                                    }
                                }
                                MenuItem {
                                    width: parent.width
                                    font.pixelSize: 14
                                    text: "Копировать оба названия"
                                    onPressed: {
                                        copyToClipboard(page.openedRelease.title + ", " + page.openedRelease.originalName);
                                    }
                                }
                                MenuItem {
                                    width: parent.width
                                    font.pixelSize: 14
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
                            iconColor: "black"
                            iconPath: "../Assets/Icons/vk.svg"
                            iconWidth: 26
                            iconHeight: 26
                            onButtonPressed: {
                                vkontakteMenu.open();
                            }
                            Menu {
                                id: vkontakteMenu
                                width: 300

                                MenuItem {
                                    width: parent.width
                                    font.pixelSize: 14
                                    text: "Авторизоваться для комментариев"
                                    onPressed: {
                                        webView.url = "https://oauth.vk.com/authorize?client_id=-1&display=widget&widget=4&redirect_uri=https://vk.com/";
                                    }
                                }
                                MenuItem {
                                    width: parent.width
                                    font.pixelSize: 14
                                    text: "Переоткрыть комментарии"
                                    onPressed: {
                                        webView.url = getVkontakteCommentPage();
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

                    Button {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.leftMargin: 10
                        anchors.left: parent.left
                        text: qsTr("Скачать")
                        onClicked: {
                            dowloadTorrent.open();
                        }

                        Menu {
                            id: dowloadTorrent
                            y: parent.height - parent.height - (torrentsModel.count * 40)
                            width: 300

                            Repeater {
                                model: torrentsModel
                                MenuItem {
                                    width: parent.width
                                    font.pixelSize: 14
                                    text: "Скачать " + modelData.quality + " [" + modelData.series + "]"
                                    onPressed: {
                                        const torrentUri = synchronizationService.combineWithWebSiteUrl(modelData.url);
                                        synchronizationService.downloadTorrent(torrentUri);
                                    }
                                }
                            }
                        }
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.left: parent.left
                        anchors.leftMargin: 100
                        font.pixelSize: 14
                        text: "Доступно "+ (page.openedRelease ? page.openedRelease.countTorrents : "0" ) + " торрентов"
                    }

                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: watchButton.left
                        anchors.rightMargin: 10
                        font.pixelSize: 14
                        text: "Доступно "+ (page.openedRelease ? page.openedRelease.countVideos : "0" ) + " серий онлайн"
                    }

                    Button {
                        id: watchButton
                        text: qsTr("Смотреть")
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        onClicked: {
                            watchRelease(page.openedRelease.id, page.openedRelease.videos);
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
                    url: page.openedRelease ? getVkontakteCommentPage() : "https://vk.com/";
                    onLoadingChanged: {
                        if (loadRequest.errorString) console.error(loadRequest.errorString);
                    }
                    onUrlChanged: {
                        console.log("changed url" + url);
                    }
                }
            }
        }

    }

    MessageDialog {
        id: notImplementedDialog
        title: "Не реализовано"
        text: "Пока указанная функция не реализована"
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
                sortingDirectionComboBox.currentIndex == 1 ? true : false
            )
        );
    }

    function fillNextReleases() {
        page.pageIndex += 1;
        const nextPageReleases = getReleasesByFilter();
        for (const displayRelease of nextPageReleases) releasesModel.append({ model: displayRelease });
    }

    function refreshAllReleases() {
        page.pageIndex = 1;
        releasesModel.clear();
        const displayReleases = getReleasesByFilter();
        for (const displayRelease of displayReleases) releasesModel.append({ model: displayRelease });
        scrollview.contentY = 0;
    }

    function changeSection(section) {
        page.selectedSection = section;
        if (section in page.sectionSortings) {
            const defaultSorting = page.sectionSortings[section];
            sortingComboBox.currentIndex = defaultSorting.field;
            sortingDirectionComboBox.currentIndex = defaultSorting.direction;
        }

        refreshAllReleases();
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
        torrentsModel.clear();

        const torrents = JSON.parse(release.torrents);

        for (const torrent of torrents) {
            torrentsModel.append({ model: torrent });
        }

        page.openedRelease = release;
        localStorage.setToReleaseHistory(release.id, 0);
    }

    function copyToClipboard(text) {
        hiddenTextField.text = text;
        hiddenTextField.selectAll();
        hiddenTextField.copy();
    }

    Component.onCompleted: {
        refreshAllReleases();
        refreshSchedule();

        const userSettings = JSON.parse(localStorage.getUserSettings());
        downloadTorrentMode.currentIndex = userSettings.torrentDownloadMode;
        notificationForFavorites.checked = userSettings.notificationForFavorites;
    }
}
