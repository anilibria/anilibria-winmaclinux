
import QtQuick 2.12
import QtQuick.Templates 2.12 as T
import CustomStyle 1.0

T.Drawer {
    id: control

    parent: T.Overlay.overlay

    implicitWidth: Math.max(implicitBackgroundWidth + leftInset + rightInset,
                            contentWidth + leftPadding + rightPadding)
    implicitHeight: Math.max(implicitBackgroundHeight + topInset + bottomInset,
                             contentHeight + topPadding + bottomPadding)

    topPadding: !dim && edge === Qt.BottomEdge
    leftPadding: !dim && edge === Qt.RightEdge
    rightPadding: !dim && edge === Qt.LeftEdge
    bottomPadding: !dim && edge === Qt.TopEdge

    enter: Transition { SmoothedAnimation { velocity: 5 } }
    exit: Transition { SmoothedAnimation { velocity: 5 } }

    background: Rectangle {
        color: control.CustomStyle.drawerDialogColor

        Rectangle {
            readonly property bool horizontal: control.edge === Qt.LeftEdge || control.edge === Qt.RightEdge
            width: horizontal ? 1 : parent.width
            height: horizontal ? parent.height : 1
            color: control.CustomStyle.drawerDividerColor
            x: control.edge === Qt.LeftEdge ? parent.width - 1 : 0
            y: control.edge === Qt.TopEdge ? parent.height - 1 : 0
            visible: !control.dim
        }

        layer.enabled: control.position > 0
        layer.effect: ElevationEffect {
            elevation: 0
            fullHeight: true
        }
    }

    T.Overlay.modal: Rectangle {
        color: control.CustomStyle.drawerDimColor
        Behavior on opacity { NumberAnimation { duration: 150 } }
    }

    T.Overlay.modeless: Rectangle {
        color: control.CustomStyle.drawerDimColor
        Behavior on opacity { NumberAnimation { duration: 150 } }
    }
}
