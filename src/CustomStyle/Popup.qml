import QtQuick 2.15
import QtQuick.Templates 2.12 as T
import CustomStyle 1.0

T.Popup {
    id: control

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    padding: 12

    enter: Transition {
        NumberAnimation { property: "scale"; from: 0.9; to: 1.0; easing.type: Easing.OutQuint; duration: 220 }
        NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; easing.type: Easing.OutCubic; duration: 150 }
    }

    exit: Transition {
        NumberAnimation { property: "scale"; from: 1.0; to: 0.9; easing.type: Easing.OutQuint; duration: 220 }
        NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; easing.type: Easing.OutCubic; duration: 150 }
    }

    background: Rectangle {
        radius: 2
        color: control.CustomStyle.popupBackgroundColor

        layer.enabled: true
        layer.effect: ElevationEffect {
            elevation: 204
            backgroundColor: control.CustomStyle.backgroundDimColor
        }
    }

    T.Overlay.modal: Rectangle {
        color: control.CustomStyle.backgroundDimColor
        Behavior on opacity { NumberAnimation { duration: 150 } }
    }

    T.Overlay.modeless: Rectangle {
        color: control.CustomStyle.backgroundDimColor
        Behavior on opacity { NumberAnimation { duration: 150 } }
    }
}
