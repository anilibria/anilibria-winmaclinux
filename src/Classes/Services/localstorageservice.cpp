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
#include "../Models/releasemodel.h"
#include "../Models/fullreleasemodel.h"
#include "../Models/changesmodel.h"
#include "../Models/seenmodel.h"
#include "../Models/seenmarkmodel.h"

using namespace std;

const int FavoriteSection = 1;
const int NewReleasesSection = 2;
const int NewOnlineSeriesSection = 3;
const int NewTorrentsSection = 4;
const int ScheduleSection = 5;
const int NewTorrentSeriesSection = 6;
const int HistorySection = 7;
const int WatchHistorySection = 8;
const int SeenHistorySection = 9;
const int SeeningHistorySection = 10;
const int NotSeeningHistorySection = 11;

LocalStorageService::LocalStorageService(QObject *parent) : QObject(parent),
    m_CachedReleases(new QList<FullReleaseModel>()),
    m_ChangesModel(new ChangesModel()),
    m_SeenModels(new QHash<int, SeenModel*>()),
    m_SeenMarkModels(new QHash<QString, bool>()),
    m_HistoryModels(new QHash<int, HistoryModel*>()),
    m_UserSettingsModel(new UserSettingsModel()),
    m_IsChangesExists(false)
{
    m_AllReleaseUpdatedWatcher = new QFutureWatcher<void>(this);

    QDir cacheDicrectory(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    QDir().mkpath(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    qDebug() << "Cache location: " << QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);

    createIfNotExistsFile(getReleasesCachePath(), "[]");
    createIfNotExistsFile(getScheduleCachePath(), "{}");
    createIfNotExistsFile(getFavoritesCachePath(), "[]");
    createIfNotExistsFile(getSeensCachePath(), "[]");
    createIfNotExistsFile(getSeenMarksCachePath(), "[]");
    createIfNotExistsFile(getHistoryCachePath(), "[]");
    createIfNotExistsFile(getUserSettingsCachePath(), "{}");
    createIfNotExistsFile(getNotificationCachePath(), "{ \"newReleases\": [], \"newOnlineSeries\": [], \"newTorrents\": [], \"newTorrentSeries\": [] }");
    createIfNotExistsFile(getYoutubeCachePath(), "[]");
    QString favoritespath = getFavoritesCachePath();

    updateReleasesInnerCache();

    auto changesJson = getChanges();
    m_ChangesModel->fromJson(changesJson);

    loadSeens();   
    loadSeenMarks();
    loadHistory();
    loadSettings();

    resetChanges();    

    connect(m_AllReleaseUpdatedWatcher, SIGNAL(finished()), this, SLOT(allReleasesUpdated()));
}

bool LocalStorageService::isChangesExists()
{
    return m_IsChangesExists;
}

void LocalStorageService::setIsChangesExists(bool isChangesExists)
{
    if (m_IsChangesExists == isChangesExists) return;

    m_IsChangesExists = isChangesExists;
    emit isChangesExistsChanged();
}

void LocalStorageService::updateAllReleases(const QString &releases)
{
    QFuture<void> future = QtConcurrent::run(
        [=] {
            QJsonParseError jsonError;
            QJsonDocument jsonDocument = QJsonDocument::fromJson(releases.toUtf8(), &jsonError);
            if (jsonError.error != 0) return; //TODO: handle this situation and show message

            auto jsonReleases = jsonDocument.array();
            auto variantList = jsonReleases.toVariantList();
            auto newReleases = m_ChangesModel->newReleases();
            auto newOnlineSeries = m_ChangesModel->newOnlineSeries();
            auto newTorrents = m_ChangesModel->newTorrents();
            auto newTorrentSeries = m_ChangesModel->newTorrentSeries();
            bool isEmptyReleases = m_CachedReleases->count() == 0;

            foreach (QJsonValue jsonRelease, jsonReleases) {
                ReleaseModel releaseModel;
                releaseModel.readFromApiModel(jsonRelease.toObject());

                FullReleaseModel currentReleaseCacheModel = getReleaseFromCache(releaseModel.id());

                FullReleaseModel newReleaseModel = mapToFullReleaseModel(releaseModel);

                if (currentReleaseCacheModel.id() > -1) {
                    auto releaseId = currentReleaseCacheModel.id();
                    if (currentReleaseCacheModel.countOnlineVideos() != newReleaseModel.countOnlineVideos()) {
                        auto isExists = newOnlineSeries->contains(releaseId);
                        if (!isExists) newOnlineSeries->append(releaseId);
                    }
                    if (currentReleaseCacheModel.countTorrents() != newReleaseModel.countTorrents()) {
                        if (!newTorrents->contains(currentReleaseCacheModel.id())) newTorrents->append(currentReleaseCacheModel.id());
                    }
                    if (currentReleaseCacheModel.torrents() != newReleaseModel.torrents()) {
                        if (!newTorrentSeries->contains(currentReleaseCacheModel.id())) newTorrentSeries->append(currentReleaseCacheModel.id());
                    }
                    m_CachedReleases->removeOne(currentReleaseCacheModel);

                } else {
                    if (!isEmptyReleases) {
                        int newReleaseId = newReleaseModel.id();
                        if (!newReleases->contains(newReleaseId)) newReleases->append(newReleaseId);
                    }
                }
                m_CachedReleases->append(newReleaseModel);
            }

            saveCachedReleasesToFile();
            updateReleasesInnerCache();
            saveChanges();
        }
    );
    m_AllReleaseUpdatedWatcher->setFuture(future);
}

void LocalStorageService::updateYoutubeItems(const QString &youtubeItems)
{
    QFile youtubeFile(getYoutubeCachePath());
    if (!youtubeFile.open(QFile::WriteOnly | QFile::Text)) {
        //TODO: handle this situation
    }

    youtubeFile.write(youtubeItems.toUtf8());

    youtubeFile.close();
}

QString LocalStorageService::videosToJson(QList<OnlineVideoModel> &videos)
{
    QJsonArray videosArray;
    foreach (auto video, videos) {
        QJsonObject jsonObject;
        video.writeToJson(jsonObject);
        videosArray.append(jsonObject);
    }
    QJsonDocument videoDocument(videosArray);
    QString videosJson(videoDocument.toJson());
    return videosJson;
}

QString LocalStorageService::torrentsToJson(QList<ReleaseTorrentModel> &torrents)
{
    QJsonArray torrentsArray;
    foreach (auto torrent, torrents) {
        QJsonObject jsonObject;
        torrent.writeToJson(jsonObject);
        torrentsArray.append(jsonObject);
    }
    QJsonDocument torrentDocument(torrentsArray);
    QString torrentJson(torrentDocument.toJson());
    return torrentJson;
}

FullReleaseModel LocalStorageService::getReleaseFromCache(int id)
{
    foreach (auto cacheRelease, *m_CachedReleases) if (cacheRelease.id() == id) return cacheRelease;

    FullReleaseModel nullObject;
    nullObject.setId(-1);
    return nullObject;
}

FullReleaseModel LocalStorageService::mapToFullReleaseModel(ReleaseModel &releaseModel)
{
    FullReleaseModel model;

    auto torrents = releaseModel.torrents();
    auto torrentJson = torrentsToJson(torrents);

    auto videos = releaseModel.videos();
    auto videosJson = videosToJson(videos);

    auto voices = releaseModel.voices().join(", ");
    if (voices.length() == 0) voices = "Не указано";

    auto genres = releaseModel.genres().join(", ");
    if (genres.length() == 0) genres = "Не указано";

    model.setId(releaseModel.id());
    model.setTitle(releaseModel.title());
    model.setCode(releaseModel.code());
    model.setOriginalName(releaseModel.names().last());
    model.setRating(releaseModel.rating());
    model.setSeries(releaseModel.series());
    model.setStatus(releaseModel.status());
    model.setType(releaseModel.type());
    model.setTimestamp(releaseModel.timestamp().toInt());
    model.setYear(releaseModel.year());
    model.setSeason(releaseModel.season());
    model.setCountTorrents(torrents.count());
    model.setCountOnlineVideos(videos.count());
    model.setDescription(releaseModel.description());
    model.setAnnounce(releaseModel.announce());
    model.setVoicers(voices);
    model.setGenres(genres);
    model.setVideos(videosJson);
    model.setTorrents(torrentJson);
    model.setPoster(releaseModel.poster());

    return model;
}

void LocalStorageService::saveCachedReleasesToFile()
{
    QJsonArray releasesArray;
    foreach (auto release, *m_CachedReleases) {
        QJsonObject jsonObject;
        release.writeToJson(jsonObject);
        releasesArray.append(jsonObject);
    }
    QJsonDocument document(releasesArray);

    QFile file(getReleasesCachePath());
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(document.toJson());
    file.close();
}

QStringList LocalStorageService::getAllFavorites()
{
    QFile favoritesCacheFile(getFavoritesCachePath());
    favoritesCacheFile.open(QFile::ReadOnly | QFile::Text);
    QString favoritesJson = favoritesCacheFile.readAll();
    favoritesCacheFile.close();

    QJsonParseError jsonError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(favoritesJson.toUtf8(), &jsonError);
    auto favorites = jsonDocument.array();
    QStringList result;
    foreach (auto favorite, favorites) result.append(QString::number(favorite.toInt()));

    return result;
}

QMap<int, int> LocalStorageService::getScheduleAsMap()
{
    QFile scheduleCacheFile(getScheduleCachePath());
    scheduleCacheFile.open(QFile::ReadOnly | QFile::Text);
    QString scheduleJson = scheduleCacheFile.readAll();
    scheduleCacheFile.close();

    QJsonParseError jsonError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(scheduleJson.toUtf8(), &jsonError);
    auto schedule = jsonDocument.object();
    auto keys = schedule.keys();
    QMap<int, int> result;
    foreach (auto key, keys) {
        auto scheduleDay = schedule.value(key).toString();
        result[key.toInt()] = scheduleDay.toInt();
    }

    return result;
}

bool LocalStorageService::checkOrCondition(QStringList source, QStringList target)
{
    foreach(QString sourceItem, source) {
        if (target.filter(sourceItem, Qt::CaseInsensitive).count() > 0) return true;
    }

    return false;
}

bool LocalStorageService::checkAllCondition(QStringList source, QStringList target)
{
    int counter = 0;
    foreach(QString sourceItem, source) {
        if (target.filter(sourceItem, Qt::CaseInsensitive).count() > 0) counter++;
    }

    return counter == source.count();
}

void LocalStorageService::removeTrimsInStringCollection(QStringList& list) {
    QMutableStringListIterator iterator(list);
    while (iterator.hasNext()) {
        QString value = iterator.next();
        iterator.setValue(value.trimmed());
    }
}

int LocalStorageService::randomBetween(int low, int high, uint seed)
{
    qsrand(seed);
    return (qrand() % ((high + 1) - low) + low);
}

QString LocalStorageService::getReleasesCachePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/releases.cache";
}

