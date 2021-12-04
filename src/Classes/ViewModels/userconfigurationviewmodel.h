#ifndef USERCONFIGURATIONVIEWMODEL_H
#define USERCONFIGURATIONVIEWMODEL_H

#include <QObject>

class UserConfigurationViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int opacityPlayerPanel READ opacityPlayerPanel WRITE setOpacityPlayerPanel NOTIFY opacityPlayerPanelChanged)
    Q_PROPERTY(bool notCloseReleaseCardAfterWatch READ notCloseReleaseCardAfterWatch WRITE setNotCloseReleaseCardAfterWatch NOTIFY notCloseReleaseCardAfterWatchChanged)

private:
    QString m_cacheFileName { "userconfiguration.cache" };
    QString m_opacityPlayerPanelField { "opacityPlayerPanel" };
    QString m_notCloseReleaseCardAfterWatchField { "notCloseReleaseCardAfterWatch" };
    int m_opacityPlayerPanel { 50 };
    bool m_notCloseReleaseCardAfterWatch { false };

public:
    explicit UserConfigurationViewModel(QObject *parent = nullptr);

    bool notCloseReleaseCardAfterWatch() const noexcept { return m_notCloseReleaseCardAfterWatch; }
    void setNotCloseReleaseCardAfterWatch(const bool notCloseReleaseCardAfterWatch) noexcept;

    int opacityPlayerPanel() const noexcept { return m_opacityPlayerPanel; }
    void setOpacityPlayerPanel(int opacityPlayerPanel) noexcept;
    Q_INVOKABLE void saveSettingsToFile();

private:

    void readSettingsFromFile();

signals:
    void opacityPlayerPanelChanged();
    void notCloseReleaseCardAfterWatchChanged();

};

#endif // USERCONFIGURATIONVIEWMODEL_H
