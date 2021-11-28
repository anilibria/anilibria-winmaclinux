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
#include <QRandomGenerator>
#include "../Models/releasemodel.h"
#include "../Models/fullreleasemodel.h"
#include "../Models/changesmodel.h"
#include "../Models/seenmodel.h"
#include "../Models/seenmarkmodel.h"
#include "../Models/externalplaylistvideo.h"

LocalStorageService::LocalStorageService(QObject *parent) : QObject(parent),
    m_UserSettingsModel(new UserSettingsModel()),
    m_CinemaHall(new QVector<int>()),
    m_Downloads(new QVector<DownloadItemModel*>()),
    m_CountCinemahall(0)
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
    createIfNotExistsFile(getCinemahallCachePath(), "[]");
    createIfNotExistsFile(getDownloadsCachePath(), "[]");

    loadSettings();
    loadDownloads();
    loadCinemahall();

    m_OfflineImageCacheService = new OfflineImageCacheService(this);
}

void LocalStorageService::setup(QSharedPointer<QList<FullReleaseModel*>> releases)
{
    m_releases = releases;
}

void LocalStorageService::invalidateReleasePoster(int id)
{
    m_OfflineImageCacheService->invalidateReleasePoster(id);
}

void LocalStorageService::setCountCinemahall(int countCinemahall) noexcept
{
    if (m_CountCinemahall == countCinemahall) return;

    m_CountCinemahall = countCinemahall;
    emit countCinemahallChanged();
}

/*void LocalStorageService::updateAllReleases(const QString &releases)
{
    QFuture<void> future = QtConcurrent::run(
        [=] {
            QJsonParseError jsonError;
            QJsonDocument jsonDocument = QJsonDocument::fromJson(releases.toUtf8(), &jsonError);
            if (jsonError.error != 0) return; //TODO: handle this situation and show message

            auto jsonReleases = jsonDocument.array();
            auto newReleasesCount = m_ChangesModel->newReleases()->count();
            auto newOnlineSeriesCount = m_ChangesModel->newOnlineSeries()->count();
            auto newTorrentsCount = m_ChangesModel->newTorrents()->count();
            auto newTorrentSeriesCount = m_ChangesModel->newTorrentSeries()->count();
            auto newReleases = m_ChangesModel->newReleases();
            auto newOnlineSeries = m_ChangesModel->newOnlineSeries();
            auto newTorrents = m_ChangesModel->newTorrents();
            auto newTorrentSeries = m_ChangesModel->newTorrentSeries();
            bool isEmptyReleases = m_CachedReleases->count() == 0;

            foreach (QJsonValue jsonRelease, jsonReleases) {
                ReleaseModel releaseModel;
                releaseModel.readFromApiModel(jsonRelease.toObject());

                FullReleaseModel* currentReleaseCacheModel = getReleaseFromCache(releaseModel.id());

                FullReleaseModel* newReleaseModel = mapToFullReleaseModel(releaseModel);

                if (currentReleaseCacheModel->id() > -1) {
                    auto releaseId = currentReleaseCacheModel->id();
                    if (currentReleaseCacheModel->countOnlineVideos() != newReleaseModel->countOnlineVideos()) {
                        auto isExists = newOnlineSeries->contains(releaseId);
                        if (!isExists) newOnlineSeries->append(releaseId);
                    }
                    if (currentReleaseCacheModel->countTorrents() != newReleaseModel->countTorrents()) {
                        if (!newTorrents->contains(currentReleaseCacheModel->id())) newTorrents->append(currentReleaseCacheModel->id());
                    }
                    if (currentReleaseCacheModel->torrents() != newReleaseModel->torrents()) {
                        if (!newTorrentSeries->contains(currentReleaseCacheModel->id())) newTorrentSeries->append(currentReleaseCacheModel->id());
                    }
                    if (currentReleaseCacheModel->poster() != newReleaseModel->poster()) m_OfflineImageCacheService->invalidateReleasePoster(currentReleaseCacheModel->id());

                    m_CachedReleases->removeOne(currentReleaseCacheModel);

                } else {
                    if (!isEmptyReleases) {
                        int newReleaseId = newReleaseModel->id();
                        if (!newReleases->contains(newReleaseId)) newReleases->append(newReleaseId);
                    }
                }
                m_CachedReleases->append(newReleaseModel);
            }

            saveCachedReleasesToFile();
            //updateReleasesInnerCache();
            saveChanges();

            QString newEntities;
            if (newReleases->count() > newReleasesCount) newEntities += "Новых релизов " + QString::number(newReleases->count() - newReleasesCount) + " ";
            if (newOnlineSeries->count() > newOnlineSeriesCount) newEntities += "Новых серий " + QString::number(newOnlineSeries->count() - newOnlineSeriesCount) + " ";
            if (newTorrents->count() > newTorrentsCount) newEntities += "Новых торрентов " + QString::number(newTorrents->count() - newTorrentsCount) + " ";
            if (newTorrentSeries->count() > newTorrentSeriesCount) newEntities += "Новых серий в торрентах " + QString::number(newTorrentSeries->count() - newTorrentSeriesCount);
            setNewEntities(newEntities);
        }
    );
    m_AllReleaseUpdatedWatcher->setFuture(future);
}*/

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

