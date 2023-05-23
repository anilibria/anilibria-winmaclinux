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

void UserConfigurationViewModel::setAutoSkipOpening(bool autoSkipOpening) noexcept
{
    if (m_autoSkipOpening == autoSkipOpening) return;

    m_autoSkipOpening = autoSkipOpening;
    emit autoSkipOpeningChanged();
}

void UserConfigurationViewModel::setHideSkipOpening(bool hideSkipOpening) noexcept
{
    if (m_hideSkipOpening == hideSkipOpening) return;

    m_hideSkipOpening = hideSkipOpening;
    emit hideSkipOpeningChanged();
}

void UserConfigurationViewModel::setFixedControlPanel(bool fixedControlPanel) noexcept
{
    if (m_fixedControlPanel == fixedControlPanel) return;

    m_fixedControlPanel = fixedControlPanel;
    emit fixedControlPanelChanged();
}

void UserConfigurationViewModel::setHidedQuality(bool hidedQuality) noexcept
{
    if (m_hidedQuality == hidedQuality) return;

    m_hidedQuality = hidedQuality;
    emit hidedQualityChanged();
}

void UserConfigurationViewModel::setHidedSpeed(bool hidedSpeed) noexcept
{
    if (m_hidedSpeed == hidedSpeed) return;

    m_hidedSpeed = hidedSpeed;
    emit hidedSpeedChanged();
}

void UserConfigurationViewModel::setPlayerBuffer(int playerBuffer) noexcept
{
    if (m_playerBuffer == playerBuffer) return;

    m_playerBuffer = playerBuffer;
    emit playerBufferChanged();
}

void UserConfigurationViewModel::setUsingVideoProxy(bool usingVideoProxy) noexcept
{
    if (m_usingVideoProxy == usingVideoProxy) return;

    m_usingVideoProxy = usingVideoProxy;
    emit usingVideoProxyChanged();
}

void UserConfigurationViewModel::setTorrentStreamPath(const QString &torrentStreamPath) noexcept
{
    if (m_torrentStreamPath == torrentStreamPath) return;

    m_torrentStreamPath = torrentStreamPath;
    emit torrentStreamPathChanged();
}

void UserConfigurationViewModel::setRemoveAllDownloadedTorrent(bool removeAllDownloadedTorrent) noexcept
{
    if (m_removeAllDownloadedTorrent == removeAllDownloadedTorrent) return;

    m_removeAllDownloadedTorrent = removeAllDownloadedTorrent;
    emit removeAllDownloadedTorrentChanged();
}

void UserConfigurationViewModel::setIsCroppedPlayer(bool isCroppedPlayer) noexcept
{
    if (m_isCroppedPlayer == isCroppedPlayer) return;

    m_isCroppedPlayer = isCroppedPlayer;
    emit isCroppedPlayerChanged();
}

void UserConfigurationViewModel::setHideNextInReleaseSeries(bool hideNextInReleaseSeries) noexcept
{
    if (m_hideNextInReleaseSeries == hideNextInReleaseSeries) return;

    m_hideNextInReleaseSeries = hideNextInReleaseSeries;
    emit hideNextInReleaseSeriesChanged();
}

void UserConfigurationViewModel::setHideCurrentSeason(bool hideCurrentSeason) noexcept
{
    if (m_hideCurrentSeason == hideCurrentSeason) return;

    m_hideCurrentSeason = hideCurrentSeason;
    emit hideCurrentSeasonChanged();
}

void UserConfigurationViewModel::setHideActualInCurrentSeason(bool hideActualInCurrentSeason) noexcept
{
    if (m_hideActualInCurrentSeason == hideActualInCurrentSeason) return;

    m_hideActualInCurrentSeason = hideActualInCurrentSeason;
    emit hideActualInCurrentSeasonChanged();
}

void UserConfigurationViewModel::setHideRecommendedByVoices(bool hideRecommendedByVoices) noexcept
{
    if (m_hideRecommendedByVoices == hideRecommendedByVoices) return;

    m_hideRecommendedByVoices = hideRecommendedByVoices;
    emit hideRecommendedByVoicesChanged();
}

void UserConfigurationViewModel::setRemoveCopyFromName(bool removeCopyFromName) noexcept
{
    if (m_removeCopyFromName == removeCopyFromName) return;

    m_removeCopyFromName = removeCopyFromName;
    emit removeCopyFromNameChanged();
}

void UserConfigurationViewModel::setNotSaveWindowPosition(bool notSaveWindowPosition) noexcept
{
    if (m_notSaveWindowPosition == notSaveWindowPosition) return;

    m_notSaveWindowPosition = notSaveWindowPosition;
    emit notSaveWindowPositionChanged();
}

void UserConfigurationViewModel::setNeedSavePlayer(bool needSavePlayer) noexcept
{
    if (m_needSavePlayer == needSavePlayer) return;

    m_needSavePlayer = needSavePlayer;
    emit needSavePlayerChanged();
}

void UserConfigurationViewModel::setLastSelectedPlayer(const QString &lastSelectedPlayer)
{
    if (m_lastSelectedPlayer == lastSelectedPlayer) return;

    m_lastSelectedPlayer = lastSelectedPlayer;
    emit lastSelectedPlayerChanged();
}

