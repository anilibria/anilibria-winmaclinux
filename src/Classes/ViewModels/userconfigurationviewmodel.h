#ifndef USERCONFIGURATIONVIEWMODEL_H
#define USERCONFIGURATIONVIEWMODEL_H

#include <QObject>

class UserConfigurationViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int opacityPlayerPanel READ opacityPlayerPanel WRITE setOpacityPlayerPanel NOTIFY opacityPlayerPanelChanged)

private:
    QString m_cacheFileName { "userconfiguration.cache" };
    QString m_opacityPlayerPanelField { "opacityPlayerPanel" };
    int m_opacityPlayerPanel { 50 };

public:
    explicit UserConfigurationViewModel(QObject *parent = nullptr);

    int opacityPlayerPanel() const noexcept { return m_opacityPlayerPanel; }
    void setOpacityPlayerPanel(int opacityPlayerPanel) noexcept;
    Q_INVOKABLE void saveSettingsToFile();

private:

    void readSettingsFromFile();

signals:
    void opacityPlayerPanelChanged();

};

#endif // USERCONFIGURATIONVIEWMODEL_H