QString LocalStorageService::getFavoritesCachePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/favorites.cache";
}

QString LocalStorageService::getScheduleCachePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/schedule.cache";
}

QString LocalStorageService::getSeensCachePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/seen.cache";
}

QString LocalStorageService::getSeenMarksCachePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/seenmark.cache";
}

QString LocalStorageService::getHistoryCachePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/history.cache";
}

QString LocalStorageService::getYoutubeCachePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/youtube.cache";
}

QString LocalStorageService::getUserSettingsCachePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/usersettings.cache";
}

QString LocalStorageService::getNotificationCachePath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/notification.cache";
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

void LocalStorageService::saveChanges()
{
    QFile notificationFile(getNotificationCachePath());
    if (!notificationFile.open(QFile::WriteOnly | QFile::Text)) {
        //TODO: handle this situation
    }
    notificationFile.write(m_ChangesModel->toJson().toUtf8());
    notificationFile.close();

    resetChanges();
}

void LocalStorageService::resetChanges()
{
    bool isChanges = false;
    if (m_ChangesModel->newReleases()->count() > 0) isChanges = true;
    if (m_ChangesModel->newOnlineSeries()->count() > 0) isChanges = true;
    if (m_ChangesModel->newTorrents()->count() > 0) isChanges = true;
    if (m_ChangesModel->newTorrentSeries()->count() > 0) isChanges = true;

    setIsChangesExists(isChanges);
}

