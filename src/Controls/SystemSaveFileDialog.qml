import QtQuick 2.12
import QtQuick.Dialogs 1.2

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
        selectExisting: false
        onAccepted: {
            needSaveFile(saveTorrentFileDialog.fileUrl);
        }
    }
}

