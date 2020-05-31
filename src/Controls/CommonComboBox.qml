import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0

ComboBox {
    id: control
    property real fontPointSize: 11
    font.pointSize: fontPointSize + windowSettings.dpiSeparation
    delegate: ItemDelegate {
        width: control.width
        contentItem: Text {
            text: modelData
            color: control.currentIndex === index ? Material.accent : Material.foreground
            font.pointSize: fontPointSize + windowSettings.dpiSeparation
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: control.highlightedIndex === index
    }
}
