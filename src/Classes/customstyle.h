#ifndef CUSTOMSTYLE_H
#define CUSTOMSTYLE_H

#include <QObject>
#include <QtQml>

class CustomStyle : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString tooltipColor READ tooltipColor WRITE setTooltipColor NOTIFY tooltipColorChanged FINAL)
    Q_PROPERTY(QString colorTooltipText READ colorTooltipText WRITE setColorTooltipText NOTIFY colorTooltipTextChanged FINAL)
    Q_PROPERTY(QString labelColor READ labelColor WRITE setLabelColor NOTIFY labelColorChanged)

private:
    QString m_tooltipColor { "black" };
    QString m_colorTooltipText { "black" };
    QString m_labelColor { "black" };

public:
    explicit CustomStyle(QObject *parent = nullptr);

    static CustomStyle *qmlAttachedProperties(QObject *object);

    QString tooltipColor() const noexcept;
    void setTooltipColor(QString tooltipColor) noexcept;
    void resetTooltipColor() noexcept;

    QString colorTooltipText() const noexcept;
    void setColorTooltipText(QString colorTooltipText) noexcept;

    QString labelColor() const noexcept { return m_labelColor; }
    void setLabelColor(QString labelColor) noexcept;

signals:
    void tooltipColorChanged();
    void colorTooltipTextChanged();
    void labelColorChanged();

};

QML_DECLARE_TYPEINFO(CustomStyle, QML_HAS_ATTACHED_PROPERTIES)

#endif // CUSTOMSTYLE_H
