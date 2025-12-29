import QtQuick
import QtQuick.Dialogs

Item {
    id: root
    property string title: ''
    property alias nameFilters: saveFileDialog.nameFilters

    signal needOpenFile(string fileUrl)
    signal open()

    onOpen: {
        saveFileDialog.open();
    }

    FileDialog {
        id: saveFileDialog
        fileMode: FileDialog.OpenFile
        onAccepted: {
            root.needOpenFile(saveFileDialog.selectedFile);
        }
    }
}

