import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

Popup {
    id: backgroundPopup
    width: 450
    height: 330
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    PlainText {
        id: imagePathLabel
        anchors.top: parent.top
        width: parent.width
        fontPointSize: 11
        text: "Изображение"
    }

    TextField {
        id: imagePathTextField
        width: parent.width
        anchors.top: imagePathLabel.bottom
        anchors.rightMargin: 10
        placeholderText: "Путь к файлу или url"
        text: releasesViewModel.imageBackgroundViewModel.imagePath
        onTextChanged: {
            releasesViewModel.imageBackgroundViewModel.imagePath = imagePathTextField.text;
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
        onActivated: {
            releasesViewModel.imageBackgroundViewModel.imageModes.selectedItem = currentValue;
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
        onActivated: {
            releasesViewModel.imageBackgroundViewModel.alignmentModes.selectedItem = currentValue;
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
        value: releasesViewModel.imageBackgroundViewModel.opacity
        from: 1
        to: 100
        width: parent.width
        onMoved: {
            releasesViewModel.imageBackgroundViewModel.opacity = value;
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
                releasesViewModel.imageBackgroundViewModel.saveCurrentState();

                backgroundPopup.close();
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

    function setupComboboxes() {
        imageModesComboBox.currentIndex = imageModesComboBox.indexOfValue(releasesViewModel.imageBackgroundViewModel.imageModes.selectedItem);
        aligmentComboBox.currentIndex = aligmentComboBox.indexOfValue(releasesViewModel.imageBackgroundViewModel.alignmentModes.selectedItem);
    }

    function cancelModal() {
        releasesViewModel.imageBackgroundViewModel.restoreToSavedState();

        setupComboboxes();
        backgroundPopup.close();
    }

    onOpened: {
        setupComboboxes();
    }

    onClosed: {
        cancelModal();
    }
}
