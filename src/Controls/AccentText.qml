import QtQuick 2.15

Text {
    property real fontPointSize: 0

    font.pointSize: fontPointSize + windowSettings.dpiSeparation
    font.family: userConfigurationViewModel.textFont
    color: applicationThemeViewModel.headerTextColor
}
