#include "customstyle.h"

CustomStyle::CustomStyle(QObject *parent)
    : QObject{parent}
{

}

CustomStyle *CustomStyle::qmlAttachedProperties(QObject *object)
{
    return new CustomStyle(object);
}

QString CustomStyle::tooltipColor() const noexcept
{
    return m_tooltipColor;
}

void CustomStyle::setTooltipColor(QString tooltipColor) noexcept
{
    if (m_tooltipColor == tooltipColor) return;

    m_tooltipColor = tooltipColor;
    emit tooltipColorChanged();
}

QString CustomStyle::colorTooltipText() const noexcept
{
    return m_colorTooltipText;
}

void CustomStyle::setColorTooltipText(QString colorTooltipText) noexcept
{
    if (m_colorTooltipText == colorTooltipText) return;

    m_colorTooltipText = colorTooltipText;
    emit colorTooltipTextChanged();
}

void CustomStyle::setLabelColor(QString labelColor) noexcept
{
    if (m_labelColor == labelColor) return;

    m_labelColor = labelColor;
    emit labelColorChanged();
}

void CustomStyle::setBackgroundDimColor(const QString &backgroundDimColor) noexcept
{
    if (m_backgroundDimColor == backgroundDimColor) return;

    m_backgroundDimColor = backgroundDimColor;
    emit backgroundDimColorChanged();
}

void CustomStyle::setPopupBackgroundColor(const QString &popupBackgroundColor) noexcept
{
    if (m_popupBackgroundColor == popupBackgroundColor) return;

    m_popupBackgroundColor = popupBackgroundColor;
    emit popupBackgroundColorChanged();
}

void CustomStyle::setSwitchHighlightedRippleColor(const QString &switchHighlightedRippleColor) noexcept
{
    if (m_switchHighlightedRippleColor == switchHighlightedRippleColor) return;

    m_switchHighlightedRippleColor = switchHighlightedRippleColor;
    emit switchHighlightedRippleColorChanged();
}

void CustomStyle::setSwitchRippleColor(const QString &switchRippleColor) noexcept
{
    if (m_switchRippleColor == switchRippleColor) return;

    m_switchRippleColor = switchRippleColor;
    emit switchRippleColorChanged();
}

void CustomStyle::setSwitchCheckedTrack(const QString &switchCheckedTrack) noexcept
{
    if (m_switchCheckedTrack == switchCheckedTrack) return;

    m_switchCheckedTrack = switchCheckedTrack;
    emit switchCheckedTrackChanged();
}

void CustomStyle::setSwitchUncheckedTrack(const QString &switchUncheckedTrack) noexcept
{
    if (m_switchUncheckedTrack == switchUncheckedTrack) return;

    m_switchUncheckedTrack = switchUncheckedTrack;
    emit switchUncheckedTrackChanged();
}

void CustomStyle::setSwitchDisabledTrack(const QString &switchDisabledTrack) noexcept
{
    if (m_switchDisabledTrack == switchDisabledTrack) return;

    m_switchDisabledTrack = switchDisabledTrack;
    emit switchDisabledTrackChanged();
}

void CustomStyle::setSwitchCheckedHandle(const QString &switchCheckedHandle) noexcept
{
    if (m_switchCheckedHandle == switchCheckedHandle) return;

    m_switchCheckedHandle = switchCheckedHandle;
    emit switchCheckedHandleChanged();
}

void CustomStyle::setSwitchUncheckedHandle(const QString &switchUncheckedHandle) noexcept
{
    if (m_switchUncheckedHandle == switchUncheckedHandle) return;

    m_switchUncheckedHandle = switchUncheckedHandle;
    emit switchUncheckedHandleChanged();
}

void CustomStyle::setSwitchDisabledHandle(const QString &switchDisabledHandle) noexcept
{
    if (m_switchDisabledHandle == switchDisabledHandle) return;

    m_switchDisabledHandle = switchDisabledHandle;
    emit switchDisabledHandleChanged();
}

void CustomStyle::setTextFieldForeground(const QString &textFieldForeground) noexcept
{
    if (m_textFieldForeground == textFieldForeground) return;

    m_textFieldForeground = textFieldForeground;
    emit textFieldForegroundChanged();
}

