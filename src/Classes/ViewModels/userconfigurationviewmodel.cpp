/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2021 Roman Vladimirov

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

#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "userconfigurationviewmodel.h"
#include "../../globalhelpers.h"

UserConfigurationViewModel::UserConfigurationViewModel(QObject *parent) : QObject(parent)
{
    createIfNotExistsFile(getCachePath(m_cacheFileName), "{}");
    readSettingsFromFile();
}

void UserConfigurationViewModel::setNotCloseReleaseCardAfterWatch(const bool notCloseReleaseCardAfterWatch) noexcept
{
    if (m_notCloseReleaseCardAfterWatch == notCloseReleaseCardAfterWatch) return;

    m_notCloseReleaseCardAfterWatch = notCloseReleaseCardAfterWatch;
    emit notCloseReleaseCardAfterWatchChanged();
}

void UserConfigurationViewModel::setOpacityPlayerPanel(int opacityPlayerPanel) noexcept
{
    if (m_opacityPlayerPanel == opacityPlayerPanel) return;

    m_opacityPlayerPanel = opacityPlayerPanel;
    emit opacityPlayerPanelChanged();
}

void UserConfigurationViewModel::setUsingScrollAcceleration(const bool usingScrollAcceleration) noexcept
{
    if (m_usingScrollAcceleration == usingScrollAcceleration) return;

    m_usingScrollAcceleration = usingScrollAcceleration;
    emit usingScrollAccelerationChanged();
}

void UserConfigurationViewModel::setHideStatistics(const bool hideStatistics) noexcept
{
    if (m_hideStatistics == hideStatistics) return;

    m_hideStatistics = hideStatistics;
    emit hideStatisticsChanged();
}

void UserConfigurationViewModel::setHideUpdatesByFavorites(const bool hideUpdatesByFavorites) noexcept
{
    if (m_hideUpdatesByFavorites == hideUpdatesByFavorites) return;

    m_hideUpdatesByFavorites = hideUpdatesByFavorites;
    emit hideUpdatesByFavoritesChanged();
}

void UserConfigurationViewModel::setHideUpdatesFromStart(const bool hideUpdatesFromStart) noexcept
{
    if (m_hideUpdatesFromStart == hideUpdatesFromStart) return;

    m_hideUpdatesFromStart = hideUpdatesFromStart;
    emit hideUpdatesFromStartChanged();
}

void UserConfigurationViewModel::setHideUpdatesLastTwoDays(const bool hideUpdatesLastTwoDays) noexcept
{
    if (m_hideUpdatesLastTwoDays == hideUpdatesLastTwoDays) return;

    m_hideUpdatesLastTwoDays = hideUpdatesLastTwoDays;
    emit hideUpdatesLastTwoDaysChanged();
}

void UserConfigurationViewModel::setHideAbandonedSeens(const bool hideAbandonedSeens) noexcept
{
    if (m_hideAbandonedSeens == hideAbandonedSeens) return;

    m_hideAbandonedSeens = hideAbandonedSeens;
    emit hideAbandonedSeensChanged();
}

void UserConfigurationViewModel::setMarkAsReadAfterDownload(const bool markAsReadAfterDownload) noexcept
{
    if (m_markAsReadAfterDownload == markAsReadAfterDownload) return;

    m_markAsReadAfterDownload = markAsReadAfterDownload;
    emit markAsReadAfterDownloadChanged();
}

void UserConfigurationViewModel::setStartPage(const QString &startPage) noexcept
{
    if (m_startPage == startPage) return;

    m_startPage = startPage;
    emit startPageChanged();
}

void UserConfigurationViewModel::setHideRecommendByGenres(const bool hideRecommendByGenres) noexcept
{
    if (m_hideRecommendByGenres == hideRecommendByGenres) return;

    m_hideRecommendByGenres = hideRecommendByGenres;
    emit hideRecommendByGenresChanged();
}

void UserConfigurationViewModel::setHideWillWatch(bool hideWillWatch) noexcept
{
    if (m_hideWillWatch == hideWillWatch) return;

    m_hideWillWatch = hideWillWatch;
    emit hideWillWatchChanged();
}

