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
import QtGraphicalEffects 1.0
import Anilibria.Services 1.0
import "../Controls"
import "../Theme"

Page {
    id: root
    anchors.fill: parent

    signal navigateFrom()
    signal navigateTo()

    background: Rectangle {
        color: ApplicationTheme.pageBackground
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
            Rectangle {
                color: ApplicationTheme.pageVerticalPanel
                width: 40
                Layout.fillHeight: true
                Column {
                    LeftPanelIconButton {
                        overlayVisible: false
                        iconWidth: 28
                        iconHeight: 28
                        iconPath: assetsLocation.iconsPath + "coloreddrawer.svg"
                        onButtonPressed: {
                            drawer.open();
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
                }

                Flickable {
                    id: scrollview
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignJustify
                    clip: true
                    contentWidth: parent.width
                    contentHeight: 100
                    ScrollBar.vertical: ScrollBar {
                        active: true
                    }

                    Rectangle {
                        width: scrollview.width - 10
                        height: 500
                        color: "transparent"
                        border.color: "white"
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
                                    text: "Импорт релизов из файла"
                                    onClicked: {
                                        importReleasesFileDialog.open();
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
                                    text: "Позволяет импортировать релизы из файла. Импортированные релизы не будут обновляться."
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
                                    text: "Изменить адрес api"
                                    onClicked: {
                                        apiAddress.text = apiServiceConfigurator.apiAddress;
                                        staticAddress.text = apiServiceConfigurator.staticAddress;

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
                                    text: "Позволяет изменить адрес api а также адрес для статики. Внимание! Стоит пользоваться этим функционалом только если Вы понимаете зачем Вам это нужно, иначе Вы просто сломаете приложение. Изменения вступят в силу после перезапуска приложения."
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
                                    text: "Сбросить удаленные"
                                    onClicked: {
                                        releasesViewModel.clearDeletedInCacheMarks();
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
                                    text: "Если Вы столкнулись с проблемой что каких-то релизов нет в приложении но есть на сайте то Вы можете сбросить удаленные релизы."
                                }
                            }



                        }
                    }
                }

                Popup {
                    id: apiAddressPopup
                    x: window.width / 2 - apiAddressPopup.width / 2
                    y: window.height / 2 - apiAddressPopup.height / 2
                    width: 450
                    height: 250
                    modal: true
                    focus: true
                    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                    ColumnLayout {
                        width: parent.width
                        spacing: 10

                        AccentText {
                            width: apiAddressPopup.width
                            text: "Адрес api"
                            fontPointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        Rectangle {
                            width: apiAddressPopup.width - 30
                            height: apiAddress.height

                            TextField {
                                id: apiAddress
                                width: parent.width
                                placeholderText: "Введите url"
                            }
                        }


                        AccentText {
                            width: apiAddressPopup.width
                            text: "Адрес статики"
                            fontPointSize: 12
                            font.bold: true
                            elide: Text.ElideRight
                        }

                        Rectangle {
                            width: apiAddressPopup.width - 30
                            height: apiAddress.height

                            TextField {
                                id: staticAddress
                                width: parent.width
                                placeholderText: "Введите url"
                            }
                        }

                        Rectangle {
                            color: "transparent"
                            width: apiAddressPopup.width - 20
                            height: 70

                            RoundedActionButton {
                                anchors.right: saveButton.left
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                visible: !apiServiceConfigurator.isDefault
                                text: "Умолчание"
                                width: 100
                                onClicked: {
                                    apiServiceConfigurator.restoreDefault();

                                    apiAddressPopup.close();
                                }
                            }

                            RoundedActionButton {
                                id: saveButton
                                anchors.right: cancelButton.left
                                anchors.rightMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                enabled: apiAddress.text !== `` && staticAddress.text !== ``
                                text: "Сохранить"
                                width: 100
                                onClicked: {
                                    apiServiceConfigurator.saveApiConfiguration(apiAddress.text, staticAddress.text);

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

                Popup {
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

                        Rectangle {
                            width: proxyPopup.width - 30
                            height: proxyType.height

                            ComboBox {
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

                        Rectangle {
                            width: proxyPopup.width - 30
                            height: proxyAddress.height

                            TextField {
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

                        Rectangle {
                            width: proxyPopup.width - 30
                            height: proxyPort.height

                            TextField {
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

                        Rectangle {
                            width: proxyPopup.width - 30
                            height: proxyUsername.height

                            TextField {
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

                            TextField {
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
}

