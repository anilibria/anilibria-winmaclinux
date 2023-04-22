import QtQuick 2.12
import QtQuick.Controls 2.12
import CustomStyle 1.0

Popup {
    Overlay.modal: Rectangle {
        color: "#50000000"

        Behavior on opacity {
            NumberAnimation {
                duration: 120
            }
        }
    }
    CustomStyle.popupBackgroundColor: applicationThemeViewModel.currentItems.colorPopupBackground
    CustomStyle.backgroundDimColor: applicationThemeViewModel.currentItems.colorPopupBackgroundShadow
}
