#include "usersettingsmodel.h"

UserSettingsModel::UserSettingsModel(): m_Quality(1),
    m_Volume(0.8),
    m_AutoNextVideo(true),
    m_AutoTopMost(false),
    m_TorrentDownloadMode(0),
    m_NotificationForFavorites(false),
    m_JumpMinute(1),
    m_JumpSecond(15)
{

}

int UserSettingsModel::quality()
{
    return m_Quality;
}

double UserSettingsModel::volume()
{
    return m_Volume;
}

bool UserSettingsModel::autoNextVideo()
{
    return m_AutoNextVideo;
}

bool UserSettingsModel::autoTopMost()
{
    return m_AutoTopMost;
}

int UserSettingsModel::torrentDownloadMode()
{
    return m_TorrentDownloadMode;
}

bool UserSettingsModel::notificationForFavorites()
{
    return m_NotificationForFavorites;
}

int UserSettingsModel::jumpMinute() const
{
    return m_JumpMinute;
}

int UserSettingsModel::jumpSecond() const
{
    return m_JumpSecond;
}

void UserSettingsModel::setQuality(int quality)
{
    m_Quality = quality;
}

void UserSettingsModel::setVolume(double volume)
{
    m_Volume = volume;
}

void UserSettingsModel::setAutoNextVideos(bool autoNextVideo)
{
    m_AutoNextVideo = autoNextVideo;
}

void UserSettingsModel::setAutoTopMost(bool autoTopMost)
{
    m_AutoTopMost = autoTopMost;
}

void UserSettingsModel::setTorrentDownloadMode(int torrentDownloadMode)
{
    m_TorrentDownloadMode = torrentDownloadMode;
}

void UserSettingsModel::setNotificationForFavorites(bool notificationForFavorites)
{
    m_NotificationForFavorites = notificationForFavorites;
}

void UserSettingsModel::setJumpMinute(int jumpMinute)
{
    m_JumpMinute = jumpMinute;
}

void UserSettingsModel::setJumpSecond(int jumpSecond)
{
    m_JumpSecond = jumpSecond;
}

void UserSettingsModel::fromJson(QString json)
{
    if (json.isEmpty()) return;

    auto document = QJsonDocument::fromJson(json.toUtf8());
    auto jsonSettings = document.object();

    if (jsonSettings.contains("quality")) setQuality(jsonSettings.value("quality").toInt());
    if (jsonSettings.contains("volume")) setVolume(jsonSettings.value("volume").toDouble());
    if (jsonSettings.contains("autoNextVideo")) setAutoNextVideos(jsonSettings.value("autoNextVideo").toBool());
    if (jsonSettings.contains("autoTopMost")) setAutoTopMost(jsonSettings.value("autoTopMost").toBool());
    if (jsonSettings.contains("torrentDownloadMode")) setTorrentDownloadMode(jsonSettings.value("torrentDownloadMode").toInt());
    if (jsonSettings.contains("jumpMinute")) setJumpMinute(jsonSettings.value("jumpMinute").toInt());
    if (jsonSettings.contains("jumpSecond")) setJumpSecond(jsonSettings.value("jumpSecond").toInt());
    if (jsonSettings.contains("notificationForFavorites")) setNotificationForFavorites(jsonSettings.value("notificationForFavorites").toBool());
}

QString UserSettingsModel::toJson()
{
    QJsonObject object;

    object["quality"] = quality();
    object["volume"] = volume();
    object["autoNextVideo"] = autoNextVideo();
    object["autoTopMost"] = autoTopMost();
    object["torrentDownloadMode"] = torrentDownloadMode();
    object["notificationForFavorites"] = notificationForFavorites();
    object["jumpMinute"] = jumpMinute();
    object["jumpSecond"] = jumpSecond();

    QJsonDocument saveDoc(object);
    return saveDoc.toJson();
}

