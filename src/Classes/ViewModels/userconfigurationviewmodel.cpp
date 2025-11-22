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
#include <QFont>
#include <QGuiApplication>
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

void UserConfigurationViewModel::setCustomScriptFile(const QString &customScriptFile) noexcept
{
    if (m_customScriptFile == customScriptFile) return;

    m_customScriptFile = customScriptFile;
#ifdef Q_OS_WIN
    m_customScriptFile = m_customScriptFile.replace("file:///", "");
#else
    m_customScriptFile = m_customScriptFile.replace("file://", "");
#endif
    emit customScriptFileChanged();
}

void UserConfigurationViewModel::setRemotePort(int remotePort) noexcept
{
    if (m_remotePort == remotePort) return;

    m_remotePort = remotePort;
    emit remotePortChanged();
}

void UserConfigurationViewModel::setSendVolumeToRemote(bool sendVolumeToRemote) noexcept
{
    if (m_sendVolumeToRemote == sendVolumeToRemote) return;

    m_sendVolumeToRemote = sendVolumeToRemote;
    emit sendVolumeToRemoteChanged();
}

void UserConfigurationViewModel::setSendPlaybackToRemote(bool sendPlaybackToRemote) noexcept
{
    if (m_sendPlaybackToRemote == sendPlaybackToRemote) return;

    m_sendPlaybackToRemote = sendPlaybackToRemote;
    emit sendPlaybackToRemoteChanged();
}

void UserConfigurationViewModel::setUsingVideoProxyVLC(bool usingVideoProxyVLC) noexcept
{
    if (m_usingVideoProxyVLC == usingVideoProxyVLC) return;

    m_usingVideoProxyVLC = usingVideoProxyVLC;
    emit usingVideoProxyVLCChanged();
}

void UserConfigurationViewModel::setTextFont(const QString& textFont) noexcept
{
    if (m_textFont == textFont) return;

    m_textFont = textFont;
    emit textFontChanged();

    changeFont(m_textFont);
}

void UserConfigurationViewModel::setUsingStrongProxy(bool usingStrongProxy) noexcept
{
    if (m_usingStrongProxy == usingStrongProxy) return;

    m_usingStrongProxy = usingStrongProxy;
    emit usingStrongProxyChanged();
}

void UserConfigurationViewModel::setUsingVideoProxyMPV(bool usingVideoProxyMPV) noexcept
{
    if (m_usingVideoProxyMPV == usingVideoProxyMPV) return;

    m_usingVideoProxyMPV = usingVideoProxyMPV;
    emit usingVideoProxyMPVChanged();
}

void UserConfigurationViewModel::setShowVideoPreview(bool showVideoPreview) noexcept
{
    if (m_showVideoPreview == showVideoPreview) return;

    m_showVideoPreview = showVideoPreview;
    emit showVideoPreviewChanged();
}

void UserConfigurationViewModel::setShowReleaseInfo(bool showReleaseInfo) noexcept
{
    if (m_showReleaseInfo == showReleaseInfo) return;

    m_showReleaseInfo = showReleaseInfo;
    emit showReleaseInfoChanged();
}

void UserConfigurationViewModel::setAutoNextVideo(bool autoNextVideo) noexcept
{
    if (m_autoNextVideo == autoNextVideo) return;

    m_autoNextVideo = autoNextVideo;
    emit autoNextVideoChanged();
}

void UserConfigurationViewModel::setAutoPlayerTopMost(bool autoPlayerTopMost) noexcept
{
    if (m_autoPlayerTopMost == autoPlayerTopMost) return;

    m_autoPlayerTopMost = autoPlayerTopMost;
    emit autoPlayerTopMostChanged();
}

void UserConfigurationViewModel::setApiv2host(QString apiv2host) noexcept
{
    if (m_apiv2host == apiv2host) return;

    m_apiv2host = apiv2host;
    emit apiv2hostChanged();
}

void UserConfigurationViewModel::setV2token(const QString &v2token) noexcept
{
    if (m_v2token == v2token) return;

    m_v2token = v2token;
    emit v2tokenChanged();
}

void UserConfigurationViewModel::setTorrentDownloadMode(int torrentDownloadMode) noexcept
{
    if (m_torrentDownloadMode == torrentDownloadMode) return;

    m_torrentDownloadMode = torrentDownloadMode;
    emit torrentDownloadModeChanged();
}

void UserConfigurationViewModel::setCachehost(const QString& cachehost) noexcept
{
    if (m_cachehost == cachehost) return;

    m_cachehost = cachehost;
    emit cachehostChanged();
}

