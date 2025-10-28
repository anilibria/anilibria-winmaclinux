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

#include <QClipboard>
#include <QGuiApplication>
#include <QPixmap>
#include <QImageReader>
#include <QDesktopServices>
#include <QtConcurrent>
#include <QFuture>
#include "releasesviewmodel.h"
#include "../../globalhelpers.h"

ReleasesViewModel::ReleasesViewModel(QObject *parent) : QObject(parent)
{
    m_items = new ReleasesListModel(this);
    m_items->setup(m_releases, m_scheduleReleases, m_userFavorites, m_hiddenReleases, &m_extendedSeenMarks, m_historyItems, m_releaseChanges, m_cinemahall, m_customGroups, &m_onlineVideosMap, &m_collections);
    m_cinemahall->setup(m_releases, &m_extendedSeenMarks, &m_onlineVideosMap);
    connect(m_cinemahall.get(), &CinemahallListModel::hasItemsChanged, this, &ReleasesViewModel::cinemahallItemsChanged);

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
    m_sectionNames.append("");
    m_sectionNames.append("Фильмы");
    m_sectionNames.append("Самые популярные");
    m_sectionNames.append("Часть серии");
    m_sectionNames.append("Завершенные");
    m_sectionNames.append("Просмотренные до конца");
    m_sectionNames.append("Просмотренные не до конца");
    m_sectionNames.append("Популярное в текущем году");
    m_sectionNames.append("В кинозале");
    m_sectionNames.append("Текущий сезон");
    m_sectionNames.append("Не текущий сезон");
    m_sectionNames.append("Свой фильтр");
    m_sectionNames.append("Коллекция Запланировано");
    m_sectionNames.append("Коллекция Смотрю");
    m_sectionNames.append("Коллекция Просмотрено");
    m_sectionNames.append("Коллекция Отложено");
    m_sectionNames.append("Коллекция Брошено");

    int sectionIterator = 0;
    foreach (auto section, m_sectionNames) {
        if (section.isEmpty()) {
            sectionIterator++;
            continue;
        }
        m_sectionCounters.append(sectionIterator);

        sectionIterator++;
    }

    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(1, 0));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(7, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(8, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(3, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(4, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(12, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(4, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(4, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));
    m_sectionSorting->append(std::make_tuple<int, int>(0, 1));

    createIfNotExistsFile(getCachePath(scheduleCacheFileName), "{}");
    createIfNotExistsFile(getCachePath(favoriteCacheFileName), "[]");
    createIfNotExistsFile(getCachePath(hidedReleasesCacheFileName), "[]");
    createIfNotExistsFile(getCachePath(extendedSeenMarkCacheFileName), "[]");
    createIfNotExistsFile(getCachePath(historyCacheFileName), "[]");
    createIfNotExistsFile(getCachePath(notificationCacheFileName), "{ \"newReleases\": [], \"newOnlineSeries\": [], \"newTorrents\": [], \"newTorrentSeries\": [] }");
    createIfNotExistsFile(getCachePath(collectionsCacheFileName), "[]");

    loadSchedules();
    loadFavorites();
    loadHidedReleases();
    loadSeenMarks();
    loadHistory();
    loadChanges();
    loadCollections();
    loadReleases();

    m_items->refresh();

    connect(m_cinemahall.get(), &CinemahallListModel::needDeleteFavorites, this, &ReleasesViewModel::needDeleteFavorites);
}

void ReleasesViewModel::setUserActivity(const UserActivityViewModel *viewModel) noexcept
{
    if (m_userActivity == viewModel) return;

    m_userActivity = const_cast<UserActivityViewModel*>(viewModel);
    emit userActivityChanged();
}

void ReleasesViewModel::setCountReleases(const int& countReleases) noexcept
{
    if (countReleases == m_countReleases) return;

    m_countReleases = countReleases;
    emit countReleasesChanged();
}

bool ReleasesViewModel::hasCinemahallNotSeenVideos() const noexcept
{
    auto releases = m_cinemahall->getCinemahallReleases();
    foreach (auto release, releases) {
        if (m_items->getReleaseSeenMarkCount(release->id()) < release->countOnlineVideos()) return true;
    }

    return false;
}

void ReleasesViewModel::setLocalStorage(LocalStorageService *localStorage) noexcept
{
    if (m_localStorage == localStorage) return;

    auto isFirstTime = m_localStorage == nullptr;

    m_localStorage = localStorage;
    emit localStorageChanged();

    if (isFirstTime) m_localStorage->setup(m_releases);
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

void ReleasesViewModel::setSelectMode(bool selectMode) noexcept
{
    if (m_selectMode == selectMode) return;

    m_selectMode = selectMode;
    emit selectModeChanged();

    if (!selectMode) m_items->clearSelected();
}

void ReleasesViewModel::setSynchronizationEnabled(bool synchronizationEnabled) noexcept
{
    if (m_synchronizationEnabled == synchronizationEnabled) return;

    m_synchronizationEnabled = synchronizationEnabled;
    emit synchronizationEnabledChanged();
}

void ReleasesViewModel::setNewEntities(QString newEntities) noexcept
{
    if (m_newEntities == newEntities) return;

    m_newEntities = newEntities;
    emit newEntitiesChanged();
    emit isChangesExistsChanged();
    emit newReleasesCountChanged();
    emit newOnlineSeriesCountChanged();
    emit newTorrentsCountChanged();
    emit newTorrentSeriesCountChanged();
}

void ReleasesViewModel::setApplicationSettings(ApplicationSettings *applicationSettings) noexcept
{
    if (m_applicationSettings == applicationSettings) return;

    m_applicationSettings = applicationSettings;
    emit applicationSettingsChanged();
}

void ReleasesViewModel::setNotCloseReleaseCardAfterWatch(const bool notCloseReleaseCardAfterWatch) noexcept
{
    if (m_notCloseReleaseCardAfterWatch == notCloseReleaseCardAfterWatch) return;

    m_notCloseReleaseCardAfterWatch = notCloseReleaseCardAfterWatch;
    emit notCloseReleaseCardAfterWatchChanged();
}

void ReleasesViewModel::setSynchronizationServicev2(const Synchronizev2Service *synchronizationServicev2) noexcept
{
    if (m_synchronizationServicev2 == synchronizationServicev2) return;
    if (m_synchronizationServicev2 != nullptr) return;

    m_synchronizationServicev2 = const_cast<Synchronizev2Service *>(synchronizationServicev2);
    emit synchronizationServicev2Changed();

    connect(m_synchronizationServicev2, &Synchronizev2Service::userFavoritesReceivedV2, this,&ReleasesViewModel::userFavoritesReceivedV2);

    reloadApiHostInItems();
}

void ReleasesViewModel::setProxyPort(int proxyPort) noexcept
{
    if (m_proxyPort == proxyPort) return;

    m_proxyPort = proxyPort;
    emit proxyPortChanged();
}

void ReleasesViewModel::setReleaseLinkedSeries(ReleaseLinkedSeries *releaseLinkedSeries) noexcept
{
    if (m_releaseLinkedSeries == releaseLinkedSeries) return;

    m_releaseLinkedSeries = releaseLinkedSeries;
    emit releaseLinkedSeriesChanged();

    if (m_releaseLinkedSeries == nullptr) return;

    m_items->setupLinkedSeries(m_releaseLinkedSeries);
    auto getReleaseCountSeens = [=](QList<int>& releases) {
        return this->items()->getReleasesSeenMarkCount(releases);
    };
    m_releaseLinkedSeries->setup(m_releases, m_userFavorites, &m_torrentItems, getReleaseCountSeens);
}

void ReleasesViewModel::setOpenedReleseExtensionContent(const QString &openedReleseExtensionContent) noexcept
{
    if (m_openedReleseExtensionContent == openedReleseExtensionContent) return;

    m_openedReleseExtensionContent = openedReleseExtensionContent;
    emit openedReleseExtensionContentChanged();
}

QString ReleasesViewModel::openedReleaseStatusDisplay() const noexcept
{
    if (m_openedRelease == nullptr) return "";

    return "<b>Статус:</b> <a href=\"http://years\">" + m_openedRelease->status() + "</a>";
}

QString ReleasesViewModel::openedReleaseYearDisplay() const noexcept
{
    if (m_openedRelease == nullptr) return "";

    return "<b>Год:</b> <a href=\"http://years\">" + m_openedRelease->year() + "</a>";
}

QString ReleasesViewModel::openedReleaseInScheduleDisplay() const noexcept
{
    if (m_openedRelease == nullptr) return "";
    if (!m_scheduleReleases->contains(m_openedRelease->id())) return "";

    auto dayId = m_scheduleReleases->value(m_openedRelease->id());
    return "<b>В расписании:</b> " + m_items->getScheduleDay(dayId);
}

QString ReleasesViewModel::openedReleaseSeasonDisplay() const noexcept
{
    if (m_openedRelease == nullptr) return "";

    return "<b>Сезон:</b> <a href=\"http://seasons\">" + m_openedRelease->season() + "</a>";
}

QString ReleasesViewModel::openedReleaseTypeDisplay() const noexcept
{
    if (m_openedRelease == nullptr) return "";

    return "<b>Тип:</b> " + m_openedRelease->type();
}

QString ReleasesViewModel::openedReleaseGenresDisplay() const noexcept
{
    if (m_openedRelease == nullptr) return "";

    return "<b>Жанры:</b> " + getMultipleLinks(m_openedRelease->genres());
}

QString ReleasesViewModel::openedReleaseVoicesDisplay() const noexcept
{
    if (m_openedRelease == nullptr) return "";

    return "<b>Команда:</b> " + getMultipleLinks(m_openedRelease->voicers());
}

bool ReleasesViewModel::openedReleaseIsAllSeen() const noexcept
{
    if (m_openedRelease == nullptr) return "";

    return m_openedRelease->countOnlineVideos() == m_items->getReleaseSeenMarkCount(m_openedRelease->id());
}

bool ReleasesViewModel::openedReleaseIsRutube() const noexcept
{
    if (m_openedRelease == nullptr) return false;

    //TODO: remake on new model
    return false;
}

bool ReleasesViewModel::openedReleaseInCollections() const noexcept
{
    if (m_openedRelease == nullptr) return false;

    return m_collections.contains(m_openedRelease->id());
}

QStringList ReleasesViewModel::getMostPopularGenres() const noexcept
{
    QStringList allGenres;
    QSet<int> alreadyProcessed;
    QHash<QString, int> genresCount;

    auto keys = m_extendedSeenMarks.keys();
    foreach (auto key, keys) {
        if (!m_onlineVideosMap.contains(key)) continue;

        auto videoReleaseId = m_onlineVideosMap[key]->releaseId();
        if (alreadyProcessed.contains(videoReleaseId)) continue;

        alreadyProcessed.insert(videoReleaseId);

        auto release = getReleaseById(videoReleaseId);
        if (release == nullptr) continue;
        auto genres = release->genres().split(",");
        foreach (auto genre, genres) {
            genre = genre.trimmed();
            if (!genresCount.contains(genre)) {
                allGenres.append(genre);
                genresCount[genre] = 1;
            } else {
                genresCount[genre] += 1;
            }
        }
    }

    std::sort(
        allGenres.begin(),
        allGenres.end(),
        [genresCount](QString left, QString right){
            return genresCount[left] > genresCount[right];
        }
    );

    alreadyProcessed.clear();
    genresCount.clear();

    if (allGenres.length() <= 3) return allGenres;

    auto iteratorIndex = 0;
    QStringList result;
    foreach (auto item, allGenres) {
        if (iteratorIndex == 3) break;

        result.append(item);
        iteratorIndex++;
    }

    return result;
}

QStringList ReleasesViewModel::getMostPopularVoices() const noexcept
{
    QStringList allVoices;
    QSet<int> alreadyProcessed;
    QHash<QString, int> voicesCount;

    auto keys = m_extendedSeenMarks.keys();

    foreach (auto key, keys) {
        if (!m_onlineVideosMap.contains(key)) continue;

        auto videoReleaseId = m_onlineVideosMap[key]->releaseId();

        if (alreadyProcessed.contains(videoReleaseId)) continue;

        alreadyProcessed.insert(videoReleaseId);

        auto release = getReleaseById(videoReleaseId);
        if (release == nullptr) continue;
        auto voicers = release->voicers().split(",");
        foreach (auto voicer, voicers) {
            voicer = voicer.trimmed();
            if (!voicesCount.contains(voicer)) {
                allVoices.append(voicer);
                voicesCount[voicer] = 1;
            } else {
                voicesCount[voicer] += 1;
            }
        }
    }

    std::sort(
        allVoices.begin(),
        allVoices.end(),
        [voicesCount](QString left, QString right){
            return voicesCount[left] > voicesCount[right];
        }
    );

    alreadyProcessed.clear();
    voicesCount.clear();

    if (allVoices.length() <= 3) return allVoices;

    auto iteratorIndex = 0;
    QStringList result;
    foreach (auto item, allVoices) {
        if (iteratorIndex == 3) break;

        result.append(item.toLower());
        iteratorIndex++;
    }

    return result;
}

void ReleasesViewModel::fillMyAnilibriaData() noexcept
{
    m_myAnilibriaNewInFavorites.clear();
    m_myAnilibriaNewFromStart.clear();
    m_myAnilibriaNewFromLastTwoDays.clear();
    m_myAnilibriaCurrentSeasons.clear();
    m_myAnilibriaAbandonReleases.clear();
    m_myAnilibriaWillWatchReleases.clear();
    m_myAnilibriaNextInReleaseSeries.clear();
    m_myAnilibriaRecommendsForGenres.clear();
    m_myAnilibriaRecommendsForVoices.clear();

    // new in favorites
    QSet<int> favorites;
    foreach (auto favorite, *m_userFavorites) favorites.insert(favorite);

    // new from start
    auto applicationStart = m_userActivity->previousApplicationStart();

    // new from last two days
    auto now = QDateTime::currentDateTimeUtc().addDays(-3);
    auto timestamp = static_cast<int>(now.toSecsSinceEpoch());

    // current season
    auto currentYear = QString::number(QDate::currentDate().year());
    auto currentSeason = m_items->getCurrentSeason();

    // abandon seens
    auto abandonNow = QDateTime::currentDateTimeUtc().addDays(-18);
    auto adandonTimestamp = static_cast<int>(abandonNow.toSecsSinceEpoch());
    QMap<int, int> adandonReleasesCount;
    foreach (auto onlineVideo, m_onlineVideos) {
        auto videoId = onlineVideo->uniqueId();

        if (m_extendedSeenMarks.contains(videoId)) {
            auto item = m_extendedSeenMarks.value(videoId);
            auto mark = std::get<0>(item);
            if (!mark) continue;

            auto releaseId = onlineVideo->releaseId();
            if (adandonReleasesCount.contains(releaseId)) {
                auto count = adandonReleasesCount[releaseId];
                count++;
                adandonReleasesCount[releaseId] = count;
            } else {
                adandonReleasesCount.insert(releaseId, 1);
            }
        }
    }

    // next in release series
    auto linkedReleases = m_items->getFullLinkedReleases();
    auto releasesMap = m_releasesMap.get();

    // recommends for genres
    auto genres = getMostPopularGenres();
    if (!genres.isEmpty()) {
        QMutableStringListIterator iterator(genres);
        while (iterator.hasNext()) {
            auto value = iterator.next();
            iterator.setValue(value.toLower());
        }
    }

    // recommends for voices
    auto popularVoices = getMostPopularVoices();

    foreach (auto release, *m_releases) {
        if (release->countOnlineVideos() == 0) continue;

        auto releaseId = release->id();
        auto hasInHistory = m_historyItems->contains(releaseId);

        // new in favorites
        if (favorites.contains(release->id())) {
            auto seenVideos = m_items->getReleaseSeenMarkCount(releaseId);
            if (release->countOnlineVideos() > seenVideos) m_myAnilibriaNewInFavorites.append(releaseId);
        }

        // new from start
        if (release->timestamp() > applicationStart) m_myAnilibriaNewFromStart.append(releaseId);

        // new from last two days
        if (release->timestamp() >= timestamp) m_myAnilibriaNewFromLastTwoDays.append(releaseId);

        // current season
        if (release->year() == currentYear && release->status().toLower() == "в работе" && release->season() == currentSeason) m_myAnilibriaCurrentSeasons.append(releaseId);

        // abandon seens
        if (hasInHistory) {
            auto historyItem = m_historyItems->value(releaseId);
            auto isAdandonRelease = historyItem->watchTimestamp() > 0 && adandonReleasesCount.contains(releaseId) &&
                adandonReleasesCount.value(releaseId) < release->countOnlineVideos() &&
                historyItem->watchTimestamp() < adandonTimestamp;

            if (isAdandonRelease) m_myAnilibriaAbandonReleases.append(releaseId);
        }

        // will watch
        if (m_userFavorites->contains(releaseId)) {
            auto seenVideos = adandonReleasesCount[releaseId];
            int watchTimestamp = 0;
            if (hasInHistory) {
                auto item = m_historyItems->value(releaseId);
                watchTimestamp = item->watchTimestamp();
            }

            if (seenVideos == 0 && watchTimestamp == 0) m_myAnilibriaWillWatchReleases.append(releaseId);
        }

        if (hasInHistory) {
            auto historyItem = m_historyItems->value(releaseId);

            if (historyItem->watchTimestamp() == 0) {
                // recommends for genres
                if (m_myAnilibriaRecommendsForGenres.count() < 30) {
                    auto releaseGenres = release->genres().toLower();
                    foreach (auto genre, genres) {
                        if (releaseGenres.contains(genre)) m_myAnilibriaRecommendsForGenres.append(releaseId);
                    }
                }
                // recommends for voices
                if (m_myAnilibriaRecommendsForVoices.count() < 30) {
                    auto releaseVoices = release->voicers().toLower();
                    foreach (auto voice, popularVoices) {
                        if (releaseVoices.contains(voice)) m_myAnilibriaRecommendsForVoices.append(releaseId);
                    }
                }
            }
        }



    }

    // next in release series
    foreach (auto group, linkedReleases) {
        auto countWatched = 0;
        auto notFullWatched = QList<FullReleaseModel *>();
        foreach (auto releaseId, group) {
            if (!releasesMap->contains(releaseId)) continue;
            auto release = releasesMap->value(releaseId);
            if (release->countOnlineVideos() == 0) continue;

            auto isFullWatch = release->countOnlineVideos() == adandonReleasesCount.value(releaseId);
            if (isFullWatch) {
                countWatched += 1;
            } else {
                if (notFullWatched.count() < 4) notFullWatched.append(release);
            }
        }

        if (countWatched > 0 && !notFullWatched.isEmpty()) {
            foreach (auto notFullWatchedItem, notFullWatched) m_myAnilibriaNextInReleaseSeries.append(notFullWatchedItem->id());
        }
    }

    adandonReleasesCount.clear();

    if (m_myAnilibriaRecommendsForGenres.count() > 5) {
        if (randomBetween(0, 10) > 5) std::reverse(m_myAnilibriaRecommendsForGenres.begin(), m_myAnilibriaRecommendsForGenres.end());

        m_seedValue += QRandomGenerator::system()->generate();
        QRandomGenerator generator(m_seedValue);

        auto index = 0;
        while (m_myAnilibriaRecommendsForGenres.count() > 5) {
            if (generator.bounded(0, 1000) % 2 == 0) {
                m_myAnilibriaRecommendsForGenres.removeAt(randomBetween(0, m_myAnilibriaRecommendsForGenres.count() - 1));
            }

            index++;
            if (index >= m_myAnilibriaRecommendsForGenres.count()) index = 0;
        }
    }
    if (m_myAnilibriaRecommendsForVoices.count() > 5) {
        if (randomBetween(0, 10) > 5) std::reverse(m_myAnilibriaRecommendsForVoices.begin(), m_myAnilibriaRecommendsForVoices.end());

        m_seedValue += QRandomGenerator::system()->generate();
        QRandomGenerator generator(m_seedValue);

        auto index = 0;
        while (m_myAnilibriaRecommendsForVoices.count() > 5) {
            if (generator.bounded(0, 1000) % 2 == 0) {
                m_myAnilibriaRecommendsForVoices.removeAt(randomBetween(0, m_myAnilibriaRecommendsForVoices.count() - 1));
            }

            index++;
            if (index >= m_myAnilibriaRecommendsForVoices.count()) index = 0;
        }
    }
}

void ReleasesViewModel::getFilledMyAnilibriaSection(int mode, QList<FullReleaseModel *> *list) noexcept
{
    QList<int> listReleases;
    switch (mode) {
        case 0:
            listReleases = m_myAnilibriaNewInFavorites;
            break;
        case 1:
            listReleases = m_myAnilibriaNewFromStart;
            break;
        case 2:
            listReleases = m_myAnilibriaNewFromLastTwoDays;
            break;
        case 3:
            listReleases = m_myAnilibriaCurrentSeasons;
            break;
        case 4:
            listReleases = m_myAnilibriaAbandonReleases;
            break;
        case 5:
            listReleases = m_myAnilibriaWillWatchReleases;
            break;
        case 6:
            listReleases = m_myAnilibriaNextInReleaseSeries;
            break;
        case 7:
            listReleases = m_myAnilibriaRecommendsForGenres;
            break;
        case 8:
            listReleases = m_myAnilibriaRecommendsForVoices;
            break;
    }
    auto releases = m_releasesMap.get();

    foreach (auto item, listReleases) {
        if (!releases->contains(item)) continue;

        list->append(releases->value(item));
    }
}

void ReleasesViewModel::fillNewInFavorites(QList<FullReleaseModel *>* list) const noexcept
{
    QSet<int> favorites;
    foreach (auto favorite, *m_userFavorites) {
        favorites.insert(favorite);
    }

    foreach (auto release, *m_releases) {
        if (release->countOnlineVideos() == 0) continue;
        if (!favorites.contains(release->id())) continue;

        auto seenVideos = m_items->getReleaseSeenMarkCount(release->id());
        if (release->countOnlineVideos() > seenVideos) {
            list->append(release);
        }
    }
}

void ReleasesViewModel::fillNewFromStart(QList<FullReleaseModel *> *list) const noexcept
{
    if (m_userActivity == nullptr) return;

    auto applicationStart = m_userActivity->previousApplicationStart();
    foreach (auto release, *m_releases) {
        if (release->countOnlineVideos() == 0) continue;
        if (release->timestamp() < applicationStart) continue;

        list->append(release);
    }
}

void ReleasesViewModel::fillNewFromLastTwoDays(QList<FullReleaseModel *> *list) const noexcept
{
    auto now = QDateTime::currentDateTimeUtc().addDays(-3);
    auto timestamp = static_cast<int>(now.toSecsSinceEpoch());
    foreach (auto release, *m_releases) {
        if (release->countOnlineVideos() == 0) continue;
        if (release->timestamp() < timestamp) continue;

        list->append(release);
    }
}

void ReleasesViewModel::fillAbandonedSeens(QList<FullReleaseModel *> *list) const noexcept
{
    auto now = QDateTime::currentDateTimeUtc().addDays(-18);
    auto timestamp = static_cast<int>(now.toSecsSinceEpoch());

    QMap<int, int> releasesCount;
    foreach (auto onlineVideo, m_onlineVideos) {
        auto videoId = onlineVideo->uniqueId();

        if (m_extendedSeenMarks.contains(videoId)) {
            auto item = m_extendedSeenMarks.value(videoId);
            auto mark = std::get<0>(item);
            if (!mark) continue;

            auto releaseId = onlineVideo->releaseId();
            if (releasesCount.contains(releaseId)) {
                auto count = releasesCount[releaseId];
                count++;
                releasesCount[releaseId] = count;
            } else {
                releasesCount.insert(releaseId, 1);
            }
        }
    }

    foreach (auto release, *m_releases) {
        if (release->countOnlineVideos() == 0) continue;
        auto releaseId = release->id();
        if (!m_historyItems->contains(releaseId)) continue;

        auto historyItem = m_historyItems->value(releaseId);
        if (historyItem->watchTimestamp() == 0) continue;

        if (releasesCount.contains(releaseId) && releasesCount.value(releaseId) < release->countOnlineVideos() && historyItem->watchTimestamp() < timestamp) {
            list->append(release);
        }
    }

    releasesCount.clear();
}

void ReleasesViewModel::fillRecommendsByGenres(QList<FullReleaseModel *> *list) noexcept
{
    auto genres = getMostPopularGenres();
    if (genres.isEmpty()) return;

    QMutableStringListIterator iterator(genres);
    while (iterator.hasNext()) {
        auto value = iterator.next();
        iterator.setValue(value.toLower());
    }

    foreach (auto release, *m_releases) {
        if (release->countOnlineVideos() == 0) continue;
        auto historyItem = m_historyItems->value(release->id());
        if (historyItem == nullptr || historyItem->watchTimestamp() != 0) continue; // if you opened release in video player it means it not fit our condition

        auto releaseGenres = release->genres().toLower();
        foreach (auto genre, genres) {
            if (releaseGenres.contains(genre)) {
                list->append(release);
                break;
            }
        }
    }

    if (list->count() <= 5) return;

    if (randomBetween(0, 10) > 5) std::reverse(list->begin(), list->end());

    m_seedValue += QRandomGenerator::system()->generate();
    QRandomGenerator generator(m_seedValue);

    auto index = 0;
    while (list->count() > 5) {
        if (generator.bounded(0, 1000) % 2 == 0) {
            list->removeAt(randomBetween(0, list->count() - 1));
        }

        index++;
        if (index >= list->count()) index = 0;
    }
}

void ReleasesViewModel::fillWillWatch(QList<FullReleaseModel *> *list) noexcept
{
    QMap<int, int> releasesCount;
    foreach (auto onlineVideo, m_onlineVideos) {
        auto videoId = onlineVideo->uniqueId();

        if (m_extendedSeenMarks.contains(videoId)) {
            auto item = m_extendedSeenMarks.value(videoId);
            auto mark = std::get<0>(item);
            if (!mark) continue;

            auto releaseId = onlineVideo->releaseId();
            if (releasesCount.contains(releaseId)) {
                auto count = releasesCount[releaseId];
                count++;
                releasesCount[releaseId] = count;
            } else {
                releasesCount.insert(releaseId, 1);
            }
        }
    }

    foreach (auto release, *m_releases) {
        if (release->countOnlineVideos() == 0) continue;
        auto releaseId = release->id();
        if (!m_userFavorites->contains(releaseId)) continue;

        auto seenVideos = releasesCount[releaseId];
        int watchTimestamp = 0;
        if (m_historyItems->contains(releaseId)) {
            auto item = m_historyItems->value(releaseId);
            watchTimestamp = item->watchTimestamp();
        }

        if (seenVideos == 0 && watchTimestamp == 0) list->append(release);
    }

    releasesCount.clear();
}

void ReleasesViewModel::fillNextInReleaseSeries(QList<FullReleaseModel *> *list) noexcept
{
    auto linkedReleases = m_items->getFullLinkedReleases();
    auto releasesMap = m_releasesMap.get();

    QMap<int, int> releasesCount;
    foreach (auto onlineVideo, m_onlineVideos) {
        auto videoId = onlineVideo->uniqueId();

        if (m_extendedSeenMarks.contains(videoId)) {
            auto item = m_extendedSeenMarks.value(videoId);
            auto mark = std::get<0>(item);
            if (!mark) continue;

            auto releaseId = onlineVideo->releaseId();
            if (releasesCount.contains(releaseId)) {
                auto count = releasesCount[releaseId];
                count++;
                releasesCount[releaseId] = count;
            } else {
                releasesCount.insert(releaseId, 1);
            }
        }
    }

    foreach (auto group, linkedReleases) {
        auto countWatched = 0;
        auto notFullWatched = QList<FullReleaseModel *>();
        foreach (auto releaseId, group) {
            if (!releasesMap->contains(releaseId)) continue;
            auto release = releasesMap->value(releaseId);
            if (release->countOnlineVideos() == 0) continue;

            auto isFullWatch = release->countOnlineVideos() == releasesCount.value(releaseId);
            if (isFullWatch) {
                countWatched += 1;
            } else {
                if (notFullWatched.count() < 4) notFullWatched.append(release);
            }
        }

        if (countWatched > 0 && !notFullWatched.isEmpty()) list->append(notFullWatched);
    }

    releasesCount.clear();
}

void ReleasesViewModel::fillCurrentSeason(QList<FullReleaseModel *> *list) noexcept
{
    auto currentYear = QString::number(QDate::currentDate().year());
    auto currentSeason = m_items->getCurrentSeason();

    foreach (auto release, *m_releases) {
        if (release->countOnlineVideos() == 0) continue;
        if (release->year() != currentYear) continue;
        if (release->status().toLower() != "в работе") continue;
        if (release->season() != currentSeason) continue;

        list->append(release);
    }
}

void ReleasesViewModel::fillRecommendedByVoices(QList<FullReleaseModel *> *list) noexcept
{
    auto voices = getMostPopularVoices();
    if (voices.isEmpty()) return;

    QMutableStringListIterator iterator(voices);
    while (iterator.hasNext()) {
        auto value = iterator.next();
        iterator.setValue(value.toLower());
    }

    auto releases = m_releases;
    m_seedValue += QRandomGenerator::system()->generate();
    auto seedValue = m_seedValue;
    auto historyItems = m_historyItems;

    QFuture<QList<FullReleaseModel *>> future = QtConcurrent::run(
        [voices, releases, seedValue, historyItems] {
            auto list = QList<FullReleaseModel *>();

            foreach (auto release, *releases) {
                auto historyItem = historyItems->value(release->id());
                if (historyItem == nullptr || historyItem->watchTimestamp() != 0) continue; // if you opened release in video player it means it not fit our condition

                auto releaseVoices = release->voicers().toLower();
                foreach (auto voice, voices) {
                    if (releaseVoices.contains(voice)) {
                        list.append(release);
                        break;
                    }
                }
            }

            if (list.count() <= 5) return list;

            if (QRandomGenerator::global()->bounded(0, 10) > 5) std::reverse(list.begin(), list.end());

            QRandomGenerator generator(seedValue);

            auto index = 0;
            while (list.count() > 5) {
                if (generator.bounded(0, 1000) % 2 == 0) {
                    list.removeAt(QRandomGenerator::global()->bounded(0, list.count() - 1));
                }

                index++;
                if (index >= list.count()) index = 0;
            }

            return list;
        }
    );

    list->append(future.result());
}

void ReleasesViewModel::getFavoritesReleases(QList<FullReleaseModel *> *list) const noexcept
{
    QSet<int> favorites;
    foreach (auto favorite, *m_userFavorites) {
        favorites.insert(favorite);
    }

    foreach (auto release, *m_releases) {
        if (!favorites.contains(release->id())) continue;

        auto seenVideos = m_items->getReleaseSeenMarkCount(release->id());
        if (release->countOnlineVideos() > seenVideos) {
            list->append(release);
        }
    }
}

void ReleasesViewModel::getFavoritesIds(QList<int> *list) const noexcept
{
    list->append(*m_userFavorites);
}

QString ReleasesViewModel::getReleaseCodeFromUrl(const QString &url) const noexcept
{
    if (url.indexOf("anilibria.cf") > -1 || url.indexOf("anilib.top") > -1 || url.indexOf("anilibria.tv") > -1) {
        auto startIndex = url.indexOf("release/") + 8;
        auto code = url.mid(startIndex).replace(".html", "");
        if (code.indexOf("?") > -1) code = code.mid(0, code.indexOf("?"));
        return code;
    }

    return "";
}

void ReleasesViewModel::fillFullSearch(QList<FullReleaseModel *> &list, const QString &filter) noexcept
{
    auto filterLower = filter.toLower();
#if QT_VERSION <= QT_VERSION_CHECK(5, 15, 0)
    auto words = filterLower.split(" ", QString::SkipEmptyParts);
#else
    auto words = filterLower.split(" ", Qt::SkipEmptyParts);
#endif
    if (words.isEmpty()) return;

    foreach (auto release, *m_releases) {
        auto anyCheck = true;
        foreach (auto word, words) {
            if (!fullSearchCheck(word, release)) anyCheck = false;
        }

        if (anyCheck) list.append(release);

        if (list.count() == 4) break;
    }
}

bool ReleasesViewModel::fullSearchCheck(const QString &word, const FullReleaseModel *release) noexcept
{
    if (release->title().toLower().contains(word)) {
        return true;
    }
    if (release->originalName().toLower().contains(word)) {
        return true;
    }
    if (release->year().toLower().contains(word)) {
        return true;
    }
    if (release->season().toLower().contains(word)) {
        return true;
    }
    if (release->genres().toLower().contains(word)) {
        return true;
    }
    if (release->voicers().toLower().contains(word)) {
        return true;
    }

    return false;
}

void ReleasesViewModel::iterateOnReleases(std::function<void (FullReleaseModel *)> func) noexcept
{
    foreach (auto release, *m_releases) {
        func(release);
    }
}

QList<ReleaseOnlineVideoModel *> ReleasesViewModel::getReleaseVideos(int releaseId) noexcept
{
    QList<ReleaseOnlineVideoModel *> result;
    foreach (auto video, m_onlineVideos) {
        if (video->releaseId() == releaseId) {
            result.append(video);
        }
    }

    return result;
}

QList<ApiTorrentModel *> ReleasesViewModel::getReleaseTorrents(int releaseId) noexcept
{
    QList<ApiTorrentModel *> result;
    foreach (auto torrent, m_torrentItems) {
        if (torrent->releaseId() == releaseId) {
            result.append(torrent);
        }
    }

    return result;
}

void ReleasesViewModel::getSeenIds(QList<int>* list)
{
    QMap<int, int> releasesCount;

    foreach (auto onlineVideo, m_onlineVideos) {
        auto videoId = onlineVideo->uniqueId();

        if (m_extendedSeenMarks.contains(videoId)) {
            auto releaseId = onlineVideo->releaseId();
            if (releasesCount.contains(releaseId)) {
                auto count = releasesCount[releaseId];
                count++;
                auto allCount = m_releasesMap->value(releaseId)->countOnlineVideos();
                if (count >= allCount) {
                    list->append(releaseId);
                    releasesCount.remove(releaseId);
                }
                releasesCount[releaseId] = count;
            }
        }
    }
    releasesCount.clear();
}

void ReleasesViewModel::synchronizeCollections(QMap<int, QString> &&items) noexcept
{
    auto keys = items.keys();
    foreach (auto key, keys) {
        auto collection = items.value(key);
        if (m_collections.contains(key)) {
            m_collections[key] = collection;
        } else {
            m_collections.insert(key, collection);
        }
    }
    auto savedRelases = m_collections.keys();
    foreach (auto saveReleaseId, savedRelases) {
        if (!keys.contains(saveReleaseId)) m_collections.remove(saveReleaseId);
    }

    items.clear();

    saveCollections();
}

void ReleasesViewModel::setReleasesToCollection(QList<int> ids, const QString &collection) noexcept
{
    foreach (auto id, ids) {
        if (m_collections.contains(id)) {
            m_collections[id] = collection;
        } else {
            m_collections.insert(id, collection);
        }
    }
}

void ReleasesViewModel::removeReleasesFromCollections(QList<int> ids) noexcept
{
    foreach (auto id, ids) {
        if (m_collections.contains(id)) m_collections.remove(id);
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

void ReleasesViewModel::selectRelease(int id) noexcept
{
    if (m_selectMode) {
        if (isOpenedCard()) hideReleaseCard();
        m_items->selectItem(id);
    } else {
        showReleaseCard(id);
    }
}

void ReleasesViewModel::toggleRelease(int id) noexcept
{
    if (m_selectMode) {
        if (isOpenedCard()) hideReleaseCard();
        m_items->toggleItem(id);
    } else {
        showReleaseCard(id);
    }
}

void ReleasesViewModel::clearSelectedReleases() noexcept
{
    m_items->clearSelected();
}

void ReleasesViewModel::addReleaseToFavorites(int id) noexcept
{
    if (!m_userFavorites->contains(id)) {
        m_userFavorites->append(id);
        QList<int> favoritesIds;
        favoritesIds.append(id);
        m_synchronizationServicev2->addUserFavorites(favoritesIds);
        m_items->refreshItem(id);
        saveFavorites();
        setCountFavorites(m_userFavorites->count());
    }
    if (m_openedRelease != nullptr) emit openedReleaseInFavoritesChanged();
}

void ReleasesViewModel::removeReleaseFromFavorites(int id) noexcept
{
    if (m_userFavorites->contains(id)) {
        m_userFavorites->removeOne(id);
        QList<int> favoritesIds;
        favoritesIds.append(id);
        m_synchronizationServicev2->removeUserFavorites(favoritesIds);
        m_items->refreshItem(id);
        saveFavorites();
        setCountFavorites(m_userFavorites->count());
    }

    if (m_openedRelease != nullptr) emit openedReleaseInFavoritesChanged();
}

void ReleasesViewModel::addSelectedReleaseToFavorites() noexcept
{
    auto selectedReleases = m_items->getSelectedReleases();
    bool needSave = false;
    QList<int> ids;
    foreach (auto selectedRelease, *selectedReleases) {
        if (!m_userFavorites->contains(selectedRelease)) {
            m_userFavorites->append(selectedRelease);
            m_items->refreshItem(selectedRelease);
            ids.append(selectedRelease);
            needSave = true;
        }
    }

    if (needSave) {
        m_synchronizationServicev2->addUserFavorites(ids);

        saveFavorites();
        setCountFavorites(m_userFavorites->count());
    }
    clearSelectedReleases();
}

void ReleasesViewModel::removeSelectedReleaseFromFavorites() noexcept
{
    auto selectedReleases = m_items->getSelectedReleases();
    bool needSave = false;
    QList<int> ids;
    foreach (auto selectedRelease, *selectedReleases) {
        if (m_userFavorites->contains(selectedRelease)) {
            m_userFavorites->removeOne(selectedRelease);
            ids.append(selectedRelease);
            m_items->refreshItem(selectedRelease);
            needSave = true;
        }
    }

    if (needSave) {
        m_synchronizationServicev2->removeUserFavorites(ids);

        saveFavorites();
        setCountFavorites(m_userFavorites->count());
    }
    clearSelectedReleases();
}

void ReleasesViewModel::closeReleaseCard() noexcept
{
    m_openedRelease = nullptr;

    emit isOpenedCardChanged();
    emit openedReleaseIdChanged();
}

void ReleasesViewModel::openDescriptionLink(const QString &link) noexcept
{
    auto codeFromLink = getReleaseCodeFromUrl(link);

    if (!codeFromLink.isEmpty()) {
        auto release = getReleaseByCode(codeFromLink);
        showReleaseCard(release->id());
    } else {
        QDesktopServices::openUrl(QUrl(link));
    }
}

void ReleasesViewModel::showReleaseCard(int id, bool needHandle) noexcept
{
    auto release = getReleaseById(id);
    m_openedRelease = release;

    auto torrents = getReleaseTorrents(id);
    m_releaseTorrentsList->loadTorrentsFromJson(torrents);

    setToReleaseHistory(release->id(), 0);

    resetReleaseChanges(release->id());

    refreshOpenedReleaseCard();

    m_customGroups->setupReleaseId(id);

    emit isOpenedCardChanged();

    if (needHandle) emit releaseCardOpened();
}

void ReleasesViewModel::showRandomRelease() noexcept
{
    if (m_releases->isEmpty()) return;

    auto count = m_releases->count() - 1;

    auto position = randomBetween(1, count);

    auto release = m_releases->at(position);

    showReleaseCard(release->id());
}

void ReleasesViewModel::showRandomReleaseFromFavorites() noexcept
{
    if (m_userFavorites->count() < 5) return;

    auto count = m_userFavorites->count() - 1;

    auto position = randomBetween(1, count);

    auto releaseId = m_userFavorites->at(position);

    showReleaseCard(releaseId);
}

void ReleasesViewModel::showRandomReleaseInFiltered() noexcept
{
    auto count = m_items->countFilteredReleases() - 1;
    if (count < 5) return;

    auto position = randomBetween(1, count);

    auto releaseId = m_items->getReleaseIdByIndex(position);

    showReleaseCard(releaseId);
}

void ReleasesViewModel::showRandomReleaseInSeen() noexcept
{
    auto seensCounts = getAllSeenMarkCount();
    QList<int> releaseIds;
    foreach (auto release, *m_releases) {
        auto id = release->id();
        if (!seensCounts.contains(id)) continue;

        if (seensCounts.value(id) < release->countOnlineVideos()) releaseIds.append(id);
    }

    if (releaseIds.count() < 5) return;

    auto position = randomBetween(1, releaseIds.count() - 1);

    showReleaseCard(releaseIds.value(position));
}

void ReleasesViewModel::hideReleaseCard() noexcept
{
    m_openedRelease = nullptr;
    emit isOpenedCardChanged();
}

void ReleasesViewModel::setSeenMarkAllSeries(int id, int countSeries, bool marked)
{
    Q_UNUSED(countSeries);

    QList<QString> result;

    foreach (auto item, m_onlineVideos) {
        if (item->releaseId() != id) continue;

        setSeenMarkInternal(item->uniqueId(), marked);
        result.append(item->uniqueId());
    }

    saveSeenMarks();

    m_items->refreshItem(id);

    synchronizeSeenMarkForRelease(result, marked);
}

void ReleasesViewModel::setSeenMarkAllSeriesSelectedReleases(bool marked)
{
    QList<QString> result;
    auto selectedReleases = m_items->getSelectedReleases();
    foreach (auto selectedRelease, *selectedReleases) {
        auto release = getReleaseById(selectedRelease);

        foreach (auto item, m_onlineVideos) {
            if (item->releaseId() != release->id()) continue;

            setSeenMarkInternal(item->uniqueId(), marked);
            result.append(item->uniqueId());
        }
        m_items->refreshItem(selectedRelease);
    }

    saveSeenMarks();
    synchronizeSeenMarkForRelease(result, marked);
}

void ReleasesViewModel::setSeenMarkForSingleRelease(int id, bool marked)
{
    auto release = getReleaseById(id);
    if (release == nullptr) return;

    QList<QString> result;

    foreach (auto item, m_onlineVideos) {
        if (item->releaseId() != id) continue;

        setSeenMarkInternal(item->uniqueId(), marked);
        result.append(item->uniqueId());
    }

    m_items->refreshItem(id);
    saveSeenMarks();

    synchronizeSeenMarkForRelease(result, marked);
}

void ReleasesViewModel::synchronizeSeenMarkForRelease(const QList<QString>& ids, bool marked)
{
    QList<QString> videosIds;
    foreach (auto video, m_onlineVideos) {
        auto id = video->uniqueId();
        if (!ids.contains(id)) continue;

        videosIds.append(video->uniqueId());
    }

    if (marked) {
        emit addedSeenMarks(videosIds);
    } else {
        emit removeSeenMarks(videosIds);
    }
}

void ReleasesViewModel::synchronizeSeenMarkForRelease(int id, int countSeries, bool marked)
{
    QList<std::tuple<int, QString>> videosIds;
    foreach (auto video, m_onlineVideos) {
        if (video->releaseId() != id) continue;

        auto tuple = std::make_tuple(video->order(),  video->uniqueId());
        videosIds.append(tuple);
    }

    std::sort(
        videosIds.begin(),
        videosIds.end(),
        [](std::tuple<int, QString> left, std::tuple<int, QString> right) {
            auto leftIndex = std::get<0>(left);
            auto rightIndex = std::get<0>(right);

            return leftIndex < rightIndex;
        }
    );
    videosIds.removeAt(countSeries + 1);

    QList<QString> result;
    foreach (auto videosId, videosIds) {
        result.append(std::get<1>(videosId));
    }

    if (marked) {
        emit addedSeenMarks(result);
    } else {
        emit removeSeenMarks(result);
    }
}

void ReleasesViewModel::synchronizeSeenMarkForSingleSeria(const QString& id, bool marked)
{
    QList<QString> result;
    result.append(id);

    if (marked) {
        emit addedSeenMarks(result);
    } else {
        emit removeSeenMarks(result);
    }
}

void ReleasesViewModel::refreshOpenedReleaseCard()
{
    emit openedReleaseIdChanged();
    emit openedReleasePosterChanged();
    emit openedReleaseTitleChanged();
    emit openedReleaseOriginalNameChanged();
    emit openedReleaseStatusChanged();
    emit openedReleaseStatusDisplayChanged();
    emit openedReleaseYearChanged();
    emit openedReleaseYearDisplayChanged();
    emit openedReleaseInScheduleChanged();
    emit openedReleaseInScheduleDisplayChanged();
    emit openedReleaseSeasonChanged();
    emit openedReleaseSeasonDisplayChanged();
    emit openedReleaseTypeChanged();
    emit openedReleaseTypeDisplayChanged();
    emit openedReleaseGenresChanged();
    emit openedReleaseGenresDisplayChanged();
    emit openedReleaseVoicesChanged();
    emit openedReleaseVoicesDisplayChanged();
    emit openedReleaseIsAllSeenChanged();
    emit openedReleaseDescriptionChanged();
    emit openedReleaseCodeChanged();
    emit openedReleaseCountTorrentsChanged();
    emit openedReleaseCountVideosChanged();
    emit openedReleaseInFavoritesChanged();
    emit openedReleaseInHidedChanged();
    emit openedReleaseSeenCountVideosChanged();
    emit openedReleaseAnnounceChanged();
    emit openedReleaseIsRutubeChanged();
    emit openedReleaseInCollectionsChanged();
}

void ReleasesViewModel::setSeenMark(int id, const QString& seriaId, bool marked)
{
    Q_UNUSED(id);

    setSeenMarkInternal(seriaId, marked);
    saveSeenMarks();
    synchronizeSeenMarkForSingleSeria(seriaId, marked);
}

void ReleasesViewModel::toggleSeenMark(int id, const QString& seriaId) noexcept
{
    Q_UNUSED(id);

    auto newState = false;
    if (m_extendedSeenMarks.contains(seriaId)) {
        auto oldItem = m_extendedSeenMarks.value(seriaId);
        auto oldValue = std::get<0>(oldItem);
        newState = !oldValue;
    } else {
        newState = true;
    }

    setSeenMarkInternal(seriaId, newState);
    saveSeenMarks();
    synchronizeSeenMarkForSingleSeria(seriaId, newState);
}

bool ReleasesViewModel::getSeriaSeenMark(int id, const QString& seriaId) const noexcept
{
    Q_UNUSED(id);

    if (m_extendedSeenMarks.contains(seriaId)) {
        auto item = m_extendedSeenMarks.value(seriaId);
        return std::get<0>(item);
    }

    return false;
}

QHash<QString, std::tuple<bool, int>>& ReleasesViewModel::getSeenMarks()
{
    return m_extendedSeenMarks;
}

QMap<QString, ReleaseOnlineVideoModel *> &ReleasesViewModel::getVideosMap()
{
    return m_onlineVideosMap;
}

uint32_t ReleasesViewModel::getCountFromChanges(const QList<int> *releases, bool filterByFavorites)
{
    if (filterByFavorites) {
        uint32_t count = 0;
        foreach (auto releaseId, *releases) {
            if (m_userFavorites->contains(releaseId)) count++;
        }
        return count;
    }
    return releases->count();
}

void ReleasesViewModel::openInExternalPlayer(const QString &url)
{
    if (url.isEmpty()) return;

    QDesktopServices::openUrl(QUrl(url));
}

void ReleasesViewModel::prepareTorrentsForListItem(const int id)
{
    auto torrents = getReleaseTorrents(id);
    m_itemTorrents->loadFromJson(torrents);
}

QString ReleasesViewModel::packAsM3UAndOpen(int id, QString quality)
{
    auto videos = getReleaseVideos(id);
    return m_localStorage->packAsM3UAndOpen(id, quality, videos);
}

QString ReleasesViewModel::packAsMPCPLAndOpen(int id, QString quality)
{
    auto videos = getReleaseVideos(id);
    return m_localStorage->packAsMPCPLAndOpen(id, quality, videos);
}

void ReleasesViewModel::savePreviousReleases(int previousLastTimeStamp)
{
    if (previousLastTimeStamp == 0) return;

    m_oldReleasesCountVideos.clear();
    m_oldReleasesCountTorrents.clear();
    m_oldReleasesIds.clear();
    m_oldReleasesTorrentsSeries.clear();

    auto keys = m_releasesMap->keys();
    foreach (auto releaseId, keys) {
        auto release = m_releasesMap->value(releaseId);
        m_oldReleasesIds.insert(releaseId);
        m_oldReleasesCountVideos.insert(releaseId, release->countOnlineVideos());
        m_oldReleasesCountTorrents.insert(releaseId, release->countTorrents());
    }

    foreach (auto torrentItem, m_torrentItems) {
        auto releaseId = torrentItem->releaseId();
        if (m_oldReleasesTorrentsSeries.contains(releaseId)) {
            auto newValue = torrentItem->description() + m_oldReleasesTorrentsSeries.value(releaseId);
            m_oldReleasesTorrentsSeries.remove(releaseId);
            m_oldReleasesTorrentsSeries.insert(releaseId, newValue);
        }
        m_oldReleasesTorrentsSeries.insert(releaseId, torrentItem->description());
    }
}

void ReleasesViewModel::refreshApiHost()
{
    reloadApiHostInItems();
}

void ReleasesViewModel::copyOpenedReleaseMagnetTorrent(int identifier) noexcept
{
    auto link = m_releaseTorrentsList->getMagnetLink(identifier);
    if (link == nullptr) return;

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(link);
}

void ReleasesViewModel::openOpenedReleaseMagnetTorrent(int identifier) noexcept
{
    auto link = m_releaseTorrentsList->getMagnetLink(identifier);
    if (link == nullptr) return;

    QDesktopServices::openUrl(QUrl(link));
}

void ReleasesViewModel::synchronizeSeens(const QVariantMap items) noexcept
{
    auto keys = items.keys();
    foreach (auto key, keys) {
        auto value = items.value(key);
        auto map = value.toMap();
        auto state = map["mark"].toBool();
        auto time = map["time"].toInt();

        if (m_extendedSeenMarks.contains(key)) {
            m_extendedSeenMarks[key] = std::make_tuple(state, time);
        } else {
            m_extendedSeenMarks.insert(key, std::make_tuple(state, time));
        }
    }
    saveSeenMarks();
    m_items->refresh(); // update releases after save seen marks
}

void ReleasesViewModel::synchronizeLocalSeensToExternal() noexcept
{
}

void ReleasesViewModel::recheckOpenedReleaseInCollections() noexcept
{
    emit openedReleaseInCollectionsChanged();
}

QString ReleasesViewModel::getReleasePoster(int releaseId) noexcept
{
    auto release = getReleaseById(releaseId);
    if (release == nullptr) return "";

    auto poster = release->poster();
    return m_localStorage->getReleasePosterPath(releaseId, poster);
}

void ReleasesViewModel::refreshMyAnilibriaCache() noexcept
{
    fillMyAnilibriaData();
}

FullReleaseModel *ReleasesViewModel::getReleaseById(int id) const noexcept
{
    auto iterator = std::find_if(
        m_releases->begin(),
        m_releases->end(),
        [id](FullReleaseModel* item)
        {
            return item->id() == id;
        }
    );

    if(iterator == m_releases->end()) return nullptr;

    return *iterator;
}

void ReleasesViewModel::resetReleaseChanges(int releaseId) noexcept
{
    if (m_releaseChanges->newReleases()->contains(releaseId)) m_releaseChanges->newReleases()->removeOne(releaseId);
    if (m_releaseChanges->newOnlineSeries()->contains(releaseId)) m_releaseChanges->newOnlineSeries()->removeOne(releaseId);
    if (m_releaseChanges->newTorrents()->contains(releaseId)) m_releaseChanges->newTorrents()->removeOne(releaseId);
    if (m_releaseChanges->newTorrentSeries()->contains(releaseId)) m_releaseChanges->newTorrentSeries()->removeOne(releaseId);

    saveChanges();
}

void ReleasesViewModel::setSeenMarkInternal(const QString& id, bool marked)
{
    if (m_extendedSeenMarks.contains(id)) {
        auto oldItem = m_extendedSeenMarks[id];
        auto oldTime = std::get<1>(oldItem);
        m_extendedSeenMarks[id] = std::make_tuple(marked, oldTime);
    } else {
        m_extendedSeenMarks.insert(id, std::make_tuple(marked, 0));
    }
}

void ReleasesViewModel::removeAllSeenMark()
{
    m_extendedSeenMarks.clear();

    //TODO: synchronize all

    saveSeenMarks();
    m_items->refresh();
}

void ReleasesViewModel::reloadReleases()
{
    loadReleases();
    m_items->refresh();
    emit releasesFullyLoaded();

    if (m_oldReleasesIds.isEmpty()) return;

    int newReleasesCount = 0;
    int newSeriesCount = 0;
    int newTorrentsCount = 0;
    int updatedTorrentsCount = 0;

    if (m_releases->size() != m_oldReleasesIds.size()) {
        foreach (auto release, *m_releases) {
            if (m_oldReleasesIds.contains(release->id())) continue;

            if (!m_releaseChanges->newReleases()->contains(release->id())) m_releaseChanges->newReleases()->append(release->id());
        }

        if (m_releases->size() > m_oldReleasesIds.size()) newReleasesCount = m_releases->size() - m_oldReleasesIds.size();
    }

    auto videos = m_oldReleasesCountVideos.keys();
    foreach (auto oldReleasesId, videos) {
        if (!m_releasesMap->contains(oldReleasesId)) continue;

        auto release = m_releasesMap->value(oldReleasesId);
        if (release->countOnlineVideos() != m_oldReleasesCountVideos.value(oldReleasesId)) {
            if (!m_releaseChanges->newOnlineSeries()->contains(oldReleasesId)) m_releaseChanges->newOnlineSeries()->append(oldReleasesId);

            if (release->countOnlineVideos() > m_oldReleasesCountVideos.value(oldReleasesId)) {
                newSeriesCount += release->countOnlineVideos() - m_oldReleasesCountVideos.value(oldReleasesId);
            }
        }
    }

    auto torrentKeys = m_oldReleasesCountTorrents.keys();
    foreach (auto oldReleasesId, torrentKeys) {
        if (!m_releasesMap->contains(oldReleasesId)) continue;

        auto release = m_releasesMap->value(oldReleasesId);
        if (release->countTorrents() > m_oldReleasesCountTorrents.value(oldReleasesId)) {
            if (!m_releaseChanges->newTorrents()->contains(oldReleasesId)) m_releaseChanges->newTorrents()->append(oldReleasesId);

            newTorrentsCount += release->countTorrents() - m_oldReleasesCountTorrents.value(oldReleasesId);
        }
    }

    QMap<int, QString> newTorrentSeries;
    foreach (auto torrentItem, m_torrentItems) {
        auto releaseId = torrentItem->releaseId();
        if (newTorrentSeries.contains(releaseId)) {
            auto newValue = torrentItem->description() + newTorrentSeries.value(releaseId);
            newTorrentSeries.remove(releaseId);
            newTorrentSeries.insert(releaseId, newValue);
        }
        newTorrentSeries.insert(releaseId, torrentItem->description());
    }

    auto torrentSeriesKeys = m_oldReleasesTorrentsSeries.keys();
    foreach (auto oldReleasesId, torrentSeriesKeys) {
        if (!newTorrentSeries.contains(oldReleasesId)) continue;

        auto newValue = newTorrentSeries.value(oldReleasesId);
        auto oldValue = m_oldReleasesTorrentsSeries.value(oldReleasesId);
        if (newValue != oldValue) {
            if (!m_releaseChanges->newTorrentSeries()->contains(oldReleasesId)) m_releaseChanges->newTorrentSeries()->append(oldReleasesId);

            updatedTorrentsCount += 1;
        }
    }

    m_oldReleasesIds.clear();
    m_oldReleasesCountTorrents.clear();
    m_oldReleasesCountVideos.clear();
    m_oldReleasesTorrentsSeries.clear();

    saveChanges();

    QString newEntities = "";
    if (newReleasesCount > 0) newEntities += "Новых релизов " + QString::number(newReleasesCount) + " ";
    if (newSeriesCount > 0) newEntities += "Новых серий " + QString::number(newSeriesCount) + " ";
    if (newTorrentsCount > 0) newEntities += "Новых торрентов " + QString::number(newTorrentsCount) + " ";
    if (updatedTorrentsCount > 0) newEntities += "Новых серий в торрентах " + QString::number(updatedTorrentsCount);
    setNewEntities(newEntities);
}

void ReleasesViewModel::setToReleaseHistory(int id, int type) noexcept
{
    HistoryModel* item;
    if (m_historyItems->contains(id)) {
        item = m_historyItems->value(id);
    } else {
        item = new HistoryModel();
        item->setId(id);
        m_historyItems->insert(id, item);
    }

    QDateTime now = QDateTime::currentDateTime();
    int timestamp = static_cast<int>(now.toSecsSinceEpoch());

    if (type == HistoryReleaseCardMode) {
        item->setTimestamp(timestamp);
    }
    if (type == HistoryWatchReleaseCardMode) {
        item->setWatchTimestamp(timestamp);
    }

    saveHistory();
}

QString ReleasesViewModel::getReleasePoster(int id) const noexcept
{
    auto release = getReleaseById(id);
    if (release == nullptr) return "";

    return release->poster();
}

QString ReleasesViewModel::getReleaseTitle(int id) const noexcept
{
    auto release = getReleaseById(id);
    if (release == nullptr) return "";

    return release->title();
}

void ReleasesViewModel::addToHidedReleases(const QList<int> &ids) noexcept
{
    foreach(auto id, ids) {
        if (m_hiddenReleases->contains(id)) continue;

        m_hiddenReleases->append(id);
    }

    saveHidedReleases();
}

void ReleasesViewModel::addToHidedSelectedReleases() noexcept
{
    auto selectedReleases = m_items->getSelectedReleases();
    foreach(auto selectedRelease, *selectedReleases) {
        if (m_hiddenReleases->contains(selectedRelease)) continue;

        m_hiddenReleases->append(selectedRelease);
    }

    saveHidedReleases();
}

void ReleasesViewModel::removeFromHidedReleases(const QList<int> &ids) noexcept
{
    foreach (auto id, ids) {
        auto index = m_hiddenReleases->indexOf(id);
        if (index == -1) continue;

        m_hiddenReleases->removeAt(index);
    }

    saveHidedReleases();
}

void ReleasesViewModel::removeFromHidedSelectedReleases() noexcept
{
    auto selectedReleases = m_items->getSelectedReleases();
    foreach(auto selectedRelease, *selectedReleases) {
        auto index = m_hiddenReleases->indexOf(selectedRelease);
        if (index == -1) continue;

        m_hiddenReleases->removeAt(index);
    }

    saveHidedReleases();
}

void ReleasesViewModel::removeAllHidedReleases() noexcept
{
    m_hiddenReleases->clear();

    saveHidedReleases();
    m_items->refresh();
}

void ReleasesViewModel::addToCinemahallSelectedReleases()
{
    auto selectedReleases = m_items->getSelectedReleases();

#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    auto items = selectedReleases->toList();
#else
    QList<int> items(selectedReleases->begin(), selectedReleases->end());
#endif
    m_cinemahall->addReleases(items);

    m_userActivity->addCinemahallMarkToCounter(selectedReleases->count());

    emit hasCinemahallNotSeenVideosChanged();
}

void ReleasesViewModel::setupSortingForSection() const noexcept
{
    auto currentSection = m_items->section();
    if (currentSection >= m_sectionSorting->count()) return;
    auto sortings = *m_sectionSorting;
    auto tuple = sortings[currentSection];
    auto sortingField = std::get<0>(tuple);
    auto sortingDescending = std::get<1>(tuple);

    m_items->setSortingField(sortingField);
    m_items->setSortingDescending(sortingDescending);
}

void ReleasesViewModel::loadReleases()
{
    loadNextReleasesWithoutReactive();

    setCountReleases(m_releases->count());
}

void ReleasesViewModel::loadNextReleasesWithoutReactive()
{
    while (m_releases->count()) delete m_releases->takeLast();
    m_releases->clear();
    m_releasesMap->clear();
    m_onlineVideos.clear();
    m_onlineVideosMap.clear();
    m_torrentItems.clear();

    QFile metadataCacheFile(getCachePath(metadataCacheFileName));
    if (!metadataCacheFile.open(QFile::ReadOnly | QFile::Text)) return;

    QString metadataContent = metadataCacheFile.readAll();
    auto metadata = QJsonDocument::fromJson(metadataContent.toUtf8()).object();
    auto countReleases = metadata.contains("countReleases") ? metadata.value("countReleases").toInt(0) : 0;
    auto countEpisodes = metadata.contains("countEpisodes") ? metadata.value("countEpisodes").toInt(0) : 0;

    for (int i = 0; i < countReleases; i++) {
        auto partPath = getCachePath("releases" + QString::number(i) + ".cache");
        if (!QFile::exists(partPath)) continue;

        QFile releaseCacheFile(partPath);
        if (!releaseCacheFile.open(QFile::ReadOnly | QFile::Text)) continue;

        QString releasesJson = releaseCacheFile.readAll();
        releaseCacheFile.close();

        auto releasesArray = QJsonDocument::fromJson(releasesJson.toUtf8()).array();

        foreach (auto release, releasesArray) {
            FullReleaseModel* jsonRelease = new FullReleaseModel();
            jsonRelease->readFromJson(release.toObject());
            if (m_synchronizationServicev2 != nullptr) jsonRelease->setPosterHost(m_synchronizationServicev2->apiv2host());

            m_releases->append(jsonRelease);
            m_releasesMap->insert(jsonRelease->id(), jsonRelease);
        }
    }

    for (int i = 0; i < countEpisodes; i++) {
        auto partPath = getCachePath("episodes" + QString::number(i) + ".cache");
        if (!QFile::exists(partPath)) continue;

        QFile episodesCacheFile(partPath);
        if (!episodesCacheFile.open(QFile::ReadOnly | QFile::Text)) continue;

        QString episodesJson = episodesCacheFile.readAll();
        episodesCacheFile.close();

        auto episodesArray = QJsonDocument::fromJson(episodesJson.toUtf8()).array();

        foreach (auto releaseEpisodes, episodesArray) {
            auto releaseEpisode = releaseEpisodes.toObject();
            if (!releaseEpisode.contains("releaseId") || !releaseEpisode.contains("items")) continue;

            auto releaseId = releaseEpisode.value("releaseId").toInt(0);
            auto videos = releaseEpisode.value("items").toArray();

            foreach (auto video, videos) {
                ReleaseOnlineVideoModel* jsonVideo = new ReleaseOnlineVideoModel();
                auto object = video.toObject();
                if (m_synchronizationServicev2 != nullptr) jsonVideo->setPosterHost(m_synchronizationServicev2->apiv2host());
                jsonVideo->readFromApiModel(object, releaseId);

                m_onlineVideos.append(jsonVideo);
                m_onlineVideosMap.insert(jsonVideo->uniqueId(), jsonVideo);
            }
        }
    }

    auto torrentPath = getCachePath("torrents.cache");
    if (!QFile::exists(torrentPath)) return;

    QFile torrentsCacheFile(torrentPath);
    if (!torrentsCacheFile.open(QFile::ReadOnly | QFile::Text)) return;

    QString torrentsJson = torrentsCacheFile.readAll();
    torrentsCacheFile.close();

    auto torrentsArray = QJsonDocument::fromJson(torrentsJson.toUtf8()).array();
    foreach (auto torrentItem, torrentsArray) {
        auto torrentModel = new ApiTorrentModel();
        torrentModel->readFromJson(torrentItem.toObject());
        if (m_synchronizationServicev2 != nullptr) torrentModel->setTorrentHost(m_synchronizationServicev2->apiv2host());
        m_torrentItems.append(torrentModel);
    }
}

void ReleasesViewModel::reloadApiHostInItems()
{
    auto newHost = m_synchronizationServicev2->apiv2host();
    foreach (auto release, *m_releases) {
        release->setPosterHost(newHost);
    }
    foreach (auto video, m_onlineVideos) {
        video->setPosterHost(newHost);
    }
    foreach (auto torrentItem, m_torrentItems) {
        torrentItem->setTorrentHost(newHost);
    }
}

void ReleasesViewModel::loadSchedules()
{
    m_scheduleReleases->clear();

    QFile scheduleCacheFile(getCachePath(scheduleCacheFileName));
    if (!scheduleCacheFile.open(QFile::ReadOnly | QFile::Text)) return;
    QString scheduleJson = scheduleCacheFile.readAll();
    scheduleCacheFile.close();

    QJsonDocument jsonDocument = QJsonDocument::fromJson(scheduleJson.toUtf8());
    auto schedule = jsonDocument.object();
    auto keys = schedule.keys();
    foreach (auto key, keys) {
        auto scheduleDay = schedule.value(key).toInt();
        m_scheduleReleases->insert(key.toInt(), scheduleDay);
    }
}

void ReleasesViewModel::saveSchedule(QString json)
{
    if (json.isEmpty()) return;

    auto jsonDocument = QJsonDocument::fromJson(json.toUtf8());
    auto root = jsonDocument.object();
    if (!root.contains("data")) return;
    if (!root["data"].isArray()) return;
    auto data = root["data"].toArray();

    QJsonObject savedObject;

    foreach (auto dataItem, data) {
        auto itemObject = dataItem.toObject();
        auto day = itemObject["day"].toString();
        auto items = itemObject["items"].toArray();
        foreach (auto item, items) {
            auto key = QString::number(itemObject["id"].toInt());
            savedObject[key] = day;
        }
    }

    QJsonDocument document(savedObject);

    QFile scheduleCacheFile(getCachePath(scheduleCacheFileName));
    scheduleCacheFile.open(QFile::WriteOnly | QFile::Text);
    scheduleCacheFile.write(document.toJson());
    scheduleCacheFile.close();

    loadSchedules();
}

void ReleasesViewModel::saveFavoritesFromJson(QString data)
{
    if (data.isEmpty()) return;

    auto jsonDocument = QJsonDocument::fromJson(data.toUtf8());
    auto root = jsonDocument.object();
    if (!root.contains("data")) return;
    if (!root["data"].isObject()) return;
    auto responseData = root["data"].toObject();
    if (!responseData.contains("items")) return;
    if (!responseData["items"].isArray()) return;
    auto items = responseData["items"].toArray();

    QList<int> ids;
    foreach (auto item, items) {
        auto currentObject = item.toObject();
        ids.append(currentObject["id"].toInt());
    }

    saveFavoritesFromArray(ids);
}

void ReleasesViewModel::saveFavoritesFromArray(const QList<int> ids)
{
    QJsonArray array;
    foreach (auto item, ids) {
        QJsonValue value(item);
        array.append(value);
    }

    QJsonDocument document(array);

    QFile favoritesCacheFile(getCachePath(favoriteCacheFileName));
    favoritesCacheFile.open(QFile::WriteOnly | QFile::Text);
    favoritesCacheFile.write(document.toJson());
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

    auto jsonDocument = QJsonDocument::fromJson(favoritesJson.toUtf8());
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
    QFile seenMarkFile(getCachePath(extendedSeenMarkCacheFileName));
    if (!seenMarkFile.open(QFile::ReadOnly | QFile::Text)) return;
    auto seenMarkJson = seenMarkFile.readAll();
    seenMarkFile.close();

    auto document = QJsonDocument::fromJson(seenMarkJson);
    auto jsonSeenMarks = document.array();

    m_extendedSeenMarks.clear();

    foreach (auto item, jsonSeenMarks) {
        auto object = item.toObject();
        auto id = object.value("id").toString();
        auto mark = object.value("mark").toBool();
        auto time = object.value("time").toInt();
        auto tuple = std::make_tuple(mark, time);
        m_extendedSeenMarks.insert(id, tuple);
    }

    recalculateSeenCounts();
}

void ReleasesViewModel::recalculateSeenCounts()
{
    QMap<int, int> releasesCount;
    int countSeens = 0;
    foreach (auto onlineVideo, m_onlineVideos) {
        auto videoId = onlineVideo->uniqueId();

        if (m_extendedSeenMarks.contains(videoId)) {
            auto item = m_extendedSeenMarks.value(videoId);
            auto mark = std::get<0>(item);
            if (!mark) continue;

            auto releaseId = onlineVideo->releaseId();
            if (releasesCount.contains(releaseId)) {
                auto count = releasesCount[releaseId];
                count++;
                auto allCount = 0;
                if (m_releasesMap->contains(releaseId)) allCount = m_releasesMap->value(releaseId)->countOnlineVideos();
                if (count >= allCount) {
                    countSeens++;
                    continue;
                }
                releasesCount[releaseId] = count;
            } else {
                releasesCount.insert(releaseId, 1);
            }
        }
    }
    releasesCount.clear();

    setCountSeens(countSeens);
    emit hasCinemahallNotSeenVideosChanged();
}

void ReleasesViewModel::saveSeenMarks()
{
    QJsonArray array;

    auto keys = m_extendedSeenMarks.keys();

    foreach(auto key, keys) {
        auto item = m_extendedSeenMarks.value(key);

        bool mark = std::get<0>(item);
        int time = std::get<1>(item);
        QJsonObject object;
        object["id"] = key;
        object["mark"] = mark;
        object["time"] = time;

        array.append(object);
    }

    QJsonDocument seenDocument(array);
    QString seenMarkJson(seenDocument.toJson());

    QFile seenMarkFile(getCachePath(extendedSeenMarkCacheFileName));
    if (!seenMarkFile.open(QFile::WriteOnly | QFile::Text)) return;

    seenMarkFile.write(seenMarkJson.toUtf8());
    seenMarkFile.close();

    recalculateSeenCounts();
}

void ReleasesViewModel::loadCollections()
{
    QFile collectionFile(getCachePath(collectionsCacheFileName));
    if (!collectionFile.open(QFile::ReadOnly | QFile::Text)) return;
    auto collectionJson = collectionFile.readAll();
    collectionFile.close();

    auto document = QJsonDocument::fromJson(collectionJson);
    auto jsonCollections = document.array();

    m_collections.clear();

    foreach (auto item, jsonCollections) {
        auto itemObject = item.toObject();
        auto id = itemObject.value("id").toInt();
        auto collection = itemObject.value("collection").toString();

        m_collections.insert(id, collection);
    }
}

void ReleasesViewModel::saveCollections()
{
    QJsonArray array;

    auto keys = m_collections.keys();

    foreach(auto key, keys) {
        auto item = m_collections.value(key);

        QJsonObject object;
        object["id"] = key;
        object["collection"] = item;

        array.append(object);
    }

    QJsonDocument seenDocument(array);
    QString seenMarkJson(seenDocument.toJson());

    QFile collectionFile(getCachePath(collectionsCacheFileName));
    if (!collectionFile.open(QFile::WriteOnly | QFile::Text)) return;

    collectionFile.write(seenMarkJson.toUtf8());
    collectionFile.close();

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

QString ReleasesViewModel::getMultipleLinks(QString text) const noexcept
{
    if (text.isEmpty()) return "";
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    auto parts = text.split(",");
#else
    auto parts = text.split(",", Qt::SkipEmptyParts);
#endif
    for (int i = 0; i < parts.count(); i++) {
        auto value = parts[i];
        parts[i] = "<a href=\"" + value + "\">" + value + "</a>";
    }
    return parts.join(", ");
}

FullReleaseModel *ReleasesViewModel::getReleaseByCode(QString code) const noexcept
{
    auto iterator = std::find_if(
        m_releases->begin(),
        m_releases->end(),
        [code](FullReleaseModel* item)
        {
            return item->code() == code;
        }
    );

    if(iterator == m_releases->end()) return nullptr;

    return *iterator;
}

int ReleasesViewModel::randomBetween(int low, int high) const noexcept
{
    return QRandomGenerator::global()->bounded(low, high);
}

QHash<int, int> ReleasesViewModel::getAllSeenMarkCount() noexcept
{
    QHash<int, int> releasesCount;

    foreach (auto onlineVideo, m_onlineVideos) {
        auto videoId = onlineVideo->uniqueId();

        if (m_extendedSeenMarks.contains(videoId)) {
            auto releaseId = onlineVideo->releaseId();
            if (releasesCount.contains(releaseId)) {
                auto count = releasesCount[releaseId];
                count++;
                releasesCount[releaseId] = count;
            }
        }
    }

    return releasesCount;
}

void ReleasesViewModel::synchronizedSchedule(const QString &data)
{
    saveSchedule(data);
}

void ReleasesViewModel::userFavoritesReceived(const QString &data)
{
    saveFavoritesFromJson(data);
    loadFavorites();
    m_items->refresh();
}

void ReleasesViewModel::userFavoritesReceivedV2(const QList<int>& data)
{
    saveFavoritesFromArray(data);
    loadFavorites();
    m_items->refresh();
}

void ReleasesViewModel::cinemahallItemsChanged()
{
    emit hasCinemahallNotSeenVideosChanged();
}

void ReleasesViewModel::needDeleteFavorites(const QList<int> &ids)
{
    foreach (auto id, ids) {
        removeReleaseFromFavorites(id);
    }
}
