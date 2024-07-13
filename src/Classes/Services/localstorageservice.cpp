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

#include "localstorageservice.h"
#include <QStandardPaths>
#include <QVariant>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QtConcurrent>
#include <QFuture>
#include <QDebug>
#include <QDir>
#include <QFutureWatcher>
#include <QDateTime>
#include <QHashIterator>
#include <QDateTime>
#include <QProcess>
#include <QCoreApplication>
#include <QOperatingSystemVersion>
#include "../Models/fullreleasemodel.h"
#include "../Models/externalplaylistvideo.h"
#include "../../globalhelpers.h"
#include "../../globalconstants.h"

LocalStorageService::LocalStorageService(QObject *parent) : QObject(parent),
    m_UserSettingsModel(new UserSettingsModel()),
    m_Downloads(new QVector<DownloadItemModel*>())
{
    if (IsPortable) {
        auto cachePath = QDir::currentPath() + "/imagecache";
        QDir().mkpath(cachePath);
        qDebug() << "Portable Cache location: " << cachePath;
    } else {
        QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
        QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/imagecache");
        qDebug() << "Cache location: " << QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    }

    createIfNotExistsFile(getUserSettingsCachePath(), "{}");
    createIfNotExistsFile(getDownloadsCachePath(), "[]");

    loadSettings();
    loadDownloads();

    m_OfflineImageCacheService = new OfflineImageCacheService(this);

    m_cacheFiles.append("cinemahall.cache");
    m_cacheFiles.append("downloads.cache");
    m_cacheFiles.append("favorites.cache");
    m_cacheFiles.append("hidedreleases.cache");
    m_cacheFiles.append("history.cache");
    m_cacheFiles.append("myanilibrialist.cache");
    m_cacheFiles.append("notification.cache");
    m_cacheFiles.append("releases.cache");
    m_cacheFiles.append("releasesbackground.cache");
    m_cacheFiles.append("releaseseries.cache");
    m_cacheFiles.append("schedule.cache");
    m_cacheFiles.append("seen.cache");
    m_cacheFiles.append("seenmark.cache");
    m_cacheFiles.append("useractivity.cache");
    m_cacheFiles.append("userconfiguration.cache");
    m_cacheFiles.append("usersettings.cache");
    m_cacheFiles.append("youtube.cache");
    m_cacheFiles.append("applicationtheme.cache");
    m_cacheFiles.append("groups.cache");
    m_cacheFiles.append("lefttoolbar.cache");
}

void LocalStorageService::setup(QSharedPointer<QList<FullReleaseModel*>> releases)
{
    m_releases = releases;
}

void LocalStorageService::invalidateReleasePoster(int id)
{
    m_OfflineImageCacheService->invalidateReleasePoster(id);
}

FullReleaseModel* LocalStorageService::getReleaseFromCache(int id)
{
    foreach (auto cacheRelease, *m_releases) if (cacheRelease->id() == id) return cacheRelease;

    FullReleaseModel* nullObject = new FullReleaseModel();
    nullObject->setId(-1);
    return nullObject;
}

QString LocalStorageService::getUserSettingsCachePath() const
{
    if (IsPortable) {
        return QDir::currentPath() + "/usersettings.cache";
    } else {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/usersettings.cache";
    }
}

QString LocalStorageService::getCinemahallCachePath() const
{
    if (IsPortable) {
        return QDir::currentPath() + "/cinemahall.cache";
    } else {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/cinemahall.cache";
    }
}

QString LocalStorageService::getDownloadsCachePath() const
{
    if (IsPortable) {
        return QDir::currentPath() + "/downloads.cache";
    } else {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/downloads.cache";
    }
}

void LocalStorageService::createIfNotExistsFile(QString path, QString defaultContent)
{
    if (!QFile::exists(path)) {
        QFile createReleasesCacheFile(path);
        createReleasesCacheFile.open(QFile::WriteOnly | QFile::Text);
        createReleasesCacheFile.write(defaultContent.toUtf8());
        createReleasesCacheFile.close();
    }
}