void UserConfigurationViewModel::setUseCacheFolder(bool useCacheFolder) noexcept
{
    if (m_useCacheFolder == useCacheFolder) return;

    m_useCacheFolder = useCacheFolder;
    emit useCacheFolderChanged();
}

void UserConfigurationViewModel::setCacheFolder(const QString &cacheFolder) noexcept
{
    if (m_cacheFolder == cacheFolder) return;

    m_cacheFolder = cacheFolder;
    emit cacheFolderChanged();
}

void UserConfigurationViewModel::setVideoServer(int videoServer) noexcept
{
    if (m_videoServer == videoServer) return;

    m_videoServer = videoServer;
    emit videoServerChanged();
}

void UserConfigurationViewModel::setRestoreVideoMode(int restoreVideoMode) noexcept
{
    if (m_restoreVideoMode == restoreVideoMode) return;

    m_restoreVideoMode = restoreVideoMode;
    emit restoreVideoModeChanged();
}

void UserConfigurationViewModel::setAutoSkipEnding(bool autoSkipEnding) noexcept
{
    if (m_autoSkipEnding == autoSkipEnding) return;

    m_autoSkipEnding = autoSkipEnding;
    emit autoSkipEndingChanged();
}

void UserConfigurationViewModel::setShowFullTeam(bool showFullTeam) noexcept
{
    if (m_showFullTeam == showFullTeam) return;

    m_showFullTeam = showFullTeam;
    emit showFullTeamChanged();
}

void UserConfigurationViewModel::setUseTorrentStreamLibrary(bool useTorrentStreamLibrary) noexcept
{
    if (m_useTorrentStreamLibrary == useTorrentStreamLibrary) return;

    m_useTorrentStreamLibrary = useTorrentStreamLibrary;
    emit useTorrentStreamLibraryChanged();
}

void UserConfigurationViewModel::setTorrentStreamUI(bool torrentStreamUI) noexcept
{
    if (m_torrentStreamUI == torrentStreamUI) return;

    m_torrentStreamUI = torrentStreamUI;
    emit torrentStreamUIChanged();
}

void UserConfigurationViewModel::setPathToTSContent(QString pathToTSContent) noexcept
{
    if (m_pathToTSContent == pathToTSContent) return;

    m_pathToTSContent = pathToTSContent;
    emit pathToTSContentChanged();
}

void UserConfigurationViewModel::setPlayerVolume(double playerVolume) noexcept
{
    if (playerVolume < 0) playerVolume = 0;

    if (m_playerVolume == playerVolume) return;

    m_playerVolume = playerVolume;
    emit playerVolumeChanged();
}

