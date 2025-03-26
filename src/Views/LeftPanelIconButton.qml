import QtQuick 2.15
import QtQuick.Controls 2.15
import CustomStyle 1.0
import "../Controls"

IconButton {
    id: root
    property string tooltipMessage
    height: 45
    width: 40
    hoverColor: applicationThemeViewModel.filterIconButtonHoverColor
    iconWidth: 30
    iconHeight: 30

    ToolTip {
        delay: 1000
        visible: hovered && tooltipMessage
        text: tooltipMessage
        CustomStyle.tooltipColor: window.tooltipColor
        CustomStyle.colorTooltipText: window.tooltipColorText
    }
}
