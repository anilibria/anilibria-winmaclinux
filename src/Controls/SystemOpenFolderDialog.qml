import QtQuick 2.12
import QtQuick.Dialogs 1.2

Item {
    id: root
    property alias title: selectFolderDialog.title

    signal needOpenFolder(string folderUrl)
    signal open()

    onOpen: {
        selectFolderDialog.open();
    }

    FileDialog {
        id: selectFolderDialog
        selectExisting: true
        selectFolder: true
        onAccepted: {
            needOpenFolder(selectFolderDialog.fileUrl);
        }
    }
}