void UserConfigurationViewModel::setPlayerQuality(const QString& playerQuality) noexcept
{
    if (m_playerQuality == playerQuality) return;

    m_playerQuality = playerQuality;
    emit playerQualityChanged();
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
    emit customScriptFileChanged();
    emit usingVideoProxyChanged();
    emit usingVideoProxyVLCChanged();
    emit textFontChanged();
    emit autoNextVideoChanged();
    emit autoPlayerTopMostChanged();
    emit apiv2hostChanged();
    emit v2tokenChanged();
    emit torrentDownloadModeChanged();
    emit cachehostChanged();
    emit useCacheFolderChanged();
    emit cacheFolderChanged();
    emit videoServerChanged();
    emit restoreVideoModeChanged();
    emit autoSkipEndingChanged();
    emit useTorrentStreamLibraryChanged();
    emit torrentStreamUIChanged();
    emit pathToTSContentChanged();
    emit playerVolumeChanged();
    emit playerQualityChanged();
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
    object[m_customScriptFileField] = m_customScriptFile;
    object[m_remotePortField] = m_remotePort;
    object[m_sendVolumeToRemoteField] = m_sendVolumeToRemote;
    object[m_sendPlaybackToRemoteField] = m_sendPlaybackToRemote;
    object[m_usingVideoProxyVLCField] = m_usingVideoProxyVLC;
    object[m_textFontField] = m_textFont;
    object[m_usingStrongProxyField] = m_usingStrongProxy;
    object[m_usingVideoProxyMPVField] = m_usingVideoProxyMPV;
    object[m_showVideoPreviewField] = m_showVideoPreview;
    object[m_showReleaseInfoField] = m_showReleaseInfo;
    object[m_autoNextVideoField] = m_autoNextVideo;
    object[m_autoPlayerTopMostField] = m_autoPlayerTopMost;
    object[m_apiv2hostField] = m_apiv2host;
    object[m_v2tokenField] = m_v2token;
    object[m_torrentDownloadModeField] = m_torrentDownloadMode;
    object[m_cachehostField] = m_cachehost;
    object[m_useCacheFolderField] = m_useCacheFolder;
    object[m_cacheFolderField] = m_cacheFolder;
    object[m_videoServerField] = m_videoServer;
    object[m_restoreVideoModeField] = m_restoreVideoMode;
    object[m_autoSkipEndingField] = m_autoSkipEnding;
    object[m_showFullTeamField] = m_showFullTeam;
    object[m_useTorrentStreamLibraryField] = m_useTorrentStreamLibrary;
    object[m_torrentStreamUIField] = m_torrentStreamUI;
    object[m_pathToTSContentField] = m_pathToTSContent;
    object[m_playerVolumeField] = m_playerVolume;
    object[m_playerQualityField] = m_playerQuality;

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
    m_playerBuffer = object.contains(m_playerBufferField) ? object[m_playerBufferField].toInt() : 5082;
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
    m_customScriptFile = object.contains(m_customScriptFileField) ? object[m_customScriptFileField].toString() : "";
    m_remotePort = object.contains(m_remotePortField) ? object[m_remotePortField].toInt() : 12345;
    m_sendVolumeToRemote = object.contains(m_sendVolumeToRemoteField) ? object[m_sendVolumeToRemoteField].toBool() : true;
    m_sendPlaybackToRemote = object.contains(m_sendPlaybackToRemoteField) ? object[m_sendPlaybackToRemoteField].toBool() : false;
    m_usingVideoProxyVLC = object.contains(m_usingVideoProxyVLCField) ? object[m_usingVideoProxyVLCField].toBool() : false;
    m_textFont = object.contains(m_textFontField) ? object[m_textFontField].toString() : "Default";
    m_usingStrongProxy = object.contains(m_usingStrongProxyField) ? object[m_usingStrongProxyField].toBool() : true;
    m_usingVideoProxyMPV = object.contains(m_usingVideoProxyMPVField) ? object[m_usingVideoProxyMPVField].toBool() : true;
    m_showVideoPreview = object.contains(m_showVideoPreviewField) ? object[m_showVideoPreviewField].toBool() : false;
    m_showReleaseInfo = object.contains(m_showReleaseInfoField) ? object[m_showReleaseInfoField].toBool() : true;
    m_autoNextVideo = object.contains(m_autoNextVideoField) ? object[m_autoNextVideoField].toBool() : true;
    m_autoPlayerTopMost = object.contains(m_autoPlayerTopMostField) ? object[m_autoPlayerTopMostField].toBool() : false;
    m_apiv2host = object.contains(m_apiv2hostField) ? object[m_apiv2hostField].toString() : "https://anilibria.top";
    m_v2token = object.contains(m_v2tokenField) ? object[m_v2tokenField].toString() : "";
    m_torrentDownloadMode = object.contains(m_torrentDownloadModeField) ? object[m_torrentDownloadModeField].toInt() : 0;
    m_cachehost = object.contains(m_cachehostField) ? object[m_cachehostField].toString() : "https://raw.githubusercontent.com/trueromanus/LocalCacheChecker/main/cache";
    m_useCacheFolder = object.contains(m_useCacheFolderField) ? object[m_useCacheFolderField].toBool() : false;
    m_cacheFolder = object.contains(m_cacheFolderField) ? object[m_cacheFolderField].toString() : "";
    m_videoServer = object.contains(m_videoServerField) ? object[m_videoServerField].toInt() : 0;
    m_restoreVideoMode = object.contains(m_restoreVideoModeField) ? object[m_restoreVideoModeField].toInt() : 0;
    m_autoSkipEnding = object.contains(m_autoSkipEndingField) ? object[m_autoSkipEndingField].toBool() : false;
    m_showFullTeam = object.contains(m_showFullTeamField) ? object[m_showFullTeamField].toBool() : true;
    m_useTorrentStreamLibrary = object.contains(m_useTorrentStreamLibraryField) ? object[m_useTorrentStreamLibraryField].toBool() : false;
    m_torrentStreamUI = object.contains(m_torrentStreamUIField) ? object[m_torrentStreamUIField].toBool() : false;
    m_pathToTSContent = object.contains(m_pathToTSContentField) ? object[m_pathToTSContentField].toString() : "";
    m_playerVolume = object.contains(m_playerVolumeField) ? object[m_playerVolumeField].toInt() : 80;
    m_playerQuality = object.contains(m_playerQualityField) ? object[m_playerQualityField].toString() : "hd";

    if (m_textFont != "Default") changeFont(m_textFont);
}

void UserConfigurationViewModel::changeFont(const QString &family)
{
    auto instance = QGuiApplication::instance();
    auto guiInstance = static_cast<QGuiApplication*>(instance);
    guiInstance->setFont(QFont(family));
}
