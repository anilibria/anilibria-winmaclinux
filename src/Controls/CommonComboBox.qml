import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0
import CustomStyle 1.0

ComboBox {
    id: control
    property real fontPointSize: 11
    font.pointSize: fontPointSize + windowSettings.dpiSeparation

    CustomStyle.comboBoxAccent: applicationThemeViewModel.currentItems.colorComboBoxAccent
    CustomStyle.comboBoxHint: applicationThemeViewModel.currentItems.colorComboBoxHint
    CustomStyle.comboBoxBackgroundColor: applicationThemeViewModel.currentItems.colorComboBoxBackground
    CustomStyle.popupBackgroundColor: applicationThemeViewModel.currentItems.colorPopupBackground
    CustomStyle.backgroundDimColor: applicationThemeViewModel.currentItems.colorPopupBackgroundShadow
    CustomStyle.textFieldAccent: applicationThemeViewModel.currentItems.colorTextFieldAccent
    CustomStyle.textFieldForeground: applicationThemeViewModel.currentItems.colorTextFieldForeground
    CustomStyle.textFieldHint: applicationThemeViewModel.currentItems.colorTextFieldHint
    CustomStyle.textFieldHighlight: applicationThemeViewModel.currentItems.colorTextFieldHighlight
    CustomStyle.textFieldText: applicationThemeViewModel.currentItems.colorTextFieldText

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
