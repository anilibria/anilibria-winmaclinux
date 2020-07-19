/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2020 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "usersettingsmodel.h"

UserSettingsModel::UserSettingsModel(): m_Quality(1),
    m_Volume(0.8),
    m_AutoNextVideo(true),
    m_AutoTopMost(false),
    m_TorrentDownloadMode(0),
    m_NotificationForFavorites(false),
    m_JumpMinute(1),
    m_JumpSecond(15),
    m_ShowReleaseInfo(false),
    m_ClearFiltersAfterChangeSection(true),
    m_CompactMode(false)
{

}

void UserSettingsModel::setQuality(const int quality) noexcept
{
    m_Quality = quality;
}

void UserSettingsModel::setVolume(const double volume) noexcept
{
    m_Volume = volume;
}

void UserSettingsModel::setAutoNextVideos(const bool autoNextVideo) noexcept
{
    m_AutoNextVideo = autoNextVideo;
}

void UserSettingsModel::setAutoTopMost(const bool autoTopMost) noexcept
{
    m_AutoTopMost = autoTopMost;
}

void UserSettingsModel::setTorrentDownloadMode(const int torrentDownloadMode) noexcept
{
    m_TorrentDownloadMode = torrentDownloadMode;
}

void UserSettingsModel::setNotificationForFavorites(const bool notificationForFavorites) noexcept
{
    m_NotificationForFavorites = notificationForFavorites;
}

void UserSettingsModel::setJumpMinute(const int jumpMinute) noexcept
{
    m_JumpMinute = jumpMinute;
}

void UserSettingsModel::setJumpSecond(const int jumpSecond) noexcept
{
    m_JumpSecond = jumpSecond;
}

void UserSettingsModel::setShowReleaseInfo(const bool showReleaseInfo) noexcept
{
    m_ShowReleaseInfo = showReleaseInfo;
}

void UserSettingsModel::setClearFiltersAfterChangeSection(const bool clearFiltersAfterChangeSection) noexcept
{
    m_ClearFiltersAfterChangeSection = clearFiltersAfterChangeSection;
}

void UserSettingsModel::setCompactMode(const bool compactMode) noexcept
{
    m_CompactMode = compactMode;
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
    if (jsonSettings.contains("showReleaseInfo")) setShowReleaseInfo(jsonSettings.value("showReleaseInfo").toBool());
    if (jsonSettings.contains("clearFiltersAfterChangeSection")) setClearFiltersAfterChangeSection(jsonSettings.value("clearFiltersAfterChangeSection").toBool());
    if (jsonSettings.contains("compactMode")) setCompactMode(jsonSettings.value("compactMode").toBool());
}

QString UserSettingsModel::toJson() noexcept
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
    object["showReleaseInfo"] = showReleaseInfo();
    object["clearFiltersAfterChangeSection"] = clearFiltersAfterChangeSection();
    object["compactMode"] = compactMode();

    QJsonDocument saveDoc(object);
    return saveDoc.toJson();
}

