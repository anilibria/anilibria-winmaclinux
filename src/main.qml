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
import QtGraphicalEffects 1.12
import QtQuick.Dialogs 1.2
import Anilibria.Services 1.0
import Anilibria.ListModels 1.0
import Anilibria.ViewModels 1.0
import "Views"
import "Controls"
import "Theme"

ApplicationWindow {
    id: window
    visible: true
    width: 800
    minimumWidth: 800
    minimumHeight: 600
    height: 600
    title: qsTr("AniLibria.Qt")
    font.capitalization: Font.MixedCase
    flags: Qt.FramelessWindowHint | Qt.Window | Qt.WindowMinimizeButtonHint
    property var userModel: ({})
    property string tempTorrentPath: ""
    property bool isShowFullScreenSize: false
    property int previousX
    property int previousY

    Material.accent: Material.Red
    Material.theme: ApplicationTheme.isDarkTheme ? Material.Dark : Material.Light

    onClosing: {
        analyticsService.sendEvent("Session", "End");
        onlinePlayerWindow.closeWindow();
        onlinePlayerWindow.hide();
    }

    onActiveChanged: {
        if (!active) videoplayer.windowNotActived();
    }

    Component.onCompleted: {
        if (!applicationSettings.useCustomToolbar) window.flags = 1;

        const savedWidth = applicationSettings.windowWidth;
        const savedHeight = applicationSettings.windowHeight;
        const savedX = applicationSettings.windowX;
        const savedY = applicationSettings.windowY;
        if (savedWidth > 0 && savedHeight > 0) {
            window.x = savedX;
            window.y = savedY;
            window.width = savedWidth;
            window.height = savedHeight;
        }
    }

    Component.onDestruction: {
        applicationSettings.windowWidth = window.width;
        applicationSettings.windowHeight = window.height;
        applicationSettings.windowX = window.x;
        applicationSettings.windowY = window.y;
    }

    header: Rectangle {
        id: toolBar
        visible: applicationSettings.useCustomToolbar
        width: window.width
        height: 35
        color: ApplicationTheme.notificationCenterBackground

        Rectangle {
            color: "black"
            width: 1
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }

        Rectangle {
            color: "black"
            width: 1
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }

        Rectangle {
            color: "black"
            height: 1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: parent.top
        }

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
                fontPointSize: 12
                text: "AniLibria.Qt - "
            }
            AccentText {
                id: currentPageTitle
                anchors.left: taskbarTitle.right
                anchors.verticalCenter: parent.verticalCenter
                fontPointSize: 12
                text: mainViewModel.currentPageDisplayName
            }
        }
        IconButton {
            id: goToReleasePage
            overlayVisible: false
            anchors.left: parent.left
            anchors.leftMargin: 1
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            iconColor: ApplicationTheme.filterIconButtonColor
            hoverColor: ApplicationTheme.filterIconButtonHoverColor
            iconPath: "Assets/Icons/catalogmenu.svg"
            iconWidth: 20
            iconHeight: 20
            tooltipMessage: "Перейти на страницу Каталог Релизов"
            onButtonPressed: {
                mainViewModel.selectPage("release");
            }
        }
        IconButton {
            id: goToOnlineVideoPage
            overlayVisible: false
            anchors.left: goToReleasePage.right
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            iconColor: ApplicationTheme.filterIconButtonColor
            hoverColor: ApplicationTheme.filterIconButtonHoverColor
            iconPath: "Assets/Icons/videoplayermenu.svg"
            iconWidth: 20
            iconHeight: 20
            tooltipMessage: "Перейти на страницу Видеоплеер"
            onButtonPressed: {
                mainViewModel.selectPage("videoplayer");
            }
        }
        IconButton {
            id: goToCinemaHall
            overlayVisible: false
            anchors.left: goToOnlineVideoPage.right
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            iconColor: ApplicationTheme.filterIconButtonColor
            hoverColor: ApplicationTheme.filterIconButtonHoverColor
            iconPath: "Assets/Icons/cinemahallmenu.svg"
            iconWidth: 20
            iconHeight: 20
            tooltipMessage: "Перейти на страницу Кинозал"
            onButtonPressed: {
                mainViewModel.selectPage("cinemahall");
            }
        }
        IconButton {
            id: goToReleaseSeries
            overlayVisible: false
            anchors.left: goToCinemaHall.right
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            iconColor: ApplicationTheme.filterIconButtonColor
            hoverColor: ApplicationTheme.filterIconButtonHoverColor
            iconPath: "Assets/Icons/seriesmenu.svg"
            iconWidth: 20
            iconHeight: 20
            tooltipMessage: "Перейти на страницу Связанные релизы"
            onButtonPressed: {
                mainViewModel.selectPage("releaseseries");
            }
        }
        IconButton {
            id: leftHalfScreenWindow
            anchors.right: rightHalfScreenWindow.left
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            iconColor: ApplicationTheme.filterIconButtonColor
            hoverColor: ApplicationTheme.filterIconButtonHoverColor
            iconPath: "Assets/Icons/lefthalf.svg"
            iconWidth: 18
            iconHeight: 18
            tooltipMessage: "Выставить размер окна - левая половина экрана"
            onButtonPressed: {
                let currentScreen = getCurrentScreen();
                if (!currentScreen) return;

                window.x = currentScreen.virtualX;
                window.width = currentScreen.width / 2;
                window.y = 0;
                window.height = currentScreen.desktopAvailableHeight;
            }
        }
        IconButton {
            id: rightHalfScreenWindow
            anchors.right: minimizeWindow.left
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            iconColor: ApplicationTheme.filterIconButtonColor
            hoverColor: ApplicationTheme.filterIconButtonHoverColor
            iconPath: "Assets/Icons/righthalf.svg"
            iconWidth: 18
            iconHeight: 18
            tooltipMessage: "Выставить размер окна - правая половина экрана"
            onButtonPressed: {
                let currentScreen = getCurrentScreen();
                if (!currentScreen) return;

                window.x = currentScreen.virtualX + currentScreen.width / 2;
                window.width = currentScreen.width / 2;
                window.y = 0;
                window.height = currentScreen.desktopAvailableHeight;
            }
        }
        IconButton {
            id: minimizeWindow
            anchors.right: windowOrFullScreenSize.left
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            iconColor: ApplicationTheme.filterIconButtonColor
            hoverColor: ApplicationTheme.filterIconButtonHoverColor
            iconPath: "Assets/Icons/minimize.svg"
            iconWidth: 15
            iconHeight: 15
            tooltipMessage: "Минимизировать окно в панель задач"
            onButtonPressed: {
                window.showMinimized();
            }
        }
        IconButton {
            id: windowOrFullScreenSize
            anchors.right: closeWindow.left
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            iconColor: ApplicationTheme.filterIconButtonColor
            hoverColor: ApplicationTheme.filterIconButtonHoverColor
            iconPath: window.isShowFullScreenSize ? "Assets/Icons/windowsize.svg" : "Assets/Icons/fullscreensize.svg"
            iconWidth: 15
            iconHeight: 15
            tooltipMessage: window.isShowFullScreenSize ? "Вернуть окну нормальный размер" : "Открыть окно на полный экран"
            onButtonPressed: {
                if (window.isShowFullScreenSize) {
                    window.isShowFullScreenSize = false;
                    window.showNormal();
                } else {
                    window.isShowFullScreenSize = true;
                    window.showMaximized();
                }
            }
        }
        IconButton {
            id: closeWindow
            anchors.right: parent.right
            anchors.rightMargin: 1
            anchors.top: parent.top
            anchors.topMargin: 1
            height: 34
            width: 40
            iconColor: ApplicationTheme.filterIconButtonColor
            hoverColor: ApplicationTheme.filterIconButtonHoverColor
            iconPath: "Assets/Icons/closewindow.svg"
            iconWidth: 15
            iconHeight: 15
            tooltipMessage: "Выйти из приложения"
            onButtonPressed: {
                window.close();
            }
        }

        MouseArea {
            id: windowDraggingArea
            enabled: applicationSettings.useCustomToolbar
            anchors.left: goToReleaseSeries.right
            anchors.right: leftHalfScreenWindow.left
            height: parent.height
            property variant clickPosition: "1,1"
            onPressed: {
                windowDraggingArea.clickPosition = Qt.point(mouse.x, mouse.y);
            }
            onPositionChanged: {
                const delta = Qt.point(mouse.x - clickPosition.x, mouse.y - clickPosition.y);
                const deltaPosition = Qt.point(window.x + delta.x, window.y + delta.y);
                window.x = deltaPosition.x;
                window.y = deltaPosition.y;
            }
        }

        MouseArea {
            id: topWindowResize
            enabled: applicationSettings.useCustomToolbar
            height: 3
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.right: parent.right
            cursorShape: Qt.SizeVerCursor
            onPressed: {
                previousY = mouseY
            }
            onMouseYChanged: {
                const delta = mouseY - previousY;
                window.y += delta;
                window.height -= delta;
            }
        }
    }

    footer: Rectangle {
        id: windowFooter
        visible: true
        width: window.width
        height: 16
        color: ApplicationTheme.notificationCenterBackground

        Rectangle {
            visible: applicationSettings.useCustomToolbar
            color: "black"
            width: 1
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }

        Rectangle {
            visible: applicationSettings.useCustomToolbar
            color: "black"
            width: 1
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
        }

        Rectangle {
            visible: applicationSettings.useCustomToolbar
            color: "black"
            height: 1
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }

        Rectangle {
            color: "transparent"
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
            anchors.right: notificationCounter.left
            anchors.verticalCenter: parent.verticalCenter
            fontPointSize: 10
            text: "Релизов " + releasesViewModel.countReleases + " В избранном " + releasesViewModel.countFavorites + " Просмотрено " + releasesViewModel.countSeens + " В кинозале " + releasesViewModel.cinemahall.countCinemahall + " "
        }

        Row {
            id: notificationCounter
            anchors.right: parent.right

            IconButton {
                id: notificationPopupButton
                height: 16
                width: 16
                iconColor: notificationViewModel.countNotifications > 0 ? "#9e2323" : ApplicationTheme.filterIconButtonColor
                hoverColor: ApplicationTheme.filterIconButtonColor
                iconPath: "../Assets/Icons/notification.svg"
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

        MouseArea {
            id: rightbottomWindowResize
            enabled: applicationSettings.useCustomToolbar
            width: 3
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            cursorShape: Qt.SizeFDiagCursor
            onPressed: {
                previousX = mouseX
                previousY = mouseY
            }
            onMouseXChanged: {
                window.width += mouseX - previousX;
            }
            onMouseYChanged: {
                window.height += mouseY - previousY;
            }
        }

        MouseArea {
            id: leftbottomWindowResize
            enabled: applicationSettings.useCustomToolbar
            width: 3
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            cursorShape: Qt.SizeBDiagCursor
            onPressed: {
                previousX = mouseX
                previousY = mouseY
            }
            onMouseXChanged: {
                const delta = mouseX - previousX;
                window.x = window.x + delta;
                window.width = window.width - delta;
            }
            onMouseYChanged: {
                window.height += mouseY - previousY;
            }
        }

        MouseArea {
            id: bottomWindowResize
            enabled: applicationSettings.useCustomToolbar
            height: 3
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            cursorShape: Qt.SizeVerCursor
            onPressed: {
                previousY = mouseY
            }
            onMouseYChanged: {
                window.height += mouseY - previousY;
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
            if (applicationSettings.useCustomToolbar) {
                window.flags = Qt.FramelessWindowHint | Qt.Window | Qt.WindowMinimizeButtonHint | Qt.WindowStaysOnTopHint;
            } else {
                window.flags = Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint;
            }

            windowSettings.isTopMost = true;
        }

        onUnsetStayOnTop: {
            if (!windowSettings.isTopMost) return;

            if (applicationSettings.useCustomToolbar) {
                window.flags = Qt.FramelessWindowHint | Qt.Window | Qt.WindowMinimizeButtonHint;
            } else {
                window.flags = 1;
            }

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
            ApplicationTheme.isDarkTheme = applicationSettings.isDarkTheme;
            if (!applicationSettings.userToken) return;

            synchronizationService.getUserData(applicationSettings.userToken);

            analyticsService.sendEvent("Session", "Start");
        }
    }

    LocalStorage {
        id: localStorage
    }

    AnalyticsService {
        id: analyticsService
    }

    AlphabetListModel {
        id: alphabetListModel
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
            synchronizeReleases();
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
        background: LinearGradient {
            anchors.fill: parent
            start: Qt.point(0, 0)
            end: Qt.point(0, parent.height)
            gradient: Gradient {
                GradientStop { position: 0.0; color: ApplicationTheme.drawerGradiendStep0 }
                GradientStop { position: 0.3; color: ApplicationTheme.drawerGradiendStep1 }
                GradientStop { position: 0.5; color: ApplicationTheme.drawerGradiendStep2 }
                GradientStop { position: 0.7; color: ApplicationTheme.drawerGradiendStep3 }
                GradientStop { position: 1.0; color: ApplicationTheme.drawerGradiendStep4 }
            }
        }

        Column {            
            anchors.fill: parent

            Item {
                visible: mainViewModel.notVisibleSignin
                width: drawer.width
                height: 64

                Image {
                    id: userAvatarImage
                    anchors.leftMargin: 6
                    anchors.topMargin: 2
                    anchors.left: parent.left
                    anchors.verticalCenter: parent.verticalCenter
                    source: userModel.avatar ? userModel.avatar : '../Assets/Icons/donate.jpg'
                    fillMode: Image.PreserveAspectCrop
                    width: 60
                    height: 60
                    layer.enabled: true
                    layer.effect: OpacityMask {
                        maskSource: Rectangle {
                            width: 60
                            height: 60
                            radius: 30
                            visible: false
                        }
                    }
                }

                Text {
                    anchors.left: userAvatarImage.right
                    anchors.leftMargin: 10
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Вы авторизованы как:\n" + (userModel.login ? userModel.login : "")
                    color: "white"
                    elide: Text.ElideRight
                    antialiasing: true
                    wrapMode: Text.WordWrap
                    width: drawer.width - userAvatarImage.width - logoutButton.width - 40
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
                    overlayVisible: false
                    iconPath: "Assets/Icons/logout.svg"
                    iconWidth: 28
                    iconHeight: 28
                    tooltipMessage: "Выйти из аккаунта"
                    onButtonPressed: {
                        synchronizationService.signout(applicationSettings.userToken);
                        drawer.close();
                    }
                }
            }

            Column {
                Repeater {
                    model: mainViewModel.mainMenuListModel
                    delegate: Control {
                        id: mainMenuControl

                        property bool isHovered

                        width: drawer.width
                        height: 50
                        Rectangle {
                            id: mainMenuDelegate
                            color: mainMenuControl.isHovered ?  Qt.rgba(0, 0, 0, .1) : "transparent"
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
                                    source: assetsLocation.iconsPath + icon
                                    sourceSize.width: 30
                                    sourceSize.height: 30
                                }
                                Label {
                                    anchors.verticalCenter: parent.verticalCenter
                                    color: "white"
                                    text: title
                                }
                            }
                        }
                    }
                }
            }
        }

        Row {
            anchors.bottom: parent.bottom

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
                        color: "white"
                        font.pointSize: 11
                        text: qsTr("AniLibria.Qt")
                    }
                    Text {
                        color: "white"
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

        onIsFullScreenChanged: {
            if (isFullScreen) {
                window.showFullScreen();
                if (applicationSettings.useCustomToolbar) toolBar.visible = false;
            } else {
                if (window.isShowFullScreenSize) {
                    window.showMaximized();
                } else {
                    window.showNormal();
                }

                if (applicationSettings.useCustomToolbar) toolBar.visible = true;
            }
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
    }

    Rectangle {
        id: leftWindowResizeArea
        visible: applicationSettings.useCustomToolbar
        color: "black"
        width: 1
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
    }

    Rectangle {
        color: "black"
        visible: applicationSettings.useCustomToolbar
        width: 1
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
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
            analyticsService.sendView("releasecard", "show", "%2Freleases");
            releases.setWebViewUrl();
            vkCommentsWindow.refreshComments();
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
        anchors.leftMargin: applicationSettings.useCustomToolbar ? 1 : 0
        anchors.rightMargin: applicationSettings.useCustomToolbar ? 1 : 0

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
                onlinePlayerViewModel.customPlaylistPosition = startSeria;
                onlinePlayerViewModel.navigateReleaseId = releaseId;
                onlinePlayerViewModel.navigateVideos = videos;
                onlinePlayerViewModel.navigatePoster = poster;

                mainViewModel.selectPage("videoplayer");
                onlinePlayerViewModel.setupForSingleRelease();
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

        MouseArea {
            id: leftWindowResize
            enabled: applicationSettings.useCustomToolbar
            width: 1
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            cursorShape:  Qt.SizeHorCursor
            onPressed: {
                previousX = mouseX
            }

            onMouseXChanged: {
                const delta = mouseX - previousX;
                window.x = window.x + delta;
                window.width = window.width - delta;
            }
        }

        MouseArea {
            id: rightWindowResize
            enabled: applicationSettings.useCustomToolbar
            width: 1
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            cursorShape:  Qt.SizeHorCursor
            onPressed: {
                previousX = mouseX
            }
            onMouseXChanged: {
                window.width += mouseX - previousX;
            }
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
                border.color: ApplicationTheme.selectedItem
                border.width: 2
                radius: 8
                color: ApplicationTheme.notificationCenterItemBackground

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
        color: ApplicationTheme.notificationCenterPanelBackground
        border.color: ApplicationTheme.selectedItem
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
                    iconColor: ApplicationTheme.filterIconButtonColor
                    hoverColor: ApplicationTheme.filterIconButtonHoverColor
                    iconPath: "../Assets/Icons/close.svg"
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
                                border.color: ApplicationTheme.selectedItem
                                border.width: 2
                                color: ApplicationTheme.notificationCenterItemBackground

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
                                    iconColor: ApplicationTheme.selectedItem
                                    hoverColor: ApplicationTheme.filterIconButtonHoverColor
                                    iconPath: "../Assets/Icons/close.svg"
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
        onOnlinePlayerPageFromNavigated: {
            console.log(`fromnavigated`);
            videoplayer.navigateFrom();
            windowFooter.visible = true;
        }
        onOnlinePlayerPageNavigated: {
            console.log(`navigated`);
            videoplayer.navigateTo();
            windowFooter.visible = false;
            console.log(`windowFooter.visible`, windowFooter.visible);
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
            userConfigurationViewModel.saveSettingsToFile();
        }
    }

    OnlinePlayerWindowViewModel {
        id: onlinePlayerWindowViewModel
    }

    OnlinePlayerWindow {
        id: onlinePlayerWindow
        videoSource: videoplayer.videoPlayerSource
        videoOutput: videoplayer.videoOutputSource
    }

    VkCommentsWindow {
        id: vkCommentsWindow
    }

    Item {
        id: assetsLocation
        property string path: Qt.resolvedUrl("../Assets/")
        property string iconsPath: Qt.resolvedUrl("../Assets/Icons/")
    }
}