void LocalStorageService::loadSettings()
{
    QFile userSettingsFile(getUserSettingsCachePath());
    if (!userSettingsFile.open(QFile::ReadOnly | QFile::Text)) {
        //TODO: handle this situation
    }
    auto json = userSettingsFile.readAll();
    userSettingsFile.close();

    m_UserSettingsModel->fromJson(json);
}

void LocalStorageService::saveSettings()
{
    QFile settingsFile(getUserSettingsCachePath());
    if (!settingsFile.open(QFile::WriteOnly | QFile::Text)) {
        //TODO: handle this situation
    }

    settingsFile.write(m_UserSettingsModel->toJson().toUtf8());

    settingsFile.close();
}

void LocalStorageService::loadDownloads()
{
    QFile downloadsFile(getDownloadsCachePath());
    if (!downloadsFile.open(QFile::ReadOnly | QFile::Text)) {
        //TODO: handle this situation
    }
    auto json = downloadsFile.readAll();
    downloadsFile.close();

    auto document = QJsonDocument::fromJson(json);
    auto jsonArray = document.array();

    m_Downloads->clear();
    foreach (auto item, jsonArray) {
        auto downloadItem = new DownloadItemModel();
        downloadItem->readFromJsonObject(item.toObject());
        m_Downloads->append(downloadItem);
    }
}

void LocalStorageService::saveDownloads()
{
    QJsonArray downloadsArray;

    foreach (auto downloadItem, *m_Downloads) {
        QJsonObject value;
        downloadItem->writeToJsonObject(value);
        downloadsArray.append(value);
    }

    QFile downloadsFile(getDownloadsCachePath());
    if (!downloadsFile.open(QFile::WriteOnly | QFile::Text)) {
        //TODO: handle this situation
    }

    auto document = QJsonDocument(downloadsArray);
    downloadsFile.write(document.toJson());

    downloadsFile.close();
}

void LocalStorageService::setVolume(double volume)
{
    if (volume < 0) return;

    m_UserSettingsModel->setVolume(volume);

    saveSettings();
}

void LocalStorageService::setVideoQuality(int quality)
{
    m_UserSettingsModel->setQuality(quality);

    saveSettings();
}

void LocalStorageService::setAutoNextVideo(bool autoNextVideo)
{
    m_UserSettingsModel->setAutoNextVideos(autoNextVideo);

    saveSettings();
}

void LocalStorageService::setAutoTopMost(bool autoTopMost)
{
    m_UserSettingsModel->setAutoTopMost(autoTopMost);

    saveSettings();
}

void LocalStorageService::setTorrentDownloadMode(int torrentDownloadMode)
{
    m_UserSettingsModel->setTorrentDownloadMode(torrentDownloadMode);

    saveSettings();
}

bool LocalStorageService::isUseTorrentStreamMode()
{
    return m_UserSettingsModel->torrentDownloadMode() == 2;
}

void LocalStorageService::setNotificationForFavorites(bool notificationForFavorites)
{
    m_UserSettingsModel->setNotificationForFavorites(notificationForFavorites);

    saveSettings();
}

void LocalStorageService::setJumpMinute(int jumpMinute)
{
    m_UserSettingsModel->setJumpMinute(jumpMinute);

    saveSettings();
}

void LocalStorageService::setJumpSecond(int jumpSecond)
{
    m_UserSettingsModel->setJumpSecond(jumpSecond);

    saveSettings();
}

void LocalStorageService::setShowReleaseInfo(bool showReleaseInfo)
{
    m_UserSettingsModel->setShowReleaseInfo(showReleaseInfo);

    saveSettings();
}

void LocalStorageService::setClearFiltersAfterChangeSection(bool clearFiltersAfterChangeSection)
{
    m_UserSettingsModel->setClearFiltersAfterChangeSection(clearFiltersAfterChangeSection);

    saveSettings();
}

void LocalStorageService::setCompactMode(bool compactMode)
{
    m_UserSettingsModel->setCompactMode(compactMode);

    saveSettings();
}

void LocalStorageService::setHideCinemhallButton(bool hideCinemhallButton)
{
    m_UserSettingsModel->setHideCinemhallButton(hideCinemhallButton);

    saveSettings();
}

