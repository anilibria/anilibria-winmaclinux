#include <QClipboard>
#include <QGuiApplication>
#include <QPixmap>
#include <QImageReader>
#include "releasesviewmodel.h"
#include "../../globalhelpers.h"

ReleasesViewModel::ReleasesViewModel(QObject *parent) : QObject(parent)
{
    m_items = new ReleasesListModel(m_releases, m_scheduleReleases, m_userFavorites, m_hiddenReleases, m_seenMarks, m_historyItems, this);

    m_imageBackgroundViewModel->setOptionFilePath("releasesbackground");

    m_sectionNames.append("Все релизы");
    m_sectionNames.append("Избранное");
    m_sectionNames.append("Новые релизы");
    m_sectionNames.append("Релизы с новыми сериями");
    m_sectionNames.append("Релизы с новыми торрентами");
    m_sectionNames.append("Расписание");
    m_sectionNames.append("Релизы с обновленными торрентами");
    m_sectionNames.append("История");
    m_sectionNames.append("История просмотра");
    m_sectionNames.append("Просмотренные");
    m_sectionNames.append("Просматриваемые");
    m_sectionNames.append("Не просмотренные");
    m_sectionNames.append("Скрытые релизы");

    createIfNotExistsFile(getCachePath(releasesCacheFileName), "[]");
    createIfNotExistsFile(getCachePath(scheduleCacheFileName), "{}");
    createIfNotExistsFile(getCachePath(favoriteCacheFileName), "[]");
    createIfNotExistsFile(getCachePath(hidedReleasesCacheFileName), "[]");
    createIfNotExistsFile(getCachePath(seenMarkCacheFileName), "[]");
    createIfNotExistsFile(getCachePath(historyCacheFileName), "[]");
    createIfNotExistsFile(getCachePath(notificationCacheFileName), "{ \"newReleases\": [], \"newOnlineSeries\": [], \"newTorrents\": [], \"newTorrentSeries\": [] }");

    loadReleases();
    loadSchedules();
    loadFavorites();
    loadHidedReleases();
    loadSeenMarks();
    loadHistory();
    loadChanges();
}

void ReleasesViewModel::setCountReleases(int countReleases) noexcept
{
    if (countReleases == m_countReleases) return;

    m_countReleases = countReleases;
    emit countReleasesChanged();
}

void ReleasesViewModel::setCountSeens(int countSeens) noexcept
{
    if (m_countSeens == countSeens) return;

    m_countSeens = countSeens;
    emit countSeensChanged();
}

bool ReleasesViewModel::isChangesExists() const noexcept
{
    if (m_releaseChanges->newReleases()->count() > 0) return true;

    auto newOnlineSeries = m_notificationForFavorites ? getCountOnlyFavorites(m_releaseChanges->newOnlineSeries()) : m_releaseChanges->newOnlineSeries()->count();
    if (newOnlineSeries > 0) return true;

    auto newTorrents = m_notificationForFavorites ? getCountOnlyFavorites(m_releaseChanges->newTorrents()) : m_releaseChanges->newTorrents()->count();
    if (newTorrents > 0) return true;

    auto newTorrentSeries = m_notificationForFavorites ? getCountOnlyFavorites(m_releaseChanges->newTorrentSeries()) : m_releaseChanges->newTorrentSeries()->count();
    if (newTorrentSeries > 0) return true;

    return false;
}

void ReleasesViewModel::setCountFavorites(int countFavorites) noexcept
{
    if (m_countFavorites == countFavorites) return;

    m_countFavorites = countFavorites;
    emit countFavoritesChanged();
}

int ReleasesViewModel::newOnlineSeriesCount() const noexcept
{
    if (m_notificationForFavorites) {
        return getCountOnlyFavorites(m_releaseChanges->newOnlineSeries());
    } else {
        return m_releaseChanges->newOnlineSeries()->count();
    }
}

int ReleasesViewModel::newTorrentsCount() const noexcept
{
    if (m_notificationForFavorites) {
        return getCountOnlyFavorites(m_releaseChanges->newTorrents());
    } else {
        return m_releaseChanges->newTorrents()->count();
    }
}

int ReleasesViewModel::newTorrentSeriesCount() const noexcept
{
    if (m_notificationForFavorites) {
        return getCountOnlyFavorites(m_releaseChanges->newTorrentSeries());
    } else {
        return m_releaseChanges->newTorrentSeries()->count();
    }
}

void ReleasesViewModel::setNotificationForFavorites(bool notificationForFavorites) noexcept
{
    if (m_notificationForFavorites == notificationForFavorites) return;

    m_notificationForFavorites = notificationForFavorites;
    emit notificationForFavoritesChanged();
    emit isChangesExistsChanged();
    emit newOnlineSeriesCountChanged();
    emit newTorrentsCountChanged();
    emit newTorrentSeriesCountChanged();
}

void ReleasesViewModel::setShowSidePanel(bool showSidePanel) noexcept
{
    if (m_showSidePanel == showSidePanel) return;

    m_showSidePanel = showSidePanel;
    emit showSidePanelChanged();
}

