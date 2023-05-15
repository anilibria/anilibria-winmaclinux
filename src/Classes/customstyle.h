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
    Q_PROPERTY(QString backgroundDimColor READ backgroundDimColor WRITE setBackgroundDimColor NOTIFY backgroundDimColorChanged)
    Q_PROPERTY(QString popupBackgroundColor READ popupBackgroundColor WRITE setPopupBackgroundColor NOTIFY popupBackgroundColorChanged)
    Q_PROPERTY(QString switchHighlightedRippleColor READ switchHighlightedRippleColor WRITE setSwitchHighlightedRippleColor NOTIFY switchHighlightedRippleColorChanged)
    Q_PROPERTY(QString switchRippleColor READ switchRippleColor WRITE setSwitchRippleColor NOTIFY switchRippleColorChanged)
    Q_PROPERTY(QString switchCheckedTrack READ switchCheckedTrack WRITE setSwitchCheckedTrack NOTIFY switchCheckedTrackChanged)
    Q_PROPERTY(QString switchUncheckedTrack READ switchUncheckedTrack WRITE setSwitchUncheckedTrack NOTIFY switchUncheckedTrackChanged)
    Q_PROPERTY(QString switchDisabledTrack READ switchDisabledTrack WRITE setSwitchDisabledTrack NOTIFY switchDisabledTrackChanged)
    Q_PROPERTY(QString switchCheckedHandle READ switchCheckedHandle WRITE setSwitchCheckedHandle NOTIFY switchCheckedHandleChanged)
    Q_PROPERTY(QString switchUncheckedHandle READ switchUncheckedHandle WRITE setSwitchUncheckedHandle NOTIFY switchUncheckedHandleChanged)
    Q_PROPERTY(QString switchDisabledHandle READ switchDisabledHandle WRITE setSwitchDisabledHandle NOTIFY switchDisabledHandleChanged)

private:
    QString m_tooltipColor { "black" };
    QString m_colorTooltipText { "black" };
    QString m_labelColor { "black" };
    QString m_backgroundDimColor { "black" };
    QString m_popupBackgroundColor { "black" };
    QString m_switchHighlightedRippleColor { "black" };
    QString m_switchRippleColor { "black" };

    QString m_switchCheckedTrack { "black" };
    QString m_switchUncheckedTrack { "black" };
    QString m_switchDisabledTrack { "black" };
    QString m_switchCheckedHandle { "black" };
    QString m_switchUncheckedHandle { "black" };
    QString m_switchDisabledHandle { "black" };

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

    QString backgroundDimColor() const noexcept { return m_backgroundDimColor; }
    void setBackgroundDimColor(const QString& backgroundDimColor) noexcept;

    QString popupBackgroundColor() const noexcept { return m_popupBackgroundColor; }
    void setPopupBackgroundColor(const QString& popupBackgroundColor) noexcept;

    QString switchHighlightedRippleColor() const noexcept { return m_switchHighlightedRippleColor; }
    void setSwitchHighlightedRippleColor(const QString& switchHighlightedRippleColor) noexcept;

    QString switchRippleColor() const noexcept { return m_switchRippleColor; }
    void setSwitchRippleColor(const QString& switchRippleColor) noexcept;

    QString switchCheckedTrack() const noexcept { return m_switchCheckedTrack; }
    void setSwitchCheckedTrack(const QString& switchCheckedTrack) noexcept;

    QString switchUncheckedTrack() const noexcept { return m_switchUncheckedTrack; }
    void setSwitchUncheckedTrack(const QString& switchUncheckedTrack) noexcept;

    QString switchDisabledTrack() const noexcept { return m_switchDisabledTrack; }
    void setSwitchDisabledTrack(const QString& switchDisabledTrack) noexcept;

    QString switchCheckedHandle() const noexcept { return m_switchCheckedHandle; }
    void setSwitchCheckedHandle(const QString& switchCheckedHandle) noexcept;

    QString switchUncheckedHandle() const noexcept { return m_switchUncheckedHandle; }
    void setSwitchUncheckedHandle(const QString& switchUncheckedHandle) noexcept;

    QString switchDisabledHandle() const noexcept { return m_switchDisabledHandle; }
    void setSwitchDisabledHandle(const QString& switchDisabledHandle) noexcept;

signals:
    void tooltipColorChanged();
    void colorTooltipTextChanged();
    void labelColorChanged();
    void backgroundDimColorChanged();
    void popupBackgroundColorChanged();
    void switchHighlightedRippleColorChanged();
    void switchRippleColorChanged();

    void switchCheckedTrackChanged();
    void switchUncheckedTrackChanged();
    void switchDisabledTrackChanged();
    void switchCheckedHandleChanged();
    void switchUncheckedHandleChanged();
    void switchDisabledHandleChanged();

};

QML_DECLARE_TYPEINFO(CustomStyle, QML_HAS_ATTACHED_PROPERTIES)

#endif // CUSTOMSTYLE_H
