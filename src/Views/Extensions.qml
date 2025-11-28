import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import "../Controls"

Page {
    id: root
    anchors.fill: parent

    background: Rectangle {
        color: applicationThemeViewModel.currentItems.pageBackground
    }

    Loader {
        anchors.fill: parent
        sourceComponent: root.visible ? extensionsPageContent : null
    }

    Component {
        id: extensionsPageContent

        Item {
            anchors.fill: parent

            Image {
                id: backgroundFile
                asynchronous: true
                visible: applicationThemeViewModel.extensionsPageBackground.activated
                fillMode: applicationThemeViewModel.extensionsPageBackground.activated ? applicationThemeViewModel.extensionsPageBackground.imageMode : Image.Pad
                source: applicationThemeViewModel.extensionsPageBackground.activated ? applicationThemeViewModel.extensionsPageBackground.url : ''
                opacity: applicationThemeViewModel.extensionsPageBackground.activated ? applicationThemeViewModel.extensionsPageBackground.opacity / 100 : 1
                horizontalAlignment: applicationThemeViewModel.extensionsPageBackground.activated ? applicationThemeViewModel.extensionsPageBackground.halign : Image.AlignLeft
                verticalAlignment: applicationThemeViewModel.extensionsPageBackground.activated ? applicationThemeViewModel.extensionsPageBackground.valign : Image.AlignTop
                width: root.width
                height: root.height
            }

            RowLayout {
                id: panelContainer
                anchors.fill: parent
                spacing: 0

                Rectangle {
                    color: applicationThemeViewModel.currentItems.pageVerticalPanel
                    width: 40
                    Layout.fillHeight: true
                    Column {
                        LeftPanelIconButton {
                            iconPath: applicationThemeViewModel.currentItems.iconInfo
                            iconWidth: 29
                            iconHeight: 29
                            tooltipMessage: "Открыть ссылки на обучающие материалы"
                            onButtonPressed: {
                                informationPopup.open();
                            }

                            DefaultPopup {
                                id: informationPopup
                                x: 40
                                y: parent.height - 10
                                width: 410
                                height: 80
                                modal: true
                                focus: true
                                closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                                Column {
                                    LinkedText {
                                        fontPointSize: 11
                                        text: "<a href='https://github.com/anilibria/anilibria-winmaclinux/blob/master/extensions.md'>Как создавать расширения для приложения AniLiberty.Qt</a>"
                                    }
                                }
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
                        color: applicationThemeViewModel.currentItems.pageUpperPanel

                        AccentText {
                            height: parent.height
                            anchors.left: parent.left
                            anchors.leftMargin: 4
                            anchors.right: openGlobalVariablesButton.left
                            anchors.rightMargin: 4
                            fontPointSize: 8
                            maximumLineCount: 2
                            verticalAlignment: Text.AlignVCenter
                            wrapMode: Text.WordWrap
                            elide: Text.ElideRight
                            text: "Автор приложения не несет ответственности за любые последствия запуска расширений. Если Вы не уверены или видите подозрительные действия то лучше удалить расширение(я)."
                        }

                        RoundedActionButton {
                            id: openGlobalVariablesButton
                            anchors.right: deleteSelectedButton.left
                            anchors.verticalCenter: parent.verticalCenter
                            width: 210
                            textSize: 10
                            text: "Переменные"
                            onClicked: {
                                mainViewModel.selectPage("extensions:globalvariables");
                            }
                        }

                        RoundedActionButton {
                            id: deleteSelectedButton
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            width: 210
                            textSize: 10
                            text: "Добавить расширение"
                            onClicked: {
                                selectFolderExtensionDialog.open();
                            }
                        }
                    }

                    Item {
                        id: scrollView
                        Layout.fillHeight: true
                        Layout.fillWidth: true

                        ListView {
                            id: extensionsListView
                            anchors.fill: parent
                            boundsBehavior: Flickable.StopAtBounds
                            model: extensionsViewModel.displayedExtensions
                            clip: true
                            delegate: Item {
                                id: itemRoot
                                width: extensionsListView.width
                                height: 150

                                property string modelIndentifier: modelData.indentifier

                                Rectangle {
                                    id: itemContainer
                                    anchors.verticalCenter: parent.verticalCenter
                                    anchors.horizontalCenter: parent.horizontalCenter
                                    width: extensionsListView.width - 10
                                    height: parent.height - 10
                                    radius: 10
                                    color: applicationThemeViewModel.currentItems.panelBackground

                                    PlainText {
                                        width: itemContainer.width - 80
                                        height: itemContainer.height - 10
                                        fontPointSize: 11
                                        anchors.left: parent.left
                                        anchors.leftMargin: 10
                                        anchors.verticalCenter: parent.verticalCenter
                                        verticalAlignment: Text.AlignVCenter
                                        maximumLineCount: 4
                                        text: modelData.extensionTitle + (modelData.author ? "\n" + modelData.author : "")
                                    }

                                    Column {
                                        id: elements
                                        anchors.verticalCenter: parent.verticalCenter
                                        width: 100
                                        anchors.right: parent.right
                                        anchors.rightMargin: 10
                                        spacing: 3

                                        IconButton {
                                            anchors.right: parent.right
                                            anchors.rightMargin: 26
                                            width: 30
                                            height: 30
                                            iconWidth: 20
                                            iconHeight: 20
                                            hoverColor: applicationThemeViewModel.currentItems.filterIconButtonHoverColor
                                            iconPath: applicationThemeViewModel.currentItems.iconContextMenu
                                            onButtonPressed: {
                                                extensionsContextMenu.open();
                                            }

                                            CommonMenu {
                                                id: extensionsContextMenu
                                                autoWidth: true

                                                Repeater {
                                                    model: modelData.menuItems
                                                    delegate: CommonMenuItem {
                                                        text: modelData
                                                        onPressed: {
                                                            extensionsViewModel.runMenuCommand(itemRoot.modelIndentifier, index);
                                                            extensionsContextMenu.close();
                                                        }
                                                    }
                                                }
                                            }
                                        }
                                        IconButton {
                                            anchors.right: parent.right
                                            anchors.rightMargin: 26
                                            width: 30
                                            height: 30
                                            iconWidth: 20
                                            iconHeight: 20
                                            hoverColor: applicationThemeViewModel.currentItems.filterIconButtonHoverColor
                                            iconPath: applicationThemeViewModel.currentItems.iconDeleteItem
                                            tooltipMessage: "Удалить расширение из приложения"
                                            onButtonPressed: {
                                                extensionDeleteMessage.open();

                                                extensionsViewModel.deleteExtension(itemRoot.modelIndentifier);                                                
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            ExtensionsCard {
                id: extensionsCard
            }

            MessageModal {
                id: extensionDeleteMessage
                header: "Расширение удалено"
                message: "Запущенное расширение будет продолжать работать до перезапуска приложения"
                content: Row {
                    spacing: 6
                    anchors.right: parent.right

                    RoundedActionButton {
                        text: "Закрыть"
                        width: 100
                        onClicked: {
                            extensionDeleteMessage.close();
                        }
                    }
                }
            }

            SystemOpenFolderDialog {
                id: selectFolderExtensionDialog
                title: "Выберите папку с расширением"
                onNeedOpenFolder: {
                    extensionsViewModel.addExtension(folderUrl);
                }
            }
        }
    }
}
