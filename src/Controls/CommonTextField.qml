import QtQuick 2.12
import QtQuick.Controls 2.12
import CustomStyle 1.0

TextField {
    CustomStyle.textFieldAccent: applicationThemeViewModel.currentItems.colorTextFieldAccent
    CustomStyle.textFieldForeground: applicationThemeViewModel.currentItems.colorTextFieldForeground
    CustomStyle.textFieldHint: applicationThemeViewModel.currentItems.colorTextFieldHint
    CustomStyle.textFieldHighlight: applicationThemeViewModel.currentItems.colorTextFieldHighlight
    CustomStyle.textFieldText: applicationThemeViewModel.currentItems.colorTextFieldText
}
