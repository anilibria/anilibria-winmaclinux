import QtQuick 2.15
import QtQuick.Controls 2.15
import CustomStyle 1.0

Switch {
    property string tooltipMessage

    CustomStyle.switchHighlightedRippleColor: applicationThemeViewModel.currentItems.colorSwitchHighlightedRipple
    CustomStyle.switchRippleColor: applicationThemeViewModel.currentItems.colorSwitchRipple
    CustomStyle.switchCheckedTrack: applicationThemeViewModel.currentItems.colorSwitchCheckedTrack
    CustomStyle.switchUncheckedTrack: applicationThemeViewModel.currentItems.colorSwitchUncheckedTrack
    CustomStyle.switchDisabledTrack: applicationThemeViewModel.currentItems.colorSwitchDisabledTrack
    CustomStyle.switchCheckedHandle: applicationThemeViewModel.currentItems.colorSwitchCheckedHandle
    CustomStyle.switchUncheckedHandle: applicationThemeViewModel.currentItems.colorSwitchUncheckedHandle
    CustomStyle.switchDisabledHandle: applicationThemeViewModel.currentItems.colorSwitchDisabledHandle

    ToolTip {
        delay: 2000
        visible: hovered && tooltipMessage
        text: tooltipMessage
        CustomStyle.tooltipColor: applicationThemeViewModel.currentItems.colorTooltipBackground
        CustomStyle.colorTooltipText: applicationThemeViewModel.currentItems.colorTooltipText
    }
}