QString ReleasesViewModel::getScheduleDay(const QString &dayNumber) const noexcept
{
    if (dayNumber.isEmpty()) return "";

    auto day = dayNumber.toInt();
    switch (day){
        case 1: return QString("понедельник");
        case 2: return QString("вторник");
        case 3: return QString("среда");
        case 4: return QString("четверг");
        case 5: return QString("пятница");
        case 6: return QString("суббота");
        case 7: return QString("воскресенье");
        default: return "";
    }
}

void ReleasesViewModel::copyToClipboard(const QString &text) const noexcept
{
    if (text.isEmpty()) return;

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);
}

void ReleasesViewModel::copyImageToClipboard(const QString &imagePath) const
{
    if (imagePath.isEmpty()) return;
    auto pathToImage = imagePath;
#ifdef Q_OS_WIN
    pathToImage = pathToImage.replace("file:///", "");
#else
    pathToImage = pathToImage.replace("file://", "");
#endif

    QPixmap pixmap;
    if (!pixmap.load(pathToImage, "jpg")) return;

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setPixmap(pixmap);
}

QString ReleasesViewModel::getVkontakteCommentPage(const QString &code) const noexcept
{
    return "https://vk.com/widget_comments.php?app=5315207&width=100%&_ver=1&limit=8&norealtime=0&url=https://www.anilibria.tv/release/" + code + ".html";
}

void ReleasesViewModel::resetAllChanges() noexcept
{
    m_releaseChanges->newReleases()->clear();
    m_releaseChanges->newOnlineSeries()->clear();
    m_releaseChanges->newTorrents()->clear();
    m_releaseChanges->newTorrentSeries()->clear();

    saveChanges();
}

void ReleasesViewModel::loadReleases()
{
    m_releases->clear();
    m_releasesMap->clear();

    QFile releasesCacheFile(getCachePath(releasesCacheFileName));

    if (!releasesCacheFile.open(QFile::ReadOnly | QFile::Text)) return;

    QString releasesJson = releasesCacheFile.readAll();
    releasesCacheFile.close();
    auto releasesArray = QJsonDocument::fromJson(releasesJson.toUtf8()).array();

    foreach (auto release, releasesArray) {
        FullReleaseModel* jsonRelease = new FullReleaseModel();
        jsonRelease->readFromJson(release);

        m_releases->append(jsonRelease);
        m_releasesMap->insert(jsonRelease->id(), jsonRelease);
    }

    setCountReleases(m_releases->count());
}

void ReleasesViewModel::loadSchedules()
{
    m_scheduleReleases->clear();

    QFile scheduleCacheFile(getCachePath(scheduleCacheFileName));
    if (!scheduleCacheFile.open(QFile::ReadOnly | QFile::Text)) return;
    QString scheduleJson = scheduleCacheFile.readAll();
    scheduleCacheFile.close();

    QJsonParseError jsonError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(scheduleJson.toUtf8(), &jsonError);
    auto schedule = jsonDocument.object();
    auto keys = schedule.keys();
    foreach (auto key, keys) {
        auto scheduleDay = schedule.value(key).toString();
        m_scheduleReleases->insert(key.toInt(), scheduleDay.toInt());
    }
}

void ReleasesViewModel::saveSchedule(QString json)
{
    QFile scheduleCacheFile(getCachePath(scheduleCacheFileName));
    scheduleCacheFile.open(QFile::WriteOnly | QFile::Text);
    scheduleCacheFile.write(json.toUtf8());
    scheduleCacheFile.close();

    //reload schedules after saving
    loadSchedules();
}

void ReleasesViewModel::saveFavoritesFromJson(QString data)
{
    QFile favoritesCacheFile(getCachePath(favoriteCacheFileName));
    favoritesCacheFile.open(QFile::WriteOnly | QFile::Text);
    favoritesCacheFile.write(data.toUtf8());
    favoritesCacheFile.close();
}

void ReleasesViewModel::saveFavorites()
{
    QJsonArray array;
    foreach (auto item, *m_userFavorites) {
        QJsonValue value(item);
        array.append(value);
    }

    QJsonDocument document(array);
    auto json = document.toJson();

    QFile favoritesCacheFile(getCachePath(favoriteCacheFileName));
    favoritesCacheFile.open(QFile::WriteOnly | QFile::Text);
    favoritesCacheFile.write(json);
    favoritesCacheFile.close();
}

void ReleasesViewModel::loadFavorites()
{
    QFile favoritesCacheFile(getCachePath(favoriteCacheFileName));
    if (!favoritesCacheFile.open(QFile::ReadOnly | QFile::Text)) return;
    QString favoritesJson = favoritesCacheFile.readAll();
    favoritesCacheFile.close();

    QJsonParseError jsonError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(favoritesJson.toUtf8(), &jsonError);
    auto favorites = jsonDocument.array();
    m_userFavorites->clear();
    foreach (auto favorite, favorites) m_userFavorites->append(favorite.toInt());

    setCountFavorites(m_userFavorites->count());
}

