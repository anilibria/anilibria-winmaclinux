import QtQuick
import QtQuick.Dialogs

Item {   
    property alias title: colorDialog.title
    property bool showAlphaChannel

    onShowAlphaChannelChanged: {
        if (showAlphaChannel) {
            colorDialog.options = ColorDialog.ShowAlphaChannel;
        }
    }

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
