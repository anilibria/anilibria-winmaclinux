import QtQuick 2.15
import QtQuick.Controls 2.15
import CustomStyle 1.0

Menu {
    id: root
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

    property bool autoWidth: false

    CustomStyle.menuDialogBackground: applicationThemeViewModel.currentItems.colorMenuDialogBackground
    CustomStyle.menuOverlayBackground: applicationThemeViewModel.currentItems.colorMenuOverlayBackground

    Overlay.modal: Rectangle {
        color: "#50000000"

        Behavior on opacity {
            NumberAnimation {
                duration: 120
            }
        }
    }

    onOpened: {
        if (!root.autoWidth) return;

        let result = 0;
        let padding = 0;
        const countItems = count;
        for (let i = 0; i < countItems; i++) {
            const item = itemAt(i);
            result = Math.max(item.contentItem.implicitWidth, result);
            padding = Math.max(item.padding, padding);
        }
        root.width  = result + padding * 2;
    }
}
