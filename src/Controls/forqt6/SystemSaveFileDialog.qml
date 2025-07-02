import QtQuick
import QtQuick.Dialogs

Item {
    id: root
    property alias title: saveFileDialog.title
    property alias nameFilters: saveFileDialog.nameFilters

    signal needSaveFile(string fileUrl)
    signal open()

    onOpen: {
        saveFileDialog.open();
    }

    FileDialog {
        id: saveFileDialog
        selectExisting: true
        onAccepted: {
            root.needSaveFile(saveFileDialog.fileUrl);
        }
    }
}