void UserConfigurationViewModel::setShowedVideoForNewcomers(bool showedVideoForNewcomers) noexcept
{
    if (m_showedVideoForNewcomers == showedVideoForNewcomers) return;

    m_showedVideoForNewcomers = showedVideoForNewcomers;
    emit showedVideoForNewcomersChanged();
}

void UserConfigurationViewModel::saveSettingsToFile()
{
    QJsonObject object;
    object[m_opacityPlayerPanelField] = m_opacityPlayerPanel;
    object[m_notCloseReleaseCardAfterWatchField] = m_notCloseReleaseCardAfterWatch;
    object[m_usingScrollAccelerationField] = m_usingScrollAcceleration;
    object[m_hideStatisticsField] = m_hideStatistics;
    object[m_hideUpdatesByFavoritesField] = m_hideUpdatesByFavorites;
    object[m_hideUpdatesFromStartField] = m_hideUpdatesFromStart;
    object[m_hideUpdatesLastTwoDaysField] = m_hideUpdatesLastTwoDays;
    object[m_hideAbandonedSeensField] = m_hideAbandonedSeens;
    object[m_markAsReadAfterDownloadField] = m_markAsReadAfterDownload;
    object[m_startPageField] = m_startPage;
    object[m_hideRecommendByGenresField] = m_hideRecommendByGenres;
    object[m_hideWillWatchField] = m_hideWillWatch;
    object[m_showedVideoForNewcomersField] = m_showedVideoForNewcomers;

    QFile file(getCachePath(m_cacheFileName));
    file.open(QFile::WriteOnly | QFile::Text);
    file.write(QJsonDocument(object).toJson());
    file.close();
}

void UserConfigurationViewModel::readSettingsFromFile()
{
    QFile file(getCachePath(m_cacheFileName));
    file.open(QFile::ReadOnly | QFile::Text);
    auto json = file.readAll();
    file.close();

    auto document = QJsonDocument::fromJson(json);
    auto object = document.object();
    m_opacityPlayerPanel = object.contains(m_opacityPlayerPanelField) ? object[m_opacityPlayerPanelField].toInt() : 50;
    m_notCloseReleaseCardAfterWatch = object.contains(m_notCloseReleaseCardAfterWatchField) ? object[m_notCloseReleaseCardAfterWatchField].toBool() : false;
    m_usingScrollAcceleration = object.contains(m_usingScrollAccelerationField) ? object[m_usingScrollAccelerationField].toBool() : true;
    m_hideStatistics = object.contains(m_hideStatisticsField) ? object[m_hideStatisticsField].toBool() : false;
    m_hideUpdatesByFavorites = object.contains(m_hideUpdatesByFavoritesField) ? object[m_hideUpdatesByFavoritesField].toBool() : false;
    m_hideUpdatesFromStart = object.contains(m_hideUpdatesFromStartField) ? object[m_hideUpdatesFromStartField].toBool() : false;
    m_hideUpdatesLastTwoDays = object.contains(m_hideUpdatesLastTwoDaysField) ? object[m_hideUpdatesLastTwoDaysField].toBool() : false;
    m_hideAbandonedSeens = object.contains(m_hideAbandonedSeensField) ? object[m_hideAbandonedSeensField].toBool() : false;
    m_markAsReadAfterDownload = object.contains(m_markAsReadAfterDownloadField) ? object[m_markAsReadAfterDownloadField].toBool() : false;
    m_startPage = object.contains(m_startPageField) ? object[m_startPageField].toString() : "release";
    m_hideRecommendByGenres = object.contains(m_hideRecommendByGenresField) ? object[m_hideRecommendByGenresField].toBool() : false;
    m_hideWillWatch = object.contains(m_hideWillWatchField) ? object[m_hideWillWatchField].toBool() : false;
    m_showedVideoForNewcomers = object.contains(m_showedVideoForNewcomersField) ? object[m_showedVideoForNewcomersField].toBool() : false;
}
