import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import Anilibria.Services 1.0
import Anilibria.ListModels 1.0
import Anilibria.ViewModels 1.0
import CustomStyle 1.0
import QtQuick.Particles 2.15
import "Views"
import "Controls"
import "Views/Videoplayer"


ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600
    minimumWidth: 300
    font.family: userConfigurationViewModel.textFont
    minimumHeight: 300
    title: qsTr("AniLiberty.Qt")
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
    property color tooltipColor: applicationThemeViewModel.currentItems.colorTooltipBackground
    property color tooltipColorText: applicationThemeViewModel.currentItems.colorTooltipText
    property color colorMenuForeground: applicationThemeViewModel.currentItems.colorMenuForeground
    property color colorMenuItemHint: applicationThemeViewModel.currentItems.colorMenuItemHint
    property color colorMenuRipple: applicationThemeViewModel.currentItems.colorMenuRipple
    property color colorMenuHighlight: applicationThemeViewModel.currentItems.colorMenuHighlight
    property color colorPosterFilter: applicationThemeViewModel.currentItems.colorPosterFilter

    property color iconReleaseCatalogSearchFounded: applicationThemeViewModel.currentItems.iconReleaseCatalogSearchFounded
    property color iconCustomGroupFounded: applicationThemeViewModel.currentItems.iconCustomGroupFounded

    property real dp: Screen.pixelDensity * 25.4 / 160

    Material.accent: applicationThemeViewModel.materialAccent
    Material.theme: applicationThemeViewModel.basedOnDark ? Material.Dark : Material.Light
    Material.foreground: applicationThemeViewModel.colorMaterialText

    onClosing: {
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
        visible: window.visibility !== Window.FullScreen && !windowSettings.isTopMost
        width: window.width
        height: toolBar.visible ? 35 : 0
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
        ListView {
            id: leftToolbarListView
            height: parent.height
            anchors.left: openDrawerButton.right
            anchors.right: openInfoButton.left
            model: mainViewModel.leftToolbar
            orientation: ListView.Horizontal
            boundsBehavior: Flickable.StopAtBounds
            clip: true
            delegate: Item {
                id: itemContainer
                visible: !mainViewModel.isSmallSizeMode
                height: 34
                width: 40

                Loader {
                    anchors.fill: parent
                    sourceComponent: !mainViewModel.isControlButton(modelData.identifier) && mainViewModel.editLeftToolbar ? editPageButton : pageButton
                }

                DropArea {
                    anchors.fill: parent
                    onEntered: {
                        mainViewModel.dropIndex = modelData.identifier;
                    }
                }

                Component {
                    id: editPageButton

                    Item {
                        id: dragContainer
                        anchors.verticalCenter: parent.verticalCenter
                        Drag.active: itemMouseArea.drag.active
                        Drag.hotSpot.x: width / 2
                        Drag.hotSpot.y: height / 2

                        Rectangle {
                            anchors.fill: parent
                            color: mainViewModel.dropIndex !== mainViewModel.dragIndex &&
                                mainViewModel.dropIndex === modelData.identifier ? applicationThemeViewModel.currentItems.filterIconButtonHoverColor : "transparent"
                        }

                        Image {
                            id: iconImage
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            width: 20
                            height: 20
                            mipmap: true
                            asynchronous: true
                            source: applicationThemeViewModel.currentItems[modelData.itemIcon]
                        }

                        MouseArea {
                            id: itemMouseArea
                            anchors.fill: parent
                            acceptedButtons: Qt.LeftButton | Qt.RightButton
                            drag.target: parent
                            drag.onActiveChanged: {
                                if (itemMouseArea.drag.active) {
                                    mainViewModel.dragIndex = modelData.identifier;
                                    mainViewModel.dropIndex = "";
                                    return;
                                }

                                mainViewModel.reorderMenu();
                            }
                            onClicked: {
                                if(mouse.button & Qt.RightButton) mainViewModel.toggleEditToolBarMode();
                            }
                        }

                        states: [
                            State {
                                when: itemMouseArea.drag.active
                                ParentChange {
                                    target: dragContainer
                                    parent: leftToolbarListView
                                }
                            }
                        ]
                    }
                }

                Component {
                    id: pageButton

                    IconButton {
                        anchors.fill: parent
                        hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                        iconPath: applicationThemeViewModel.currentItems[modelData.itemIcon]
                        iconWidth: 20
                        iconHeight: 20
                        tooltipMessage: modelData.title
                        onButtonPressed: {
                            if (mainViewModel.isSmallSizeMode) return;

                            if (modelData.identifier === `additem`) {
                                addItemToToolbarPopup.open();
                            } else {
                                mainViewModel.selectPage(modelData.identifier);
                            }
                        }
                        onRightButtonPressed: {
                            if (mainViewModel.isSmallSizeMode) return;

                            mainViewModel.toggleEditToolBarMode();
                        }
                    }
                }
            }

            DefaultPopup {
                id: addItemToToolbarPopup
                width: 400
                height: 130
                modal: true
                focus: true
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                PlainText {
                    id: addPageLabel
                    text: "Выберите страницу"
                    fontPointSize: 10
                }

                CommonComboBox {
                    id: addPageComboBox
                    anchors.top: addPageLabel.bottom
                    anchors.topMargin: 4
                    width: parent.width
                    displayText: addPageComboBox.currentIndex > -1 ? mainViewModel.otherLeftToolbar[addPageComboBox.currentIndex].key : ""
                    model: mainViewModel.otherLeftToolbar
                    usePropertyKey: true
                }

                RoundedActionButton {
                    anchors.top: addPageComboBox.bottom
                    anchors.right: parent.right
                    anchors.topMargin: 4
                    text: "Добавить"
                    onClicked: {
                        mainViewModel.addOptionToToolbar(addPageComboBox.currentIndex);
                        addItemToToolbarPopup.close();
                    }
                }
            }
        }

        RoundedActionButton {
            visible: applicationsViewModel.isInstalledInstaller && versionChecker.isHaveNewVersion
            anchors.verticalCenter: parent.verticalCenter
            anchors.right: openInfoButton.left
            text: "Обновить"
            onClicked: {
                applicationsViewModel.runInstaller();
            }
        }
        IconButton {
            id: openInfoButton
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
                height: themePopupColumn.implicitHeight
                modal: true
                focus: true
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                Column {
                    id: themePopupColumn
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

                    PlainText {
                        id: selectFontLabel
                        fontPointSize: 11
                        text: "Шрифт"
                    }
                    CommonComboBox {
                        id: selectFontComboBox
                        width: 320
                        model: mainViewModel.fontFamilies

                        onActivated: {
                            userConfigurationViewModel.textFont = mainViewModel.fontFamilies[currentIndex]
                        }
                        onModelChanged: {
                            currentIndex = mainViewModel.fontFamilies.indexOf(userConfigurationViewModel.textFont);
                        }
                        Component.onCompleted: {
                            currentIndex = mainViewModel.fontFamilies.indexOf(userConfigurationViewModel.textFont);
                        }
                    }
                    PlainText {
                        id: enableScrollDecelerationLabel
                        fontPointSize: 11
                        text: "Поменять чувствительность скролирования"
                    }

                    Item {
                        height: usingVideoProxySwitch.height + 10
                        width: 120

                        CommonSwitch {
                            id: usingVideoProxySwitch
                            anchors.verticalCenter: parent.verticalCenter
                            checked: userConfigurationViewModel.usingScrollAcceleration
                            tooltipMessage: "Если режим включен то используется режим с инерцией и возможностью потянуть мышкой список,\n если выключено то потянуть мышкой будет нельзя но по колесику мыши скролирование будет более похожим на системное"
                            onCheckedChanged: {
                                userConfigurationViewModel.usingScrollAcceleration = usingVideoProxySwitch.checked;
                            }
                        }
                    }

                    Item {
                        width: 10
                        height: 20
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
            iconPath: mainViewModel.hasForwardHistory ? applicationThemeViewModel.currentItems.iconRightHalfScreen : applicationThemeViewModel.currentItems.iconMaximizeWindow
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
                text: synchronizationServicev2.isAuhorized ? "Вы авторизованы как " + synchronizationServicev2.nickName : "Вы не авторизованы"
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
    }

    LocalStorage {
        id: localStorage
    }

    AnalyticsService {
        id: analyticsService
        Component.onCompleted: {
            analyticsService.sendVersion();
        }
    }

    ReleaseLinkedSeries {
        id: releaseLinkedSeries
        apiv2host: userConfigurationViewModel.apiv2host
    }

    SystemSaveFileDialog {
        id: saveTorrentFileDialog
        title: "Выберите куда и как сохранить торрент файл"
        nameFilters: [ "Torrents (*.torrent)" ]
        onNeedSaveFile: {
            localStorage.copyTorrentToFile(window.tempTorrentPath, fileUrl);
        }
    }

    Synchronizev2Service {
        id: synchronizationServicev2
        apiv2host: userConfigurationViewModel.apiv2host
        token: userConfigurationViewModel.v2token
        cachehost: userConfigurationViewModel.cachehost
        cacheHostIsFolder: userConfigurationViewModel.useCacheFolder
        cacheFolder: userConfigurationViewModel.cacheFolder
        torrentDownloadMode: userConfigurationViewModel.torrentDownloadMode
        torrentStreamPort: userConfigurationViewModel.playerBuffer

        onUserCompleteAuthentificated: {
            notificationViewModel.sendInfoNotification(`Вы успешно вошли в аккаунт.`);

            mainViewModel.selectPage("release");
            synchronizationServicev2.getUserData();
        }

        onUserFailedAuthentificated: {
            notificationViewModel.sendInfoNotification(errorMessage);
        }

        onSynchronizeSeensFailed: {
            notificationViewModel.sendInfoNotification("Синхронизация просмотренного: " + errorMessage);
        }

        onSynchronizeFavoritesFailed: {
            notificationViewModel.sendInfoNotification(`Не удалось получить избранное:` + errorMessage);
        }

        onUserSignouted: {
            mainViewModel.notVisibleSignin = false;

            notificationViewModel.sendInfoNotification(`Вы успешно вышли из аккаунта. Чтобы войти обратно перейдите на страницу Войти.`)
        }

        onGetUserFailed: {
            notificationViewModel.sendInfoNotification("Не удалось получить данные пользователя: " + errorMessage);
        }

        onSynchronizeSeensCompleted: {
            releasesViewModel.synchronizeSeens(synchronizationServicev2.getUserSynchronizedSeens());
        }

        onSynchronizeCollectionFailed: {
            notificationViewModel.sendInfoNotification(`Не удалось получить коллекции:` + errorMessage);
        }

        onTokenChanged: {
            userConfigurationViewModel.v2token = synchronizationServicev2.token;
        }

        onIsAuhorizedChanged: {
            if (synchronizationServicev2.isAuhorized) {
                userAvatarCanvas.loadImage(synchronizationServicev2.userAvatar);
                mainViewModel.notVisibleSignin = true;
            } else {
                mainViewModel.notVisibleSignin = false;
            }
        }

        Component.onCompleted: {
            if (synchronizationServicev2.token) synchronizationServicev2.getUserData();

            synchronizationServicev2.synchronizeFullCache();
        }

        onSaveDownloadedTorrent: {
            window.tempTorrentPath = torrentPath;
            saveTorrentFileDialog.open();
        }

        onTorrentDownloaded: {
            userActivityViewModel.addDownloadedTorrentToCounter();
        }

        onSynchronizeCacheFailed: {
            notificationViewModel.sendInfoNotification("Ошибка во время синхронизации: " + errorMessage);
        }

        onSynchronizationCompletedNoChanges: {
            notificationViewModel.sendInfoNotification("Синхронизация релизов не требуется потому что нет изменений на " + new Date().toLocaleTimeString());
        }

        onSynchronizationCompleted: {
            releasesViewModel.savePreviousReleases(previousLastTimeStamp);

            filterDictionariesViewModel.refreshDictionaries();

            notificationViewModel.sendInfoNotification("Синхронизация релизов успешно завершена в " + new Date().toLocaleTimeString());

            releasesViewModel.reloadReleases();
        }

        onDownloadInTorrentStream: {
            notificationViewModel.sendInfoNotification("Релиз добавлен в TorrentStream");
        }

        onCheckNetworkAvailibilityCompletedChanged: {
            notificationViewModel.sendInfoNotification("Проверка доступности произошла успешно");
        }

        onCheckNetworkAvailibilityFailedChanged: {
            notificationViewModel.sendInfoNotification(message);
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

                    ctx.drawImage(synchronizationServicev2.userAvatar, 0, 0, 60, 60);

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

            PlainText {
                anchors.left: userAvatarCanvas.right
                anchors.leftMargin: 10
                anchors.verticalCenter: parent.verticalCenter
                text: "Вы авторизованы как:\n" + synchronizationServicev2.nickName
                color: applicationThemeViewModel.currentItems.colorDrawerItemText
                elide: Text.ElideRight
                wrapMode: Text.WordWrap
                width: drawer.width - userAvatarCanvas.width - logoutButton.width - 40
                maximumLineCount: 2
                fontPointSize: 10
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
                    synchronizationServicev2.logout();
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
                    source: applicationThemeViewModel.basedOnDark ? assetsLocation.iconsPath + "anilibrialogodark.svg" : assetsLocation.iconsPath + "anilibrialogodefault.svg"
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
                    PlainText {
                        color: applicationThemeViewModel.currentItems.colorDrawerItemText
                        fontPointSize: 11
                        text: qsTr("AniLiberty.Qt")
                    }
                    PlainText {
                        color: applicationThemeViewModel.currentItems.colorDrawerItemText
                        fontPointSize: 11
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
        needProxified: (
                (userConfigurationViewModel.usingVideoProxy && onlinePlayerWindowViewModel.isSelectedQtAv) ||
                (userConfigurationViewModel.usingVideoProxyVLC && onlinePlayerWindowViewModel.isSelectedVlc) ||
                (userConfigurationViewModel.usingVideoProxyMPV && onlinePlayerWindowViewModel.isSelectedMpv)
            ) &&
            torrentNotifierViewModel.activated
        needProxyFallback: userConfigurationViewModel.usingStrongProxy
        sendVolumeToRemote: userConfigurationViewModel.sendVolumeToRemote
        sendPlaybackToRemoteSwitch: userConfigurationViewModel.sendPlaybackToRemote
        remotePlayer.port: userConfigurationViewModel.remotePort
        torrentStream: torrentNotifierViewModel
        videoServerOverride: userConfigurationViewModel.videoServer
        restoreVideoMode: userConfigurationViewModel.restoreVideoMode
        isMaximized: window.visibility === Window.Maximized
        onIsFullScreenChanged: {
            if (isFullScreen) {
                window.showFullScreen();

            } else {
                let currentScreen = getCurrentScreen();
                if (!currentScreen) return;

                if (onlinePlayerViewModel.wasMaximized) {
                    window.showMaximized();
                } else {
                    window.showNormal();
                }
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
        onPauseInPlayer: {
            videoplayer.pauseInPlayer();
        }
        onStopInPlayer: {
            videoplayer.stopInPlayer();
        }
        onWatchedMinuteInPlayer: {
            userActivityViewModel.addWatchDurationMinute();
        }
        onPlayerPlaybackStateChanged: {
            //I'm using hardcode constant because multimedia module loading dynamically
            if (playerPlaybackState === "play") {
                osExtras.startPreventSleepMode();
            } else {
                osExtras.stopPreventSleepMode();
            }
        }
        onRutubeVideoIdChanged: {
            if (rutubeVideoId) Qt.openUrlExternally("https://rutube.ru/play/embed/" + rutubeVideoId);
        }
        onHidePanelIfItVisible: {
            videoplayer.needHidePlayer();
        }
        onVideoSourceChanged: {
            if (onlinePlayerViewModel.selectedRelease <= 0) return;

            const releaseTitle = releasesViewModel.getReleaseTitle(onlinePlayerViewModel.selectedRelease);
            extensionsViewModel.releaseOpenedInVideoPlayer(onlinePlayerViewModel.selectedRelease, releaseTitle, onlinePlayerViewModel.selectedVideo);
        }
        onReachEndingChanged: {
            if (!onlinePlayerViewModel.reachEnding) return;

            releasesViewModel.setSeenMark(onlinePlayerViewModel.selectedRelease, onlinePlayerViewModel.selectedVideoId, true);
            onlinePlayerViewModel.refreshSingleVideo(onlinePlayerViewModel.selectedRelease, onlinePlayerViewModel.selectedVideo);
            releasesViewModel.items.refreshSingleItem(onlinePlayerViewModel.selectedRelease);
            if (onlinePlayerViewModel.isLastSeriaIsSingleRelease()) {
                const nextReleaseId = releaseLinkedSeries.getNextLinkedRelease(onlinePlayerViewModel.selectedRelease);
                if (nextReleaseId > 0) {
                    onlinePlayerViewModel.showNextPosterRelease = true;
                    onlinePlayerViewModel.nextReleasePoster = releasesViewModel.getReleasePoster(nextReleaseId);
                }
            }
        }
    }

    ReleasesViewModel {
        id: releasesViewModel
        synchronizationServicev2: synchronizationServicev2
        synchronizationEnabled: synchronizationServicev2.synchronizeCacheActived
        applicationSettings: applicationSettings
        proxyPort: userConfigurationViewModel.playerBuffer
        localStorage: localStorage
        notCloseReleaseCardAfterWatch: userConfigurationViewModel.notCloseReleaseCardAfterWatch
        imageBackgroundViewModel.containerWidth: releases.backgroundImageWidth
        imageBackgroundViewModel.containerHeight: releases.backgroundImageHeight
        releaseLinkedSeries: releaseLinkedSeries
        items.scriptFilePath: userConfigurationViewModel.customScriptFile
        items.showFullTeam: userConfigurationViewModel.showFullTeam
        Component.onDestruction: {
            releasesViewModel.customGroups.saveState();
        }
        items.onScriptError: {
            notificationViewModel.sendInfoNotification(message);
        }
        userActivity: userActivityViewModel
        onReleaseCardOpened: {
            mainViewModel.selectPage("release:" + releasesViewModel.openedReleaseId);

            analyticsService.sendView("releasecard", "open", "%2Frelease");
            userActivityViewModel.addOpenedCardToCounter();

            releasesViewModel.openedReleseExtensionContent = extensionsViewModel.releaseOpenedInReleaseCard(releasesViewModel.openedReleaseId);
        }
        onReleasesFullyLoaded: {
            releaseLinkedSeries.refreshSeries();
            filterDictionariesViewModel.refreshDictionaries();
            myAnilibriaViewModel.refreshAllSectionsModels();
        }
        onNewEntitiesChanged: {
            if (releasesViewModel.newEntities) notificationViewModel.sendInfoNotification(releasesViewModel.newEntities);
        }
        onAddedSeenMarks: {
            synchronizationServicev2.addSeenMarks(uniqueIds, true);
        }
        onRemoveSeenMarks: {
            synchronizationServicev2.addSeenMarks(uniqueIds, false);
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
        }

        Releases {
            id: releases
            visible: mainViewModel.isReleasesPageVisible
            focus: true
            onWatchSingleRelease: {
                if (!onlinePlayerViewModel.releaseHasVideos(releaseId)) return;
                if (onlinePlayerViewModel.releaseIsRutube(releaseId)) {
                    if (!externalPlayerViewModel.torrentStreamActive) {
                        //TODO: show error message
                        return;
                    }
                    externalPlayerViewModel.setWebSocketPlayer(releaseId);
                    externalPlayerWindow.showWindow();
                    return;
                }

                onlinePlayerViewModel.quickSetupForSingleRelease(releaseId, startSeria);
                mainViewModel.selectPage("videoplayer");
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

        Applications {
            id: applications
            visible: mainViewModel.isApplicationsPageVisible
        }

        Extensions {
            id: extensions
            visible: mainViewModel.isExtensionsPageVisible
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
                                    padding: 8
                                    maximumLineCount: 4
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
        //uncomment for christmas version
        onIsOnlinePlayerPageVisibleChanged: {
            /*if (mainViewModel.isOnlinePlayerPageVisible) {
                particleEffect.visible = false;
                particleSystem.reset();
                emitter.reset();
                particleSystem.running = false;
            } else {
                particleEffect.visible = true;
                particleSystem.running = true;
            }*/
        }
        onChangeReleasesParameters: {
            if (!parameters.length) {
                releasesViewModel.closeReleaseCard();
                return;
            }

            releasesViewModel.showReleaseCard(parseInt(parameters), false);
        }
        onReleasesSeriesPageToNavigated: {
            releaseseries.navigateTo();
        }
        onChangeReleaseSeriesParameters: {
            if (!parameters.length) {
                releaseLinkedSeries.closeCard();
                return;
            }

            releaseLinkedSeries.selectByIndex(parseInt(parameters));
        }
        onChangeExtensionsParameters: {
            if (!parameters.length) {
                extensionsViewModel.closeCard();
                return;
            }

            extensionsViewModel.openCard();
        }

        Component.onDestruction: {
            mainViewModel.saveState();
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
        Component.onCompleted: {
            releasesViewModel.refreshApiHost();
        }
        Component.onDestruction: {
            if (!userConfigurationViewModel.showedVideoForNewcomers) userConfigurationViewModel.showedVideoForNewcomers = true;
            userConfigurationViewModel.saveSettingsToFile();
        }
    }

    OnlinePlayerWindowViewModel {
        id: onlinePlayerWindowViewModel
        startupPlayer: userConfigurationViewModel.needSavePlayer ? userConfigurationViewModel.lastSelectedPlayer : ""
    }

    EmbeddedVideoPlayer {
        id: embeddedVideoPlayer

        Component.onCompleted: {
            embeddedVideoPlayer.x = window.width / 2 - embeddedVideoPlayer.width / 2;
            embeddedVideoPlayer.y = window.height / 2 - embeddedVideoPlayer.height / 2;
        }
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
        objectName: "applicationThemeViewModel"
        notAddCopyToName: userConfigurationViewModel.removeCopyFromName
        Component.onDestruction: {
            applicationThemeViewModel.saveCurrentState();
        }
        fieldList.onErrorMessage: {
            notificationViewModel.sendInfoNotification(message);
        }
    }

    FilterDictionariesViewModel {
        id: filterDictionariesViewModel
        releases: releasesViewModel
    }

    ExternalPlayerViewModel {
        id: externalPlayerViewModel
        torrentStreamActive: torrentNotifierViewModel.activated
        torrentStreamPort: userConfigurationViewModel.playerBuffer
        torrentStreamHost: "localhost"
        releasesViewModel: releasesViewModel
    }

    ExternalPlayerWindow {
        id: externalPlayerWindow
    }

    TorrentNotifierViewModel {
        id: torrentNotifierViewModel
        torrentStreamPath: userConfigurationViewModel.torrentStreamPath
        removeAllData: userConfigurationViewModel.removeAllDownloadedTorrent
        port: userConfigurationViewModel.playerBuffer
        releasesViewModel: releasesViewModel
        onTorrentFullyDownloaded: {
            notificationViewModel.sendInfoNotification("Торрент скачан " + releaseName);
            torrentNotifierViewModel.startGetTorrentData(false);
        }
        onTorrentStreamNotConfigured: {
            torrentNotifierViewModel.startGetNotifiers();
        }
        onTorrentStreamStarted: {
            torrentNotifierViewModel.startGetNotifiers();
        }
        onActivatedChanged: {
            if (activated) torrentNotifierViewModel.startGetTorrentData(false);
        }
        onPrepareWatchTorrentFiles: {
            onlinePlayerViewModel.quickSetupForSingleDownloadedTorrent(files, releaseId);
            mainViewModel.selectPage("videoplayer");
        }
        onTorrentsRefreshed: {
            if (!onlinePlayerWindowViewModel.isSelectedVlc && !onlinePlayerWindowViewModel.isSelectedMpv) {
                if (onlinePlayerWindowViewModel.isHasVlc) onlinePlayerWindowViewModel.changePlayer("VLC");
                if (onlinePlayerWindowViewModel.isHasMpv) onlinePlayerWindowViewModel.changePlayer("mpv");
            }

            const torrentId = torrentNotifierViewModel.lastRefreshIdentifier;
            torrentNotifierViewModel.lastRefreshIdentifier = -1;
            onlinePlayerViewModel.quickSetupForSingleTorrentRelease(
                releasesViewModel.openedReleaseId,
                torrentId,
                userConfigurationViewModel.playerBuffer
            );

            mainViewModel.selectPage("videoplayer");
        }
        Component.onCompleted: {
            torrentNotifierViewModel.tryStartTorrentStreamApplication();
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

    ApplicationsViewModel {
        id: applicationsViewModel
        onChangeTorrentStreamPath: {
            userConfigurationViewModel.torrentStreamPath = fullPath;
        }
    }

    SynchronizationHub {
        id: synchronizationHub
        releases: releasesViewModel
        synchronization: synchronizationServicev2
    }

    ExtensionsViewModel {
        id: extensionsViewModel
        releases: releasesViewModel
        onSendInformationNotificationReceived: {
            notificationViewModel.sendInfoNotification(message);
        }
        Component.onDestruction: {
            extensionsViewModel.saveValues();
        }
    }

    Item {
        id: assetsLocation
        property string path: Qt.resolvedUrl("../Assets/")
        property string backgroundsPath: Qt.resolvedUrl("../Assets/Backgrounds/")
        property string iconsPath: Qt.resolvedUrl("../Assets/Icons/")
    }

    //uncomment for christmas version
    /*Item {
        id: particleEffect
        anchors.fill: parent

        ParticleSystem {
            id: particleSystem
        }

        Emitter {
            id: emitter
            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            width: parent.width
            height: 80
            system: particleSystem
            emitRate: 1
            lifeSpan: 25000
            lifeSpanVariation: 10
            size: 15
            endSize: 20
            velocity: AngleDirection {
                angle: 90
                angleVariation: 40
                magnitude: 30
                magnitudeVariation: 10
            }
        }

        ImageParticle {
            source: assetsLocation.iconsPath + "snowflake.svg"
            system: particleSystem
            rotation: 15
            rotationVariation: 5
            rotationVelocity: 60
            rotationVelocityVariation: 15
        }
    }*/
}
