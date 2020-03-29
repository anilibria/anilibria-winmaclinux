import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

Button {
    id: _button
    Material.foreground: "white"
    background: Rectangle {
        color: _button.hovered || _button.down ? "#881919" : "#b32121"
        border.color: "transparent"
        border.width: 1
        radius: 18
    }
}
