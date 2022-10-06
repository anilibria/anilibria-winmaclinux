import QtQuick 2.12
import QtQuick.Controls 2.12

Popup {
    Overlay.modal: Rectangle {
        color: "#50000000"

        Behavior on opacity {
            NumberAnimation {
                duration: 120
            }
        }
    }
}
