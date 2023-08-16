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
    Q_PROPERTY(QString textFieldForeground READ textFieldForeground WRITE setTextFieldForeground NOTIFY textFieldForegroundChanged)
    Q_PROPERTY(QString textFieldHint READ textFieldHint WRITE setTextFieldHint NOTIFY textFieldHintChanged)
    Q_PROPERTY(QString textFieldHighlight READ textFieldHighlight WRITE setTextFieldHighlight NOTIFY textFieldHighlightChanged)
    Q_PROPERTY(QString textFieldText READ textFieldText WRITE setTextFieldText NOTIFY textFieldTextChanged)
    Q_PROPERTY(QString textFieldAccent READ textFieldAccent WRITE setTextFieldAccent NOTIFY textFieldAccentChanged)
    Q_PROPERTY(QString menuDialogBackground READ menuDialogBackground WRITE setMenuDialogBackground NOTIFY menuDialogBackgroundChanged)
    Q_PROPERTY(QString menuOverlayBackground READ menuOverlayBackground WRITE setMenuOverlayBackground NOTIFY menuOverlayBackgroundChanged)
    Q_PROPERTY(QString menuForeground READ menuForeground WRITE setMenuForeground NOTIFY menuForegroundChanged)
    Q_PROPERTY(QString menuItemHint READ menuItemHint WRITE setMenuItemHint NOTIFY menuItemHintChanged)
    Q_PROPERTY(QString menuRipple READ menuRipple WRITE setMenuRipple NOTIFY menuRippleChanged)
    Q_PROPERTY(QString menuHighlight READ menuHighlight WRITE setMenuHighlight NOTIFY menuHighlightChanged)
    Q_PROPERTY(QString drawerDividerColor READ drawerDividerColor WRITE setDrawerDividerColor NOTIFY drawerDividerColorChanged)
    Q_PROPERTY(QString drawerDimColor READ drawerDimColor WRITE setDrawerDimColor NOTIFY drawerDimColorChanged)
    Q_PROPERTY(QString drawerDialogColor READ drawerDialogColor WRITE setDrawerDialogColor NOTIFY drawerDialogColorChanged)
    Q_PROPERTY(QString comboBoxAccent READ comboBoxAccent WRITE setComboBoxAccent NOTIFY comboBoxAccentChanged)
    Q_PROPERTY(QString comboBoxHint READ comboBoxHint WRITE setComboBoxHint NOTIFY comboBoxHintChanged)
    Q_PROPERTY(QString comboBoxBackgroundColor READ comboBoxBackgroundColor WRITE setComboBoxBackgroundColor NOTIFY comboBoxBackgroundColorChanged)

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
    QString m_textFieldForeground { "black" };
    QString m_textFieldHint { "black" };
    QString m_textFieldHighlight { "black" };
    QString m_textFieldText { "black" };
    QString m_textFieldAccent { "black" };
    QString m_menuDialogBackground { "black" };
    QString m_menuOverlayBackground { "black" };
    QString m_menuForeground { "black" };
    QString m_menuItemHint { "black" };
    QString m_menuRipple { "black" };
    QString m_menuHighlight { "black" };
    QString m_drawerDividerColor { "black" };
    QString m_drawerDimColor { "black" };
    QString m_drawerDialogColor { "black" };
    QString m_comboBoxAccent { "black" };
    QString m_comboBoxHint { "black" };
    QString m_comboBoxBackgroundColor { "black" };

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

    QString textFieldForeground() const noexcept { return m_textFieldForeground; }
    void setTextFieldForeground(const QString& textFieldForeground) noexcept;

    QString textFieldHint() const noexcept { return m_textFieldHint; }
    void setTextFieldHint(const QString& textFieldHint) noexcept;

    QString textFieldHighlight() const noexcept { return m_textFieldHighlight; }
    void setTextFieldHighlight(const QString& textFieldHighlight) noexcept;

    QString textFieldText() const noexcept { return m_textFieldText; }
    void setTextFieldText(const QString& textFieldText) noexcept;

    QString textFieldAccent() const noexcept { return m_textFieldAccent; }
    void setTextFieldAccent(const QString& textFieldAccent) noexcept;

    QString menuDialogBackground() const noexcept { return m_menuDialogBackground; }
    void setMenuDialogBackground(const QString& menuDialogBackground) noexcept;

    QString menuOverlayBackground() const noexcept { return m_menuOverlayBackground; }
    void setMenuOverlayBackground(const QString& menuOverlayBackground) noexcept;

    QString menuForeground() const noexcept { return m_menuForeground; }
    void setMenuForeground(const QString& menuForeground) noexcept;

    QString menuItemHint() const noexcept { return m_menuItemHint; }
    void setMenuItemHint(const QString& menuItemHint) noexcept;

    QString menuRipple() const noexcept { return m_menuRipple; }
    void setMenuRipple(const QString& menuRipple) noexcept;

    QString menuHighlight() const noexcept { return m_menuHighlight; }
    void setMenuHighlight(const QString& menuHighlight) noexcept;

    QString drawerDividerColor() const noexcept { return m_drawerDividerColor; }
    void setDrawerDividerColor(const QString& drawerDividerColor) noexcept;

    QString drawerDimColor() const noexcept { return m_drawerDimColor; }
    void setDrawerDimColor(const QString& drawerDimColor) noexcept;

    QString drawerDialogColor() const noexcept { return m_drawerDialogColor; }
    void setDrawerDialogColor(const QString& drawerDialogColor) noexcept;

    QString comboBoxAccent() const noexcept { return m_comboBoxAccent; }
    void setComboBoxAccent(const QString& comboBoxAccent) noexcept;

    QString comboBoxHint() const noexcept { return m_comboBoxHint; }
    void setComboBoxHint(const QString& comboBoxHint) noexcept;

    QString comboBoxBackgroundColor() const noexcept { return m_comboBoxBackgroundColor; }
    void setComboBoxBackgroundColor(const QString& comboBoxBackgroundColor) noexcept;

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
    void textFieldForegroundChanged();
    void textFieldHintChanged();
    void textFieldHighlightChanged();
    void textFieldTextChanged();
    void textFieldAccentChanged();
    void menuDialogBackgroundChanged();
    void menuOverlayBackgroundChanged();
    void menuForegroundChanged();
    void menuItemHintChanged();
    void menuRippleChanged();
    void menuHighlightChanged();
    void drawerDividerColorChanged();
    void drawerDimColorChanged();
    void drawerDialogColorChanged();

    void comboBoxAccentChanged();
    void comboBoxHintChanged();
    void comboBoxBackgroundColorChanged();
};

QML_DECLARE_TYPEINFO(CustomStyle, QML_HAS_ATTACHED_PROPERTIES)

#endif // CUSTOMSTYLE_H
