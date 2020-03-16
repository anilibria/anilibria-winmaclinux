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
#include "../../globalconstants.h"

class LocalStorageService : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool isChangesExists READ isChangesExists WRITE setIsChangesExists NOTIFY isChangesExistsChanged)
private:
    QFutureWatcher<void>* m_AllReleaseUpdatedWatcher;
    QList<FullReleaseModel>* m_CachedReleases;
    ChangesModel* m_ChangesModel;
    QHash<int, SeenModel*>* m_SeenModels;
    QHash<QString,bool>* m_SeenMarkModels;
    QHash<int, HistoryModel*>* m_HistoryModels;
    UserSettingsModel* m_UserSettingsModel;
    bool m_IsChangesExists;

    QString videosToJson(QList<OnlineVideoModel>& videos);
    QString torrentsToJson(QList<ReleaseTorrentModel>& torrents);
    FullReleaseModel getReleaseFromCache(int id);
    FullReleaseModel mapToFullReleaseModel(ReleaseModel& releaseModel);
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
    QString getSeensCachePath() const;
    QString getSeenMarksCachePath() const;
    QString getHistoryCachePath() const;
    QString getYoutubeCachePath() const;
    QString getUserSettingsCachePath() const;
    QString getNotificationCachePath() const;
    void createIfNotExistsFile(QString path, QString defaultContent);
    void saveChanges();
    void resetChanges();
    void loadSeens();
    void loadSeenMarks();
    void saveSeenMarks();
    void loadHistory();
    void saveHistory();
    void loadSettings();
    void saveSettings();
    QHash<int, int> getAllSeenMarkCount();

public:
    explicit LocalStorageService(QObject *parent = nullptr);

    bool isChangesExists();
    void setIsChangesExists(bool isChangesExists);

    Q_INVOKABLE void updateAllReleases(const QString& releases);
    Q_INVOKABLE void updateYoutubeItems(const QString& youtubeItems);
    Q_INVOKABLE QString getRelease(int id);
    Q_INVOKABLE QString getRandomRelease();
    Q_INVOKABLE QString getChanges();    
    Q_INVOKABLE QString getReleasesByFilter(int page, QString title, int section, QString description, QString type, QString genres, bool genresOr, QString voices, bool voicesOr, QString years, QString seasones, QString statuses, int sortingField, bool soringDescending);
    Q_INVOKABLE void setSchedule(QString schedule);
    Q_INVOKABLE QString getSchedule();
    Q_INVOKABLE void updateFavorites(QString data);
    Q_INVOKABLE QList<int> getFavorites();
    Q_INVOKABLE void clearFavorites();
    Q_INVOKABLE void updateReleasesInnerCache();
    Q_INVOKABLE QList<int> getChangesCounts();
    Q_INVOKABLE void resetAllChanges();
    Q_INVOKABLE QString getVideoSeens();
    Q_INVOKABLE QString getVideoSeen(int id);
    Q_INVOKABLE QString getLastVideoSeen();
    Q_INVOKABLE void setVideoSeens(int id, int videoId, double videoPosition);
    Q_INVOKABLE void saveVideoSeens();
    Q_INVOKABLE void setSeenMark(int id, int seriaId, bool marked);    
    Q_INVOKABLE QList<int> getReleseSeenMarks(int id, int count);
    Q_INVOKABLE void setToReleaseHistory(int id, int type);
    Q_INVOKABLE void setVolume(double volume);
    Q_INVOKABLE void setVideoQuality(int quality);
    Q_INVOKABLE void setAutoNextVideo(bool autoNextVideo);
    Q_INVOKABLE void setAutoTopMost(bool autoTopMost);
    Q_INVOKABLE void setTorrentDownloadMode(int torrentDownloadMode);
    Q_INVOKABLE void setNotificationForFavorites(bool notificationForFavorites);
    Q_INVOKABLE QString getUserSettings();
    Q_INVOKABLE QString getYoutubeItems();
    Q_INVOKABLE void copyTorrentToFile(QString source, QString target);

signals:
    void allReleasesFinished();
    void isChangesExistsChanged();

public slots:
    void allReleasesUpdated();    

};

#endif // LOCALSTORAGESERVICE_H
