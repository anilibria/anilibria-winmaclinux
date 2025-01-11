import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Anilibria.Services 1.0
import "../Controls"

Page {
    id: root
    anchors.fill: parent

    signal navigateFrom()
    signal navigateTo()

    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
    }

    ApiServiceConfigurator {
        id: apiServiceConfigurator
    }

    ProxyConfigurator {
        id: proxyConfigurator
    }

    Loader {
        anchors.fill: parent
        sourceComponent: root.visible ? maintenceContent : null
    }

    Component {
        id: maintenceContent

        RowLayout {
            id: panelContainer
            anchors.fill: parent
            spacing: 0

            ColumnLayout {
                Layout.fillHeight: true
                Layout.fillWidth: true
                spacing: 2

                Rectangle {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 45
                    height: 45
                    color: applicationThemeViewModel.pageUpperPanel
                }

                Flickable {
                    id: scrollview
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignJustify
                    clip: true
                    contentWidth: parent.width
                    contentHeight: flickableContent.height
                    ScrollBar.vertical: ScrollBar {
                        active: true
                    }

                    Rectangle {
                        id: flickableContent
                        width: scrollview.width - 10
                        height: 840
                        color: "transparent"
                        border.color: applicationThemeViewModel.colorBorderInPanel
                        border.width: 1

                        GridLayout {
                            width: parent.width
                            columns: 2

                            Item {
                                width: 220
                                height: 100

                                RoundedActionButton {
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    width: parent.width - 10
                                    text: "Очистить кеш постеров"
                                    onClicked: {
                                        localStorage.clearPostersCache();
                                    }
                                }
                            }

                            Rectangle {
                                color: "transparent"
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                PlainText {
                                    fontPointSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    width: parent.width
                                    wrapMode: Text.WordWrap
                                    text: "Удалить все сохраненные локально сохраненные постеры, после выполнения операции необходимо перезапустить приложение"
                                }
                            }

                            Item {
                                width: 220
                                height: 100

                                RoundedActionButton {
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    width: parent.width - 10
                                    text: "nextAPI"
                                    onClicked: {
                                        apiAddressPopup.open();
                                    }
                                }
                            }

                            Rectangle {
                                color: "transparent"
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                PlainText {
                                    fontPointSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    width: parent.width
                                    wrapMode: Text.WordWrap
                                    text: "Настройка nextAPI и кеш серверов, позволяет менять адреса и проверять их работоспособность."
                                }
                            }

                            Item {
                                width: 220
                                height: 100

                                RoundedActionButton {
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    width: parent.width - 10
                                    text: "Настроить прокси"
                                    onClicked: {
                                        proxyType.currentIndex = 0;
                                        switch (proxyConfigurator.proxyType) {
                                            case `SOCKS5`:
                                                proxyType.currentIndex = 1;
                                                break;
                                            case `Http`:
                                                proxyType.currentIndex = 2;
                                                break;
                                        }
                                        proxyPort.text = proxyConfigurator.port;
                                        proxyAddress.text = proxyConfigurator.address;
                                        proxyUsername.text = proxyConfigurator.userName;
                                        proxyPassword.text = proxyConfigurator.password;

                                        proxyPopup.open();
                                    }
                                }
                            }

                            Rectangle {
                                color: "transparent"
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                PlainText {
                                    fontPointSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    width: parent.width
                                    wrapMode: Text.WordWrap
                                    text: "Позволяет указать настройки проксирования запросов для всего приложения."
                                }
                            }

                            Item {
                                width: 220
                                height: 100

                                RoundedActionButton {
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    width: parent.width - 10
                                    text: "Удалить лишнее"
                                    onClicked: {
                                        localStorage.clearRedundantFilesFromCache();
                                    }
                                }
                            }

                            Rectangle {
                                color: "transparent"
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                PlainText {
                                    fontPointSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    width: parent.width
                                    wrapMode: Text.WordWrap
                                    text: "Удалить из кеша лишние файлы которые могли появиться во время работы приложения, например torrent файлы, m3u файлы и т.п."
                                }
                            }

                            Item {
                                width: 220
                                height: 100

                                RoundedActionButton {
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    width: parent.width - 10
                                    text: "Бэкап кеша приложения"
                                    onClicked: {
                                        selectFolderBackupDialog.open();
                                    }
                                }
                            }

                            Rectangle {
                                color: "transparent"
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                PlainText {
                                    fontPointSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    width: parent.width
                                    wrapMode: Text.WordWrap
                                    text: "Вы можете сделать бэкап файлов кеша приложения содержащего историю просмотра, кинозал, настройки и многое другое. Очень полезно если Вы хотите переезжать на другую машину или переустанавливать систему на текущей."
                                }
                            }

                            Item {
                                width: 220
                                height: 100

                                RoundedActionButton {
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    anchors.leftMargin: 10
                                    width: parent.width - 10
                                    text: "Восстановить из бэкапа"
                                    onClicked: {
                                        selectFolderRestoreDialog.open();
                                    }
                                }
                            }

                            Rectangle {
                                color: "transparent"
                                Layout.fillWidth: true
                                Layout.fillHeight: true

                                PlainText {
                                    fontPointSize: 12
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.left: parent.left
                                    width: parent.width
                                    wrapMode: Text.WordWrap
                                    text: "Вы можете восстановить состояние из созданного ранее бекапа кеша файлов приложения. Очень важно чтобы на момент выполнения синхронизация релизов была завершена иначе кеш может побится! После выполнения перезапустите приложение!"
                                }
                            }
                        }
                    }
                }

                DefaultPopup {
                    id: apiAddressPopup
                    x: window.width / 2 - apiAddressPopup.width / 2
                    y: window.height / 2 - apiAddressPopup.height / 2
                    width: 450
                    height: 400
                    modal: true
                    focus: true
                    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                    ColumnLayout {
                        width: parent.width
                        spacing: 10

                        AccentText {
                            width: apiAddressPopup.width
                            text: "Адрес nextAPI"
                            fontPointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        Rectangle {
                            color: "transparent"
                            width: apiAddressPopup.width - 30
                            height: serversComboBox.height

                            CommonComboBox {
                                id: serversComboBox
                                width: parent.width
                                model: ListModel {
                                    ListElement {
                                        text: "Основной домен"
                                    }
                                    ListElement {
                                        text: "Зеркало app"
                                    }
                                }


                                Component.onCompleted: {
                                    if (userConfigurationViewModel.apiv2host === synchronizationServicev2.mainNextAPIServer) {
                                        serversComboBox.currentIndex = 0;
                                    }
                                    if (userConfigurationViewModel.apiv2host === synchronizationServicev2.appMirrorNextAPIServer) {
                                        serversComboBox.currentIndex = 1;
                                    }
                                }
                            }
                        }

                        AccentText {
                            width: apiAddressPopup.width
                            text: localFolder.checked ? "Папка с кешем" : "Адрес кеш сервера"
                            fontPointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        Rectangle {
                            color: "transparent"
                            width: apiAddressPopup.width - 30
                            height: cacheServersComboBox.height

                            CommonTextField {
                                id: cacheFolderTextField
                                visible: localFolder.checked
                                width: parent.width - 30
                                placeholderText: "Нажмите на иконку справа для выбора"
                                readOnly: true
                                text: userConfigurationViewModel.cacheFolder
                            }

                            IconButton {
                                visible: localFolder.checked
                                anchors.left: cacheFolderTextField.right
                                anchors.leftMargin: 4
                                hoverColor: applicationThemeViewModel.currentItems.filterIconButtonHoverColor
                                iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogCompilation
                                iconWidth: 20
                                iconHeight: 20
                                width: 30
                                height: 30
                                onButtonPressed: {
                                    selectCacheFolderDialog.open();
                                }

                                SystemOpenFolderDialog {
                                    id: selectCacheFolderDialog
                                    onNeedOpenFolder: {
                                        if (Qt.platform.os === 'windows') {
                                            cacheFolderTextField.text = folderUrl.replace("file:///", "");
                                        } else {
                                            cacheFolderTextField.text = folderUrl.replace("file://", "");
                                        }
                                    }
                                }
                            }

                            CommonComboBox {
                                id: cacheServersComboBox
                                visible: !localFolder.checked
                                width: parent.width
                                model: ListModel {
                                    ListElement {
                                        text: "Основной github"
                                    }
                                }

                                Component.onCompleted: {
                                    if (userConfigurationViewModel.cachehost === synchronizationServicev2.mainGithubCacheServer) {
                                        cacheServersComboBox.currentIndex = 0;
                                    }
                                }
                            }
                        }

                        AccentText {
                            width: apiAddressPopup.width
                            text: "Локальная папка вместо кеш сервера"
                            fontPointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        Rectangle {
                            color: "transparent"
                            width: apiAddressPopup.width - 30
                            height: localFolder.height - 10

                            CommonSwitch {
                                id: localFolder
                                tooltipMessage: "Использовать локальную папку для чтения кеша вместо вебсайта"
                                checked: userConfigurationViewModel.useCacheFolder
                            }
                        }

                        AccentText {
                            width: apiAddressPopup.width
                            text: "Использовать сервер видео"
                            fontPointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        Rectangle {
                            color: "transparent"
                            width: apiAddressPopup.width - 30
                            height: videoServersComboBox.height

                            CommonComboBox {
                                id: videoServersComboBox
                                width: parent.width
                                model: ListModel {
                                    ListElement {
                                        text: "Использовать из кеша"
                                    }
                                    ListElement {
                                        text: "Всегда использовать РФ"
                                    }
                                    ListElement {
                                        text: "Всегда использовать вне РФ"
                                    }
                                }

                                Component.onCompleted: {
                                    videoServersComboBox.currentIndex = userConfigurationViewModel.videoServer;
                                }
                            }
                        }

                        Rectangle {
                            color: "transparent"
                            width: apiAddressPopup.width - 20
                            height: 50

                            RoundedActionButton {
                                anchors.right: saveButton.left
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                visible: !apiServiceConfigurator.isDefault
                                text: "Проверить"
                                width: 100
                                onClicked: {
                                    let message = "";
                                    if (localFolder.checked) {
                                        message = synchronizationServicev2.checkFolderAvailability(cacheFolderTextField.text);
                                    } else {
                                        synchronizationServicev2.checkNetworkAvailability("");
                                    }

                                    if (message) notificationViewModel.sendInfoNotification(message);

                                    apiAddressPopup.close();
                                }
                            }

                            RoundedActionButton {
                                id: saveButton
                                anchors.right: cancelButton.left
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                text: "Сохранить"
                                width: 100
                                onClicked: {
                                    switch (cacheServersComboBox.currentIndex) {
                                        case 0:
                                            userConfigurationViewModel.cachehost = synchronizationServicev2.mainGithubCacheServer;
                                            break;
                                    }

                                    switch (serversComboBox.currentIndex) {
                                        case 0:
                                            userConfigurationViewModel.apiv2host = synchronizationServicev2.mainNextAPIServer;
                                            break;
                                        case 1:
                                            userConfigurationViewModel.apiv2host = synchronizationServicev2.appMirrorNextAPIServer;
                                            break;
                                    }

                                    userConfigurationViewModel.useCacheFolder = localFolder.checked;
                                    userConfigurationViewModel.cacheFolder = cacheFolderTextField.text;
                                    userConfigurationViewModel.videoServer = videoServersComboBox.currentIndex;


                                    apiAddressPopup.close();
                                }
                            }

                            RoundedActionButton {
                                id: cancelButton
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                text: "Отмена"
                                width: 100
                                onClicked: {
                                    apiAddressPopup.close();
                                }
                            }
                        }
                    }
                }

                DefaultPopup {
                    id: proxyPopup
                    x: window.width / 2 - proxyPopup.width / 2
                    y: window.height / 2 - proxyPopup.height / 2
                    width: 450
                    height: 500
                    modal: true
                    focus: true
                    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                    ColumnLayout {
                        width: parent.width
                        spacing: 10

                        AccentText {
                            width: proxyPopup.width
                            text: "Тип"
                            fontPointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        Item {
                            width: proxyPopup.width - 30
                            height: proxyType.height

                            CommonComboBox {
                                id: proxyType
                                width: parent.width
                                model: [ "", "SOCKS5", "Http" ]
                            }
                        }


                        AccentText {
                            width: proxyPopup.width
                            text: "Адрес сервера"
                            fontPointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        Item {
                            width: proxyPopup.width - 30
                            height: proxyAddress.height

                            CommonTextField {
                                id: proxyAddress
                                width: parent.width
                                placeholderText: "Введите url"
                            }
                        }

                        AccentText {
                            width: proxyPopup.width
                            text: "Порт"
                            fontPointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        Item {
                            width: proxyPopup.width - 30
                            height: proxyPort.height

                            CommonTextField {
                                id: proxyPort
                                width: parent.width
                                inputMethodHints: Qt.ImhFormattedNumbersOnly
                                placeholderText: "Введите число"
                            }
                        }

                        AccentText {
                            width: proxyPopup.width
                            text: "Логин"
                            fontPointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        Item {
                            width: proxyPopup.width - 30
                            height: proxyUsername.height

                            CommonTextField {
                                id: proxyUsername
                                width: parent.width
                                placeholderText: "Введите имя пользователя"
                            }
                        }

                        AccentText {
                            width: proxyPopup.width
                            text: "Пароль"
                            fontPointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        Rectangle {
                            width: proxyPopup.width - 30
                            height: proxyPassword.height

                            CommonTextField {
                                id: proxyPassword
                                width: parent.width
                                placeholderText: "Введите пароль"
                            }
                        }

                        Rectangle {
                            color: "transparent"
                            width: proxyPopup.width - 20
                            height: 70

                            RoundedActionButton {
                                anchors.right: saveProxyButton.left
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                text: "Отключить"
                                width: 100
                                onClicked: {
                                    proxyConfigurator.disableProxy();

                                    proxyPopup.close();
                                }
                            }

                            RoundedActionButton {
                                id: saveProxyButton
                                anchors.right: cancelProxyButton.left
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                enabled: proxyType.text !== `` && proxyPort.text !== `0` && proxyAddress.text !== ``
                                text: "Сохранить"
                                width: 100
                                onClicked: {
                                    proxyConfigurator.proxyType = proxyType.currentText;
                                    proxyConfigurator.port = proxyPort.text;
                                    proxyConfigurator.address = proxyAddress.text;
                                    proxyConfigurator.userName = proxyUsername.text;
                                    proxyConfigurator.password = proxyPassword.text;
                                    proxyConfigurator.saveProxy();

                                    proxyPopup.close();
                                }
                            }

                            RoundedActionButton {
                                id: cancelProxyButton
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                text: "Отмена"
                                width: 100
                                onClicked: {
                                    proxyPopup.close();
                                }
                            }
                        }
                    }
                }
            }            
        }
    }

    Connections {
        target: localStorage
        function onBackupFilesCopied() {
            notificationViewModel.sendInfoNotification("Кеш приложения скопирован!");
        }
        function onRestoreFileCopied() {
            userActivityViewModel.refreshUserActivity();
            userConfigurationViewModel.refreshConfiguration();
            notificationViewModel.sendInfoNotification("Кеш восстановлен! Перезапустите приложение!");
        }
    }

    SystemOpenFolderDialog {
        id: selectFolderBackupDialog
        title: "Выберите папку для создания бэкапа"
        onNeedOpenFolder: {
            localStorage.backupCache(folderUrl);
        }
    }

    SystemOpenFolderDialog {
        id: selectFolderRestoreDialog
        title: "Выберите папку для восстановления из бэкапа"
        onNeedOpenFolder: {
            localStorage.restoreBackupCache(folderUrl);
        }
    }
}

