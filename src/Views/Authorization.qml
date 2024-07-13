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
                width: 400
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
                        AccentText {
                            text: authorizationViewModel.errorMessage
                            fontPointSize: 10
                            anchors.left: parent.left
                            anchors.leftMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            wrapMode: Text.WordWrap
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
}