void UserConfigurationViewModel::refreshConfiguration() noexcept
{
    readSettingsFromFile();

    emit opacityPlayerPanelChanged();
    emit notCloseReleaseCardAfterWatchChanged();
    emit usingScrollAccelerationChanged();
    emit hideStatisticsChanged();
    emit hideUpdatesByFavoritesChanged();
    emit hideUpdatesFromStartChanged();
    emit hideUpdatesLastTwoDaysChanged();
    emit hideAbandonedSeensChanged();
    emit markAsReadAfterDownloadChanged();
    emit startPageChanged();
    emit hideRecommendByGenresChanged();
    emit hideWillWatchChanged();
    emit showedVideoForNewcomersChanged();
    emit autoSkipOpeningChanged();
    emit fixedControlPanelChanged();
    emit hideSkipOpeningChanged();
    emit isCroppedPlayerChanged();
    emit removeCopyFromNameChanged();
    emit hideRecommendedByVoicesChanged();
    emit hideActualInCurrentSeasonChanged();
    emit hideCurrentSeasonChanged();
    emit hideNextInReleaseSeriesChanged();
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
    object[m_autoSkipOpeningField] = m_autoSkipOpening;
    object[m_hideSkipOpeningField] = m_hideSkipOpening;
    object[m_fixedControlPanelField] = m_fixedControlPanel;
    object[m_hidedQualityField] = m_hidedQuality;
    object[m_hidedSpeedField] = m_hidedSpeed;
    object[m_playerBufferField] = m_playerBuffer;
    object[m_usingVideoProxyField] = m_usingVideoProxy;
    object[m_torrentStreamPathField] = m_torrentStreamPath;
    object[m_removeAllDownloadedTorrentField] = m_removeAllDownloadedTorrent;
    object[m_isCroppedPlayerField] = m_isCroppedPlayer;
    object[m_hideNextInReleaseSeriesField] = m_hideNextInReleaseSeries;
    object[m_hideCurrentSeasonField] = m_hideCurrentSeason;
    object[m_hideActualInCurrentSeasonField] = m_hideActualInCurrentSeason;
    object[m_hideRecommendedByVoicesField] = m_hideRecommendedByVoices;
    object[m_removeCopyFromNameField] = m_removeCopyFromName;
    object[m_notSaveWindowPositionField] = m_notSaveWindowPosition;
    object[m_needSavePlayerField] = m_needSavePlayer;
    object[m_lastSelectedPlayerField] = m_lastSelectedPlayer;

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
    m_autoSkipOpening = object.contains(m_autoSkipOpeningField) ? object[m_autoSkipOpeningField].toBool() : false;
    m_hideSkipOpening = object.contains(m_hideSkipOpeningField) ? object[m_hideSkipOpeningField].toBool() : false;
    m_fixedControlPanel = object.contains(m_fixedControlPanelField) ? object[m_fixedControlPanelField].toBool() : false;
    m_hidedQuality = object.contains(m_hidedQualityField) ? object[m_hidedQualityField].toBool() : false;
    m_hidedSpeed = object.contains(m_hidedSpeedField) ? object[m_hidedSpeedField].toBool() : false;
    m_playerBuffer = object.contains(m_playerBufferField) ? object[m_playerBufferField].toInt() : 0;
    m_usingVideoProxy = object.contains(m_usingVideoProxyField) ? object[m_usingVideoProxyField].toBool() : true;
    m_torrentStreamPath = object.contains(m_torrentStreamPathField) ? object[m_torrentStreamPathField].toString() : "torrentStream/TorrentStream.exe";
    m_removeAllDownloadedTorrent = object.contains(m_removeAllDownloadedTorrentField) ? object[m_removeAllDownloadedTorrentField].toBool() : true;
    m_isCroppedPlayer = object.contains(m_isCroppedPlayerField) ? object[m_isCroppedPlayerField].toBool() : false;
    m_hideNextInReleaseSeries = object.contains(m_hideNextInReleaseSeriesField) ? object[m_hideNextInReleaseSeriesField].toBool() : false;
    m_hideCurrentSeason = object.contains(m_hideCurrentSeasonField) ? object[m_hideCurrentSeasonField].toBool() : false;
    m_hideActualInCurrentSeason = object.contains(m_hideActualInCurrentSeasonField) ? object[m_hideActualInCurrentSeasonField].toBool() : false;
    m_hideRecommendedByVoices = object.contains(m_hideRecommendedByVoicesField) ? object[m_hideRecommendedByVoicesField].toBool() : false;
    m_removeCopyFromName = object.contains(m_removeCopyFromNameField) ? object[m_removeCopyFromNameField].toBool() : false;
    m_notSaveWindowPosition = object.contains(m_notSaveWindowPositionField) ? object[m_notSaveWindowPositionField].toBool() : false;
    m_needSavePlayer = object.contains(m_needSavePlayerField) ? object[m_needSavePlayerField].toBool() : false;
    m_lastSelectedPlayer = object.contains(m_lastSelectedPlayerField) ? object[m_lastSelectedPlayerField].toString() : "";
}
