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

public:
    UserSettingsModel();

    int quality();
    double volume();
    bool autoNextVideo();
    bool autoTopMost();
    int torrentDownloadMode();
    bool notificationForFavorites();

    void setQuality(int quality);
    void setVolume(double volume);
    void setAutoNextVideos(bool autoNextVideo);
    void setAutoTopMost(bool autoTopMost);
    void setTorrentDownloadMode(int torrentDownloadMode);
    void setNotificationForFavorites(bool notificationForFavorites);

    void fromJson(QString json);
    QString toJson();

};

#endif // USERSETTINGSMODEL_H
