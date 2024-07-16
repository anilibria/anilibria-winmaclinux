import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.15
import "../Controls"

Page {
    id: authorizePage
    anchors.fill: parent
    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
    }

    signal navigateFrom()
    signal navigateTo()

    onNavigateTo: {
        authorizationViewModel.resetModel();
    }

    RowLayout {
        id: panelContainer
        anchors.fill: parent
        spacing: 0
        Rectangle {
            color: "transparent"
            Layout.fillHeight: true
            Layout.fillWidth: true

            Image {
                anchors.fill: parent
                source: assetsLocation.backgroundsPath + "authbackground.jpg"
                fillMode: Image.PreserveAspectCrop
            }

            Rectangle {
                id: fieldsContainer
                radius: 8
                color: applicationThemeViewModel.panelBackground
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter
                width: 450
                height: 200

                ColumnLayout {
                    id: authForm
                    anchors.fill: parent
                    anchors.margins: 8
                    CommonTextField {
                        id: emailTextBox
                        Layout.fillWidth: true
                        Layout.topMargin: 8
                        placeholderText: "Логин"
                        text: authorizationViewModel.login
                        onTextChanged: {
                            authorizationViewModel.login = text;
                        }
                    }
                    CommonTextField {
                        id: passwordTextBox
                        Layout.fillWidth: true
                        Layout.topMargin: 8
                        echoMode: "PasswordEchoOnEdit"
                        placeholderText: "Пароль"
                        text: authorizationViewModel.password
                        onTextChanged: {
                            authorizationViewModel.password = text;
                        }
                    }
                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                    Item {
                        Layout.fillWidth: true
                        height: 40

                        IconButton {
                            id: vkButton
                            anchors.left: parent.left
                            anchors.leftMargin: 1
                            hoverColor: applicationThemeViewModel.currentItems.filterIconButtonHoverColor
                            iconPath: applicationThemeViewModel.currentItems.iconAuthorizationVk
                            height: 34
                            width: 40
                            iconWidth: 20
                            iconHeight: 20
                            tooltipMessage: "Авторизация через ВКонтакте"
                            onButtonPressed: {
                                synchronizationServicev2.authorizeSocial("vk");
                            }
                        }
                        IconButton {
                            id: googleButton
                            anchors.left: vkButton.right
                            anchors.leftMargin: 2
                            hoverColor: applicationThemeViewModel.currentItems.filterIconButtonHoverColor
                            iconPath: applicationThemeViewModel.currentItems.iconAuthorizationGoogle
                            height: 34
                            width: 40
                            iconWidth: 20
                            iconHeight: 20
                            tooltipMessage: "Авторизация через Google"
                            onButtonPressed: {
                                synchronizationServicev2.authorizeSocial("google");
                            }
                        }
                        IconButton {
                            id: patreonButton
                            anchors.left: googleButton.right
                            anchors.leftMargin: 1
                            hoverColor: applicationThemeViewModel.currentItems.filterIconButtonHoverColor
                            iconPath: applicationThemeViewModel.currentItems.iconAuthorizationPatreon
                            height: 34
                            width: 40
                            iconWidth: 20
                            iconHeight: 20
                            tooltipMessage: "Авторизация через Patreon"
                            onButtonPressed: {
                                synchronizationServicev2.authorizeSocial("patreon");
                            }
                        }
                        IconButton {
                            id: discordButton
                            anchors.left: patreonButton.right
                            anchors.leftMargin: 1
                            hoverColor: applicationThemeViewModel.currentItems.filterIconButtonHoverColor
                            iconPath: applicationThemeViewModel.currentItems.iconAuthorizationDiscord
                            height: 34
                            width: 40
                            iconWidth: 20
                            iconHeight: 20
                            tooltipMessage: "Авторизация через Discord"
                            onButtonPressed: {
                                synchronizationServicev2.authorizeSocial("discord");
                            }
                        }

                        RoundedActionButton {
                            id: saveButton
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            text: "Войти"
                            onClicked: {
                                synchronizationServicev2.authorize(authorizationViewModel.login, authorizationViewModel.password);
                            }
                        }

                        LinkedText {
                            visible: authorizationViewModel.errorMessage.length === 0
                            anchors.top: saveButton.top
                            anchors.topMargin: 10
                            anchors.right: saveButton.left
                            anchors.rightMargin: 6
                            fontPointSize: 10
                            text: authorizationViewModel.registerLink
                        }
                    }
                }
            }
        }
    }

    Item {
        visible: synchronizationServicev2.isSocialAuthentification
        anchors.fill: parent

        MouseArea {
            anchors.fill: parent
            onPressed: {
                mouse.accepted = true;
            }
        }

        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: .5
        }

        Rectangle {
            width: 80
            height: 80
            color: "white"
            radius: 20
            opacity: 0.8
            anchors.centerIn: parent
        }

        AnimatedImage {
            id: spinner
            anchors.centerIn: parent
            paused: !synchronizationServicev2.isSocialAuthentification
            playing: synchronizationServicev2.isSocialAuthentification
            source: synchronizationServicev2.isSocialAuthentification ? "../Assets/Icons/spinner.gif" : ""
        }
    }
}