void ReleasesViewModel::clearFavorites()
{
    saveFavoritesFromJson("[]");
    m_userFavorites->clear();
    setCountFavorites(0);
}

void ReleasesViewModel::loadHidedReleases()
{
    QFile hidedReleasesFile(getCachePath(hidedReleasesCacheFileName));
    if (!hidedReleasesFile.open(QFile::ReadOnly | QFile::Text)) return;
    auto json = hidedReleasesFile.readAll();
    hidedReleasesFile.close();

    auto document = QJsonDocument::fromJson(json);
    auto jsonArray = document.array();

    m_hiddenReleases->clear();
    foreach (auto item, jsonArray) m_hiddenReleases->append(item.toInt());
}

void ReleasesViewModel::saveHidedReleases()
{
    QJsonArray array;
    foreach (auto item, *m_hiddenReleases) {
        QJsonValue value(item);
        array.append(value);
    }

    QJsonDocument document(array);
    auto json = document.toJson();

    QFile favoritesCacheFile(getCachePath(hidedReleasesCacheFileName));
    favoritesCacheFile.open(QFile::WriteOnly | QFile::Text);
    favoritesCacheFile.write(json);
    favoritesCacheFile.close();
}

void ReleasesViewModel::loadSeenMarks()
{
    QFile seenMarkFile(getCachePath(seenMarkCacheFileName));
    if (!seenMarkFile.open(QFile::ReadOnly | QFile::Text)) return;
    auto seenMarkJson = seenMarkFile.readAll();
    seenMarkFile.close();

    auto document = QJsonDocument::fromJson(seenMarkJson);
    auto jsonSeenMarks = document.array();

    m_seenMarks->clear();

    foreach (auto item, jsonSeenMarks) {
        m_seenMarks->insert(item.toString(), true);
    }

    recalculateSeenCounts();
}

void ReleasesViewModel::recalculateSeenCounts()
{
    QMap<int, int> seenMap;
    auto keys = m_seenMarks->keys();
    foreach(auto item, keys) {
        auto parts = item.splitRef(".", Qt::SkipEmptyParts);
        auto key = parts[0].toInt();
        if (seenMap.contains(key)) {
            seenMap[key] += 1;
        } else {
            seenMap.insert(key, 1);
        }
    }

    QMapIterator<int, int> seenIterator(seenMap);
    int countSeens = 0;
    while (seenIterator.hasNext()) {
        seenIterator.next();

        auto release = m_releasesMap->value(seenIterator.key());
        if (release->countOnlineVideos() == seenIterator.value()) countSeens += 1;
    }

    setCountSeens(countSeens);
}

void ReleasesViewModel::loadHistory()
{
    QFile historyCacheFile(getCachePath(historyCacheFileName));
    if (!historyCacheFile.open(QFile::ReadOnly | QFile::Text)) return;
    auto historyJson = historyCacheFile.readAll();
    historyCacheFile.close();

    auto document = QJsonDocument::fromJson(historyJson);
    auto historyItems = document.array();

    foreach (auto item, historyItems) {
        HistoryModel* historyModel = new HistoryModel();
        historyModel->readFromJson(item);

        m_historyItems->insert(historyModel->id(), historyModel);
    }
}

void ReleasesViewModel::saveHistory()
{
    QJsonArray historyItems;

    QHashIterator<int, HistoryModel*> iterator(*m_historyItems);
    while(iterator.hasNext()) {
        iterator.next();

        QJsonObject jsonObject;
        HistoryModel* historyModel = iterator.value();
        historyModel->writeToJson(jsonObject);
        historyItems.append(jsonObject);
    }

    QFile historyFile(getCachePath(historyCacheFileName));
    historyFile.open(QFile::WriteOnly | QFile::Text);
    auto document = QJsonDocument(historyItems);
    historyFile.write(document.toJson());

    historyFile.close();
}

void ReleasesViewModel::loadChanges()
{
    QFile notificationFile(getCachePath(notificationCacheFileName));
    if (!notificationFile.open(QFile::ReadOnly | QFile::Text)) return;
    auto changes = notificationFile.readAll();
    notificationFile.close();

    m_releaseChanges->fromJson(changes);

    emit isChangesExistsChanged();
    emit newOnlineSeriesCountChanged();
    emit newTorrentsCountChanged();
    emit newTorrentSeriesCountChanged();
}

void ReleasesViewModel::saveChanges()
{
    QFile notificationFile(getCachePath(notificationCacheFileName));
    notificationFile.open(QFile::WriteOnly | QFile::Text);
    notificationFile.write(m_releaseChanges->toJson().toUtf8());
    notificationFile.close();

    emit isChangesExistsChanged();
    emit newOnlineSeriesCountChanged();
    emit newTorrentsCountChanged();
    emit newTorrentSeriesCountChanged();
}

int ReleasesViewModel::getCountOnlyFavorites(QList<int> *changes) const noexcept
{
    int count = 0;
    foreach (auto changeId, *changes) {
        if (m_userFavorites->contains(changeId)) count++;
    }
    return count;
}
