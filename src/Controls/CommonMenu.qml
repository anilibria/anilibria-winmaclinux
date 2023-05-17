import QtQuick 2.12
import QtQuick.Controls 2.12
import CustomStyle 1.0

Menu {
    modal: true
    focus: true
    closePolicy: Popup.CloseOnEscape | Popup.CloseOnPressOutsideParent

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
}