void LocalStorageService::loadSeens()
{
    QFile seenFile(getSeensCachePath());
    if (!seenFile.open(QFile::ReadOnly | QFile::Text)) {
        //TODO: handle this situation
    }
    auto seenJson = seenFile.readAll();
    seenFile.close();

    auto document = QJsonDocument::fromJson(seenJson);
    auto jsonSeens = document.array();

    foreach (auto item, jsonSeens) {
        SeenModel* seenModel = new SeenModel();
        seenModel->readFromJson(item);
        if (!m_SeenModels->contains(seenModel->id())) {
            m_SeenModels->insert(seenModel->id(), seenModel);
        }
    }
}

void LocalStorageService::loadSeenMarks()
{
    QFile seenMarkFile(getSeenMarksCachePath());
    if (!seenMarkFile.open(QFile::ReadOnly | QFile::Text)) {
        //TODO: handle this situation
    }
    auto seenMarkJson = seenMarkFile.readAll();
    seenMarkFile.close();

    auto document = QJsonDocument::fromJson(seenMarkJson);
    auto jsonSeenMarks = document.array();

    foreach (auto item, jsonSeenMarks) {
        m_SeenMarkModels->insert(item.toString(), true);
    }
}

void LocalStorageService::saveSeenMarks()
{
    QJsonArray array;

    QHashIterator<QString,bool> iterator(*m_SeenMarkModels);
    while (iterator.hasNext()) {
        iterator.next();

        QJsonValue value(iterator.key());
        array.append(value);
    }

    QJsonDocument seenDocument(array);
    QString seenMarkJson(seenDocument.toJson());

    QFile seenMarkFile(getSeenMarksCachePath());
    if (!seenMarkFile.open(QFile::WriteOnly | QFile::Text)) {
        //TODO: handle this situation
    }
    seenMarkFile.write(seenMarkJson.toUtf8());
    seenMarkFile.close();
}

