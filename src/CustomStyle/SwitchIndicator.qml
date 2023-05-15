import QtQuick 2.12
import CustomStyle 1.0

Item {
    id: indicator
    implicitWidth: 38
    implicitHeight: 32

    property Item control
    property alias handle: handle

    Rectangle {
        width: parent.width
        height: 14
        radius: height / 2
        y: parent.height / 2 - height / 2
        color: indicator.control.enabled ? (indicator.control.checked ? indicator.control.CustomStyle.switchCheckedTrack : indicator.control.CustomStyle.switchUncheckedTrack)
                               : indicator.control.CustomStyle.switchDisabledTrack
    }

    Rectangle {
        id: handle
        x: Math.max(0, Math.min(parent.width - width, indicator.control.visualPosition * parent.width - (width / 2)))
        y: (parent.height - height) / 2
        width: 20
        height: 20
        radius: width / 2
        color: indicator.control.enabled ? (indicator.control.checked ? indicator.control.CustomStyle.switchCheckedHandle : indicator.control.CustomStyle.switchUncheckedHandle)
                               : indicator.control.CustomStyle.switchDisabledHandle

        Behavior on x {
            enabled: !indicator.control.pressed
            SmoothedAnimation {
                duration: 300
            }
        }
        layer.enabled: true
        layer.effect: ElevationEffect {
            elevation: 0
            backgroundColor: control.CustomStyle.backgroundDimColor
        }
    }
}
