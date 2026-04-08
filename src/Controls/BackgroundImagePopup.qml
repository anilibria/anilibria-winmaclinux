import QtQuick 2.15
import QtQuick.Controls 2.15

DefaultPopup {
    id: backgroundPopup
    width: 450
    height: 390
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    property string pagesData: '{}'
    property var pagesDataObject

    signal savedData()

    PlainText {
        id: pagesLabel
        anchors.top: parent.top
        width: parent.width
        fontPointSize: 11
        text: "Страница"
    }

    DictionaryComboBox {
        id: pagesComboBox
        anchors.top: pagesLabel.bottom
        width: parent.width
        model: releasesViewModel.imageBackgroundViewModel.pages
        onActivated: {
            // save current state for previous selected item
            refreshDataForPage(releasesViewModel.imageBackgroundViewModel.pages.selectedItem);

            // fill data for new selected page
            releasesViewModel.imageBackgroundViewModel.pages.selectedItem = currentValue;
            setupFieldsForPage(releasesViewModel.imageBackgroundViewModel.pages.selectedItem);
        }
    }

    PlainText {
        id: imagePathLabel
        anchors.top: pagesComboBox.bottom
        width: parent.width
        fontPointSize: 11
        text: "Изображение"
    }

    CommonTextField {
        id: imagePathTextField
        width: parent.width
        anchors.top: imagePathLabel.bottom
        anchors.rightMargin: 10
        placeholderText: "Путь к файлу или url"
        enabled: releasesViewModel.imageBackgroundViewModel.pages.selectedItem > -1
        onTextChanged: {
            refreshDataForPage(releasesViewModel.imageBackgroundViewModel.pages.selectedItem);
        }
    }

    PlainText {
        id: imageModesLabel
        anchors.top: imagePathTextField.bottom
        width: parent.width - 20
        fontPointSize: 11
        text: "Режим изображения"
    }

    DictionaryComboBox {
        id: imageModesComboBox
        anchors.top: imageModesLabel.bottom
        width: parent.width
        model: releasesViewModel.imageBackgroundViewModel.imageModes
        enabled: releasesViewModel.imageBackgroundViewModel.pages.selectedItem > -1
        onActivated: {
            refreshDataForPage(releasesViewModel.imageBackgroundViewModel.pages.selectedItem);
        }
    }

    PlainText {
        id: aligmentLabel
        anchors.top: imageModesComboBox.bottom
        width: parent.width - 20
        fontPointSize: 11
        text: "Выравнивание"
    }

    DictionaryComboBox {
        id: aligmentComboBox
        anchors.top: aligmentLabel.bottom
        width: parent.width
        model: releasesViewModel.imageBackgroundViewModel.alignmentModes
        enabled: releasesViewModel.imageBackgroundViewModel.pages.selectedItem > -1
        onActivated: {
            refreshDataForPage(releasesViewModel.imageBackgroundViewModel.pages.selectedItem);
        }
    }

    PlainText {
        id: opacityLabel
        anchors.top: aligmentComboBox.bottom
        width: parent.width
        fontPointSize: 11
        text: "Прозрачность"
    }

    Slider {
        id: opacitySlider
        anchors.top: opacityLabel.bottom
        from: 1
        to: 100
        width: parent.width
        enabled: releasesViewModel.imageBackgroundViewModel.pages.selectedItem > -1
        onMoved: {
            refreshDataForPage(releasesViewModel.imageBackgroundViewModel.pages.selectedItem);
        }
    }

    Row {
        anchors.top: opacitySlider.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 8

        RoundedActionButton {
            width: 120
            text: "Сохранить"
            onClicked: {
                backgroundPopup.pagesData = JSON.stringify(backgroundPopup.pagesDataObject);
                backgroundPopup.close();
                savedData();
            }
        }

        RoundedActionButton {
            width: 120
            text: "Отменить"
            onClicked: {
                cancelModal();
            }
        }
    }

    function setupFieldsForPage(page) {
        const key = page.toString();
        const existingsItem = backgroundPopup.pagesDataObject[key];
        if (!existingsItem) {
            imageModesComboBox.currentIndex = 6;
            aligmentComboBox.currentIndex = 0;
            opacitySlider.value = 100;
            imagePathTextField.text = "";
        } else {
            imageModesComboBox.currentIndex = existingsItem.im;
            aligmentComboBox.currentIndex = existingsItem.al;
            opacitySlider.value = existingsItem.op;
            imagePathTextField.text = existingsItem.url;
        }
    }

    function refreshDataForPage(page) {
        const key = page.toString();
        const existingsItem = backgroundPopup.pagesDataObject.hasOwnProperty(key);
        if (!existingsItem) {
            backgroundPopup.pagesDataObject[key] = {
                "im": imageModesComboBox.currentIndex,
                "al": aligmentComboBox.currentIndex,
                "op": opacitySlider.value,
                "url": imagePathTextField.text
            }
        } else {
            backgroundPopup.pagesDataObject[key].im = imageModesComboBox.currentIndex;
            backgroundPopup.pagesDataObject[key].al = aligmentComboBox.currentIndex;
            backgroundPopup.pagesDataObject[key].op = opacitySlider.value;
            backgroundPopup.pagesDataObject[key].url = imagePathTextField.text;
        }
    }

    function cancelModal() {
        backgroundPopup.close();
    }

    onOpened: {
        if (!backgroundPopup.pagesData) backgroundPopup.pagesData = '{}';

        backgroundPopup.pagesDataObject = JSON.parse(backgroundPopup.pagesData);
        setupFieldsForPage(0);
        pagesComboBox.currentIndex = 0;
    }

    onClosed: {
        cancelModal();
    }
}