void LocalStorageService::setHideDownloadButton(bool hideDownloadButton)
{
    m_UserSettingsModel->setHideDownloadButton(hideDownloadButton);

    saveSettings();
}

void LocalStorageService::setHideRandomReleaseButton(bool hideRandomReleaseButton)
{
    m_UserSettingsModel->setHideRandomReleaseButton(hideRandomReleaseButton);

    saveSettings();
}

void LocalStorageService::setHideNotificationButton(bool hideNotificationButton)
{
    m_UserSettingsModel->setHideNotificationButton(hideNotificationButton);

    saveSettings();
}

void LocalStorageService::setHideInfoButton(bool hideInfoButton)
{
    m_UserSettingsModel->setHideInfoButton(hideInfoButton);

    saveSettings();
}

void LocalStorageService::setHideSortButton(bool hideSortButton)
{
    m_UserSettingsModel->setHideSortButton(hideSortButton);

    saveSettings();
}

void LocalStorageService::setHideFilterButton(bool hideFilterButton)
{
    m_UserSettingsModel->setHideFilterButton(hideFilterButton);

    saveSettings();
}

void LocalStorageService::setStartedSection(int startedSection)
{
    m_UserSettingsModel->setStartedSection(startedSection);

    saveSettings();
}

void LocalStorageService::setShowReleaseDescription(bool showReleaseDescription)
{
    m_UserSettingsModel->setShowReleaseDescription(showReleaseDescription);

    saveSettings();
}

void LocalStorageService::setShowVideoPreview(bool showVideoPreview)
{
    m_UserSettingsModel->setShowVideoPreview(showVideoPreview);

    saveSettings();
}

QString LocalStorageService::getUserSettings()
{
    return m_UserSettingsModel->toJson();
}

void LocalStorageService::copyTorrentToFile(QString source, QString target)
{
#ifdef Q_OS_WIN
    QFile::copy(source, target.replace("file:///", ""));
#else
    QFile::copy(source, target.replace("file://", ""));
#endif
}

QString LocalStorageService::getReleasePosterPath(int id, QString url)
{
    return m_OfflineImageCacheService->getReleasePath(id, url);
}

static bool compareExternalPlaylistVideo(const ExternalPlaylistVideo& first, const ExternalPlaylistVideo& second)
{
    return first.order() < second.order();
}

QString LocalStorageService::packAsM3UAndOpen(int id, QString quality, const QList<ReleaseOnlineVideoModel*>& videos)
{
    auto release = getReleaseFromCache(id);
    if (release->id() == -1) return "";

    QList<ExternalPlaylistVideo> result;
    foreach (auto videoItem, videos) {
        ExternalPlaylistVideo video;

        if (quality == "fullhd") video.setAddress(videoItem->fullhd());
        if (quality == "hd") video.setAddress(videoItem->hd());
        if (quality == "sd") video.setAddress(videoItem->sd());
        video.setOrder(videoItem->order());
        video.setName(videoItem->title());
        result.append(video);
    }

    std::sort(result.begin(), result.end(), compareExternalPlaylistVideo);

    QString content = "#EXTM3U\n\n";
    foreach (auto video, result) {
        content += video.exportToM3U();
    }

    QString fileName = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" + QString::number(id) + quality + ".m3u";
    QFile m3uFile(fileName);
    if (!m3uFile.open(QFile::WriteOnly | QFile::Text)) return "";

    m3uFile.write(content.toUtf8());
    m3uFile.close();

    return fileName;
}

