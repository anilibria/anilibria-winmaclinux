import QtQuick 2.12
import QtQuick.Controls 2.12
import CustomStyle 1.0

Switch {
    property string tooltipMessage

    ToolTip {
        delay: 2000
        visible: hovered && tooltipMessage
        text: tooltipMessage
        CustomStyle.tooltipColor: applicationThemeViewModel.currentItems.colorTooltipBackground
        CustomStyle.colorTooltipText: applicationThemeViewModel.currentItems.colorTooltipText
    }
}
