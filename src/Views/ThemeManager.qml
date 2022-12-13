import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3
import QtGraphicalEffects 1.0
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
                    iconPath: applicationThemeViewModel.iconMainMenu
                    iconWidth: 28
                    iconHeight: 28
                    onButtonPressed: {
                        drawer.open();
                    }
                }
                LeftPanelIconButton {
                    iconPath: assetsLocation.iconsPath + "information.svg"
                    overlayVisible: false
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
                        width: 320
                        height: 80
                        modal: true
                        focus: true
                        closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

                        Column {
                            LinkedText {
                                fontPointSize: 11
                                text: "<a href='https://github.com/anilibria/anilibria-winmaclinux/blob/master/createtheme.md#%D0%BA%D0%B0%D0%BA-%D1%81%D0%BE%D0%B7%D0%B4%D0%B0%D1%82%D1%8C-%D1%82%D0%B5%D0%BC%D1%83-%D0%B4%D0%BB%D1%8F-%D0%BF%D1%80%D0%B8%D0%BB%D0%BE%D0%B6%D0%B5%D0%BD%D0%B8%D1%8F'>Как создать тему для приложения</a>"
                            }
                            LinkedText {
                                fontPointSize: 11
                                text: "<a href='https://t.me/desktopclientanilibria/344'>Как создать тему (видеоинструкция)</a>"
                            }
                            LinkedText {
                                fontPointSize: 11
                                text: "<a href='https://github.com/anilibria/anilibria-winmaclinux/blob/master/sharetheme.md#%D0%BA%D0%B0%D0%BA-%D0%BF%D0%BE%D0%B4%D0%B5%D0%BB%D0%B8%D1%82%D1%8C%D1%81%D1%8F-%D1%81%D0%B2%D0%BE%D0%B5%D0%B9-%D1%82%D0%B5%D0%BC%D0%BE%D0%B9-%D1%81-%D0%B4%D1%80%D1%83%D0%B3%D0%B8%D0%BC%D0%B8'>Как поделиться темой с другими</a>"
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

                    Item {
                        id: localUpperPanel
                        width: parent.width
                        height: 45

                        RoundedActionButton {
                            id: importThemeFromFileButton
                            anchors.right: parent.right
                            anchors.verticalCenter: parent.verticalCenter
                            width: 200
                            textSize: 10
                            text: "Импорт темы из файла"
                            onClicked: {
                                openThemeToFileDialog.open();
                            }
                        }
                    }

                    ListView {
                        id: localThemes
                        spacing: 4
                        visible: !applicationThemeViewModel.localThemes.listIsEmpty
                        anchors.top: localUpperPanel.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        model: applicationThemeViewModel.localThemes
                        delegate: Item {
                            width: localThemes.width
                            height: 100

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
                                    Layout.fillWidth: true
                                    Layout.preferredHeight: 100

                                    AccentText {
                                        id: localTitleTheme
                                        anchors.verticalCenter: parent.verticalCenter
                                        anchors.left: parent.left
                                        anchors.leftMargin: 30
                                        fontPointSize: 12
                                        text: title
                                    }

                                    Column {
                                        width: 50
                                        height: (deleteLocalButton.visible ? 24 : 0 )
                                        anchors.right: parent.right
                                        anchors.rightMargin: 5
                                        anchors.verticalCenter: parent.verticalCenter

                                        FilterPanelIconButton {
                                            id: deleteLocalButton
                                            iconPath: assetsLocation.iconsPath + "delete.svg"
                                            overlayVisible: false
                                            tooltipMessage: "Удалить тему"
                                            onButtonPressed: {
                                                deleteLocalThemeConfirm.themeName = title;
                                                deleteLocalThemeConfirm.open();
                                            }
                                        }
                                    }

                                }
                            }
                        }
                    }

                    Item {
                        visible: applicationThemeViewModel.localThemes.listIsEmpty
                        anchors.centerIn: parent
                        width: 200
                        height: 200

                        Image {
                            id: emptyLocalItems
                            anchors.horizontalCenter: parent.horizontalCenter
                            source: assetsLocation.iconsPath + "emptybox.svg"
                            width: 80
                            height: 80
                            mipmap: true
                        }

                        PlainText {
                            anchors.top: emptyLocalItems.bottom
                            width: parent.width
                            height: 80
                            fontPointSize: 10
                            text: "Не найдено тем по текущему фильтру"
                            horizontalAlignment: Text.AlignHCenter
                            wrapMode: Text.Wrap
                            maximumLineCount: 2
                        }
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
                                        height: titleTheme.height + authorTheme.height + versionTheme.height + updatedTheme.height

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
                                            id: versionTheme
                                            fontPointSize: 10
                                            text: "Версия: " + version
                                        }

                                        Item {
                                            width: 1
                                            height: 2
                                        }

                                        PlainText {
                                            id: updatedTheme
                                            fontPointSize: 10
                                            text: "Дата обновления: " + lastUpdated
                                        }
                                    }

                                    Column {
                                        width: 50
                                        height: (downloadButton.visible ? 24 : 0 ) + (deleteDownloadedButton.visible ? 24 : 0 )
                                        anchors.right: parent.right
                                        anchors.rightMargin: 5
                                        anchors.verticalCenter: parent.verticalCenter

                                        FilterPanelIconButton {
                                            id: downloadButton
                                            visible: !isDownloaded
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
                                            id: deleteDownloadedButton
                                            visible: isDownloaded
                                            iconPath: assetsLocation.iconsPath + "delete.svg"
                                            overlayVisible: false
                                            tooltipMessage: "Удалить тему"
                                            onButtonPressed: {
                                                deleteExternalThemeConfirm.externalId = externalId;
                                                deleteExternalThemeConfirm.open();
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

                            RoundedActionButton {
                                id: createEmptyButton
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                width: 200
                                textSize: 10
                                text: "Создать пустую тему"
                                onClicked: {
                                    applicationThemeViewModel.fieldList.createBlankTheme();
                                }
                            }
                            RoundedActionButton {
                                id: copyThemeButton
                                anchors.right: createEmptyButton.left
                                anchors.verticalCenter: parent.verticalCenter
                                width: 200
                                textSize: 10
                                text: "Копировать из темы"
                                onClicked: {
                                    copyThemesMenu.open();
                                }

                                CommonMenu {
                                    id: copyThemesMenu
                                    width: 320

                                    Repeater {
                                        model: applicationThemeViewModel.copyMenuItems
                                        delegate: CommonMenuItem {
                                            text: modelData
                                            onPressed: {
                                                applicationThemeViewModel.copyThemeFromInstalled(modelData);

                                                copyThemesMenu.close();
                                            }
                                        }
                                    }
                                }
                            }
                            RoundedActionButton {
                                id: saveButton
                                anchors.right: copyThemeButton.left
                                anchors.verticalCenter: parent.verticalCenter
                                width: 200
                                buttonEnabled: applicationThemeViewModel.fieldList.hasValues
                                textSize: 10
                                text: "Сохранить"
                                onClicked: {
                                    saveMenu.open();
                                }

                                CommonMenu {
                                    id: saveMenu
                                    width: 320

                                    Repeater {
                                        model: applicationThemeViewModel.fieldList.saveMenuItems
                                        delegate: CommonMenuItem {
                                            text: modelData
                                            onPressed: {
                                                switch (index) {
                                                    case 0:
                                                        applicationThemeViewModel.saveThemeAndApply();
                                                        break;
                                                    case 1:
                                                        saveThemeToFileDialog.open();
                                                        break;
                                                }

                                                saveMenu.close();
                                            }
                                        }
                                    }
                                }
                            }
                            RoundedActionButton {
                                id: previewButton
                                anchors.right: saveButton.left
                                anchors.verticalCenter: parent.verticalCenter
                                width: 200
                                buttonEnabled: applicationThemeViewModel.fieldList.hasValues
                                textSize: 10
                                text: "Предпросмотр темы"
                                onClicked: {
                                    applicationThemeViewModel.preparePreviewItems();
                                    previewWindowLoader.sourceComponent = previewWindowComponent;
                                }
                            }

                            TextField {
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                anchors.verticalCenter: parent.verticalCenter
                                width: 180
                                placeholderText: "Фильтр для полей"
                                onTextChanged: {
                                    applicationThemeViewModel.fieldList.filter = text;
                                }
                            }
                        }
                        Item {
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            Item {
                                id: headerFields
                                anchors.top: parent.top
                                anchors.left: parent.left
                                width: parent.width
                                height: 120

                                Rectangle {
                                    anchors.fill: parent
                                    anchors.leftMargin: 2
                                    anchors.rightMargin: 2
                                    anchors.topMargin: 2
                                    anchors.bottomMargin: 2
                                    radius: 10
                                    color: applicationThemeViewModel.panelBackground
                                    layer.enabled: true
                                    layer.effect: DropShadow {
                                        transparentBorder: true
                                        horizontalOffset: 2
                                        verticalOffset: 2
                                        radius: 1
                                        samples: 3
                                        color: applicationThemeViewModel.panelBackgroundShadow
                                    }
                                }

                                Item {
                                    anchors.centerIn: parent
                                    width: 800
                                    height: parent.height

                                    AccentText {
                                        id: themeNameText
                                        width: 380
                                        anchors.top: parent.top
                                        anchors.topMargin: 25
                                        fontPointSize: 10
                                        maximumLineCount: 2
                                        elide: Text.ElideRight
                                        wrapMode: Text.WordWrap
                                        horizontalAlignment: Text.AlignRight
                                        verticalAlignment: Text.AlignVCenter
                                        text: "Имя темы"
                                    }

                                    TextField {
                                        id: themeNameTextField
                                        anchors.top: parent.top
                                        anchors.topMargin: 10
                                        anchors.left: themeNameText.right
                                        anchors.leftMargin: 10
                                        width: 210
                                        selectByMouse: true
                                        text: applicationThemeViewModel.fieldList.themeName
                                        onTextChanged: {
                                            applicationThemeViewModel.fieldList.themeName = text;
                                        }
                                    }

                                    AccentText {
                                        id: basedOnThemeText
                                        anchors.top: themeNameText.bottom
                                        anchors.topMargin: 35
                                        anchors.right: themeNameText.right
                                        fontPointSize: 10
                                        width: 380
                                        horizontalAlignment: Text.AlignRight
                                        verticalAlignment: Text.AlignVCenter
                                        text: "Базовая тема"
                                    }

                                    CommonComboBox {
                                        id: basedOnThemeComboBox
                                        anchors.left: basedOnThemeText.right
                                        anchors.top: themeNameText.bottom
                                        anchors.topMargin: 20
                                        anchors.leftMargin: 10
                                        width: 180
                                        model: ListModel {
                                            ListElement {
                                                text: ""
                                            }
                                            ListElement {
                                                text: "Светлая"
                                            }
                                            ListElement {
                                                text: "Темная"
                                            }
                                        }

                                        onActivated: {
                                            applicationThemeViewModel.fieldList.basedOnTheme = currentText;
                                        }
                                    }
                                }
                            }

                            ListView {
                                id: fieldsListView
                                anchors.top: headerFields.bottom
                                anchors.bottom: parent.bottom
                                anchors.left: parent.left
                                anchors.right: parent.right
                                clip: true
                                model: applicationThemeViewModel.fieldList
                                ScrollBar.vertical: ScrollBar {
                                    active: true
                                }
                                delegate: Item {
                                    width: fieldsListView.width
                                    height: 120

                                    Rectangle {
                                        anchors.fill: parent
                                        anchors.leftMargin: 2
                                        anchors.rightMargin: 2
                                        anchors.topMargin: 2
                                        anchors.bottomMargin: 2
                                        radius: 10
                                        color: applicationThemeViewModel.panelBackground
                                        layer.enabled: true
                                        layer.effect: DropShadow {
                                            transparentBorder: true
                                            horizontalOffset: 2
                                            verticalOffset: 2
                                            radius: 1
                                            samples: 3
                                            color: applicationThemeViewModel.panelBackgroundShadow
                                        }
                                    }

                                    Item {
                                        anchors.centerIn: parent
                                        width: 800
                                        height: parent.height

                                        AccentText {
                                            id: titleText
                                            width: 380
                                            anchors.verticalCenter: parent.verticalCenter
                                            fontPointSize: 10
                                            maximumLineCount: 2
                                            elide: Text.ElideRight
                                            wrapMode: Text.WordWrap
                                            horizontalAlignment: Text.AlignRight
                                            verticalAlignment: Text.AlignVCenter
                                            text: title
                                        }

                                        PlainText {
                                            anchors.top: titleText.bottom
                                            width: 380
                                            fontPointSize: 9
                                            maximumLineCount: 4
                                            wrapMode: Text.WordWrap
                                            elide: Text.ElideRight
                                            horizontalAlignment: Text.AlignRight
                                            verticalAlignment: Text.AlignVCenter
                                            text: description
                                        }

                                        TextField {
                                            id: valueTextField
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.left: titleText.right
                                            anchors.leftMargin: 10
                                            width: 210
                                            maximumLength: 100000
                                            visible: isDefined
                                            selectByMouse: true
                                            text: fieldValue
                                            onTextChanged: {
                                                applicationThemeViewModel.fieldList.setValueToItemByIndex(identifier, text);
                                            }
                                        }

                                        PlainText {
                                            anchors.verticalCenter: parent.verticalCenter
                                            anchors.left: titleText.right
                                            anchors.leftMargin: 10
                                            fontPointSize: 9
                                            width: 210
                                            height: parent.height
                                            visible: !isDefined
                                            verticalAlignment: Text.AlignVCenter
                                            horizontalAlignment: Text.AlignHCenter
                                            text: "Не перекрыт"
                                        }

                                        FilterPanelIconButton {
                                            id: defineValueButton
                                            width: 20
                                            height: 20
                                            iconWidth: 18
                                            iconHeight: 18
                                            anchors.left: valueTextField.right
                                            anchors.leftMargin: 10
                                            anchors.verticalCenter: parent.verticalCenter
                                            iconPath: assetsLocation.iconsPath + (isDefined ? "delete.svg" : "plus.svg")
                                            overlayVisible: false
                                            tooltipMessage: tooltipOverride
                                            onButtonPressed: {
                                                applicationThemeViewModel.fieldList.toggleDefinedField(identifier);
                                            }
                                        }

                                        Item {
                                            id: buttonsSeparator
                                            anchors.left: defineValueButton.right
                                            width: 1
                                        }

                                        FilterPanelIconButton {
                                            id: selectIconFromFileButton
                                            visible: isDefined && fieldType === 'icon'
                                            anchors.left: buttonsSeparator.right
                                            anchors.leftMargin: 10
                                            width: 20
                                            height: 20
                                            iconWidth: 18
                                            iconHeight: 18
                                            anchors.verticalCenter: parent.verticalCenter
                                            iconPath: assetsLocation.iconsPath + "themes.svg"
                                            overlayVisible: false
                                            tooltipMessage: "Выбрать файл изображения для иконки"
                                            onButtonPressed: {
                                                openIconFileDialog.selectedIconIndex = identifier;
                                                openIconFileDialog.open();
                                            }
                                        }

                                        Item {
                                            id: iconContainer
                                            anchors.left: selectIconFromFileButton.right
                                            anchors.leftMargin: 10
                                            anchors.verticalCenter: parent.verticalCenter
                                            width: 30
                                            height: 30
                                            visible: fieldType === 'icon'

                                            Image {
                                                id: iconImage
                                                visible: isDefined && valueTextField.text
                                                anchors.verticalCenter: parent.verticalCenter
                                                anchors.horizontalCenter: parent.horizontalCenter
                                                width: 28
                                                height: 28
                                                mipmap: true
                                                source: valueTextField.text.length && fieldType === 'icon' ? valueTextField.text : assetsLocation.iconsPath + "themes.svg"
                                            }
                                        }

                                        Rectangle {
                                            id: colorRectangle
                                            anchors.left: buttonsSeparator.right
                                            anchors.leftMargin: 10
                                            anchors.verticalCenter: parent.verticalCenter
                                            width: 20
                                            height: 20
                                            visible: fieldType === 'color'
                                            color: isDefined ? valueTextField.text : 'white'

                                            Canvas {
                                                id: crossIcon
                                                visible: !isDefined
                                                anchors.fill: parent
                                                opacity: .5
                                                onPaint: {
                                                    const ctx = getContext(`2d`);
                                                    const halfWidth = colorRectangle.width / 2;
                                                    const offset = 14;

                                                    ctx.lineWidth = 2;
                                                    ctx.strokeStyle = `#68b0ab`;
                                                    ctx.beginPath();
                                                    ctx.moveTo(colorRectangle.width - offset, offset);
                                                    ctx.lineTo(offset, colorRectangle.height - offset);
                                                    ctx.closePath();
                                                    ctx.stroke();

                                                    ctx.beginPath();
                                                    ctx.moveTo(offset, offset);
                                                    ctx.lineTo(colorRectangle.width - offset, colorRectangle.height - offset);
                                                    ctx.closePath();
                                                    ctx.stroke();
                                                }
                                            }


                                            MouseArea {
                                                anchors.fill: parent
                                                onPressed: {
                                                    applicationThemeViewModel.fieldList.selectedIndex = identifier;
                                                    colorDialog.color = fieldValue;
                                                    colorDialog.visible = true;
                                                }
                                            }
                                        }
                                    }
                                }
                            }

                            Rectangle {
                                color: "transparent"
                                width: 190
                                height: 50
                                anchors.right: parent.right
                                anchors.rightMargin: 20
                                anchors.bottom: parent.bottom

                                IconButton {
                                    anchors.right: parent.right
                                    anchors.top: parent.top
                                    anchors.topMargin: 8
                                    visible: fieldsListView.contentY > 100
                                    height: 30
                                    width: 30
                                    iconColor: applicationThemeViewModel.filterIconButtonColor
                                    hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
                                    iconPath: "../Assets/Icons/arrowup.svg"
                                    iconWidth: 24
                                    iconHeight: 24
                                    ToolTip.delay: 1000
                                    ToolTip.visible: hovered
                                    ToolTip.text: "Вернуться в начало списка полей"
                                    onButtonPressed: {
                                        fieldsListView.contentY = 0;
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
        id: deleteExternalThemeConfirm
        header: "Удалить установленную тему?"
        message: "Вы уверены что хотите удалить установленную тему?\nЕсли у Вас эта тема сейчас выбрана то после удаления выбранной станет Светлая тема."

        property string externalId

        content: Row {
            spacing: 6
            anchors.right: parent.right

            RoundedActionButton {
                text: "Ок"
                width: 100
                onClicked: {
                    applicationThemeViewModel.deleteThemeByExternalId(deleteExternalThemeConfirm.externalId);
                    deleteExternalThemeConfirm.close();
                }
            }
            RoundedActionButton {
                text: "Отмена"
                width: 100
                onClicked: {
                    deleteExternalThemeConfirm.close();
                }
            }
        }
    }

    MessageModal {
        id: deleteLocalThemeConfirm
        header: "Удалить локальную установленную тему?"
        message: "Вы уверены что хотите удалить установленную тему?\nЕсли у Вас эта тема сейчас выбрана то после удаления выбранной станет Светлая тема."

        property string themeName

        content: Row {
            spacing: 6
            anchors.right: parent.right

            RoundedActionButton {
                text: "Ок"
                width: 100
                onClicked: {
                    applicationThemeViewModel.deleteThemeFromLocal(deleteLocalThemeConfirm.themeName);
                    deleteLocalThemeConfirm.close();
                }
            }
            RoundedActionButton {
                text: "Отмена"
                width: 100
                onClicked: {
                    deleteLocalThemeConfirm.close();
                }
            }
        }
    }

    FileDialog {
        id: openThemeToFileDialog
        selectExisting: true
        nameFilters: ["Theme files (*.theme)"]
        onAccepted: {
            applicationThemeViewModel.importThemeFromFile(openThemeToFileDialog.fileUrl);
        }
    }

    FileDialog {
        id: saveThemeToFileDialog
        selectExisting: false
        nameFilters: ["Theme files (*.theme)"]
        onAccepted: {
            applicationThemeViewModel.fieldList.saveThemeToFile(saveThemeToFileDialog.fileUrl);
        }
    }

    FileDialog {
        id: openIconFileDialog
        selectExisting: true
        nameFilters: ["Image files (*.jpg *.jpeg *.gif *.svg *.png)"]

        property int selectedIconIndex

        onAccepted: {
            applicationThemeViewModel.fieldList.addIconFromFile(
                openIconFileDialog.fileUrl,
                openIconFileDialog.selectedIconIndex
            );
        }
    }

    ColorDialog {
        id: colorDialog
        title: "Выберите цвет из палитры"
        showAlphaChannel: true
        onAccepted: {
            applicationThemeViewModel.fieldList.setValueToItem(colorDialog.color);
        }
        onRejected: {
            applicationThemeViewModel.fieldList.selectedIndex = -1;
        }
    }

    Loader {
        id: previewWindowLoader
    }

    Component {
        id: previewWindowComponent

        ThemeManagerPreview {
            id: themeManagerPreview
            Component.onCompleted: {
                show();
            }
            onClosing: {
                previewWindowLoader.sourceComponent = null;
            }
        }
    }


    Connections {
        target: applicationThemeViewModel.fieldList
        function onBasedOnThemeChanged() {
            switch (applicationThemeViewModel.fieldList.basedOnTheme) {
                case "":
                    basedOnThemeComboBox.currentIndex = 0;
                    break;
                case "Светлая":
                    basedOnThemeComboBox.currentIndex = 1;
                    break;
                case "Темная":
                    basedOnThemeComboBox.currentIndex = 2;
                    break;
            }
        }
    }
}
