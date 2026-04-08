import QtQuick 2.15
import QtQuick.Dialogs 1.3

Item {   
    property alias title: colorDialog.title
    property alias showAlphaChannel: colorDialog.showAlphaChannel
    property alias selectedColor: colorDialog.color

    signal colorSelected(var color)
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
