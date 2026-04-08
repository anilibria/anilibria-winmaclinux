import QtQuick 2.15
import QtQuick.Controls 2.15
import CustomStyle 1.0

MenuItem {
    property bool notVisible: false

    CustomStyle.menuForeground: window.colorMenuForeground
    CustomStyle.menuItemHint: window.colorMenuItemHint
    CustomStyle.menuRipple: window.colorMenuRipple
    CustomStyle.menuHighlight: window.colorMenuHighlight

    width: parent.width
    height: notVisible ? 0 : undefined
    visible: !notVisible
    font.pointSize: 11 + windowSettings.dpiSeparation
}
