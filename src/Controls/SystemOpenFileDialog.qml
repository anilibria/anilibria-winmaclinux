import QtQuick 2.12
import QtQuick.Dialogs 1.2

Item {
    id: root
    property alias title: openFileDialog.title
    property alias nameFilters: openFileDialog.nameFilters

    signal needOpenFile(string fileUrl)
    signal open()

    onOpen: {
        openFileDialog.open();
    }

    FileDialog {
        id: openFileDialog
        selectExisting: true
        onAccepted: {
            root.needOpenFile(openFileDialog.fileUrl);
        }
    }
}