void LocalStorageService::loadCinemahall()
{
    QFile cinemahallFile(getCinemahallCachePath());
    if (!cinemahallFile.open(QFile::ReadOnly | QFile::Text)) {
        //TODO: handle this situation
    }
    auto json = cinemahallFile.readAll();
    cinemahallFile.close();

    auto document = QJsonDocument::fromJson(json);
    auto jsonArray = document.array();

    m_CinemaHall->clear();
    foreach (auto item, jsonArray) m_CinemaHall->append(item.toInt());

    setCountCinemahall(m_CinemaHall->count());
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

void LocalStorageService::saveCinemahall()
{
    QJsonArray cinemahallArray;

    foreach (auto releaseId, *m_CinemaHall) {
        QJsonValue value(releaseId);
        cinemahallArray.append(value);
    }

    QFile cinemahallFile(getCinemahallCachePath());
    if (!cinemahallFile.open(QFile::WriteOnly | QFile::Text)) {
        //TODO: handle this situation
    }

    auto document = QJsonDocument(cinemahallArray);
    cinemahallFile.write(document.toJson());

    cinemahallFile.close();

    setCountCinemahall(m_CinemaHall->count());
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

QString LocalStorageService::packAsM3UAndOpen(int id, QString quality)
{
    auto release = getReleaseFromCache(id);
    if (release->id() == -1) return "";

    auto jsonDocument = QJsonDocument::fromJson(release->videos().toUtf8());
    auto videosArray = jsonDocument.array();
    QList<ExternalPlaylistVideo> videos;
    foreach (auto jsonVideo, videosArray) {
        ExternalPlaylistVideo video;
        video.setAddress(jsonVideo[quality].toString());
        video.setOrder(jsonVideo["id"].toInt());
        video.setName("Серия " + QString::number(video.order()));
        videos.append(video);
    }

    std::sort(videos.begin(), videos.end(), compareExternalPlaylistVideo);

    QString content = "# EXTM3U\n\n";
    foreach (auto video, videos) {
        content += video.exportToM3U();
    }

    QString fileName = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" + QString::number(id) + ".m3u";
    QFile m3uFile(fileName);
    if (!m3uFile.open(QFile::WriteOnly | QFile::Text)) return "";

    m3uFile.write(content.toUtf8());
    m3uFile.close();

    return fileName;
}

QString LocalStorageService::packAsMPCPLAndOpen(int id, QString quality)
{
    auto release = getReleaseFromCache(id);
    if (release->id() == -1) return "";

    auto jsonDocument = QJsonDocument::fromJson(release->videos().toUtf8());
    auto videosArray = jsonDocument.array();
    QList<ExternalPlaylistVideo> videos;
    foreach (auto jsonVideo, videosArray) {
        ExternalPlaylistVideo video;
        video.setAddress(jsonVideo[quality].toString());
        video.setOrder(jsonVideo["id"].toInt());
        video.setName("Серия " + QString::number(video.order()));
        videos.append(video);
    }

    std::sort(videos.begin(), videos.end(), compareExternalPlaylistVideo);

    QString content = "MPCPLAYLIST\n";
    foreach (auto video, videos) {
        content += video.exportToMPCPL();
    }

    QString fileName = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" + QString::number(id) + ".mpcpl";
    QFile mpcplFile(fileName);
    if (!mpcplFile.open(QFile::WriteOnly | QFile::Text)) return "";

    mpcplFile.write(content.toUtf8());
    mpcplFile.close();

    return fileName;
}

void LocalStorageService::addToCinemahall(const QList<int>& ids)
{
    foreach(auto id, ids) {
        if (m_CinemaHall->contains(id)) continue;

        m_CinemaHall->append(id);
    }

    saveCinemahall();    
}

QString LocalStorageService::getCinemahallReleases()
{
    QVector<FullReleaseModel*> cinemahallReleases(m_CinemaHall->count());

    foreach (auto releaseItem, *m_releases) {
        if (!m_CinemaHall->contains(releaseItem->id())) continue;

        cinemahallReleases[m_CinemaHall->indexOf(releaseItem->id())] = releaseItem;
    }

    QJsonArray releases;
    foreach (auto cinemahallRelease, cinemahallReleases) {
        if (cinemahallRelease == nullptr) continue; //Remove deplicates

        QJsonObject jsonValue;
        cinemahallRelease->writeToJson(jsonValue);
        releases.append(jsonValue);
    }

    QJsonDocument saveDoc(releases);
    return saveDoc.toJson();
}

QString LocalStorageService::getReleases(const QList<int> &ids)
{
    QVector<FullReleaseModel*> resultReleases;

    foreach (auto releaseItem, *m_releases) {
        if (!ids.contains(releaseItem->id())) continue;

        resultReleases.append(releaseItem);
    }

    QJsonArray releases;
    foreach (auto release, resultReleases) {
        if (release == nullptr) continue;

        QJsonObject jsonValue;
        release->writeToJson(jsonValue);
        releases.append(jsonValue);
    }

    QJsonDocument saveDoc(releases);
    return saveDoc.toJson();
}

bool LocalStorageService::hasCinemahallReleases()
{
    return m_CinemaHall->count() > 0;
}

void LocalStorageService::reorderReleaseInCinemahall(int reorderId, int targetId)
{
    auto placeIndex = m_CinemaHall->indexOf(targetId);
    auto oldIndex = m_CinemaHall->indexOf(reorderId);

    if (placeIndex < 0 || placeIndex > m_CinemaHall->count()) return;
    if (oldIndex < 0 || oldIndex > m_CinemaHall->count()) return;

    m_CinemaHall->remove(oldIndex);
    m_CinemaHall->insert(placeIndex, reorderId);

    saveCinemahall();
}

void LocalStorageService::deleteReleasesFromCinemahall(const QList<int> &ids)
{
    foreach (auto id, ids) {
        auto index = m_CinemaHall->indexOf(id);
        if (index == -1) continue;

        m_CinemaHall->remove(index);
    }

    saveCinemahall();
}

void LocalStorageService::deleteAllReleasesFromCinemahall()
{
    m_CinemaHall->clear();

    saveCinemahall();
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

QList<QString> LocalStorageService::getDownloadsReleases()
{
    QSet<int> releaseIds;
    QList<QString> releases;
    QJsonDocument jsonDocument;

    foreach (auto downloadItem, *m_Downloads) {
        if (releaseIds.contains(downloadItem->releaseId())) continue;

        releaseIds.insert(downloadItem->releaseId());

        auto iterator = std::find_if(
            m_releases->begin(),
            m_releases->end(),
            [downloadItem](FullReleaseModel* release) -> bool {
                return release->id() == downloadItem->releaseId();
            }
        );
        if (iterator == m_releases->end()) continue;

        QJsonObject jsonValue;
        (*iterator)->writeToJson(jsonValue);

        jsonDocument.setObject(jsonValue);
        releases.append(jsonDocument.toJson());
    }

    return releases;
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
