import QtQuick
import QtQuick.Dialogs

Item {
    id: root
    property alias title: colorDialog.title
    property bool showAlphaChannel: false

    signal colorSelected(int color)
    signal cancelDialog()
    signal open()

    onOpen: {
        colorDialog.open();
    }

    ColorDialog {
        id: colorDialog
        onAccepted: {
            colorSelected(colorDialog.selectedColor);
        }
        onRejected: {
            cancelDialog();
        }
    }

    onShowAlphaChannelChanged: {
        if (root.showAlphaChannel) {
            colorDialog.options = ColorDialog.ShowAlphaChannel;
        } else {
            colorDialog.options = 0;
        }
    }

}
