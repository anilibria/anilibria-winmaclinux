import QtQuick 2.15
import QtQuick.Controls 2.15
import CustomStyle 1.0

Image {
    id: root
    mipmap: true

    property string tooltipMessage
    property bool tooltipMessageVisible

    ToolTip {
        delay: 1000
        visible: tooltipMessageVisible && tooltipMessage
        text: tooltipMessage
        CustomStyle.tooltipColor: applicationThemeViewModel.currentItems.colorTooltipBackground
        CustomStyle.colorTooltipText: applicationThemeViewModel.currentItems.colorTooltipText
    }
}