QString LocalStorageService::packAsMPCPLAndOpen(int id, QString quality, const QList<ReleaseOnlineVideoModel*>& videos)
{
    auto release = getReleaseFromCache(id);
    if (release->id() == -1) return "";

    QList<ExternalPlaylistVideo> result;
    foreach (auto videoItem, videos) {
        ExternalPlaylistVideo video;

        if (quality == "fullhd") video.setAddress(videoItem->fullhd());
        if (quality == "hd") video.setAddress(videoItem->hd());
        if (quality == "sd") video.setAddress(videoItem->sd());
        video.setOrder(videoItem->order());
        video.setName(videoItem->title());
        result.append(video);
    }

    std::sort(result.begin(), result.end(), compareExternalPlaylistVideo);

    QString content = "MPCPLAYLIST\n";
    foreach (auto video, result) {
        content += video.exportToMPCPL();
    }

    QString fileName = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" + QString::number(id) + ".mpcpl";
    QFile mpcplFile(fileName);
    if (!mpcplFile.open(QFile::WriteOnly | QFile::Text)) return "";

    mpcplFile.write(content.toUtf8());
    mpcplFile.close();

    return fileName;
}

void LocalStorageService::addDownloadItem(int releaseId, int videoId, int quality)
{
    auto result = std::find_if(
        m_Downloads->begin(),
        m_Downloads->end(),
        [releaseId, videoId, quality](DownloadItemModel* model) -> bool {
            return model->quality() == quality && model->releaseId() == releaseId && model->videoId() == videoId;
        }
    );
    if (result != m_Downloads->end()) return;

    auto model = new DownloadItemModel();
    model->setReleaseId(releaseId);
    model->setQuality(quality);
    model->setVideoId(videoId);

    m_Downloads->append(model);

    saveDownloads();
}

void LocalStorageService::finishDownloadItem(int releaseId, int videoId, int quality, const QString &downloadedPath)
{
    auto result = std::find_if(
        m_Downloads->begin(),
        m_Downloads->end(),
        [releaseId, videoId, quality](DownloadItemModel* model) -> bool {
            return model->quality() == quality && model->releaseId() == releaseId && model->videoId() == videoId;
        }
    );
    if (result == m_Downloads->end()) return;

    DownloadItemModel* downloadItem = *result;
    downloadItem->setDownloaded(true);
    downloadItem->setDownloadedUrl(downloadedPath);

    saveDownloads();
}

QString LocalStorageService::getDownloads()
{
    QJsonArray array;
    foreach (auto downloadItem, *m_Downloads) {
        QJsonObject jsonObject;
        downloadItem->writeToJsonObject(jsonObject);
        array.append(jsonObject);
    }

    QJsonDocument jsonDocument(array);

    return jsonDocument.toJson();
}

void LocalStorageService::clearPostersCache()
{
    m_OfflineImageCacheService->clearPosterCache();
}

void LocalStorageService::clearRedundantFilesFromCache()
{
    auto path = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    QDir dir(path);
    auto files = dir.entryList(QDir::Filter::Files);
    QList<QString> needToDelete;
    foreach (auto file, files) {
        auto isIncluded = file.endsWith(".torrent") || file.endsWith(".m3u") || file.endsWith(".mpcpl");
        if (isIncluded) needToDelete.append(file);
    }

    foreach (auto deleteFile, needToDelete) {
        QFile::remove(path + "/" + deleteFile);
    }
}

void LocalStorageService::backupCache(const QString &path)
{
    auto localPath = QString(path);
    auto clearedPath = removeFileProtocol(localPath) + "/";
    foreach (auto cacheFile, m_cacheFiles) {
        copyAndRewriteFile(cacheFile, clearedPath);
    }

    emit backupFilesCopied();
}

void LocalStorageService::restoreBackupCache(const QString &path)
{
    auto localPath = QString(path);
    auto clearedPath = removeFileProtocol(localPath) + "/";

    foreach (auto cacheFile, m_cacheFiles) {
        copyFileToLocalCache(cacheFile, clearedPath);
    }

    emit restoreFileCopied();
}

void LocalStorageService::copyAndRewriteFile(const QString &file, const QString &path)
{
    auto destinationFile = path + file;
    if (QFile::exists(destinationFile)) QFile::remove(destinationFile);

    QFile::copy(getCachePath(file), destinationFile);
}

void LocalStorageService::copyFileToLocalCache(const QString &file, const QString &path)
{
    auto destinationFile = path + file;
    if (!QFile::exists(destinationFile)) return;

    auto localFilePath = getCachePath(file);
    QFile::remove(localFilePath);

    QFile::copy(destinationFile, localFilePath);
}
