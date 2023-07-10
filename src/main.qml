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
import QtQuick.Window 2.12
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import QtQuick.Controls.Material 2.0
import QtQuick.Dialogs 1.2
import Anilibria.Services 1.0
import Anilibria.ListModels 1.0
import Anilibria.ViewModels 1.0
import CustomStyle 1.0
import "Views"
import "Controls"

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600
    minimumWidth: 300
    minimumHeight: 300
    title: qsTr("AniLibria.Qt")
    font.capitalization: Font.MixedCase
    property var userModel: ({})
    property string tempTorrentPath: ""
    property bool isShowFullScreenSize: false
    property int normalWindowSizeX: 0
    property int normalWindowSizeY: 0
    property int normalWindowSizeWidth: 0
    property int normalWindowSizeHeight: 0
    property int previousX
    property int previousY

    Material.accent: applicationThemeViewModel.materialAccent
    Material.theme: applicationThemeViewModel.basedOnDark ? Material.Dark : Material.Light
    Material.foreground: applicationThemeViewModel.colorMaterialText

    onClosing: {
        onlinePlayerWindow.closeWindow();
        onlinePlayerWindow.hide();
        applicationSettings.isMaximize = window.visibility === Window.Maximized;
    }

    onActiveChanged: {
        if (!active) videoplayer.windowNotActived();
    }

    onWidthChanged: {
        if (width < 750) {
            mainViewModel.isSmallSizeMode = true;
        } else {
            mainViewModel.isSmallSizeMode = false;
        }
    }

    Component.onCompleted: {
        const savedWidth = applicationSettings.windowWidth;
        const savedHeight = applicationSettings.windowHeight;
        const savedX = applicationSettings.windowX;
        const savedY = applicationSettings.windowY;

        //if coordinates not in active screen areas we restore default
        if (!isInActiveScreen(savedX, savedY)) return;

        if (applicationSettings.isMaximize) {
            window.showMaximized();
        } else {
            if (savedWidth > 0 && savedHeight > 0) {
                window.x = savedX;
                window.y = savedY;
                window.width = savedWidth;
                window.height = savedHeight;
            }
        }
    }

    Component.onDestruction: {
        if (userConfigurationViewModel.notSaveWindowPosition) {
            applicationSettings.windowWidth = 0;
            applicationSettings.windowHeight = 0;
            applicationSettings.windowX = 0;
            applicationSettings.windowY = 0;
            applicationSettings.isMaximize = false;
            return;
        }

        if (onlinePlayerViewModel.isFullScreen || applicationSettings.isMaximize) {
            applicationSettings.windowWidth = 0;
            applicationSettings.windowHeight = 0;
            applicationSettings.windowX = 0;
            applicationSettings.windowY = 0;
        } else {
            applicationSettings.windowWidth = window.width;
            applicationSettings.windowHeight = window.height;
            applicationSettings.windowX = window.x;
            applicationSettings.windowY = window.y;
        }
    }

    header: Rectangle {
        id: toolBar
        visible: window.visibility !== Window.FullScreen
        width: window.width
        height: 35
        color: applicationThemeViewModel.notificationCenterBackground

        Rectangle {
            id: titleArea
            enabled: false
            color: "transparent"
            anchors.left: parent.left
            anchors.right: parent.right
            height: parent.height

            AccentText {
                id: taskbarTitle
                anchors.centerIn: parent
                fontPointSize: mainViewModel.isSmallSizeMode ? 10 : 12
                text: mainViewModel.currentPageDisplayName
            }
        }
        IconButton {
            id: openDrawerButton
            anchors.left: parent.left
            anchors.leftMargin: 1
            anchors.top: parent.top
            anchors.topMargin: 1
            hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
            iconPath: applicationThemeViewModel.iconMainMenu
            height: 34
            width: 40
            iconWidth: 20
            iconHeight: 20
            tooltipMessage: "Открыть меню приложения"
            onButtonPressed: {
                drawer.open();
            }
        }
        IconButton {
            id: goToReleasePage
            visible: !mainViewModel.isSmallSizeMode
            anchors.left: openDrawerButton.right
            anchors.leftMargin: 1
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
            iconPath: applicationThemeViewModel.iconMainMenuReleases
            iconWidth: 20
            iconHeight: 20
            tooltipMessage: "Перейти на страницу Каталог Релизов"
            onButtonPressed: {
                mainViewModel.selectPage("release");
            }
        }
        IconButton {
            id: goToOnlineVideoPage
            visible: !mainViewModel.isSmallSizeMode
            anchors.left: goToReleasePage.right
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
            iconPath: applicationThemeViewModel.iconMainMenuVideoplayer
            iconWidth: 20
            iconHeight: 20
            tooltipMessage: "Перейти на страницу Видеоплеер"
            onButtonPressed: {
                mainViewModel.selectPage("videoplayer");
            }
        }
        IconButton {
            id: goToCinemaHall
            visible: !mainViewModel.isSmallSizeMode
            anchors.left: goToOnlineVideoPage.right
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
            iconPath: applicationThemeViewModel.iconMainMenuCinemahall
            iconWidth: 20
            iconHeight: 20
            tooltipMessage: "Перейти на страницу Кинозал"
            onButtonPressed: {
                mainViewModel.selectPage("cinemahall");
            }
        }
        IconButton {
            id: goToReleaseSeries
            visible: !mainViewModel.isSmallSizeMode
            anchors.left: goToCinemaHall.right
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
            iconPath: applicationThemeViewModel.iconMainMenuReleasesSeries
            iconWidth: 20
            iconHeight: 20
            tooltipMessage: "Перейти на страницу Связанные релизы"
            onButtonPressed: {
                mainViewModel.selectPage("releaseseries");
            }
        }
        IconButton {
            id: goToMyAnilibria
            visible: !mainViewModel.isSmallSizeMode
            anchors.left: goToReleaseSeries.right
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
            iconPath: applicationThemeViewModel.iconMainMenuMyAnilibria
            iconWidth: 20
            iconHeight: 20
            tooltipMessage: "Перейти на страницу Моя Анилибрия"
            onButtonPressed: {
                mainViewModel.selectPage("myanilibria");
            }
        }
        IconButton {
            visible: !mainViewModel.isSmallSizeMode
            anchors.right: selectThemeButton.left
            anchors.top: parent.top
            anchors.topMargin: 1
            iconPath: applicationThemeViewModel.currentItems.iconInfo
            height: 34
            width: 40
            iconWidth: 24
            iconHeight: 24
            tooltipMessage: "Просмотреть полезные ссылки связанные с приложением"
            onButtonPressed: {
                informationPopup.open();
            }

            DefaultPopup {
                id: informationPopup
                x: -300
                y: 40
                width: 320
                height: 96
                modal: true
                focus: true
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                Column {
                    LinkedText {
                        fontPointSize: 11
                        text: "<a href='https://t.me/+Le_oNL4Tw745YWUy'>Написать идею, ошибку, вопрос?</a>"
                    }
                    LinkedText {
                        fontPointSize: 11
                        text: "<a href='https://t.me/desktopclientanilibria'>Telegram канал с новостями</a>"
                    }
                    LinkedText {
                        fontPointSize: 11
                        text: "<a href='https://t.me/desktopclientanilibria/259'>Видео для новых пользователей</a>"
                    }
                    LinkedText {
                        fontPointSize: 11
                        text: "<a href='https://t.me/Libria911Bot'>Общая техподдержка Анилибрии</a>"
                    }
                }
            }
        }
        IconButton {
            id: selectThemeButton
            visible: !mainViewModel.isSmallSizeMode
            anchors.right: leftHalfScreenWindow.left
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
            iconPath: applicationThemeViewModel.currentItems.iconMainMenuThemeManager
            iconWidth: 24
            iconHeight: 24
            tooltipMessage: "Выбрать тему"
            onButtonPressed: {
                selectThemePopup.open();
            }

            DefaultPopup {
                id: selectThemePopup
                x: -250
                y: 40
                width: 350
                height: 100
                modal: true
                focus: true
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                Column {
                    height: 80
                    width: 120
                    spacing: 4

                    PlainText {
                        id: selectThemeLabel
                        fontPointSize: 11
                        text: "Тема"
                    }
                    CommonComboBox {
                        id: selectThemeComboBox
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
                }
            }
        }
        IconButton {
            id: leftHalfScreenWindow
            visible: !mainViewModel.isSmallSizeMode
            anchors.right: rightHalfScreenWindow.left
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
            iconPath: mainViewModel.hasBackHistory ? applicationThemeViewModel.currentItems.iconLeftHalfScreen : applicationThemeViewModel.currentItems.iconMinimizeWindow
            iconWidth: 24
            iconHeight: 24
            tooltipMessage: "Вернуть назад в истории переходов"
            onButtonPressed: {
                mainViewModel.backToPage();
            }
        }
        IconButton {
            id: rightHalfScreenWindow
            visible: !mainViewModel.isSmallSizeMode
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
            iconPath: mainViewModel.hasForwardHistory ? applicationThemeViewModel.iconRightHalfScreen : applicationThemeViewModel.currentItems.iconMaximizeWindow
            iconWidth: 24
            iconHeight: 24
            tooltipMessage: "Перейти вперед в истории переходов"
            onButtonPressed: {
                mainViewModel.forwardToPage();
            }
        }
    }

    footer: Rectangle {
        id: windowFooter
        width: window.width
        height: mainViewModel.isSmallSizeMode ? 1 : 16
        color: applicationThemeViewModel.notificationCenterBackground

        Item {
            visible: !mainViewModel.isSmallSizeMode
            anchors.left: parent.left
            anchors.leftMargin: 4
            width: 100
            PlainText {
                id: authentificationUser
                fontPointSize: 10
                text: window.userModel.login ? "Вы авторизованы как " + window.userModel.login : "Вы не авторизованы"
            }
        }

        PlainText {
            visible: !mainViewModel.isSmallSizeMode
            anchors.right: notificationCounter.left
            anchors.verticalCenter: parent.verticalCenter
            fontPointSize: 10
            text: "Релизов " + releasesViewModel.countReleases + " В избранном " + releasesViewModel.countFavorites + " Просмотрено " + releasesViewModel.countSeens + " В кинозале " + releasesViewModel.cinemahall.countCinemahall + " "
        }

        Row {
            id: notificationCounter
            visible: !mainViewModel.isSmallSizeMode
            anchors.right: parent.right

            IconButton {
                id: notificationPopupButton
                height: 16
                width: 16
                hoverColor: applicationThemeViewModel.filterIconButtonColor
                iconPath: notificationViewModel.countNotifications > 0 ? applicationThemeViewModel.currentItems.iconNotificationSelected : applicationThemeViewModel.currentItems.iconNotification
                iconWidth: 14
                iconHeight: 14
                onButtonPressed: {
                    notificationOverlay.visible = !notificationOverlay.visible;
                }
            }
            PlainText {
                id: countNotifications
                fontPointSize: 10
                text: notificationViewModel.countNotifications
            }
            Rectangle {
                width: 6
                height: 16
                color: "transparent"
            }
        }
    }

    function getCurrentScreen() {
        let currentScreen;
        const countScreens = Qt.application.screens.length;
        for (let i = 0; i < countScreens; i++) {
            const screen = Qt.application.screens[i];
            if (window.x >= screen.virtualX && window.x <= screen.virtualX + screen.width) {
                currentScreen = screen;
                break;
            }
        }

        return currentScreen;
    }

    function isInActiveScreen(x, y) {
        const countScreens = Qt.application.screens.length;
        for (let i = 0; i < countScreens; i++) {
            const screen = Qt.application.screens[i];
            const isInWidth = x >= screen.virtualX && x <= screen.virtualX + screen.width;
            const isInHeight = y >= screen.virtualY && y <= screen.virtualY + screen.width;
            if (isInWidth && isInHeight) return true;
        }

        return false;
    }

    NotificationViewModel {
        id: notificationViewModel
    }

    Item {
        id: windowSettings

        property real dpiSeparation: 0
        property bool isTopMost: false

        signal setStayOnTop();
        signal unsetStayOnTop();
        signal toggleStayOnTopMode();

        onSetStayOnTop: {
            window.flags += Qt.WindowStaysOnTopHint;

            windowSettings.isTopMost = true;
        }

        onUnsetStayOnTop: {
            if (!windowSettings.isTopMost) return;

            window.flags -= Qt.WindowStaysOnTopHint;

            windowSettings.isTopMost = false;
        }

        onToggleStayOnTopMode: {
            if (!windowSettings.isTopMost) {
                windowSettings.setStayOnTop();
            } else {
                windowSettings.unsetStayOnTop();
            }
        }

        Component.onCompleted: {
            if (Qt.platform.os === "linux") windowSettings.dpiSeparation = -0.8;
            if (Qt.platform.os === "osx") windowSettings.dpiSeparation = 1.87;
        }
    }

    VersionChecker {
        id: versionChecker
        onNewVersionAvailable: {
            notificationViewModel.sendInfoNotification(`Доступна новая версия ${version}. Перейдите для скачивания по <a href='${url}'>этой ссылке</a>`)
        }
    }

    ApplicationSettings {
        id: applicationSettings
        Component.onCompleted: {
            if (!applicationSettings.userToken) return;

            synchronizationService.getUserData(applicationSettings.userToken);

            analyticsService.sendVersion();
        }
    }

    LocalStorage {
        id: localStorage
    }

    AnalyticsService {
        id: analyticsService
    }

    ReleaseLinkedSeries {
        id: releaseLinkedSeries

    }

    FileDialog {
        id: saveTorrentFileDialog
        title: "Выберите куда и как сохранить торрент файл"
        selectExisting: false
        nameFilters: [ "Torrents (*.torrent)" ]

        onAccepted: {
            localStorage.copyTorrentToFile(window.tempTorrentPath, saveTorrentFileDialog.fileUrl);
        }
    }

    FileDialog {
        id: importReleasesFileDialog
        title: "Выберите файл для импорта"
        selectExisting: true
        nameFilters: [ "Releases (*.releases)" ]
        onAccepted: {
            releasesViewModel.importReleasesFromFile(importReleasesFileDialog.fileUrl);
        }
    }

    SynchronizationService {
        id: synchronizationService
        Component.onCompleted: {
            releasesViewModel.synchronizationEnabled = true;
            synchronizeReleases(1);
        }

        onUserDataReceived: {
            try {
                const userData = JSON.parse(data);
                window.userModel = userData;
            } catch(e) {
                window.userModel = {};
            }

            if (window.userModel.avatar) {
                synchronizationService.synchronizeUserFavorites(applicationSettings.userToken);
                mainViewModel.notVisibleSignin = true;
                userAvatarCanvas.loadImage(window.userModel.avatar);
            }
        }

        onUserSignouted: {
            applicationSettings.userToken = "";
            window.userModel = {};
            mainViewModel.notVisibleSignin = false;

            notificationViewModel.sendInfoNotification(`Вы успешно вышли из аккаунта. Чтобы войти обратно перейдите на страницу Войти.`)
        }

        onTorrentDownloaded: {
            const userSettings = JSON.parse(localStorage.getUserSettings());
            if (userSettings.torrentDownloadMode === 0) {
                if (Qt.platform.os === "linux" || Qt.platform.os === "unix") Qt.openUrlExternally("file://" + torrentPath);
                if (Qt.platform.os === "osx") Qt.openUrlExternally("file://" + torrentPath);
                if (Qt.platform.os === "windows") Qt.openUrlExternally("file:///" + torrentPath);
            }

            if (userSettings.torrentDownloadMode === 1) {
                window.tempTorrentPath = torrentPath;
                saveTorrentFileDialog.open();
            }
        }
    }

    Drawer {
        id: drawer
        width: 300
        dragMargin: 0
        height: window.height
        CustomStyle.drawerDialogColor: applicationThemeViewModel.currentItems.colorDrawerDivider
        CustomStyle.drawerDimColor: applicationThemeViewModel.currentItems.colorDrawerDim
        CustomStyle.drawerDividerColor: applicationThemeViewModel.currentItems.colorDrawerDialog
        background: Canvas {
            height: drawer.height
            width: drawer.width
            onPaint: {
                const ctx = getContext("2d");

                const gradient = ctx.createLinearGradient(0,0,drawer.width,drawer.height);
                gradient.addColorStop(0.0, applicationThemeViewModel.drawerGradiendStep0);
                gradient.addColorStop(0.3, applicationThemeViewModel.drawerGradiendStep1);
                gradient.addColorStop(0.5, applicationThemeViewModel.drawerGradiendStep2);
                gradient.addColorStop(0.7, applicationThemeViewModel.drawerGradiendStep3);
                gradient.addColorStop(1.0, applicationThemeViewModel.drawerGradiendStep4);
                ctx.fillStyle = gradient;
                ctx.fillRect(0,0,drawer.width,drawer.height);
            }
        }

        Item {
            id: authorizationPanel
            visible: mainViewModel.notVisibleSignin
            anchors.top: parent.top
            width: drawer.width
            height: mainViewModel.notVisibleSignin ? 64 : 0

            Canvas {
                id: userAvatarCanvas
                anchors.leftMargin: 6
                anchors.topMargin: 2
                anchors.left: parent.left
                anchors.verticalCenter: parent.verticalCenter
                width: 60
                height: 60
                onPaint: {
                    const ctx = getContext("2d");

                    ctx.save();
                    ctx.beginPath();
                    ctx.arc(30, 30, 30, 0, Math.PI * 2, true);
                    ctx.closePath();
                    ctx.clip();

                    ctx.drawImage(window.userModel.avatar, 0, 0, 60, 60);

                    ctx.beginPath();
                    ctx.arc(0, 0, 30, 0, Math.PI * 2, true);
                    ctx.clip();
                    ctx.closePath();
                    ctx.restore();
                }
                onImageLoaded: {
                    requestPaint();
                }
            }

            Text {
                anchors.left: userAvatarCanvas.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: "Вы авторизованы как:\n" + (userModel.login ? userModel.login : "")
                color: applicationThemeViewModel.currentItems.colorDrawerItemText
                elide: Text.ElideRight
                antialiasing: true
                wrapMode: Text.WordWrap
                width: drawer.width - userAvatarCanvas.width - logoutButton.width - 40
                maximumLineCount: 2
            }

            IconButton {
                id: logoutButton
                anchors.right: parent.right
                anchors.rightMargin: 1
                anchors.verticalCenter: parent.verticalCenter
                height: 40
                width: 40
                hoverColor: Qt.rgba(0, 0, 0, .1)
                iconPath: applicationThemeViewModel.currentItems.iconLogout
                iconWidth: 28
                iconHeight: 28
                tooltipMessage: "Выйти из аккаунта"
                onButtonPressed: {
                    synchronizationService.signout(applicationSettings.userToken);
                    drawer.close();
                }
            }
        }

        ListView {
            id: mainMenuItems
            anchors.top: authorizationPanel.bottom
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 60
            width: drawer.width
            clip: true
            model: mainViewModel.mainMenuListModel
            delegate: Control {
                id: mainMenuControl

                property bool isHovered

                width: drawer.width
                height: 50
                Rectangle {
                    id: mainMenuDelegate
                    color: mainMenuControl.isHovered ? applicationThemeViewModel.currentItems.colorDrawerItemHovered : "transparent"
                    anchors.fill: parent
                    MouseArea {
                        hoverEnabled: true
                        anchors.fill: parent
                        onClicked: {
                            mainViewModel.mainMenuListModel.selectItem(pageIndex);
                            drawer.close();
                        }
                        onEntered: {
                            mainMenuControl.isHovered = true;
                        }
                        onExited: {
                            mainMenuControl.isHovered = false;
                        }
                    }
                    Row {
                        anchors.leftMargin: 16
                        anchors.fill: parent
                        spacing: 10
                        Image {
                            anchors.verticalCenter: parent.verticalCenter
                            source: applicationThemeViewModel.currentItems[icon] ? applicationThemeViewModel.currentItems[icon] : '../Assets/Icons/donate.jpg'
                            sourceSize.width: 30
                            sourceSize.height: 30
                        }
                        Label {
                            anchors.verticalCenter: parent.verticalCenter
                            text: title
                            font.pointSize: 10
                            CustomStyle.labelColor: applicationThemeViewModel.currentItems.colorDrawerItemText
                        }
                    }
                }
            }
        }

        Row {
            id: aboutPanel
            anchors.top: mainMenuItems.bottom
            anchors.bottom: parent.bottom
            height: 60

            Item {
                width: 60
                height: 60
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        mainViewModel.selectPage("about");
                        drawer.close();
                    }
                }
                Image {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    source: "Assets/Icons/anilibrialogodefault.svg"
                    mipmap: true
                    width: 50
                    height: 50
                }
            }
            Item {
                width: drawer.width - 60
                height: 60
                anchors.leftMargin: 10
                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        mainViewModel.selectPage("about");
                        drawer.close();
                    }
                }
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    Text {
                        color: applicationThemeViewModel.currentItems.colorDrawerItemText
                        font.pointSize: 11
                        text: qsTr("AniLibria.Qt")
                    }
                    Text {
                        color: applicationThemeViewModel.currentItems.colorDrawerItemText
                        font.pointSize: 11
                        text: qsTr("версия " + ApplicationVersion)
                    }
                }                
            }

        }
    }

    OnlinePlayerViewModel {
        id: onlinePlayerViewModel
        releasesViewModel: releasesViewModel
        proxyPort: userConfigurationViewModel.playerBuffer
        needProxified: userConfigurationViewModel.usingVideoProxy && onlinePlayerWindowViewModel.isSelectedQtAv && torrentNotifierViewModel.activated
        onIsFullScreenChanged: {
            if (isFullScreen) {
                window.showFullScreen();

            } else {
                let currentScreen = getCurrentScreen();
                if (!currentScreen) return;

                window.showNormal();
            }
        }
        onNeedProxifiedChanged: {
            let oldSource = onlinePlayerViewModel.videoSource;
            if (oldSource.indexOf('/proxyvideolist?path=')) {
                oldSource = oldSource.substring(oldSource.indexOf("https://"));
                console.log('replaced source', oldSource);
            }
            onlinePlayerViewModel.videoSource = "";
            onlinePlayerViewModel.videoSource = oldSource;
        }
        onNeedScrollSeriaPosition: {
            videoplayer.setSerieScrollPosition();
        }
        onSaveToWatchHistory: {
            releasesViewModel.setToReleaseHistory(releaseId, 1);
        }
        onPlayInPlayer: {
            videoplayer.playInPlayer();
        }
        onStopInPlayer: {
            videoplayer.stopInPlayer();
        }
        onWatchedMinuteInPlayer: {
            userActivityViewModel.addWatchDurationMinute();
        }
        onPlayerPlaybackStateChanged: {
            //I'm using hardcode constant because multimedia module loading dynamically
            const playingState = 1;
            if (playerPlaybackState === playingState) {
                osExtras.startPreventSleepMode();
            } else {
                osExtras.stopPreventSleepMode();
            }
        }
        onRutubeVideoIdChanged: {
            if (rutubeVideoId) Qt.openUrlExternally("https://rutube.ru/play/embed/" + rutubeVideoId);
        }
    }

    ReleasesViewModel {
        id: releasesViewModel
        synchronizationService: synchronizationService
        applicationSettings: applicationSettings
        localStorage: localStorage
        notCloseReleaseCardAfterWatch: userConfigurationViewModel.notCloseReleaseCardAfterWatch
        imageBackgroundViewModel.containerWidth: releases.backgroundImageWidth
        imageBackgroundViewModel.containerHeight: releases.backgroundImageHeight
        items.releaseLinkedSeries: releaseLinkedSeries
        userActivity: userActivityViewModel
        onReleaseCardOpened: {
            mainViewModel.selectPage("release:" + releasesViewModel.openedReleaseId);

            analyticsService.sendView("releasecard", "open", "%2Frelease");
            userActivityViewModel.addOpenedCardToCounter();
        }
        onAfterSynchronizedReleases: {
            notificationViewModel.sendInfoNotification("Синхронизация релизов успешно завершена в " + new Date().toLocaleTimeString());

            if (releasesViewModel.newEntities) notificationViewModel.sendInfoNotification(releasesViewModel.newEntities);

            releaseLinkedSeries.refreshSeries();
        }
        onErrorWhileReleaseSynchronization: {
            notificationViewModel.sendErrorNotification(`Не удалось синхронизовать релизы. Попробуйте повторить синхронизацию через некоторое время.`);
        }
    }

    Rectangle {
        color: "transparent"
        anchors.fill: parent

        OnlinePlayer {
            id: videoplayer
            visible: mainViewModel.isOnlinePlayerPageVisible
            onReturnToReleasesPage: {
                mainViewModel.selectPage("release");
            }
            onPlayerCreated: {
                onlinePlayerWindow.loadPlayer();
            }
        }

        Releases {
            id: releases
            visible: mainViewModel.isReleasesPageVisible
            focus: true
            onWatchSingleRelease: {
                if (!onlinePlayerViewModel.releaseHasVideos(releaseId)) return;

                mainViewModel.selectPage("videoplayer");
                onlinePlayerViewModel.quickSetupForSingleRelease(releaseId, startSeria);
            }
            onWatchCinemahall: {
                mainViewModel.selectPage("videoplayer");
                onlinePlayerViewModel.setupForCinemahall();
            }
            onWatchMultipleReleases: {
                mainViewModel.selectPage("videoplayer");

                onlinePlayerViewModel.setupForMultipleRelease();

                releasesViewModel.clearSelectedReleases();
            }
        }

        Authorization {
            id: authorization
            visible: mainViewModel.isAuthorizationPageVisible
        }

        Youtube {
            id: youtube
            visible: mainViewModel.isYoutubePageVisible
        }

        About {
            id: about
            visible: mainViewModel.isAboutPageVisible
        }

        Downloads {
            id: download
            visible: mainViewModel.isDownloadPageVisible
        }

        Cinemahall {
            id: cinemahall
            visible: mainViewModel.isCinemahallPageVisible
            onWatchCinemahall: {
                mainViewModel.selectPage("videoplayer");
                onlinePlayerViewModel.setupForCinemahall();
            }
        }

        ReleaseSeries {
            id: releaseseries
            visible: mainViewModel.isReleasesSeriesPageVisible
        }

        Maintenance {
            id: maintenance
            visible: mainViewModel.isMaintenancePageVisible
        }

        MyAnilibria {
            id: myAnilibria
            visible: mainViewModel.isMyAnilibriaPageVisible
        }

        ThemeManager {
            id: themeManager
            visible: mainViewModel.isThemeManagerVisible
        }

        TorrentStream {
            id: torrentStream
            visible: mainViewModel.isTorrentStreamPageVisible
        }
    }

    ListView {
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 30
        anchors.bottomMargin: 10
        anchors.top: parent.top
        width: 250
        rotation: 180
        visible: notificationViewModel.popupNotifications.showNotifications
        enabled: notificationViewModel.popupNotifications.showNotifications
        layoutDirection: Qt.RightToLeft
        remove: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; to: 0; duration: 1000 }
                NumberAnimation { properties: "x,y"; to: 100; duration: 1000 }
            }
        }
        removeDisplaced: Transition {
            ParallelAnimation {
                NumberAnimation { property: "opacity"; to: 0; duration: 1000 }
                NumberAnimation { properties: "x,y"; to: 100; duration: 1000 }
            }
        }
        model: notificationViewModel.popupNotifications
        delegate: Rectangle {
            rotation: 180
            width: 250
            height: 80
            color: "transparent"

            Rectangle {
                anchors.fill: parent
                anchors.topMargin: 4
                anchors.bottomMargin: 4
                border.color: applicationThemeViewModel.selectedItem
                border.width: 2
                radius: 8
                color: applicationThemeViewModel.notificationCenterItemBackground

                LinkedText {
                    padding: 8
                    maximumLineCount: 3
                    fontPointSize: 8
                    width: parent.width
                    wrapMode: Text.WordWrap
                    elide: Text.ElideRight
                    text: message
                    anchors.verticalCenter: parent.verticalCenter
                }
            }

        }
    }

    Rectangle {
        id: notificationOverlay
        width: 240
        height: window.height
        anchors.right: parent.right
        anchors.topMargin: -2
        anchors.rightMargin: -1
        color: applicationThemeViewModel.notificationCenterPanelBackground
        border.color: applicationThemeViewModel.selectedItem
        border.width: 1
        visible: false

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.LeftButton | Qt.RightButton
            onPressed: {
            }
        }

        ColumnLayout {
            anchors.fill: parent

            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 20
                color: "transparent"

                IconButton {
                    height: 16
                    width: 16
                    anchors.right: parent.right
                    anchors.top: parent.top
                    anchors.rightMargin: 4
                    anchors.topMargin: 4
                    hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                    iconPath: applicationThemeViewModel.currentItems.iconCloseWindow
                    iconWidth: 14
                    iconHeight: 14
                    onButtonPressed: {
                        notificationOverlay.visible = false;
                    }
                }
            }
            Rectangle {
                Layout.fillWidth: true
                Layout.preferredHeight: 40
                color: "transparent"

                RoundedActionButton {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.rightMargin: 10
                    text: "Очистить все"
                    onClicked: {
                        notificationViewModel.clearAllNotifications();
                    }
                }
            }
            Flickable {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.leftMargin: 2
                Layout.rightMargin: 2
                contentWidth: parent.width
                contentHeight: notificationRepeater.height
                clip: true
                maximumFlickVelocity: 10

                Column {
                    id: notificationRepeater
                    spacing: 3
                    Repeater {
                        model: notificationViewModel
                        Rectangle {
                            color: "transparent"
                            width: 239
                            height: 70

                            Rectangle {
                                width: 230
                                anchors.horizontalCenter: parent.horizontalCenter
                                height: 70
                                radius: 8
                                border.color: applicationThemeViewModel.selectedItem
                                border.width: 2
                                color: applicationThemeViewModel.notificationCenterItemBackground

                                LinkedText {
                                    padding: 10
                                    maximumLineCount: 3
                                    fontPointSize: 8
                                    width: parent.width
                                    wrapMode: Text.WordWrap
                                    elide: Text.ElideRight
                                    text: message
                                    anchors.verticalCenter: parent.verticalCenter
                                }
                                IconButton {
                                    height: 16
                                    width: 16
                                    anchors.right: parent.right
                                    anchors.top: parent.top
                                    anchors.rightMargin: 4
                                    anchors.topMargin: 4
                                    hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                                    iconPath: applicationThemeViewModel.currentItems.iconCloseWindow
                                    iconWidth: 14
                                    iconHeight: 14
                                    onButtonPressed: {
                                        notificationViewModel.clearNotification(id);
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    YoutubeViewModel {
        id: youtubeViewModel
        Component.onCompleted: {
            youtubeViewModel.synchronize();
        }
    }

    AuthorizationViewModel {
        id: authorizationViewModel
        onSuccessAuthentificated: {
            applicationSettings.userToken = token;

            if (mainViewModel.currentPageId === "authorization") mainViewModel.selectPage("release");

            synchronizationService.getUserData(applicationSettings.userToken);
            notificationViewModel.sendInfoNotification(`Вы успешно вошли в аккаунт. Ваше избранное будет синхронизовано автоматически.`);
        }
    }

    MainViewModel {
        id: mainViewModel
        analyticsService: analyticsService
        currentPageId: userConfigurationViewModel.startPage
        startPage: userConfigurationViewModel.startPage
        onOnlinePlayerPageFromNavigated: {
            videoplayer.navigateFrom();
            windowFooter.visible = true;
        }
        onOnlinePlayerPageNavigated: {
            videoplayer.navigateTo();
            windowFooter.visible = false;
        }
        onReleasesPageToNavigated: {
            releases.navigateTo();
        }
        onChangeReleasesParameters: {
            if (!parameters.length) {
                releasesViewModel.closeReleaseCard();
                return;
            }

            releasesViewModel.showReleaseCard(parseInt(parameters), false);
        }
    }

    OsExtras {
        id: osExtras
    }

    UserActivityViewModel {
        id : userActivityViewModel
        Component.onDestruction: {
            userActivityViewModel.saveUsingApplication();
        }
    }

    UserConfigurationViewModel {
        id: userConfigurationViewModel
        Component.onDestruction: {
            if (!userConfigurationViewModel.showedVideoForNewcomers) userConfigurationViewModel.showedVideoForNewcomers = true;
            userConfigurationViewModel.saveSettingsToFile();
        }
    }

    OnlinePlayerWindowViewModel {
        id: onlinePlayerWindowViewModel
        startupPlayer: userConfigurationViewModel.needSavePlayer ? userConfigurationViewModel.lastSelectedPlayer : ""
    }

    OnlinePlayerWindow {
        id: onlinePlayerWindow
        videoSource: videoplayer.videoPlayerSource
        videoOutput: videoplayer.videoOutputSource
    }

    MyAnilibriaViewModel {
        id: myAnilibriaViewModel
        releasesViewModel: releasesViewModel
        myList.userConfiguration: userConfigurationViewModel
        Component.onDestruction: {
            myAnilibriaViewModel.saveSectionsToFile();
        }
    }

    MyAnilibriaSearchListModel {
        id: myAnilibriaSearchListModel
        releases: releasesViewModel
    }


    ApplicationThemeViewModel {
        id: applicationThemeViewModel
        notAddCopyToName: userConfigurationViewModel.removeCopyFromName
        Component.onDestruction: {
            applicationThemeViewModel.saveCurrentState();
        }
        fieldList.onErrorMessage: {
            notificationViewModel.sendInfoNotification(message);
        }
    }

    TorrentNotifierViewModel {
        id: torrentNotifierViewModel
        torrentStreamPath: userConfigurationViewModel.torrentStreamPath
        removeAllData: userConfigurationViewModel.removeAllDownloadedTorrent
        port: userConfigurationViewModel.playerBuffer
        releasesViewModel: releasesViewModel
        onTorrentFullyDownloaded: {
            notificationViewModel.sendInfoNotification("Торрент скачан " + releaseName);
            torrentNotifierViewModel.startGetTorrentData();
        }
        onTorrentStreamNotConfigured: {
            if (!userConfigurationViewModel.playerBuffer) return

            torrentNotifierViewModel.startGetNotifiers();
        }
        onTorrentStreamStarted: {
            torrentNotifierViewModel.startGetNotifiers();
        }
        onActivatedChanged: {
            if (activated) torrentNotifierViewModel.startGetTorrentData();
        }
        onPrepareWatchTorrentFiles: {
            onlinePlayerViewModel.quickSetupForSingleDownloadedTorrent(files, releaseId);
            mainViewModel.selectPage("videoplayer");
        }
        Component.onCompleted: {
            if (userConfigurationViewModel.torrentStreamPath) {
                torrentNotifierViewModel.tryStartTorrentStreamApplication();
            }
        }
        Component.onDestruction: {
            torrentNotifierViewModel.closeConnectionsAndApplication();
        }
    }

    GlobalEventTrackerViewModel {
        onBackButtonPressed: {
            mainViewModel.backToPage();
        }
        onForwardButtonPressed: {
            mainViewModel.forwardToPage();
        }
    }

    Item {
        id: assetsLocation
        property string path: Qt.resolvedUrl("../Assets/")
        property string backgroundsPath: Qt.resolvedUrl("../Assets/Backgrounds/")
        property string iconsPath: Qt.resolvedUrl("../Assets/Icons/")
    }
}
