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
    title: qsTr("AniLibria")
    font.capitalization: Font.MixedCase
    property string currentPageId: "release"
    property bool synchronizationEnabled: false
    property bool notVisibleSignin: false
    property var userModel: ({})
    property string tempTorrentPath: ""

    Material.accent: Material.Red
    Material.theme: ApplicationTheme.isDarkTheme ? Material.Dark : Material.Light

    onClosing: {
        analyticsService.sendEvent("Session", "End");
    }

    onActiveChanged: {
        if (!active) videoplayer.windowNotActived();
    }

    Component.onCompleted: {
        const savedWidth = applicationSettings.windowWidth;
        const savedHeight = applicationSettings.windowHeight;
        const savedX = applicationSettings.windowX;
        const savedY = applicationSettings.windowY;
        if (savedWidth > 0 && savedHeight > 0 && savedX !== 0 && savedY !== 0) {
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

    footer: Rectangle {
        id: windowFooter
        visible: true
        width: window.width
        height: 16
        color: ApplicationTheme.notificationCenterBackground

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
            text: "Релизов " + localStorage.countReleases + " В избранном " + releases.favoriteReleases.length + " Просмотрено " + localStorage.countSeens + " В кинозале " + localStorage.countCinemahall + " "
        }

        Row {
            id: notificationCounter
            anchors.right: parent.right

            IconButton {
                id: notificationPopupButton
                height: 16
                width: 16
                iconColor: applicationNotificationModel.count > 0 ? "#9e2323" : ApplicationTheme.filterIconButtonColor
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
                text: applicationNotificationModel.count
            }
            Rectangle {
                width: 6
                height: 16
                color: "transparent"
            }
        }

    }

    function showPage(pageId) {
        if (currentPageId === pageId){
            drawer.close();
            return;
        }

        const pages = {
            "videoplayer": videoplayer,
            "authorization": authorization,
            "release": releases,
            "youtube": youtube,
            "about": about,
            "cinemahall": cinemahall,            
            "download": download,
            "maintenance": maintenance,
            "releaseseries": releaseseries
        };

        const currentPage = pages[currentPageId];
        currentPage.navigateFrom();

        currentPage.visible = false;
        currentPage.focus = false;

        const newPage = pages[pageId];
        newPage.visible = true;
        newPage.focus = true;
        newPage.navigateTo();
        currentPageId = pageId;

        windowFooter.visible = pageId !== "videoplayer";

        analyticsService.sendView("Pages", "ChangePage", "%2F" + pageId);

        drawer.close();
    }

    ListModel {
        id: applicationNotificationModel
    }

    Item {
        id: applicationNotification

        signal sendNotification(var settings);

        onSendNotification: {
            settings.timestamp = new Date().getTime();
            applicationNotificationModel.append({model: settings});
        }
    }

    Item {
        id: windowSettings

        property real dpiSeparation: 0
        property bool isTopMost: false

        signal setStayOnTop();
        signal unsetStayOnTop();
        signal toggleStayOnTopMode();

        onSetStayOnTop: {
            window.flags = Qt.WindowStaysOnTopHint;
            windowSettings.isTopMost = true;
        }

        onUnsetStayOnTop: {
            if (window.flags === 1) return;

            window.flags = 1;
            windowSettings.isTopMost = false;
        }

        onToggleStayOnTopMode: {
            if (window.flags === 1) {
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
            applicationNotification.sendNotification(
                {
                    type: "info",
                    message: `Доступна новая версия ${version}. Перейдите для скачивания по <a href='${url}'>этой ссылке</a>`
                }
            );
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

        onAllReleasesFinished: {
            releases.refreshAllReleases();

            synchronizationService.synchronizeSchedule();            
            if (applicationSettings.userToken) synchronizationService.synchronizeUserFavorites(applicationSettings.userToken);

            window.synchronizationEnabled = false;

            applicationNotification.sendNotification(
                {
                    type: "info",
                    message: "Синхронизация релизов успешно завершена в " + new Date().toLocaleTimeString()
                }
            );

            releaseLinkedSeries.refreshSeries();
        }

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

    WorkerScript {
       id: parseReleasesWorker
       source: "parseReleases.js"
       onMessage: {
           localStorage.updateAllReleases(messageObject.releases);
       }
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
            localStorage.importReleasesFromExternalFile(importReleasesFileDialog.fileUrl);
        }
    }

    SynchronizationService {
        id: synchronizationService
        Component.onCompleted: {
            window.synchronizationEnabled = true;
            synchronizationService.synchronizeReleases();
        }

        onSynchronizedReleases: {
            if (!data || !data.length) {
                window.synchronizationEnabled = false;
                applicationNotification.sendNotification(
                    {
                        type: "info",
                        message: "Не удалось синхронизовать релизы. Попробуйте повторить синхронизацию через некоторое время."
                    }
                );
            }

            parseReleasesWorker.sendMessage({ releasesJson: data });
        }

        onSynchronizedSchedule: {
            const jsonData = JSON.parse(data);

            if (!jsonData.status) {
                //TODO: handle error situation
            }

            const scheduleItems = jsonData.data;
            const scheduleResult = {};
            for (const scheduleItem of scheduleItems) {
                for (const dayitem of scheduleItem.items) {
                    scheduleResult[dayitem.id] = scheduleItem.day;
                }
            }

            localStorage.setSchedule(JSON.stringify(scheduleResult));

            releases.refreshReleaseSchedules();

            synchronizationService.synchronizeYoutube();
        }

        onUserCompleteAuthentificated: {
            applicationSettings.userToken = token;
            if (window.currentPageId === "authorization") {
                showPage("release");
                synchronizationService.getUserData(applicationSettings.userToken);

                applicationNotification.sendNotification(
                    {
                        type: "info",
                        message: "Вы успешно вошли в аккаунт. Ваше избранное будет синхронизовано автоматически."
                    }
                );
            }
        }

        onUserFailedAuthentificated: {
            applicationSettings.userToken = "";
            if (!(window.currentPageId === "authorization")) return;

            authorization.authentificateFailed(errorMessage);
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
                window.notVisibleSignin = true;
            }
        }

        onUserSignouted: {
            applicationSettings.userToken = "";
            window.userModel = {};
            window.notVisibleSignin = false;

            localStorage.clearFavorites();
            releases.refreshFavorites();

            applicationNotification.sendNotification(
                {
                    type: "info",
                    message: "Вы успешно вышли из аккаунта. Чтобы войти обратно перейдите на страницу Войти."
                }
            );
        }

        onUserFavoritesReceived:  {
            const favoritesObject = JSON.parse(data);
            const favorites = [];
            for (const favorite of favoritesObject.data.items) {
                favorites.push(favorite.id);
            }

            localStorage.updateFavorites(JSON.stringify(favorites));
            releases.refreshFavorites();
        }

        onUserFavoritesEdited: {
            if (applicationSettings.userToken) synchronizationService.synchronizeUserFavorites(applicationSettings.userToken);
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

        onSynchronizedYoutube: {
            const json = JSON.parse(data);

            localStorage.updateYoutubeItems(JSON.stringify(json.data.items));
        }
    }

    Drawer {
        id: drawer
        width: 300
        height: window.height
        background:  LinearGradient {
            anchors.fill: parent
            start: Qt.point(0, 0)
            end: Qt.point(0, 300)
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
                visible: notVisibleSignin
                width: drawer.width
                height: 64

                Image {
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
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.verticalCenter: parent.verticalCenter
                    text: userModel.login ? userModel.login : ""
                    color: "white"
                    wrapMode: Text.WordWrap
                }

                Image {
                    anchors.rightMargin: 8
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.right: parent.right
                    source: "Assets/Icons/logout.svg"
                    width: 30
                    height: 30
                    mipmap: true
                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            synchronizationService.signout(applicationSettings.userToken);
                        }
                    }
                }
            }

            ItemDelegate {
                contentItem: Item {
                    Row {
                        spacing: 10
                        Image {
                            source: "Assets/Icons/menu.svg"
                            sourceSize.width: 30
                            sourceSize.height: 30
                        }
                        Label {
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            color: "white"
                            text: qsTr("Каталог релизов")
                        }
                    }
                }
                width: parent.width
                onClicked: {
                    showPage("release");
                }
            }
            ItemDelegate {
                contentItem: Item {
                    Row {
                        spacing: 10
                        Image {
                            source: "Assets/Icons/videoplayer.svg"
                            sourceSize.width: 30
                            sourceSize.height: 30
                            mipmap: true
                        }
                        Label {
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            color: "white"
                            text: qsTr("Видеоплеер")
                        }
                    }
                }
                width: parent.width
                onClicked: {
                    showPage("videoplayer");
                }
            }
            ItemDelegate {
                contentItem: Item {
                    Row {
                        spacing: 10
                        Image {
                            source: "Assets/Icons/youtube.svg"
                            sourceSize.width: 30
                            sourceSize.height: 30
                            mipmap: true
                        }
                        Label {
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            color: "white"
                            text: qsTr("Youtube")
                        }
                    }
                }
                width: parent.width
                onClicked: {
                    showPage("youtube");
                }
            }
            ItemDelegate {
                contentItem: Item {
                    Row {
                        spacing: 10
                        Image {
                            source: "../Assets/Icons/popcorn.svg"
                            sourceSize.width: 30
                            sourceSize.height: 30
                            mipmap: true
                        }
                        Label {
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            color: "white"
                            text: qsTr("Кинозал")
                        }
                    }
                }
                width: parent.width
                onClicked: {
                    showPage("cinemahall");
                }
            }
            ItemDelegate {
                contentItem: Item {
                    Row {
                        spacing: 10
                        Image {
                            source: "../Assets/Icons/series.svg"
                            sourceSize.width: 30
                            sourceSize.height: 30
                            mipmap: true
                        }
                        Label {
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            color: "white"
                            text: qsTr("Связанные релизы")
                        }
                    }
                }
                width: parent.width
                onClicked: {
                    showPage("releaseseries");
                }
            }
            ItemDelegate {
                contentItem: Item {
                    Row {
                        spacing: 10
                        Image {
                            source: "Assets/Icons/downloadcircle.svg"
                            sourceSize.width: 30
                            sourceSize.height: 30
                            mipmap: true
                        }
                        Label {
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            color: "white"
                            text: qsTr("Менеджер загрузок")
                        }
                    }
                }
                width: parent.width
                onClicked: {
                    showPage("download");
                    drawer.close();
                }
            }
            ItemDelegate {
                contentItem: Item {
                    Row {
                        spacing: 10
                        Image {
                            source: "Assets/Icons/maintenance.svg"
                            sourceSize.width: 30
                            sourceSize.height: 30
                            mipmap: true
                        }
                        Label {
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            color: "white"
                            text: qsTr("Обслуживание")
                        }
                    }
                }
                width: parent.width
                onClicked: {
                    showPage("maintenance");
                    drawer.close();
                }
            }
            ItemDelegate {
                contentItem: Item {
                    Row {
                        spacing: 10
                        Image {
                            source: "Assets/Icons/donate.svg"
                            sourceSize.width: 30
                            sourceSize.height: 30
                            mipmap: true
                        }
                        Label {
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            color: "white"
                            text: qsTr("Поддержать проект")
                        }
                    }
                }
                width: parent.width
                onClicked: {
                    Qt.openUrlExternally("https://www.anilibria.tv/pages/donate.php");
                    drawer.close();
                }
            }
            ItemDelegate {
                visible: !notVisibleSignin
                contentItem: Item {
                    Row {
                        spacing: 10
                        Image {
                            source: "Assets/Icons/user.svg"
                            sourceSize.width: 30
                            sourceSize.height: 30
                            mipmap: true
                        }
                        Label {
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            color: "white"
                            text: qsTr("Войти")
                        }
                    }
                }
                width: parent.width
                onClicked: {
                    showPage("authorization");
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
                        showPage("about");
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
                        showPage("about");
                    }
                }
                Column {
                    anchors.verticalCenter: parent.verticalCenter
                    Text {
                        color: "white"
                        font.pointSize: 11
                        text: qsTr("Клиент для сайта AniLibria")
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

        onIsFullScreenChanged: {
            if (isFullScreen) {
                window.showFullScreen();
            } else {
                window.showNormal();
            }
        }
        onNeedScrollSeriaPosition: {
            videoplayer.setSerieScrollPosition();
        }
        onSaveToWatchHistory: {
            localStorage.setToReleaseHistory(releaseId, 1);
        }
        onPlayInPlayer: {
            videoplayer.playInPlayer();
        }
        onRecalculateSeenCounts: {
            localStorage.recalculateSeenCountsFromFile();
        }
    }

    OnlinePlayer {
        id: videoplayer
        visible: false
        onReturnToReleasesPage: {
            window.showPage("release");
        }
    }

    Releases {
        id: releases
        visible: true
        focus: true
        synchronizeEnabled: window.synchronizationEnabled
        onWatchRelease: {
            videoplayer.setReleaseParameters = {
                releaseId,
                customPlaylistPosition: startSeria,
                videos
            };
            window.showPage("videoplayer");
            videoplayer.setReleaseVideo();
        }
        onWatchCinemahall: {
            window.showPage("videoplayer");
            videoplayer.setCinemahallVideo();
        }
        onRequestSynchronizeReleases: {
            window.synchronizationEnabled = true;
            synchronizationService.synchronizeReleases();
        }
    }

    Authorization {
        id: authorization
        visible: false
    }

    Youtube {
        id: youtube
        visible: false
    }

    About {
        id: about
        visible: false
    }

    Downloads {
        id: download
        visible: false
    }

    Cinemahall {
        id: cinemahall
        visible: false
        onWatchCinemahall: {
            window.showPage("videoplayer");
            videoplayer.setCinemahallVideo();
        }
    }

    ReleaseSeries {
        id: releaseseries
        visible: false
    }

    Maintenance {
        id: maintenance
        visible: false
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
                        applicationNotificationModel.clear();
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
                        model: applicationNotificationModel
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
                                    text: modelData.message
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
                                        let iterator = -1;
                                        const count = applicationNotificationModel.count;
                                        for (var i = 0; i < count; i++) {
                                            let item = applicationNotificationModel.get(i);
                                            if (item.model.timestamp === modelData.timestamp) {
                                                applicationNotificationModel.remove(i);
                                                break;
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
