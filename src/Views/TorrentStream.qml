import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import "../Controls"

Page {
    id: root
    anchors.fill: parent
    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
    }

    signal navigateFrom()
    signal navigateTo()

    Loader {
        anchors.fill: parent
        sourceComponent: root.visible ? pageContent : null
    }

    Component {
        id: pageContent
        Item {
            anchors.fill: parent

            RowLayout {
                id: panelContainer
                anchors.fill: parent
                spacing: 0
                Rectangle {
                    color: applicationThemeViewModel.pageVerticalPanel
                    width: 40
                    Layout.fillHeight: true
                    Column {
                        LeftPanelIconButton {
                            iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogSynchronization
                            iconWidth: 34
                            iconHeight: 34
                            tooltipMessage: "Обновить список"
                            onButtonPressed: {
                                torrentNotifierViewModel.startGetTorrentData(false);
                            }
                        }

                        LeftPanelIconButton {
                            iconPath: applicationThemeViewModel.currentItems.iconDeleteItem
                            iconWidth: 29
                            iconHeight: 29
                            tooltipMessage: "Удалить все торренты и скачанные файлы"
                            onButtonPressed: {
                                torrentNotifierViewModel.clearAllData();
                            }
                        }

                        LeftPanelIconButton {
                            iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogSettings
                            iconWidth: 29
                            iconHeight: 29
                            tooltipMessage: "Настройки страницы TorrentStream"
                            onButtonPressed: {
                                torrentStreamPopup.open();
                            }
                        }
                    }
                }
                Rectangle {
                    color: "transparent"
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Rectangle {
                        id: upperPanelContainer
                        width: parent.width
                        height: 45
                        color: applicationThemeViewModel.pageUpperPanel

                        PlainText {
                            id: statusText
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            fontPointSize: mainViewModel.isSmallSizeMode ? 10 : 12
                            text: torrentNotifierViewModel.activated ? "TorrentStream подключен" : "TorrentStream не подключен"
                        }
                        RoundedActionButton {
                            id: connectButton
                            visible: !torrentNotifierViewModel.activated
                            anchors.left: statusText.right
                            anchors.verticalCenter: parent.verticalCenter
                            text: "Подключиться"
                            onClicked: {
                                torrentNotifierViewModel.startGetNotifiers(userConfigurationViewModel.playerBuffer);
                            }
                        }
                        RoundedActionButton {
                            id: deleteRedundantButton
                            visible: !mainViewModel.isSmallSizeMode
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            text: "Удалить лишнее"
                            onClicked: {
                                torrentNotifierViewModel.removeRedundant();
                            }
                        }
                        LinkedText {
                            id: guideText
                            visible: !torrentNotifierViewModel.activated && !mainViewModel.isSmallSizeMode
                            anchors.left: connectButton.right
                            anchors.leftMargin: 4
                            anchors.verticalCenter: parent.verticalCenter
                            fontPointSize: 10
                            text: "<a href='https://github.com/anilibria/anilibria-winmaclinux/blob/master/torrentstream.md'>Инструкция об установке</a>"
                        }
                    }

                    ListView {
                        id: torrentsListView
                        anchors.top: upperPanelContainer.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        model: torrentNotifierViewModel.torrents
                        clip: true
                        spacing: 4
                        ScrollBar.vertical: ScrollBar {
                            active: true
                        }
                        delegate: Rectangle {
                            width: torrentsListView.width
                            height: 220
                            color: "transparent"

                            Rectangle {
                                anchors.fill: parent
                                anchors.leftMargin: 4
                                anchors.rightMargin: 4
                                radius: 10
                                color: applicationThemeViewModel.panelBackground

                                RowLayout {
                                    anchors.fill: parent

                                    Item {
                                        height: parent.height
                                        Layout.topMargin: 6
                                        Layout.preferredWidth: 145
                                        Layout.fillHeight: true
                                        Layout.leftMargin: 6

                                        CorneredImage {
                                            anchors.top: parent.top
                                            anchors.left: parent.left
                                            anchors.leftMargin: 0
                                            width: 140
                                            height: 210
                                            posterSource: localStorage.getReleasePosterPath(releaseId, poster)
                                            emptyBorderBackground: applicationThemeViewModel.panelBackground
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

                                            AccentText {
                                                fontPointSize: 10
                                                text: title
                                                Layout.fillWidth: true
                                                maximumLineCount: 2
                                                elide: Text.ElideRight
                                                wrapMode: Text.Wrap
                                            }

                                            PlainText {
                                                Layout.preferredHeight: 20
                                                fontPointSize: 10
                                                visible: !!torrentTitle
                                                text: torrentTitle
                                            }

                                            PlainText {
                                                Layout.preferredHeight: 20
                                                fontPointSize: 10
                                                text: "Скачано файлов " + filesDownloaded + " из " + filesCount
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
                                        Layout.topMargin: 2
                                        height: parent.height
                                        color: "transparent"

                                        Column {
                                            anchors.centerIn: parent

                                            FilterPanelIconButton {
                                                iconPath: applicationThemeViewModel.currentItems.iconContextMenu
                                                tooltipMessage: "Открыть меню операций доступных для торрента"
                                                onButtonPressed: {
                                                    torrentActionsMenu.open();
                                                }

                                                CommonMenu {
                                                    id: torrentActionsMenu
                                                    y: parent.y
                                                    width: 300

                                                    CommonMenuItem {
                                                        text: "Удалить только торрент"
                                                        onPressed: {
                                                            torrentActionsMenu.close();

                                                            torrentNotifierViewModel.clearOnlyTorrent(torrentPath);
                                                        }
                                                    }
                                                    CommonMenuItem {
                                                        text: "Удалить торрент и файлы"
                                                        onPressed: {
                                                            torrentActionsMenu.close();

                                                            torrentNotifierViewModel.clearTorrentAndData(torrentPath);
                                                        }
                                                    }
                                                }
                                            }
                                            FilterPanelIconButton {
                                                iconPath: assetsLocation.iconsPath + "videoplayermenu.svg"
                                                tooltipMessage: "Начать просмотр скачанного торрента"
                                                onButtonPressed: {
                                                    if (filesDownloaded < filesCount) {
                                                        notAllTorrentsDownloadedInfo.open();
                                                        return;
                                                    }

                                                    torrentNotifierViewModel.watchDownloadedTorrents(identifier);
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

            MessageModal {
                id: notAllTorrentsDownloadedInfo
                header: "Просмотр торрента целиком"
                message: "Чтобы просмотреть торрент целиком его необходимо вначале скачать тоже целиком.<br>
                    Когда торрент загрузиться целиком Вы получите уведомление. После чего нажмите на эту кнопку еще раз."
                content: Row {
                    spacing: 6
                    anchors.right: parent.right

                    RoundedActionButton {
                        text: "Закрыть"
                        width: 100
                        onClicked: {
                            notAllTorrentsDownloadedInfo.close();
                        }
                    }
                }
            }

            DefaultPopup {
                id: torrentStreamPopup
                x: window.width / 2 - torrentStreamPopup.width / 2
                y: window.height / 2 - torrentStreamPopup.height / 2
                width: 550
                height: 480
                modal: true
                focus: true
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                ColumnLayout {
                    width: parent.width
                    spacing: 10

                    AccentText {
                        width: torrentStreamPopup.width
                        text: "Путь к приложению"
                        fontPointSize: 12
                        font.bold: true
                        elide: Text.ElideRight
                    }

                    Item {
                        width: torrentStreamPopup.width - 30
                        height: torrentStreamPath.height

                        CommonTextField {
                            id: torrentStreamPath
                            width: parent.width
                            placeholderText: "Введите полный путь"
                            text: userConfigurationViewModel.torrentStreamPath
                        }
                    }


                    AccentText {
                        width: torrentStreamPopup.width
                        text: "Порт приложения"
                        fontPointSize: 12
                        font.bold: true
                        elide: Text.ElideRight
                    }

                    Item {
                        width: torrentStreamPopup.width - 30
                        height: torrentStreamPort.height

                        CommonTextField {
                            id: torrentStreamPort
                            width: parent.width
                            placeholderText: "Введите порт приложения TorrentStream"
                            text: userConfigurationViewModel.playerBuffer
                            validator: IntValidator {
                                top: 65535
                                bottom: 0
                            }
                        }
                    }

                    AccentText {
                        width: torrentStreamPopup.width
                        text: "Использовать проксирование видео для QtAv"
                        fontPointSize: 12
                        font.bold: true
                        elide: Text.ElideRight
                    }

                    Item {
                        width: torrentStreamPopup.width
                        height: 15

                        CommonSwitch {
                            id: usingVideoProxySwitch
                            height: 15
                            checked: userConfigurationViewModel.usingVideoProxy
                        }
                    }

                    AccentText {
                        width: torrentStreamPopup.width
                        text: "Использовать проксирование видео для VLC"
                        fontPointSize: 12
                        font.bold: true
                        elide: Text.ElideRight
                    }

                    Item {
                        width: torrentStreamPopup.width
                        height: 15

                        CommonSwitch {
                            id: usingVideoProxyVLCSwitch
                            height: 15
                            checked: userConfigurationViewModel.usingVideoProxyVLC
                        }
                    }

                    AccentText {
                        width: torrentStreamPopup.width
                        text: "Использовать проксирование видео для mpv"
                        fontPointSize: 12
                        font.bold: true
                        elide: Text.ElideRight
                    }

                    Item {
                        width: torrentStreamPopup.width
                        height: 15

                        CommonSwitch {
                            id: usingVideoProxyMpvSwitch
                            height: 15
                            checked: userConfigurationViewModel.usingVideoProxyMPV
                        }
                    }

                    AccentText {
                        width: torrentStreamPopup.width
                        text: "Использовать надежное проксирование видео"
                        fontPointSize: 12
                        font.bold: true
                        elide: Text.ElideRight
                    }

                    Item {
                        width: torrentStreamPopup.width
                        height: 15

                        CommonSwitch {
                            id: usingStrongProxySwitch
                            height: 15
                            checked: userConfigurationViewModel.usingStrongProxy
                        }
                    }

                    AccentText {
                        width: torrentStreamPopup.width
                        text: "Удалять скачанное после каждого запуска приложения"
                        fontPointSize: 12
                        font.bold: true
                        elide: Text.ElideRight
                    }

                    Item {
                        width: torrentStreamPopup.width
                        height: 15

                        CommonSwitch {
                            id: removeAllDownloadedTorrentSwitch
                            height: 15
                            checked: userConfigurationViewModel.removeAllDownloadedTorrent
                        }
                    }

                    Item {
                        width: torrentStreamPopup.width - 20
                        height: 40

                        RoundedActionButton {
                            id: torrentStreamSaveButton
                            anchors.right: torrentStreamCancelButton.left
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            text: "Сохранить"
                            width: 100
                            onClicked: {
                                const value = parseInt(torrentStreamPort.text);
                                if (value > -1) userConfigurationViewModel.playerBuffer = value;

                                userConfigurationViewModel.usingVideoProxy = usingVideoProxySwitch.checked;
                                userConfigurationViewModel.torrentStreamPath = torrentStreamPath.text;
                                userConfigurationViewModel.removeAllDownloadedTorrent = removeAllDownloadedTorrentSwitch.checked;
                                userConfigurationViewModel.usingVideoProxyVLC = usingVideoProxyVLCSwitch.checked;
                                userConfigurationViewModel.usingStrongProxy = usingStrongProxySwitch.checked;
                                userConfigurationViewModel.usingVideoProxyMPV = usingVideoProxyMpvSwitch.checked;

                                torrentStreamPopup.close();
                            }
                        }

                        RoundedActionButton {
                            id: torrentStreamCancelButton
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            text: "Отмена"
                            width: 100
                            onClicked: {
                                torrentStreamPopup.close();
                            }
                        }
                    }
                }
            }

            Component.onCompleted: {
                torrentNotifierViewModel.startGetTorrentData(false);
            }
        }
    }
}
