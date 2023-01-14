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
#include <QMutableStringListIterator>
#include "releasesviewmodel.h"
#include "../../globalhelpers.h"

ReleasesViewModel::ReleasesViewModel(QObject *parent) : QObject(parent)
{
    m_items = new ReleasesListModel(this);
    m_items->setup(m_releases, m_scheduleReleases, m_userFavorites, m_hiddenReleases, m_seenMarks, m_historyItems, m_releaseChanges, m_cinemahall);
    m_cinemahall->setup(m_releases, m_seenMarks);
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
    m_sectionNames.append("Популярное в 2021");
    m_sectionNames.append("Просмотренные до конца");
    m_sectionNames.append("Просмотренные не до конца");
    m_sectionNames.append("Популярное в 2022");
    m_sectionNames.append("В кинозале");
    m_sectionNames.append("Текущий сезон");
    m_sectionNames.append("Не текущий сезон");

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

    m_items->refresh();

    connect(m_releasesUpdateWatcher, &QFutureWatcher<bool>::finished, this, &ReleasesViewModel::releasesUpdated);
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

void ReleasesViewModel::setSynchronizationService(SynchronizationService *synchronizationService) noexcept
{
    if (m_synchronizationService == synchronizationService) return;

    bool isFirstTime = m_synchronizationService == nullptr;

    m_synchronizationService = synchronizationService;
    emit synchronizationServiceChanged();

    if (!isFirstTime) return;

    connect(m_synchronizationService, &SynchronizationService::synchronizedReleases, this, &ReleasesViewModel::synchronizedReleases);
    connect(m_synchronizationService, &SynchronizationService::synchronizedSchedule, this, &ReleasesViewModel::synchronizedSchedule);
    connect(m_synchronizationService, &SynchronizationService::userFavoritesReceived, this,&ReleasesViewModel::userFavoritesReceived);

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

    return "<b>Озвучка:</b> " + getMultipleLinks(m_openedRelease->voicers());
}

bool ReleasesViewModel::openedReleaseIsAllSeen() const noexcept
{
    if (m_openedRelease == nullptr) return "";

    return m_openedRelease->countOnlineVideos() == m_items->getReleaseSeenMarkCount(m_openedRelease->id());
}

QStringList ReleasesViewModel::getMostPopularGenres() const noexcept
{
    QStringList allGenres;
    QSet<int> alreadyProcessed;
    QHash<QString, int> genresCount;
    QHashIterator<QString, bool> iterator(*m_seenMarks);

    while(iterator.hasNext()) {
        iterator.next();

        QString key = iterator.key();
        auto parts = key.splitRef(".");

        auto id = parts[0].toInt();
        if (alreadyProcessed.contains(id)) continue;

        alreadyProcessed.insert(id);

        auto release = getReleaseById(id);
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
    QHashIterator<QString, bool> iterator(*m_seenMarks);

    while(iterator.hasNext()) {
        iterator.next();

        QString key = iterator.key();
        auto parts = key.splitRef(".");

        auto id = parts[0].toInt();
        if (alreadyProcessed.contains(id)) continue;

        alreadyProcessed.insert(id);

        auto release = getReleaseById(id);
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

        result.append(item);
        iteratorIndex++;
    }

    return result;
}

void ReleasesViewModel::fillNewInFavorites(QList<FullReleaseModel *>* list) const noexcept
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

void ReleasesViewModel::fillNewFromStart(QList<FullReleaseModel *> *list) const noexcept
{
    auto applicationStart = m_userActivity->previousApplicationStart();
    foreach (auto release, *m_releases) {
        if (release->timestamp() < applicationStart) continue;

        list->append(release);
    }
}

void ReleasesViewModel::fillNewFromLastTwoDays(QList<FullReleaseModel *> *list) const noexcept
{
    auto now = QDateTime::currentDateTimeUtc().addDays(-3);
    auto timestamp = static_cast<int>(now.toTime_t());
    foreach (auto release, *m_releases) {
        if (release->timestamp() < timestamp) continue;

        list->append(release);
    }
}

void ReleasesViewModel::fillAbandonedSeens(QList<FullReleaseModel *> *list) const noexcept
{
    auto now = QDateTime::currentDateTimeUtc().addDays(-18);
    auto timestamp = static_cast<int>(now.toTime_t());

    foreach (auto release, *m_releases) {
        if (!m_historyItems->contains(release->id())) continue;

        auto historyItem = m_historyItems->value(release->id());
        if (historyItem->watchTimestamp() == 0) continue;

        auto seenVideos = m_items->getReleaseSeenMarkCount(release->id());
        if (seenVideos > 0 && seenVideos < release->countOnlineVideos() && historyItem->watchTimestamp() < timestamp) {
            list->append(release);
        }
    }
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
    foreach (auto release, *m_releases) {
        auto releaseId = release->id();
        if (!m_userFavorites->contains(releaseId)) continue;

        auto seenVideos = m_items->getReleaseSeenMarkCount(releaseId);
        int watchTimestamp = 0;
        if (m_historyItems->contains(releaseId)) {
             auto item = m_historyItems->value(releaseId);
             watchTimestamp = item->watchTimestamp();
        }

        if (seenVideos == 0 && watchTimestamp == 0) list->append(release);
    }
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

void ReleasesViewModel::clearSelectedReleases() noexcept
{
    m_items->clearSelected();
}

void ReleasesViewModel::addReleaseToFavorites(int id) noexcept
{
    if (!m_userFavorites->contains(id)) {
        m_userFavorites->append(id);
        if (!m_applicationSettings->userToken().isEmpty()) {
            m_synchronizationService->addUserFavorites(m_applicationSettings->userToken(), QString::number(id));
        }
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
        if (!m_applicationSettings->userToken().isEmpty()) {
            m_synchronizationService->removeUserFavorites(m_applicationSettings->userToken(), QString::number(id));
        }
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
    bool needSynchronize = !m_applicationSettings->userToken().isEmpty();
    foreach (auto selectedRelease, *selectedReleases) {
        if (!m_userFavorites->contains(selectedRelease)) {
            m_userFavorites->append(selectedRelease);
            if (needSynchronize) m_synchronizationService->addUserFavorites(m_applicationSettings->userToken(), QString::number(selectedRelease));
            m_items->refreshItem(selectedRelease);
            needSave = true;
        }
    }

    if (needSave) {
        saveFavorites();
        setCountFavorites(m_userFavorites->count());
    }
    clearSelectedReleases();
}

void ReleasesViewModel::removeSelectedReleaseFromFavorites() noexcept
{
    auto selectedReleases = m_items->getSelectedReleases();
    bool needSave = false;
    bool needSynchronize = !m_applicationSettings->userToken().isEmpty();
    foreach (auto selectedRelease, *selectedReleases) {
        if (m_userFavorites->contains(selectedRelease)) {
            m_userFavorites->removeOne(selectedRelease);
            if (needSynchronize) m_synchronizationService->removeUserFavorites(m_applicationSettings->userToken(), QString::number(selectedRelease));
            m_items->refreshItem(selectedRelease);
            needSave = true;
        }
    }

    if (needSave) {
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

void ReleasesViewModel::showReleaseCard(int id) noexcept
{
    auto release = getReleaseById(id);
    m_openedRelease = release;

    m_releaseTorrentsList->loadTorrentsFromJson(release->torrents());

    setToReleaseHistory(release->id(), 0);

    resetReleaseChanges(release->id());

    refreshOpenedReleaseCard();

    emit isOpenedCardChanged();

    emit releaseCardOpened();
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

void ReleasesViewModel::hideAfterWatchReleaseCard() noexcept
{
    if (m_notCloseReleaseCardAfterWatch) return;

    m_openedRelease = nullptr;
    emit isOpenedCardChanged();
}

void ReleasesViewModel::setSeenMarkAllSeries(int id, int countSeries, bool marked)
{
    setSeenMarkForRelease(id, countSeries, marked);

    saveSeenMarks();

    m_items->refreshItem(id);
}

void ReleasesViewModel::setSeenMarkAllSeriesSelectedReleases(bool marked)
{
    auto selectedReleases = m_items->getSelectedReleases();
    foreach (auto selectedRelease, *selectedReleases) {
        auto release = getReleaseById(selectedRelease);

        setSeenMarkForRelease(selectedRelease, release->countOnlineVideos(), marked);
        m_items->refreshItem(selectedRelease);
    }

    saveSeenMarks();
}

void ReleasesViewModel::setSeenMarkForRelease(int id, int countSeries, bool marked)
{
    for (int i = 0; i < countSeries; i++) {
        auto key = QString::number(id) + "." + QString::number(i);
        if (marked) {
            if (!m_seenMarks->contains(key)) m_seenMarks->insert(key, true);
        } else {
            if (m_seenMarks->contains(key)) m_seenMarks->remove(key);
        }
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
    emit openedReleaseVideosChanged();
}

void ReleasesViewModel::setSeenMark(int id, int seriaId, bool marked)
{
    auto key = QString::number(id) + "." + QString::number(seriaId);
    if (marked) {
        if (!m_seenMarks->contains(key)) m_seenMarks->insert(key, true);
    } else {
        if (m_seenMarks->contains(key)) m_seenMarks->remove(key);
    }
    saveSeenMarks();
}

bool ReleasesViewModel::toggleSeenMark(int id, int seriaId) noexcept
{
    auto key = QString::number(id) + "." + QString::number(seriaId);

    if (m_seenMarks->contains(key)) {
        m_seenMarks->remove(key);
        saveSeenMarks();
        return false;
    }

    if (!m_seenMarks->contains(key)) {
        m_seenMarks->insert(key, true);
        saveSeenMarks();
        return true;
    }

    return false;
}

bool ReleasesViewModel::getSeriaSeenMark(int id, int seriaId) const noexcept
{
    auto key = QString::number(id) + "." + QString::number(seriaId);
    return m_seenMarks->contains(key);
}

QHash<QString, bool> *ReleasesViewModel::getSeenMarks()
{
    return m_seenMarks;
}

void ReleasesViewModel::updateAllReleases(const QList<QString> &releases, bool insideData)
{
    if (releases.isEmpty()) {
        setSynchronizationEnabled(false);
        emit errorWhileReleaseSynchronization();
        return;
    }

    foreach (auto release, releases) {
        if (release.isEmpty()) {
            setSynchronizationEnabled(false);
            emit errorWhileReleaseSynchronization();
            return;
        }
    }

    QFuture<bool> future = QtConcurrent::run(
        [=] {
            auto jsons = QList<QJsonDocument>();

            foreach (auto jsonPage, releases) {
                QJsonParseError jsonError;
                jsons.append(QJsonDocument::fromJson(jsonPage.toUtf8(), &jsonError));
                if (jsonError.error != 0) {
                    setSynchronizationEnabled(false);
                    emit errorWhileReleaseSynchronization();
                    return false;
                }
            }

            QJsonArray jsonReleases;
            if (insideData) {
                foreach (auto jsonDocument, jsons) {
                    auto rootObject = jsonDocument.object();
                    bool isValid = rootObject.contains("data") && rootObject["data"].isObject();
                    if (isValid) {
                        auto dataObject = rootObject["data"].toObject();
                        isValid = dataObject.contains("items") && dataObject["items"].isArray();
                        if (isValid) {
                            auto pageReleases = dataObject["items"].toArray();
                            foreach (auto pageRelease, pageReleases) {
                                jsonReleases.append(pageRelease);
                            }
                        }
                    }
                    if (!isValid) {
                        setSynchronizationEnabled(false);
                        emit errorWhileReleaseSynchronization();
                        return false;
                    }
                }
            } else {
                jsonReleases = jsons.first().array();
            }

            auto filterByFavorites = m_items->filterByFavorites();

            auto oldReleasesCount = m_releaseChanges->newReleases()->count();
            auto oldOnlineSeriesCount = getCountFromChanges(m_releaseChanges->newOnlineSeries(), filterByFavorites);
            auto oldTorrentsCount = getCountFromChanges(m_releaseChanges->newTorrents(), filterByFavorites);
            auto oldTorrentSeriesCount = getCountFromChanges(m_releaseChanges->newTorrentSeries(), filterByFavorites);

            auto isFirstStart = m_releases->count() == 0;

            QSharedPointer<QSet<int>> hittedMaps = QSharedPointer<QSet<int>>(new QSet<int>());

            foreach (QJsonValue jsonRelease, jsonReleases) {
                mapToFullReleaseModel(jsonRelease.toObject(), isFirstStart, hittedMaps);
            }

            //temporatily disable deleting releases because synchronization diveded on few parts
            //markDeletedReleases(hittedMaps);

            saveReleasesFromMemoryToFile();
            saveChanges();

            auto newReleasesCount = m_releaseChanges->newReleases()->count();
            auto newOnlineSeriesCount = getCountFromChanges(m_releaseChanges->newOnlineSeries(), filterByFavorites);
            auto newTorrentsCount = getCountFromChanges(m_releaseChanges->newTorrents(), filterByFavorites);
            auto newTorrentSeriesCount = getCountFromChanges(m_releaseChanges->newTorrentSeries(), filterByFavorites);

            QString newEntities;
            if (newReleasesCount > oldReleasesCount) newEntities += "Новых релизов " + QString::number(newReleasesCount - oldReleasesCount) + " ";
            if (newOnlineSeriesCount > oldOnlineSeriesCount) newEntities += "Новых серий " + QString::number(newOnlineSeriesCount - oldOnlineSeriesCount) + " ";
            if (newTorrentsCount > oldTorrentsCount) newEntities += "Новых торрентов " + QString::number(newTorrentsCount - oldTorrentsCount) + " ";
            if (newTorrentSeriesCount > oldTorrentSeriesCount) newEntities += "Новых серий в торрентах " + QString::number(newTorrentSeriesCount - oldTorrentSeriesCount);
            setNewEntities(newEntities);

            return true;
        }
    );
    m_releasesUpdateWatcher->setFuture(future);
}

uint32_t ReleasesViewModel::getCountFromChanges(const QList<int> *releases, bool filterByFavorites)
{
    if (filterByFavorites) {
        uint32_t count = 0;
        foreach (auto releaseId, *releases) {
            if (m_userFavorites->contains(releaseId)) count++;
        }
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
    auto release = m_releasesMap->value(id);
    m_itemTorrents->loadFromJson(release->torrents());
}

void ReleasesViewModel::clearDeletedInCacheMarks()
{
    foreach (auto release, *m_releases) {
        release->setIsDeleted(false);
    }
    saveReleasesFromMemoryToFile();
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

void ReleasesViewModel::removeAllSeenMark()
{
    m_seenMarks->clear();

    saveSeenMarks();
    m_items->refresh();
}

void ReleasesViewModel::reloadReleases()
{
    loadReleases();
    m_items->refresh();
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

QString ReleasesViewModel::getReleaseVideos(int id) const noexcept
{
    auto release = getReleaseById(id);
    if (release == nullptr) return "";

    return release->videos();
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

        m_hiddenReleases->remove(index);
    }

    saveHidedReleases();
}

void ReleasesViewModel::removeFromHidedSelectedReleases() noexcept
{
    auto selectedReleases = m_items->getSelectedReleases();
    foreach(auto selectedRelease, *selectedReleases) {
        auto index = m_hiddenReleases->indexOf(selectedRelease);
        if (index == -1) continue;

        m_hiddenReleases->remove(index);
    }

    saveHidedReleases();
}

void ReleasesViewModel::removeAllHidedReleases() noexcept
{
    m_hiddenReleases->clear();

    saveHidedReleases();
    m_items->refresh();
}

bool ReleasesViewModel::importReleasesFromFile(QString path)
{
    auto filePath = path.replace("file:///", "").replace("file://", "");
    QFile importFile(filePath);
    if (!importFile.open(QFile::ReadOnly | QFile::Text)) {
        qInfo() << "Error while import releases from file";
        return false;
    }

    auto json = importFile.readAll();
    importFile.close();

    QList<QString> list;
    list.append(json);
    updateAllReleases(list, false);

    return true;
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
    loadReleasesWithoutReactive();

    setCountReleases(m_releases->count());
}

void ReleasesViewModel::loadReleasesWithoutReactive()
{
    while (m_releases->count()) delete m_releases->takeLast();
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
        auto scheduleDay = schedule.value(key).toString();
        m_scheduleReleases->insert(key.toInt(), scheduleDay.toInt());
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
        auto day = dataItem["day"].toString();
        auto items = dataItem["items"].toArray();
        foreach (auto item, items) {
            auto key = QString::number(item["id"].toInt());
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

    QVector<int> ids;
    foreach (auto item, items) {
        ids.append(item["id"].toInt());
    }

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
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
        auto parts = item.splitRef(".");
#else
        auto parts = item.splitRef(".", Qt::SkipEmptyParts);
#endif
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

        if (!m_releasesMap->contains(seenIterator.key())) continue;

        auto release = m_releasesMap->value(seenIterator.key());
        if (release->countOnlineVideos() == seenIterator.value()) countSeens += 1;
    }

    setCountSeens(countSeens);
    emit hasCinemahallNotSeenVideosChanged();
}

void ReleasesViewModel::saveSeenMarks()
{
    QJsonArray array;

    QHashIterator<QString, bool> iterator(*m_seenMarks);
    while (iterator.hasNext()) {
        iterator.next();

        QJsonValue value(iterator.key());
        array.append(value);
    }

    QJsonDocument seenDocument(array);
    QString seenMarkJson(seenDocument.toJson());

    QFile seenMarkFile(getCachePath(seenMarkCacheFileName));
    if (!seenMarkFile.open(QFile::WriteOnly | QFile::Text)) return;

    seenMarkFile.write(seenMarkJson.toUtf8());
    seenMarkFile.close();

    recalculateSeenCounts();
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

void ReleasesViewModel::saveReleasesFromMemoryToFile()
{
    QJsonArray releasesArray;
    foreach (auto release, *m_releases) {
        QJsonObject jsonObject;
        release->writeToJson(jsonObject);
        releasesArray.append(jsonObject);
    }
    QJsonDocument document(releasesArray);

    QFile file(getCachePath(releasesCacheFileName));
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    file.write(document.toJson());
    file.close();
}

void ReleasesViewModel::mapToFullReleaseModel(QJsonObject &&jsonObject, const bool isFirstStart, QSharedPointer<QSet<int>> hittedIds)
{
    auto id = jsonObject.value("id").toInt();

    hittedIds->insert(id);

    FullReleaseModel* model = nullptr;

    auto isNew = !m_releasesMap->contains(id);
    if (isNew) {
        model = new FullReleaseModel();
        model->setId(id);
    } else {
        model = m_releasesMap->value(id);
    }

    if (!isFirstStart && isNew && !m_releaseChanges->newReleases()->contains(id)) {
        m_releaseChanges->newReleases()->append(id);
    }

    auto voicersJson = jsonObject.value("voices").toArray();
    QStringList voicers;
    foreach(const QJsonValue & voicer, voicersJson) voicers.append(voicer.toString());

    auto genresJson = jsonObject.value("genres").toArray();
    QStringList allGenres;
    foreach(const QJsonValue & genre, genresJson) allGenres.append(genre.toString());

    auto names = jsonObject.value("names").toArray();

    auto torrents = jsonObject.value("torrents").toArray();
    auto torrentJson = QJsonDocument(torrents).toJson();

    if (!isNew && torrents.count() != model->countTorrents() && !m_releaseChanges->newTorrents()->contains(id)) {
        m_releaseChanges->newTorrents()->append(id);
    }

    if (!isNew && !m_releaseChanges->newTorrentSeries()->contains(id)) {
        foreach (auto torrentItem, torrents) {
            auto series = torrentItem.toObject()["series"].toString();
            series = series.replace("\\", "\\\\");
            if (!model->torrents().contains("series\": \"" + series + "\"")) {
                m_releaseChanges->newTorrentSeries()->append(id);
                break;
            }
        }
    }

    auto videos = jsonObject.value("playlist").toArray();    
    auto videosJson = QJsonDocument(videos).toJson();

    if (!isNew && videos.count() != model->countOnlineVideos() && !m_releaseChanges->newOnlineSeries()->contains(id)) {
        m_releaseChanges->newOnlineSeries()->append(id);
    }

    auto voices = voicers.join(", ");
    if (voices.length() == 0) voices = "Не указано";

    auto genres = allGenres.join(", ");
    if (genres.length() == 0) genres = "Не указано";

    if (!isNew) model->setId(jsonObject.value("id").toInt());

    model->setTitle(names.first().toString());
    model->setCode(jsonObject.value("code").toString());
    model->setOriginalName(names.last().toString());
    model->setRating(jsonObject.value("favorite").toObject().value("rating").toInt(0));
    model->setSeries(jsonObject.value("series").toString());
    model->setStatus(jsonObject.value("status").toString());
    auto releaseType = jsonObject.value("type").toString();
    model->setType(releaseType.isEmpty() ? "Не указано" : releaseType);
    if (jsonObject.value("last").isString()) {
        auto timestamp = jsonObject.value("last").toString();
        model->setTimestamp(timestamp.toInt());
    } else {
        model->setTimestamp(jsonObject.value("last").toInt());
    }
    if (jsonObject.value("year").isString()) {
        model->setYear(jsonObject.value("year").toString());
    } else {
        auto yearInt = jsonObject.value("year").toInt();
        model->setYear(QString::number(yearInt));
    }
    model->setSeason(jsonObject.value("season").toString());
    model->setCountTorrents(torrents.count());
    model->setCountOnlineVideos(videos.count());
    model->setDescription(jsonObject.value("description").toString());
    model->setAnnounce(jsonObject.value("announce").toString());
    model->setVoicers(voices);
    model->setGenres(genres);
    model->setVideos(videosJson);
    model->setTorrents(torrentJson);

    auto poster = jsonObject.value("poster").toString();
    if (!isNew && poster != model->poster() && !model->poster().endsWith(poster, Qt::CaseInsensitive)) {
        m_localStorage->invalidateReleasePoster(id);
    }
    model->setPoster(jsonObject.value("poster").toString());

    if (isNew) {
        m_releases->append(model);
        m_releasesMap->insert(model->id(), model);
    }
}

void ReleasesViewModel::markDeletedReleases(QSharedPointer<QSet<int> > hittedIds)
{
    auto keys = m_releasesMap->keys();
    foreach(auto key, keys) {
        if (!hittedIds->contains(key)) {
            auto release = m_releasesMap->value(key);
            release->setIsDeleted(true);
        }
    }
}

QString ReleasesViewModel::videosToJson(QList<OnlineVideoModel> &videos)
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

QString ReleasesViewModel::torrentsToJson(QList<ReleaseTorrentModel> &torrents)
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

QHash<int, int> ReleasesViewModel::getAllSeenMarkCount() noexcept
{
    QHash<int, int> result;
    QHashIterator<QString, bool> iterator(*m_seenMarks);
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

void ReleasesViewModel::releasesUpdated()
{
    if (!m_releasesUpdateWatcher->result()) return;

    setCountReleases(m_releases->count());
    m_items->refresh();

    m_synchronizationService->synchronizeSchedule();
    if (!m_applicationSettings->userToken().isEmpty()) m_synchronizationService->synchronizeUserFavorites(m_applicationSettings->userToken());

    setSynchronizationEnabled(false);
    emit afterSynchronizedReleases();
}

void ReleasesViewModel::synchronizedReleases()
{
    updateAllReleases(m_synchronizationService->getSynchronizedReleasePages(), true);
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

void ReleasesViewModel::cinemahallItemsChanged()
{
    emit hasCinemahallNotSeenVideosChanged();
}
