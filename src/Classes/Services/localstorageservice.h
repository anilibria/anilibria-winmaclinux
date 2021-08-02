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
#include "../Models/onlinevideomodel.h"
#include "../Models/releasemodel.h"
#include "../Models/releasetorrentmodel.h"
#include "../Models/fullreleasemodel.h"
#include "../Models/changesmodel.h"
#include "../Models/seenmodel.h"
#include "../Models/seenmarkmodel.h"
#include "../Models/historymodel.h"
#include "../Models/usersettingsmodel.h"
#include "../Models/downloaditemmodel.h"
#include "../../globalconstants.h"
#include "../Services/offlineimagecacheservice.h"

class LocalStorageService : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isChangesExists READ isChangesExists WRITE setIsChangesExists NOTIFY isChangesExistsChanged)
    Q_PROPERTY(int countReleases READ countReleases WRITE setCountReleases NOTIFY countReleasesChanged)
    Q_PROPERTY(int countSeens READ countSeens WRITE setCountSeens NOTIFY countSeensChanged)
    Q_PROPERTY(int countCinemahall READ countCinemahall WRITE setCountCinemahall NOTIFY countCinemahallChanged)
    Q_PROPERTY(QString newEntities READ newEntities WRITE setNewEntities NOTIFY newEntitiesChanged)

private:
    QFutureWatcher<void>* m_AllReleaseUpdatedWatcher;
    QList<FullReleaseModel*>* m_CachedReleases;
    ChangesModel* m_ChangesModel;
    QHash<QString, bool>* m_SeenMarkModels;
    QHash<int, HistoryModel*>* m_HistoryModels;
    UserSettingsModel* m_UserSettingsModel;    
    bool m_IsChangesExists;
    OfflineImageCacheService* m_OfflineImageCacheService;
    int m_CountReleases;
    QVector<int>* m_CinemaHall;
    QVector<int>* m_HidedReleases;
    int m_CountSeens;
    QVector<DownloadItemModel*>* m_Downloads;
    int m_CountCinemahall;
    QString m_newEntities;

    QString videosToJson(QList<OnlineVideoModel>& videos);
    QString torrentsToJson(QList<ReleaseTorrentModel>& torrents);
    FullReleaseModel* getReleaseFromCache(int id);
    FullReleaseModel* mapToFullReleaseModel(ReleaseModel& releaseModel);
    void saveCachedReleasesToFile();
    QStringList getAllFavorites();
    QMap<int, int> getScheduleAsMap();
    bool checkOrCondition(QStringList source, QStringList target);
    bool checkAllCondition(QStringList source, QStringList target);
    void removeTrimsInStringCollection(QStringList& list);
    int randomBetween(int low, int high, uint seed);
    QString getReleasesCachePath() const;
    QString getFavoritesCachePath() const;
    QString getScheduleCachePath() const;
    QString getSeenMarksCachePath() const;
    QString getHistoryCachePath() const;
    QString getYoutubeCachePath() const;
    QString getUserSettingsCachePath() const;
    QString getNotificationCachePath() const;
    QString getCinemahallCachePath() const;
    QString getDownloadsCachePath() const;
    QString getHidedReleasesCachePath() const;
    QString newEntities() const { return m_newEntities; }
    void createIfNotExistsFile(QString path, QString defaultContent);
    void saveChanges();
    void resetChanges();
    void loadSeenMarks();
    void loadHistory();
    void saveHistory();
    void loadSettings();
    void saveSettings();
    void loadDownloads();
    void loadCinemahall();
    void loadHidedReleases();
    void saveDownloads();
    void saveCinemahall();
    void saveHidedReleases();
    QHash<int, int> getAllSeenMarkCount();
    int countOnlyFavorites(QList<int>* changes, QSet<int>* favorites);
    void setSeenMarkForRelease(int id, int countSeries, bool marked);
    void recalculateSeenCounts();    
    bool importReleasesFromFile(QString path);
    void afterSynchronizedReleases();
    void setNewEntities(const QString& newEntities);
