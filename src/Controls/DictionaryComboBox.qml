import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

CommonComboBox {
    id: dictionaryComboBox
    valueRole: "id"
    textRole: "title"
    font.pointSize: 11 + windowSettings.dpiSeparation
    delegate: ItemDelegate {
        width: dictionaryComboBox.width
        contentItem: Text {
            text: title
            color: dictionaryComboBox.currentIndex === index ? Material.accent : Material.foreground
            font.pointSize: 11 + windowSettings.dpiSeparation
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: dictionaryComboBox.highlightedIndex === index
    }
}
