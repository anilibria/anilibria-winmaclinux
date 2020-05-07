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
import QtQuick.Controls.Material 2.0
import "../Controls"
import "../Theme"

Page {
    id: authorizePage
    anchors.fill: parent
    background: Rectangle {
        color: ApplicationTheme.pageBackground
    }

    property alias email: emailTextBox.text
    property alias password: passwordTextBox.text
    property alias fa2code: fa2codeTextBox.text

    signal navigateFrom()
    signal navigateTo()
    signal authentificateFailed(string message)

    onNavigateFrom: {
        emailTextBox.text = "";
        passwordTextBox.text = "";
        fa2codeTextBox.text = "";
    }

    onAuthentificateFailed: {
        errorMessage.text = message;
    }

    RowLayout {
        id: panelContainer
        anchors.fill: parent
        spacing: 0
        Rectangle {
            color: ApplicationTheme.pageVerticalPanel
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
            }
        }
        Rectangle {
            color: "transparent"
            Layout.fillHeight: true
            Layout.fillWidth: true

            Rectangle {
                id: fieldsContainer
                radius: 8
                color: ApplicationTheme.panelBackground
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                width: 400
                height: 280

                ColumnLayout {
                    id: authForm
                    anchors.fill: parent
                    anchors.margins: 8
                    TextField {
                        id: emailTextBox
                        Layout.fillWidth: true
                        placeholderText: "Логин"
                    }
                    TextField {
                        id: passwordTextBox
                        Layout.fillWidth: true
                        echoMode: "PasswordEchoOnEdit"
                        placeholderText: "Пароль"
                    }
                    TextField {
                        id: fa2codeTextBox
                        Layout.fillWidth: true
                        placeholderText: "2fa код (оставить пустым если не настроено)"
                    }
                    Item {
                        Layout.fillWidth: true
                        height: 40
                        AccentText {
                            id: errorMessage
                            text: ""
                            fontPointSize: 10
                            anchors.left: parent.left
                            anchors.leftMargin: 10                            
                            anchors.verticalCenter: parent.verticalCenter
                            wrapMode: Text.WordWrap
                        }

                        RoundedActionButton {
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            text: qsTr("Войти")
                            onClicked: {
                                errorMessage.text = "";

                                if (emailTextBox.text === "") {
                                    errorMessage.text = "Поле Логин обязательное";
                                    return;
                                }
                                if (passwordTextBox.text === "") {
                                    errorMessage.text = "Поле Пароль обязательное";
                                    return;
                                }

                                synchronizationService.authorize(encodeURIComponent(authorizePage.email), encodeURIComponent(authorizePage.password), encodeURIComponent(authorizePage.fa2code));
                            }
                        }

                    }

                }
            }

            LinkedText {
                anchors.top: fieldsContainer.bottom
                anchors.right: fieldsContainer.right
                fontPointSize: 10
                text: "<a href='https://www.anilibria.tv/pages/login.php'>Регистрация</a>"
            }
        }
    }

}
