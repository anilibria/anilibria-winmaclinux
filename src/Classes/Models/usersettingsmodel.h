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

    int quality();
    double volume();
    bool autoNextVideo();
    bool autoTopMost();
    int torrentDownloadMode();
    bool notificationForFavorites();
    int jumpMinute() const;
    int jumpSecond() const;

    void setQuality(int quality);
    void setVolume(double volume);
    void setAutoNextVideos(bool autoNextVideo);
    void setAutoTopMost(bool autoTopMost);
    void setTorrentDownloadMode(int torrentDownloadMode);
    void setNotificationForFavorites(bool notificationForFavorites);
    void setJumpMinute(int jumpMinute);
    void setJumpSecond(int jumpSecond);

    void fromJson(QString json);
    QString toJson();

};

#endif // USERSETTINGSMODEL_H