public:
    explicit LocalStorageService(QObject *parent = nullptr);

    bool isChangesExists();
    void setIsChangesExists(bool isChangesExists);

    int countReleases() { return m_CountReleases; }
    void setCountReleases(int countReleases) noexcept;

    int countSeens() { return m_CountSeens; }
    void setCountSeens(int countSeens) noexcept;

    int countCinemahall() { return m_CountCinemahall; }
    void setCountCinemahall(int countCinemahall) noexcept;

    Q_INVOKABLE void updateAllReleases(const QString& releases);
    Q_INVOKABLE void updateYoutubeItems(const QString& youtubeItems);
    Q_INVOKABLE QString getRelease(int id);
    Q_INVOKABLE QString getReleaseByCode(const QString& code);
    Q_INVOKABLE QString getRandomRelease();
    Q_INVOKABLE QString getChanges();    
    Q_INVOKABLE QString getReleasesByFilter(int page, QString title, int section, QString description, QString type, QString genres, bool genresOr, QString voices, bool voicesOr, QString years, QString seasones, QString statuses, int sortingField, bool soringDescending, int favoriteMark, int seenMark, const QStringList& alphabets);
    Q_INVOKABLE void setSchedule(QString schedule);
    Q_INVOKABLE QString getSchedule();
    Q_INVOKABLE void updateFavorites(QString data);
    Q_INVOKABLE QList<int> getFavorites();
    Q_INVOKABLE void clearFavorites();
    Q_INVOKABLE void updateReleasesInnerCache();
    Q_INVOKABLE QList<int> getChangesCounts();
    Q_INVOKABLE void resetAllChanges();
    Q_INVOKABLE void resetReleaseChanges(int releaseId);
    Q_INVOKABLE void setToReleaseHistory(int id, int type);
    Q_INVOKABLE void setVolume(double volume);
    Q_INVOKABLE void setVideoQuality(int quality);
    Q_INVOKABLE void setAutoNextVideo(bool autoNextVideo);
    Q_INVOKABLE void setAutoTopMost(bool autoTopMost);
    Q_INVOKABLE void setTorrentDownloadMode(int torrentDownloadMode);
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

    Q_INVOKABLE QString getUserSettings();
    Q_INVOKABLE QString getYoutubeItems();
    Q_INVOKABLE void copyTorrentToFile(QString source, QString target);
    Q_INVOKABLE QString getReleasePosterPath(int id, QString url);   
    Q_INVOKABLE QString packAsM3UAndOpen(int id, QString quality);
    Q_INVOKABLE QString packAsMPCPLAndOpen(int id, QString quality);
    Q_INVOKABLE void addToCinemahall(const QList<int>& ids);
    Q_INVOKABLE QString getCinemahallReleases();
    Q_INVOKABLE QString getReleases(const QList<int>& ids);
    Q_INVOKABLE bool hasCinemahallReleases();
    Q_INVOKABLE void reorderReleaseInCinemahall(int reorderId, int targetId);
    Q_INVOKABLE void deleteReleasesFromCinemahall(const QList<int>& ids);
    Q_INVOKABLE void deleteAllReleasesFromCinemahall();
    Q_INVOKABLE void addDownloadItem(int releaseId, int videoId, int quality);
    Q_INVOKABLE void finishDownloadItem(int releaseId, int videoId, int quality, const QString& downloadedPath);
    Q_INVOKABLE QList<QString> getDownloadsReleases();
    Q_INVOKABLE QString getDownloads();
    Q_INVOKABLE void clearPostersCache();
    Q_INVOKABLE bool importReleasesFromExternalFile(QString path);
    Q_INVOKABLE void addToHidedReleases(const QList<int>& ids);
    Q_INVOKABLE void removeFromHidedReleases(const QList<int>& ids);
    Q_INVOKABLE void removeAllHidedReleases();
    Q_INVOKABLE bool isReleaseInHided(int id);
    Q_INVOKABLE void recalculateSeenCountsFromFile();

signals:
    void allReleasesFinished();
    void isChangesExistsChanged();
    void countReleasesChanged(int countReleases);
    void countSeensChanged(int countSeens);
    void countCinemahallChanged();
    void newEntitiesChanged();

public slots:
    void allReleasesUpdated();    

};

#endif // LOCALSTORAGESERVICE_H