void CustomStyle::setTextFieldHint(const QString &textFieldHint) noexcept
{
    if (m_textFieldHint == textFieldHint) return;

    m_textFieldHint = textFieldHint;
    emit textFieldHintChanged();
}

void CustomStyle::setTextFieldHighlight(const QString &textFieldHighlight) noexcept
{
    if (m_textFieldHighlight == textFieldHighlight) return;

    m_textFieldHighlight = textFieldHighlight;
    emit textFieldHighlightChanged();
}

void CustomStyle::setTextFieldText(const QString &textFieldText) noexcept
{
    if (m_textFieldText == textFieldText) return;

    m_textFieldText = textFieldText;
    emit textFieldTextChanged();
}

void CustomStyle::setTextFieldAccent(const QString &textFieldAccent) noexcept
{
    if (m_textFieldAccent == textFieldAccent) return;

    m_textFieldAccent = textFieldAccent;
    emit textFieldAccentChanged();
}

void CustomStyle::setMenuDialogBackground(const QString &menuDialogBackground) noexcept
{
    if (m_menuDialogBackground == menuDialogBackground) return;

    m_menuDialogBackground = menuDialogBackground;
    emit menuDialogBackgroundChanged();
}

void CustomStyle::setMenuOverlayBackground(const QString &menuOverlayBackground) noexcept
{
    if (m_menuOverlayBackground == menuOverlayBackground) return;

    m_menuOverlayBackground = menuOverlayBackground;
    emit menuOverlayBackgroundChanged();
}

void CustomStyle::setMenuForeground(const QString &menuForeground) noexcept
{
    if (m_menuForeground == menuForeground) return;

    m_menuForeground = menuForeground;
    emit menuForegroundChanged();
}

void CustomStyle::setMenuItemHint(const QString &menuItemHint) noexcept
{
    if (m_menuItemHint == menuItemHint) return;

    m_menuItemHint = menuItemHint;
    emit menuItemHintChanged();
}

void CustomStyle::setMenuRipple(const QString &menuRipple) noexcept
{
    if (m_menuRipple == menuRipple) return;

    m_menuRipple = menuRipple;
    emit menuRippleChanged();
}

void CustomStyle::setMenuHighlight(const QString &menuHighlight) noexcept
{
    if (m_menuHighlight == menuHighlight) return;

    m_menuHighlight = menuHighlight;
    emit menuHighlightChanged();
}

void CustomStyle::setDrawerDividerColor(const QString &drawerDividerColor) noexcept
{
    if (m_drawerDividerColor == drawerDividerColor) return;

    m_drawerDividerColor = drawerDividerColor;
    emit drawerDividerColorChanged();
}

void CustomStyle::setDrawerDimColor(const QString &drawerDimColor) noexcept
{
    if (m_drawerDimColor == drawerDimColor) return;

    m_drawerDimColor = drawerDimColor;
    emit drawerDimColorChanged();
}

void CustomStyle::setDrawerDialogColor(const QString &drawerDialogColor) noexcept
{
    if (m_drawerDialogColor == drawerDialogColor) return;

    m_drawerDialogColor = drawerDialogColor;
    emit drawerDialogColorChanged();
}

void CustomStyle::setComboBoxAccent(const QString &comboBoxAccent) noexcept
{
    if (m_comboBoxAccent == comboBoxAccent) return;

    m_comboBoxAccent = comboBoxAccent;
    emit comboBoxAccentChanged();
}

void CustomStyle::setComboBoxHint(const QString &comboBoxHint) noexcept
{
    if (m_comboBoxHint == comboBoxHint) return;

    m_comboBoxHint = comboBoxHint;
    emit comboBoxHintChanged();
}

void CustomStyle::setComboBoxBackgroundColor(const QString &comboBoxBackgroundColor) noexcept
{
    if (m_comboBoxBackgroundColor == comboBoxBackgroundColor) return;

    m_comboBoxBackgroundColor = comboBoxBackgroundColor;
    emit comboBoxBackgroundColorChanged();
}
