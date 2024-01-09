import QtQuick
import QtQuick.Dialogs

Item {   
    property alias title: colorDialog.title
    property alias showAlphaChannel: colorDialog.showAlphaChannel

    signal colorSelected(int color)
    signal cancelDialog()
    signal open()

    onOpen: {
        colorDialog.open();
    }

    ColorDialog {
        id: colorDialog
        onAccepted: {
            colorSelected(colorDialog.color);
        }
        onRejected: {
            cancelDialog();
        }
    }

}
