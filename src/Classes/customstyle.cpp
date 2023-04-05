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
