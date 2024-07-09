import QtQuick 2.15
import QtQuick.Controls 2.15
import CustomStyle 1.0
import "../Controls"

IconButton {
    property string tooltipMessage
    height: 30
    width: 30
    hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
    iconWidth: 24
    iconHeight: 24

    ToolTip {
        delay: 2000
        visible: hovered && tooltipMessage
        text: tooltipMessage
        CustomStyle.tooltipColor: window.tooltipColor
        CustomStyle.colorTooltipText: window.tooltipColorText
    }
}
