#ifndef USERSETTINGSMODEL_H
#define USERSETTINGSMODEL_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>

class UserSettingsModel
{

private:
    int m_Quality;
    double m_Volume;
    bool m_AutoNextVideo;
    bool m_AutoTopMost;
    int m_TorrentDownloadMode;
    bool m_NotificationForFavorites;
    int m_JumpMinute;
    int m_JumpSecond;

public:
    UserSettingsModel();

    int quality() const {return m_Quality;}
    double volume() const {return m_Volume;}
    bool autoNextVideo() const {return m_AutoNextVideo;}
    bool autoTopMost() const {return  m_AutoTopMost;}
    int torrentDownloadMode() const {return  m_TorrentDownloadMode;}
    bool notificationForFavorites() const {return  m_NotificationForFavorites;}
    int jumpMinute() const {return m_JumpMinute;}
    int jumpSecond() const {return m_JumpSecond;}

    void setQuality(const int quality) noexcept;
    void setVolume(const double volume) noexcept;
    void setAutoNextVideos(const bool autoNextVideo) noexcept;
    void setAutoTopMost(const bool autoTopMost) noexcept;
    void setTorrentDownloadMode(const int torrentDownloadMode) noexcept;
    void setNotificationForFavorites(const bool notificationForFavorites) noexcept;
    void setJumpMinute(const int jumpMinute) noexcept;
    void setJumpSecond(const int jumpSecond) noexcept;

    void fromJson(QString json);
    QString toJson() noexcept;

};

#endif // USERSETTINGSMODEL_H
