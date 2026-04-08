import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Controls"

Page {
    id: rootPage
    anchors.fill: parent
    property int page: 0
    property var allItemsPage

    signal navigateFrom()
    signal navigateTo()

    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
    }

    Loader {
        anchors.fill: parent
        sourceComponent: rootPage.visible ? applicationPageContent : null
    }

    Component {
        id: applicationPageContent

        Item {
            anchors.fill: parent

            Image {
                id: backgroundFile
                asynchronous: true
                visible: applicationThemeViewModel.applicationsPageBackground.activated
                fillMode: applicationThemeViewModel.applicationsPageBackground.activated ? applicationThemeViewModel.applicationsPageBackground.imageMode : Image.Pad
                source: applicationThemeViewModel.applicationsPageBackground.activated ? applicationThemeViewModel.applicationsPageBackground.url : ''
                opacity: applicationThemeViewModel.applicationsPageBackground.activated ? applicationThemeViewModel.applicationsPageBackground.opacity / 100 : 1
                horizontalAlignment: applicationThemeViewModel.applicationsPageBackground.activated ? applicationThemeViewModel.applicationsPageBackground.halign : Image.AlignLeft
                verticalAlignment: applicationThemeViewModel.applicationsPageBackground.activated ? applicationThemeViewModel.applicationsPageBackground.valign : Image.AlignTop
                width: rootPage.width
                height: rootPage.height
            }

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

                        PlainText {
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.horizontalCenter: parent.horizontalCenter
                            visible: applicationsViewModel.loading
                            fontPointSize: mainViewModel.isSmallSizeMode ? 10 : 12
                            text: "Выполняется загрузка..."
                        }

                        RoundedActionButton {
                            id: checkNewVersionsButton
                            anchors.right: parent.right
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            text: "Проверить новые версии"
                            onClicked: {
                                applicationsViewModel.checkNewVersions();
                            }
                        }
                    }

                    ListView {
                        id: scrollView
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        model: applicationsViewModel.items
                        clip: true
                        spacing: 4
                        ScrollBar.vertical: ScrollBar {
                            active: true
                        }
                        delegate: Rectangle {
                            width: scrollView.width
                            height: 170
                            color: "transparent"

                            Rectangle {
                                anchors.fill: parent
                                anchors.leftMargin: 4
                                anchors.rightMargin: 4
                                radius: 10
                                color: applicationThemeViewModel.panelBackground

                                RowLayout {
                                    anchors.fill: parent
                                    anchors.leftMargin: 4

                                    Rectangle {
                                        height: parent.height
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        color: "transparent"

                                        ColumnLayout {
                                            spacing: 2
                                            anchors.fill: parent
                                            anchors.leftMargin: 10

                                            Item {
                                                Layout.fillHeight: true
                                            }

                                            AccentText {
                                                fontPointSize: 10
                                                text: modelData.applicationName
                                                Layout.fillWidth: true
                                                maximumLineCount: 2
                                                elide: Text.ElideRight
                                                wrapMode: Text.Wrap
                                            }

                                            PlainText {
                                                Layout.topMargin: 8
                                                Layout.bottomMargin: 4
                                                Layout.fillWidth: true
                                                Layout.rightMargin: 30
                                                fontPointSize: 8
                                                text: modelData.description
                                                maximumLineCount: 2
                                                elide: Text.ElideRight
                                                wrapMode: Text.Wrap
                                            }

                                            PlainText {
                                                Layout.topMargin: 2
                                                fontPointSize: 9
                                                text: modelData.isInstalled ? "Установлено" : "Не установлено"
                                            }

                                            AccentText {
                                                Layout.topMargin: 2
                                                fontPointSize: 9
                                                visible: modelData.isHaveNewVersion
                                                text: modelData.isHaveNewVersion ? "Можно установить новую версию!" : ""
                                            }

                                            PlainText {
                                                Layout.topMargin: 2
                                                fontPointSize: 9
                                                visible: modelData.includedInDistributive
                                                text: "Является частью дистрибутива и не требует установки"
                                            }

                                            PlainText {
                                                Layout.topMargin: 2
                                                Layout.fillWidth: true
                                                fontPointSize: 9
                                                visible: modelData.isInstalled && !modelData.includedInDistributive
                                                text: "Путь установки: " + modelData.installedPath
                                                maximumLineCount: 2
                                                elide: Text.ElideRight
                                                wrapMode: Text.Wrap
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
                                        height: parent.height
                                        color: "transparent"

                                        Column {
                                            anchors.centerIn: parent

                                            FilterPanelIconButton {
                                                visible: !modelData.isInstalled || modelData.isHaveNewVersion
                                                iconPath: applicationThemeViewModel.currentItems.iconDownloadTheme
                                                tooltipMessage: !modelData.isInstalled ? "Установить приложение" : "Обновить приложение"
                                                onButtonPressed: {
                                                    if (applicationsViewModel.loading) return;

                                                    if (!modelData.isInstalled) {
                                                        applicationsViewModel.installIndex = modelData.applicationName;
                                                        installPathTextField.text = "";
                                                        installPopup.open();
                                                    } else {
                                                        applicationsViewModel.installIndex = modelData.applicationName;
                                                        applicationsViewModel.installByIndex();
                                                    }
                                                }
                                            }
                                            FilterPanelIconButton {
                                                visible: modelData.isInstalled && !modelData.includedInDistributive
                                                iconPath: applicationThemeViewModel.currentItems.iconDeleteItem
                                                tooltipMessage: "Удалить приложение"
                                                onButtonPressed: {
                                                    if (applicationsViewModel.loading) return;

                                                    applicationsViewModel.deleteByIndex(modelData.applicationName);
                                                }
                                            }
                                        }
                                    }
                                }
                            }                            
                        }
                    }                    
                }

                Component.onCompleted: {
                    applicationsViewModel.checkNewVersions(); // refresh versions after user open page
                }
            }

            Item {
                id: linuxReminder
                visible: applicationsViewModel.lastInstallPath.length
                anchors.fill: parent

                Rectangle {
                    width: parent.width - 80
                    height: 180
                    anchors.centerIn: parent
                    color: applicationThemeViewModel.pageUpperPanel
                    radius: 8
                }

                PlainText {
                    anchors.centerIn: parent
                    text: "Как финальный шаг, откройте консоль и введите команду ниже (кликните для копирования):\n" +
                        (applicationsViewModel.lastInstallPathPrefix + applicationsViewModel.lastInstallPath)
                    fontPointSize: 14
                }

                MouseArea {
                    anchors.fill: parent
                    onPressed: {
                        applicationsViewModel.copyLastInstalledPath();
                        notificationViewModel.sendInfoNotification("Путь скопирован");
                    }
                }
            }

            DefaultPopup {
                id: installPopup
                x: window.width / 2 - installPopup.width / 2
                y: window.height / 2 - installPopup.height / 2
                width: 550
                height: 140
                modal: true
                focus: true
                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                ColumnLayout {
                    width: parent.width
                    spacing: 10

                    AccentText {
                        width: installPopup.width
                        text: "Путь к папке установки"
                        fontPointSize: 12
                        font.bold: true
                        elide: Text.ElideRight
                    }

                    Item {
                        width: installPopup.width - 30
                        height: installPathTextField.height

                        CommonTextField {
                            id: installPathTextField
                            anchors.left: parent.left
                            width: parent.width - 45
                            placeholderText: "Введите полный путь"
                        }

                        IconButton {
                            id: selectFolderButton
                            anchors.right: parent.right
                            height: 40
                            width: 40
                            hoverColor: applicationThemeViewModel.currentItems.filterIconButtonHoverColor
                            iconPath: applicationThemeViewModel.currentItems.iconReleaseCatalogCompilation
                            iconWidth: 24
                            iconHeight: 24
                            tooltipMessage: "Выбрать папку в проводнике"
                            onButtonPressed: {
                                selectFolderForSave.open();
                            }
                        }
                    }

                    Item {
                        width: installPopup.width - 20
                        height: 40

                        RoundedActionButton {
                            id: torrentStreamSaveButton
                            anchors.right: torrentStreamCancelButton.left
                            anchors.rightMargin: 10
                            anchors.verticalCenter: parent.verticalCenter
                            buttonEnabled: !!installPathTextField.text
                            text: "Установить"
                            width: 110
                            onClicked: {
                                if (!installPathTextField.text) return;

                                applicationsViewModel.installPath = installPathTextField.text;
                                applicationsViewModel.installByIndex();
                                applicationsViewModel.clearInstallData();
                                installPopup.close();
                            }
                        }

                        RoundedActionButton {
                            id: torrentStreamCancelButton
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            text: "Отмена"
                            width: 110
                            onClicked: {
                                installPathTextField.text = "";
                                applicationsViewModel.clearInstallData();
                                installPopup.close();
                            }
                        }
                    }
                }
            }

            SystemOpenFolderDialog {
                id: selectFolderForSave
                title: "Выберите папку для сохранения "
                onNeedOpenFolder: {
                    if (Qt.platform.os === 'windows') {
                        installPathTextField.text = folderUrl.replace("file:///", "");
                    } else {
                        installPathTextField.text = folderUrl.replace("file://", "");
                    }
                }
            }
        }
    }
}

