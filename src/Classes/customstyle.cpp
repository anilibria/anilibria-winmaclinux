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