void LocalStorageService::loadHistory()
{
    QFile historyCacheFile(getHistoryCachePath());
    if (!historyCacheFile.open(QFile::ReadOnly | QFile::Text)) {
        //TODO: handle this situation
    }
    auto historyJson = historyCacheFile.readAll();
    historyCacheFile.close();

    auto document = QJsonDocument::fromJson(historyJson);
    auto historyItems = document.array();

    foreach (auto item, historyItems) {
        HistoryModel* historyModel = new HistoryModel();
        historyModel->readFromJson(item);

        m_HistoryModels->insert(historyModel->id(), historyModel);
    }
}

void LocalStorageService::saveHistory()
{
    QJsonArray historyItems;

    QHashIterator<int, HistoryModel*> iterator(*m_HistoryModels);
    while(iterator.hasNext()) {
        iterator.next();

        QJsonObject jsonObject;
        HistoryModel* historyModel = iterator.value();
        historyModel->writeToJson(jsonObject);
        historyItems.append(jsonObject);
    }

    QFile historyFile(getHistoryCachePath());
    if (!historyFile.open(QFile::WriteOnly | QFile::Text)) {
        //TODO: handle this situation
    }

    auto document = QJsonDocument(historyItems);
    historyFile.write(document.toJson());

    historyFile.close();
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

QHash<int, int> LocalStorageService::getAllSeenMarkCount()
{
    QHash<int, int> result;
    QHashIterator<QString, bool> iterator(*m_SeenMarkModels);
    while(iterator.hasNext()) {
        iterator.next();

        QString key = iterator.key();
        auto keyParts = key.split(".");
        auto releaseId = keyParts.first().toInt();
        if (!result.contains(releaseId)) {
            result[releaseId] = 1;
        } else {
            result[releaseId] += 1;
        }
    }
    return result;
}

QString LocalStorageService::getRelease(int id)
{
    QListIterator<FullReleaseModel> i(*m_CachedReleases);

    while(i.hasNext()) {
        auto release = i.next();
        if (release.id() == id) {
            QJsonObject jsonValue;
            release.writeToJson(jsonValue);

            QJsonDocument saveDoc(jsonValue);
            return saveDoc.toJson();
        }
    }

    return "{}";
}

QString LocalStorageService::getRandomRelease()
{
    auto count = m_CachedReleases->count() - 1;

    auto position = randomBetween(1, count, static_cast<uint>(QDateTime::currentMSecsSinceEpoch()));

    auto release = m_CachedReleases->at(position);

    QJsonObject jsonValue;
    release.writeToJson(jsonValue);

    QJsonDocument saveDoc(jsonValue);
    return saveDoc.toJson();
}

QString LocalStorageService::getChanges()
{
    QFile notificationFile(getNotificationCachePath());
    if (!notificationFile.open(QFile::ReadOnly | QFile::Text)) {
        //TODO: handle this situation
    }
    auto changes = notificationFile.readAll();
    notificationFile.close();

    return changes;
}

static bool compareTimeStamp(const FullReleaseModel& first, const FullReleaseModel& second)
{
    return first.timestamp() < second.timestamp();
}

static bool compareTimeStampDescending(const FullReleaseModel& first, const FullReleaseModel& second)
{
    return first.timestamp() > second.timestamp();
}

static bool compareName(const FullReleaseModel& first, const FullReleaseModel& second)
{
    return first.title() < second.title();
}

static bool compareNameDescending(const FullReleaseModel& first, const FullReleaseModel& second)
{
    return first.title() > second.title();
}

static bool compareYear(const FullReleaseModel& first, const FullReleaseModel& second)
{
    return first.year() < second.year();
}

static bool compareYearDescending(const FullReleaseModel& first, const FullReleaseModel& second)
{
    return first.year() > second.year();
}

static bool compareRating(const FullReleaseModel& first, const FullReleaseModel& second)
{
    return first.rating() < second.rating();
}

static bool compareRatingDescending(const FullReleaseModel& first, const FullReleaseModel& second)
{
    return first.rating() > second.rating();
}

static bool compareStatus(const FullReleaseModel& first, const FullReleaseModel& second)
{
    return first.status() < second.status();
}

static bool compareStatusDescending(const FullReleaseModel& first, const FullReleaseModel& second)
{
    return first.status() > second.status();
}

static bool compareOriginalName(const FullReleaseModel& first, const FullReleaseModel& second)
{
    return first.originalName() < second.originalName();
}

static bool compareOriginalNameDescending(const FullReleaseModel& first, const FullReleaseModel& second)
{
    return first.originalName() > second.originalName();
}

static bool compareSeason(const FullReleaseModel& first, const FullReleaseModel& second)
{
    return first.season() < second.season();
}

static bool compareSeasonDescending(const FullReleaseModel& first, const FullReleaseModel& second)
{
    return first.season() > second.season();
}

QString LocalStorageService::getReleasesByFilter(int page, QString title, int section, QString description, QString type, QString genres, bool genresOr, QString voices, bool voicesOr, QString years, QString seasones, QString statuses, int sortingField, bool sortingDescending)
{
    int pageSize = 12;
    int startIndex = (page - 1) * pageSize;

    QStringList userFavorites = getAllFavorites();
    QMap<int, int> scheduled = getScheduleAsMap();
    auto seenMarks = getAllSeenMarkCount();

    std::function<bool (const FullReleaseModel&, const FullReleaseModel&)> scheduleComparer = [scheduled](const FullReleaseModel& first, const FullReleaseModel& second) {
        auto firstId = first.id();
        auto firstScheduled = scheduled.contains(firstId) ? scheduled[firstId] : 9;

        auto secondId = second.id();
        auto secondScheduled = scheduled.contains(secondId) ? scheduled[secondId] : 9;

        return firstScheduled < secondScheduled;
    };

    std::function<bool (const FullReleaseModel&, const FullReleaseModel&)> scheduleDescendingComparer = [scheduled](const FullReleaseModel& first, const FullReleaseModel& second) {
        auto firstId = first.id();
        auto firstScheduled = scheduled.contains(firstId) ? scheduled[firstId] : 9;

        auto secondId = second.id();
        auto secondScheduled = scheduled.contains(secondId) ? scheduled[secondId] : 9;

        return firstScheduled > secondScheduled;
    };

    std::function<bool (const FullReleaseModel&, const FullReleaseModel&)> historyComparer = [this](const FullReleaseModel& first, const FullReleaseModel& second) {
        int leftTimestamp = 0;
        int firstId = first.id();
        if (m_HistoryModels->contains(first.id())) {
            auto historyItem = m_HistoryModels->value(firstId);
            leftTimestamp = historyItem->timestamp();
        }

        int rightTimestamp = 0;
        int secondId = second.id();
        if (m_HistoryModels->contains(second.id())) {
            auto historyItem = m_HistoryModels->value(secondId);
            rightTimestamp = historyItem->timestamp();
        }

        return leftTimestamp < rightTimestamp;
    };

    std::function<bool (const FullReleaseModel&, const FullReleaseModel&)> historyDescendingComparer = [this](const FullReleaseModel& first, const FullReleaseModel& second) {
        int leftTimestamp = 0;
        int firstId = first.id();
        if (m_HistoryModels->contains(firstId)) {
           auto historyItem = m_HistoryModels->value(firstId);
           leftTimestamp = historyItem->timestamp();
        }

        int rightTimestamp = 0;
        int secondId = second.id();
        if (m_HistoryModels->contains(secondId)) {
            auto historyItem = m_HistoryModels->value(secondId);
            rightTimestamp = historyItem->timestamp();
        }

        return leftTimestamp > rightTimestamp;
    };

    std::function<bool (const FullReleaseModel&, const FullReleaseModel&)> watchHistoryComparer = [this](const FullReleaseModel& first, const FullReleaseModel& second) {
        int leftTimestamp = 0;
        int firstId = first.id();
        if (m_HistoryModels->contains(first.id())) {
           auto historyItem = m_HistoryModels->value(firstId);
           leftTimestamp = historyItem->watchTimestamp();
        }

        int rightTimestamp = 0;
        int secondId = second.id();
        if (m_HistoryModels->contains(second.id())) {
           auto historyItem = m_HistoryModels->value(secondId);
           rightTimestamp = historyItem->watchTimestamp();
        }

        return leftTimestamp < rightTimestamp;
    };

    std::function<bool (const FullReleaseModel&, const FullReleaseModel&)> watchHistoryDescendingComparer = [this](const FullReleaseModel& first, const FullReleaseModel& second) {
        int leftTimestamp = 0;
        int firstId = first.id();
        if (m_HistoryModels->contains(first.id())) {
            auto historyItem = m_HistoryModels->value(firstId);
           leftTimestamp = historyItem->watchTimestamp();
        }

        int rightTimestamp = 0;
        int secondId = second.id();
        if (m_HistoryModels->contains(second.id())) {
            auto historyItem = m_HistoryModels->value(secondId);
           rightTimestamp = historyItem->watchTimestamp();
        }

        return leftTimestamp > rightTimestamp;
    };

    QJsonArray releases;

    switch (sortingField) {
        case 0:
            std::sort(m_CachedReleases->begin(), m_CachedReleases->end(), sortingDescending ? compareTimeStampDescending : compareTimeStamp);
            break;
        case 1: //Дню в расписании
            std::sort(m_CachedReleases->begin(), m_CachedReleases->end(), sortingDescending ? scheduleDescendingComparer : scheduleComparer);
            break;
        case 2: //Имени
            std::sort(m_CachedReleases->begin(), m_CachedReleases->end(), sortingDescending ? compareNameDescending : compareName);
            break;
        case 3: //Году
            std::sort(m_CachedReleases->begin(), m_CachedReleases->end(), sortingDescending ? compareYearDescending : compareYear);
            break;
        case 4: //Рейтингу
            std::sort(m_CachedReleases->begin(), m_CachedReleases->end(), sortingDescending ? compareRatingDescending : compareRating);
            break;
        case 5: //Статусу
            std::sort(m_CachedReleases->begin(), m_CachedReleases->end(), sortingDescending ? compareStatusDescending : compareStatus);
            break;
        case 6: //Оригинальному имени
            std::sort(m_CachedReleases->begin(), m_CachedReleases->end(), sortingDescending ? compareOriginalNameDescending : compareOriginalName);
            break;
        case 7: //История
            std::sort(m_CachedReleases->begin(), m_CachedReleases->end(), sortingDescending ? historyDescendingComparer : historyComparer);
            break;
        case 8: //История просмотра
            std::sort(m_CachedReleases->begin(), m_CachedReleases->end(), sortingDescending ? watchHistoryDescendingComparer : watchHistoryComparer);
            break;
        case 9: //Сезону
            std::sort(m_CachedReleases->begin(), m_CachedReleases->end(), sortingDescending ? compareSeasonDescending : compareSeason);
            break;
    }

    foreach (auto releaseItem, *m_CachedReleases) {

        if (!title.isEmpty() && !releaseItem.title().toLower().contains(title.toLower())) continue;
        if (!description.isEmpty() && !releaseItem.description().toLower().contains(description.toLower())) continue;
        if (!type.isEmpty() && !releaseItem.type().toLower().contains(type.toLower())) continue;

        //years
        if (!years.isEmpty()) {
            QStringList yearsList = years.split(",");
            removeTrimsInStringCollection(yearsList);
            int year = releaseItem.year().toInt();
            QStringList singleYear;
            singleYear.append(QString::number(year));

            if (!checkOrCondition(yearsList, singleYear)) continue;
        }

        //statuses
        if (!statuses.isEmpty()) {
            QStringList statusesList = statuses.split(",");
            removeTrimsInStringCollection(statusesList);
            QStringList singleStatus;
            singleStatus.append(releaseItem.status());

            if (!checkOrCondition(statusesList, singleStatus)) continue;
        }

        //seasons
        if (!seasones.isEmpty()) {
            QStringList seasonesList = seasones.split(",");
            removeTrimsInStringCollection(seasonesList);
            auto season = releaseItem.season();
            QStringList singleSeason;
            singleSeason.append(season);

            if (!checkOrCondition(seasonesList, singleSeason)) continue;
        }

        //genres
        if (!genres.isEmpty()) {
            QStringList genresList = genres.split(",");
            removeTrimsInStringCollection(genresList);
            QStringList releaseGenresList = releaseItem.genres().split(",");
            if (genresOr) {
                if (!checkAllCondition(genresList, releaseGenresList)) continue;
            } else {
                if (!checkOrCondition(genresList, releaseGenresList)) continue;
            }
        }

        //voices
        if (!voices.isEmpty()) {
            QStringList voicesList = voices.split(",");
            QStringList releaseVoicesList = releaseItem.voicers().split(",");
            if (voicesOr) {
                if (!checkAllCondition(voicesList, releaseVoicesList)) continue;
            } else {
                if (!checkOrCondition(voicesList, releaseVoicesList)) continue;
            }
        }

        //favorites section
        if (section == FavoriteSection) {
            auto releaseId = releaseItem.id();
            if (!userFavorites.contains(QString::number(releaseId))) continue;
        }

        if (section == ScheduleSection && !scheduled.contains(releaseItem.id())) continue;

        if (section == NewReleasesSection && !m_ChangesModel->newReleases()->contains(releaseItem.id())) continue;

        if (section == NewOnlineSeriesSection && !m_ChangesModel->newOnlineSeries()->contains(releaseItem.id())) continue;

        if (section == NewTorrentsSection && !m_ChangesModel->newTorrents()->contains(releaseItem.id())) continue;

        if (section == NewTorrentSeriesSection && !m_ChangesModel->newTorrentSeries()->contains(releaseItem.id())) continue;

        if (section == HistorySection && !(m_HistoryModels->contains(releaseItem.id()) && m_HistoryModels->value(releaseItem.id())->timestamp() > 0)) continue;

        if (section == WatchHistorySection && !(m_HistoryModels->contains(releaseItem.id()) && m_HistoryModels->value(releaseItem.id())->watchTimestamp() > 0)) continue;

        auto countReleaseSeenVideos = seenMarks.contains(releaseItem.id()) ? seenMarks.value(releaseItem.id()) : 0;
        auto isAllSeens = countReleaseSeenVideos == releaseItem.countOnlineVideos() && releaseItem.countOnlineVideos() > 0;
        if (section == SeenHistorySection && !isAllSeens) continue;

        if (section == SeeningHistorySection && !(countReleaseSeenVideos > 0 && !isAllSeens)) continue;

        if (section == NotSeeningHistorySection && !(countReleaseSeenVideos == 0)) continue;

        if (startIndex > 0) {
            startIndex--;
            continue;
        }

        QJsonObject jsonValue;
        releaseItem.writeToJson(jsonValue);
        releases.append(jsonValue);

        if (releases.count() >= pageSize) break;
    }

    QJsonDocument saveDoc(releases);
    return saveDoc.toJson();
}

void LocalStorageService::setSchedule(QString schedule)
{
    QFile scheduleCacheFile(getScheduleCachePath());
    scheduleCacheFile.open(QFile::WriteOnly | QFile::Text);
    scheduleCacheFile.write(schedule.toUtf8());
    scheduleCacheFile.close();
}

QString LocalStorageService::getSchedule()
{
    QFile scheduleCacheFile(getScheduleCachePath());
    scheduleCacheFile.open(QFile::ReadOnly | QFile::Text);
    QString scheduleJson = scheduleCacheFile.readAll();
    scheduleCacheFile.close();
    return scheduleJson;
}

void LocalStorageService::updateFavorites(QString data)
{
    QFile favoritesCacheFile(getFavoritesCachePath());
    favoritesCacheFile.open(QFile::WriteOnly | QFile::Text);
    favoritesCacheFile.write(data.toUtf8());
    favoritesCacheFile.close();
}

QList<int> LocalStorageService::getFavorites()
{
    auto favorites = getAllFavorites();
    QList<int> ids;
    foreach(auto favorite, favorites) ids.append(favorite.toInt());

    return ids;
}

void LocalStorageService::clearFavorites()
{
    updateFavorites("[]");
}

void LocalStorageService::updateReleasesInnerCache()
{
    m_CachedReleases->clear();

    QFile releasesCacheFile(getReleasesCachePath());

    releasesCacheFile.open(QFile::ReadOnly | QFile::Text);

    QString releasesJson = releasesCacheFile.readAll();
    releasesCacheFile.close();
    auto releasesArray = QJsonDocument::fromJson(releasesJson.toUtf8()).array();

    foreach (auto release, releasesArray) {
        FullReleaseModel jsonRelease;
        jsonRelease.readFromJson(release);

        m_CachedReleases->append(jsonRelease);
    }
}

QList<int> LocalStorageService::getChangesCounts()
{
    QList<int> result;

    result.append(m_ChangesModel->newReleases()->count());
    result.append(m_ChangesModel->newOnlineSeries()->count());
    result.append(m_ChangesModel->newTorrents()->count());
    result.append(m_ChangesModel->newTorrentSeries()->count());

    return result;
}

void LocalStorageService::resetAllChanges()
{
    m_ChangesModel->newReleases()->clear();
    m_ChangesModel->newOnlineSeries()->clear();
    m_ChangesModel->newTorrents()->clear();
    m_ChangesModel->newTorrentSeries()->clear();

    saveChanges();
}

QString LocalStorageService::getVideoSeens()
{
    QJsonArray array;

    QHashIterator<int, SeenModel*> iterator(*m_SeenModels);
    while (iterator.hasNext()) {
        iterator.next();

        QJsonObject object;
        iterator.value()->writeToJson(object);
        array.append(object);
    }

    QJsonDocument seenDocument(array);
    QString seenJson(seenDocument.toJson());
    return seenJson;
}

QString LocalStorageService::getVideoSeen(int id)
{
    if (m_SeenModels->contains(id)) {
        auto seenModel = m_SeenModels->value(id);
        QJsonObject object;
        seenModel->writeToJson(object);

        QJsonDocument seenDocument(object);
        QString seenJson(seenDocument.toJson());
        return seenJson;
    } else {
        return "{}";
    }
}

static bool compareSeenTimeStampDescending(const SeenModel* first, const SeenModel* second)
{
    return first->timestamp() > second->timestamp();
}

QString LocalStorageService::getLastVideoSeen()
{
    if (m_SeenModels->count() == 0) return "";


    auto models = m_SeenModels->values();

    std::sort(models.begin(), models.end(), compareSeenTimeStampDescending);
    auto first = models.first();

    QJsonObject jsonObject;
    first->writeToJson(jsonObject);
    QJsonDocument document(jsonObject);
    return document.toJson();
}

void LocalStorageService::setVideoSeens(int id, int videoId, double videoPosition)
{
    QDateTime now = QDateTime::currentDateTime();
    auto timestamp = now.toTime_t();
    if (!m_SeenModels->contains(id)) {
        SeenModel* seenModel = new SeenModel();
        seenModel->setId(id);
        seenModel->setVideoId(videoId);
        seenModel->setVideoPosition(videoPosition);
        seenModel->setTimestamp(static_cast<int>(timestamp));

        m_SeenModels->insert(id, seenModel);
    } else {
        auto existingSeenModel = m_SeenModels->value(id);
        existingSeenModel->setVideoId(videoId);
        existingSeenModel->setVideoPosition(videoPosition);
        existingSeenModel->setTimestamp(static_cast<int>(timestamp));
    }

    saveVideoSeens();
}

void LocalStorageService::saveVideoSeens()
{
    QJsonArray array;

    QHashIterator<int, SeenModel*> iterator(*m_SeenModels);
    while (iterator.hasNext()) {
        iterator.next();

        QJsonObject object;
        iterator.value()->writeToJson(object);
        array.append(object);
    }

    QJsonDocument seenDocument(array);
    QString seenJson(seenDocument.toJson());

    QFile seenFile(getSeensCachePath());
    if (!seenFile.open(QFile::WriteOnly | QFile::Text)) {
        //TODO: handle this situation
    }
    seenFile.write(seenJson.toUtf8());
    seenFile.close();
}

void LocalStorageService::setSeenMark(int id, int seriaId, bool marked)
{
    auto key = QString::number(id) + "." + QString::number(seriaId);
    if (marked) {
        if (!m_SeenMarkModels->contains(key)) m_SeenMarkModels->insert(key, true);
    } else {
        if (m_SeenMarkModels->contains(key)) m_SeenMarkModels->remove(key);
    }
    saveSeenMarks();
}

QList<int> LocalStorageService::getReleseSeenMarks(int id, int count)
{
    QList<int> result;
    for (int i=0; i < count; i++) {
        auto key = QString::number(id) + "." + QString::number(i);
        if (m_SeenMarkModels->contains(key)) {
            result.append(i);
        }
    }
    return result;
}

void LocalStorageService::setToReleaseHistory(int id, int type)
{
    HistoryModel* item;
    if (m_HistoryModels->contains(id)) {
        item = m_HistoryModels->value(id);
    } else {
        item = new HistoryModel();
        item->setId(id);
        m_HistoryModels->insert(id, item);
    }

    QDateTime now = QDateTime::currentDateTime();
    int timestamp = static_cast<int>(now.toTime_t());

    switch (type) {
        case HistoryReleaseCardMode:
            item->setTimestamp(timestamp);
            break;
        case HistoryWatchReleaseCardMode:
            item->setWatchTimestamp(timestamp);
            break;
    }

    saveHistory();
}

void LocalStorageService::setVolume(double volume)
{
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

QString LocalStorageService::getUserSettings()
{
    return m_UserSettingsModel->toJson();
}

QString LocalStorageService::getYoutubeItems()
{
    QFile youtubeFile(getYoutubeCachePath());
    if (!youtubeFile.open(QFile::ReadOnly | QFile::Text)) {
        //TODO: handle this situation
    }

    auto data = youtubeFile.readAll();
    youtubeFile.close();

    return data;
}

void LocalStorageService::copyTorrentToFile(QString source, QString target)
{
#ifdef Q_OS_WIN
    QFile::copy(source, target.replace("file:///", ""));
#else
    QFile::copy(source, target.replace("file://", ""));
#endif
}

void LocalStorageService::allReleasesUpdated()
{
    emit allReleasesFinished();
}
