import QtQuick 2.15
import QtQuick.Controls 2.15
//import QtQuick.Controls.Material 2.15
import CustomStyle 1.0

ComboBox {
    id: control
    property real fontPointSize: 11
    property bool usePropertyKey: false
    font.pointSize: fontPointSize + windowSettings.dpiSeparation
    font.family: userConfigurationViewModel.textFont

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
            text: control.usePropertyKey ? (modelData["key"] ? modelData.key : "") : modelData
            color: control.currentIndex === index ? applicationThemeViewModel.currentItems.colorTextFieldAccent : applicationThemeViewModel.currentItems.colorTextFieldForeground
            font.pointSize: fontPointSize + windowSettings.dpiSeparation
            font.family: userConfigurationViewModel.textFont
            elide: Text.ElideRight
            verticalAlignment: Text.AlignVCenter
        }
        highlighted: control.highlightedIndex === index
    }
}
