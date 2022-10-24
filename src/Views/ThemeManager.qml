import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
import "../Controls"

Page {
    id: root
    anchors.fill: parent
    background: Rectangle {
        color: applicationThemeViewModel.pageBackground
    }

    signal navigateFrom()
    signal navigateTo()

    onVisibleChanged: {
        if (!visible) return;

        if (!applicationThemeViewModel.service.firstLoaded) applicationThemeViewModel.service.loadThemes(applicationThemeViewModel.basedOnDark);
    }

    RowLayout {
        id: panelContainer
        anchors.fill: parent
        spacing: 0
        Rectangle {
            color: applicationThemeViewModel.pageVerticalPanel
            width: 40
            Layout.fillHeight: true
            Column {
                IconButton {
                    height: 45
                    width: 40
                    overlayVisible: false
                    iconPath: assetsLocation.iconsPath + "coloreddrawer.svg"
                    iconWidth: 28
                    iconHeight: 28
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
                color: applicationThemeViewModel.pageUpperPanel

                PlainText {
                    id: displaySection
                    text: applicationThemeViewModel.selectedMenuItemName
                    anchors.centerIn: parent
                    fontPointSize: 12
                }

                IconButton {
                    id: menuButton
                    anchors.right: parent.right
                    anchors.rightMargin: 4
                    anchors.verticalCenter: parent.verticalCenter
                    height: 26
                    width: 26
                    overlayVisible: false
                    hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                    iconWidth: 22
                    iconHeight: 22
                    iconPath: assetsLocation.iconsPath + "allreleases.svg"
                    onButtonPressed: {
                        managerMenu.open();
                    }

                    CommonMenu {
                        id: managerMenu
                        width: 330

                        Repeater {
                            model: applicationThemeViewModel.menuItems

                            CommonMenuItem {
                                text: modelData
                                onPressed: {
                                    managerMenu.close();
                                    applicationThemeViewModel.selectedMenuItem = index;
                                }
                            }
                        }
                    }
                }
            }

            Rectangle {
                color: "transparent"
                Layout.fillHeight: true
                Layout.fillWidth: true

                Item {
                    visible: applicationThemeViewModel.selectedMenuItem === 0
                    anchors.fill: parent

                    Text {
                        text: "Установленные локально"
                    }
                }

                Item {
                    visible: applicationThemeViewModel.selectedMenuItem === 1
                    anchors.fill: parent

                    ListView {
                        id: externalThemes
                        spacing: 4
                        visible: !applicationThemeViewModel.service.loading && !applicationThemeViewModel.externalThemes.listIsEmpty
                        anchors.fill: parent
                        model: applicationThemeViewModel.externalThemes
                        delegate: Item {
                            width: externalThemes.width
                            height: 200

                            Rectangle {
                                anchors.fill: parent
                                anchors.leftMargin: 4
                                anchors.rightMargin: 4
                                radius: 10
                                color: applicationThemeViewModel.panelBackground
                            }

                            RowLayout {
                                anchors.fill: parent

                                Item {
                                    Layout.preferredWidth: 300
                                    Layout.preferredHeight: 200

                                    Image {
                                        anchors.verticalCenter: parent.verticalCenter
                                        width: 300
                                        height: 190
                                        fillMode: Image.PreserveAspectFit
                                        mipmap: true
                                        source: screenshoots
                                    }
                                }
                                Item {
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 200

                                    Column {
                                        anchors.verticalCenter: parent.verticalCenter
                                        height: titleTheme.height + authorTheme.height

                                        AccentText {
                                            id: titleTheme
                                            fontPointSize: 12
                                            text: title
                                        }

                                        Item {
                                            width: 1
                                            height: 4
                                        }

                                        PlainText {
                                            id: authorTheme
                                            fontPointSize: 10
                                            text: "Автор: " + author
                                        }

                                        Item {
                                            width: 1
                                            height: 2
                                        }

                                        PlainText {
                                            fontPointSize: 10
                                            text: "Версия: " + version
                                        }

                                        Item {
                                            width: 1
                                            height: 2
                                        }

                                        PlainText {
                                            fontPointSize: 10
                                            text: "Дата обновления: " + lastUpdated
                                        }
                                    }

                                    Column {
                                        width: 50
                                        height: 48
                                        anchors.right: parent.right
                                        anchors.rightMargin: 5
                                        anchors.verticalCenter: parent.verticalCenter

                                        FilterPanelIconButton {
                                            iconPath: assetsLocation.iconsPath + "downloadtheme.svg"
                                            overlayVisible: false
                                            tooltipMessage: "Установить тему"
                                            onButtonPressed: {
                                                applicationThemeViewModel.importThemeFromExternal(themeIndex);
                                            }
                                        }
                                        /*FilterPanelIconButton {
                                            iconPath: assetsLocation.iconsPath + "updated.svg"
                                            overlayVisible: false
                                            tooltipMessage: "Обновить тему"
                                            onButtonPressed: {

                                            }
                                        }*/
                                        FilterPanelIconButton {
                                            iconPath: assetsLocation.iconsPath + "delete.svg"
                                            overlayVisible: false
                                            tooltipMessage: "Удалить тему"
                                            onButtonPressed: {

                                            }
                                        }
                                    }

                                }
                            }
                        }
                    }

                    Item {
                        visible: !applicationThemeViewModel.service.loading && applicationThemeViewModel.externalThemes.listIsEmpty
                        anchors.centerIn: parent
                        width: 200
                        height: 200

                        Image {
                            id: emptyExternalItems
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: assetsLocation.iconsPath + "emptybox.svg"
                            width: 80
                            height: 80
                            mipmap: true
                        }
                        PlainText {
                            anchors.top: emptyExternalItems.bottom
                            width: parent.width
                            height: 80
                            fontPointSize: 10
                            text: "Не найдено тем по текущему фильтру"
                            horizontalAlignment: Text.AlignHCenter
                            wrapMode: Text.Wrap
                            maximumLineCount: 2
                        }
                    }

                    Item {
                        visible: applicationThemeViewModel.service.loading
                        anchors.centerIn: parent
                        width: 200
                        height: 200

                        Image {
                            id: emptyExternalSpinner
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: assetsLocation.iconsPath + "spinner.gif"
                            width: 80
                            height: 80
                            mipmap: true
                        }
                        PlainText {
                            anchors.top: emptyExternalSpinner.bottom
                            fontPointSize: 10
                            text: "Получаем темы..."
                            maximumLineCount: 2
                            elide: Text.ElideRight
                        }
                    }
                }

                Item {
                    visible: applicationThemeViewModel.selectedMenuItem === 2
                    anchors.fill: parent

                    ColumnLayout {
                        anchors.fill: parent
                        spacing: 0

                        Item {
                            Layout.fillWidth: true
                            Layout.preferredHeight: 45

                            PlainText {
                                anchors.left: parent.left
                                anchors.leftMargin: 4
                                anchors.verticalCenter: parent.verticalCenter
                                fontPointSize: 11
                                text: applicationThemeViewModel.fieldList.editMode
                            }
                        }
                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            ListView {
                                id: fieldsListView
                                anchors.fill: parent
                                clip: true
                                model: applicationThemeViewModel.fieldList
                                ScrollBar.vertical: ScrollBar {
                                    active: true
                                }
                                delegate: Item {
                                    width: fieldsListView.width
                                    height: 90

                                    Item {
                                        anchors.centerIn: parent
                                        width: 500
                                        height: parent.height

                                        PlainText {
                                            id: titleText
                                            width: 240
                                            height: parent.height
                                            fontPointSize: 9
                                            maximumLineCount: 2
                                            elide: Text.ElideRight
                                            wrapMode: Text.WordWrap
                                            horizontalAlignment: Text.AlignRight
                                            verticalAlignment: Text.AlignVCenter
                                            text: title
                                        }

                                        Item {
                                            anchors.right: titleText.left

                                            Popup {
                                                id: informationPopup
                                                x: -width
                                                width: 280
                                                height: 75
                                                modal: false
                                                closePolicy: Popup.NoAutoClose

                                                PlainText {
                                                    width: parent.width
                                                    height: parent.height
                                                    fontPointSize: 9
                                                    maximumLineCount: 4
                                                    wrapMode: Text.WordWrap
                                                    elide: Text.ElideRight
                                                    horizontalAlignment: Text.AlignJustify
                                                    verticalAlignment: Text.AlignVCenter
                                                    text: description
                                                }
                                            }
                                        }

                                        FilterPanelIconButton {
                                            id: helpButton
                                            width: 20
                                            height: 20
                                            iconWidth: 18
                                            iconHeight: 18
                                            anchors.left: titleText.right
                                            anchors.leftMargin: 10
                                            anchors.verticalCenter: parent.verticalCenter
                                            iconPath: assetsLocation.iconsPath + "information.svg"
                                            overlayVisible: false
                                            tooltipMessage: "Нажмите чтобы получить больше информации о поле"
                                            onButtonPressed: {
                                                if (informationPopup.opened) {
                                                    informationPopup.close();
                                                } else {
                                                    informationPopup.open();
                                                }
                                            }
                                        }

                                        TextField {
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.left: helpButton.right
                                            anchors.leftMargin: 10
                                            selectByMouse: true
                                            width: 210
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

    ColorDialog {
        id: colorDialog
        title: "Выберите цвет из палитры"
        showAlphaChannel: true
        onAccepted: {
            console.log("You chose: " + colorDialog.color)
        }
        onRejected: {
            console.log("Canceled")
        }
    }
}
