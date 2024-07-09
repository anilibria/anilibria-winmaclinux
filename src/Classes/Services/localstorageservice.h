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

#ifndef LOCALSTORAGESERVICE_H
#define LOCALSTORAGESERVICE_H

#include <QObject>
#include "../Models/fullreleasemodel.h"
#include "../Models/usersettingsmodel.h"
#include "../Models/downloaditemmodel.h"
#include "../Models/releaseonlinevideomodel.h"
#include "../Services/offlineimagecacheservice.h"

class LocalStorageService : public QObject
{
    Q_OBJECT

private:
    UserSettingsModel* m_UserSettingsModel;    
    OfflineImageCacheService* m_OfflineImageCacheService;
    QVector<DownloadItemModel*>* m_Downloads;
    QSharedPointer<QList<FullReleaseModel *>> m_releases;
    QStringList m_cacheFiles { QStringList() };

    FullReleaseModel* getReleaseFromCache(int id);
    QString getUserSettingsCachePath() const;
    QString getCinemahallCachePath() const;
    QString getDownloadsCachePath() const;
    void createIfNotExistsFile(QString path, QString defaultContent);
    void loadSettings();
    void saveSettings();
    void loadDownloads();
    void saveDownloads();
    bool importReleasesFromFile(QString path);
    void afterSynchronizedReleases();
public:
    explicit LocalStorageService(QObject *parent = nullptr);

    void setup(QSharedPointer<QList<FullReleaseModel *>> releases);
    void invalidateReleasePoster(int id);

    Q_INVOKABLE void setVolume(double volume);
    Q_INVOKABLE void setVideoQuality(int quality);
    Q_INVOKABLE void setAutoNextVideo(bool autoNextVideo);
    Q_INVOKABLE void setAutoTopMost(bool autoTopMost);
    Q_INVOKABLE void setTorrentDownloadMode(int torrentDownloadMode);
    Q_INVOKABLE bool isUseTorrentStreamMode();
    Q_INVOKABLE void setNotificationForFavorites(bool notificationForFavorites);
    Q_INVOKABLE void setJumpMinute(int jumpMinute);
    Q_INVOKABLE void setJumpSecond(int jumpSecond);
    Q_INVOKABLE void setShowReleaseInfo(bool showReleaseInfo);
    Q_INVOKABLE void setClearFiltersAfterChangeSection(bool clearFiltersAfterChangeSection);
    Q_INVOKABLE void setCompactMode(bool compactMode);
    Q_INVOKABLE void setHideCinemhallButton(bool hideCinemhallButton);
    Q_INVOKABLE void setHideDownloadButton(bool hideDownloadButton);
    Q_INVOKABLE void setHideRandomReleaseButton(bool hideRandomReleaseButton);
    Q_INVOKABLE void setHideNotificationButton(bool hideNotificationButton);
    Q_INVOKABLE void setHideInfoButton(bool hideInfoButton);
    Q_INVOKABLE void setHideSortButton(bool hideSortButton);
    Q_INVOKABLE void setHideFilterButton(bool hideFilterButton);
    Q_INVOKABLE void setStartedSection(int startedSection);
    Q_INVOKABLE void setShowReleaseDescription(bool showReleaseDescription);
    Q_INVOKABLE void setShowVideoPreview(bool showVideoPreview);

    Q_INVOKABLE QString getUserSettings();
    Q_INVOKABLE void copyTorrentToFile(QString source, QString target);
    Q_INVOKABLE QString getReleasePosterPath(int id, QString url);
    Q_INVOKABLE QString packAsM3UAndOpen(int id, QString quality, const QList<ReleaseOnlineVideoModel*>& videos);
    Q_INVOKABLE QString packAsMPCPLAndOpen(int id, QString quality, const QList<ReleaseOnlineVideoModel*>& videos);
    Q_INVOKABLE void addDownloadItem(int releaseId, int videoId, int quality);
    Q_INVOKABLE void finishDownloadItem(int releaseId, int videoId, int quality, const QString& downloadedPath);
    Q_INVOKABLE QString getDownloads();
    Q_INVOKABLE void clearPostersCache();
    Q_INVOKABLE void clearRedundantFilesFromCache();
    Q_INVOKABLE void backupCache(const QString& path);
    Q_INVOKABLE void restoreBackupCache(const QString& path);

private:
    void copyAndRewriteFile(const QString& file, const QString& path);
    void copyFileToLocalCache(const QString& file, const QString& path);

signals:
    void countCinemahallChanged();
    void backupFilesCopied();
    void restoreFileCopied();
};

#endif // LOCALSTORAGESERVICE_H
