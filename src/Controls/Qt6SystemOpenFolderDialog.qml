import QtQuick
import QtQuick.Dialogs

Item {
    id: root
    property string title: ''

    signal needOpenFolder(string folderUrl)
    signal open()

    onOpen: {
        folderDialog.open();
    }

    FolderDialog {
        id: folderDialog
        onAccepted: {
            root.needOpenFolder(folderDialog.selectedFolder);
        }
    }
}

