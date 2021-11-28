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
    m_items->setup(m_releases, m_scheduleReleases, m_userFavorites, m_hiddenReleases, m_seenMarks, m_historyItems, m_releaseChanges);

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

void ReleasesViewModel::setCountReleases(const int& countReleases) noexcept
{
    if (countReleases == m_countReleases) return;

    m_countReleases = countReleases;
    emit countReleasesChanged();
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
    connect(m_synchronizationService, &SynchronizationService::synchronizedFromDL, this, &ReleasesViewModel::synchronizedFromDL);
    connect(m_synchronizationService, &SynchronizationService::synchronizedSchedule, this, &ReleasesViewModel::synchronizedSchedule);
    connect(m_synchronizationService, &SynchronizationService::userFavoritesReceived, this,&ReleasesViewModel::userFavoritesReceived);

}

void ReleasesViewModel::setApplicationSettings(ApplicationSettings *applicationSettings) noexcept
{
    if (m_applicationSettings == applicationSettings) return;

    m_applicationSettings = applicationSettings;
    emit applicationSettingsChanged();
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
        m_synchronizationService->addUserFavorites(m_applicationSettings->userToken(), QString::number(id));
        m_items->refreshItem(id);
        saveFavorites();
    }
    if (m_openedRelease != nullptr) emit openedReleaseInFavoritesChanged();
}

void ReleasesViewModel::removeReleaseFromFavorites(int id) noexcept
{
    if (m_userFavorites->contains(id)) {
        m_userFavorites->removeOne(id);
        m_synchronizationService->removeUserFavorites(m_applicationSettings->userToken(), QString::number(id));
        m_items->refreshItem(id);
        saveFavorites();
    }

    if (m_openedRelease != nullptr) emit openedReleaseInFavoritesChanged();
}

void ReleasesViewModel::addSelectedReleaseToFavorites() noexcept
{
    auto selectedReleases = m_items->getSelectedReleases();
    bool needSave = false;
    foreach (auto selectedRelease, *selectedReleases) {
        if (!m_userFavorites->contains(selectedRelease)) {
            m_userFavorites->append(selectedRelease);
            m_synchronizationService->addUserFavorites(m_applicationSettings->userToken(), QString::number(selectedRelease));
            m_items->refreshItem(selectedRelease);
            needSave = true;
        }
    }

    if (needSave) saveFavorites();
    clearSelectedReleases();
}

void ReleasesViewModel::removeSelectedReleaseFromFavorites() noexcept
{
    auto selectedReleases = m_items->getSelectedReleases();
    bool needSave = false;
    foreach (auto selectedRelease, *selectedReleases) {
        if (m_userFavorites->contains(selectedRelease)) {
            m_userFavorites->removeOne(selectedRelease);
            m_synchronizationService->removeUserFavorites(m_applicationSettings->userToken(), QString::number(selectedRelease));
            m_items->refreshItem(selectedRelease);
            needSave = true;
        }
    }

    if (needSave) saveFavorites();
    clearSelectedReleases();
}

void ReleasesViewModel::closeReleaseCard() noexcept
{
    m_openedRelease = nullptr;

    emit isOpenedCardChanged();
}

void ReleasesViewModel::openDescriptionLink(const QString &link) noexcept
{
    auto descriptionLink = link;
    if (descriptionLink.indexOf("https://www.anilibria.tv/release/") == 0 || descriptionLink.indexOf("http://www.anilibria.tv/release/") == 0) {
        auto code = descriptionLink.replace(QString("https://www.anilibria.tv/release/"), QString("")).replace("http://www.anilibria.tv/release/", "").replace(".html", "");
        if (code.indexOf("?") > -1) code = code.mid( 0, code.indexOf("?"));
        auto release = getReleaseByCode(code);
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
    auto count = m_releases->count() - 1;

    auto position = randomBetween(1, count);

    auto release = m_releases->at(position);

    showReleaseCard(release->id());
}

void ReleasesViewModel::hideReleaseCard() noexcept
{
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

void ReleasesViewModel::updateAllReleases(const QString &releases, bool insideData)
{
    if (releases.isEmpty()) {
        setSynchronizationEnabled(false);
        emit errorWhileReleaseSynchronization();
        return;
    }

    QFuture<bool> future = QtConcurrent::run(
        [=] {
            QJsonParseError jsonError;
            QJsonDocument jsonDocument = QJsonDocument::fromJson(releases.toUtf8(), &jsonError);
            if (jsonError.error != 0) {
                setSynchronizationEnabled(false);
                emit errorWhileReleaseSynchronization();
                return false;
            }

            QJsonArray jsonReleases;
            if (insideData) {
                auto rootObject = jsonDocument.object();
                bool isValid = rootObject.contains("data") && rootObject["data"].isObject();
                if (isValid) {
                    auto dataObject = rootObject["data"].toObject();
                    isValid = dataObject.contains("items") && dataObject["items"].isArray();
                    if (isValid) jsonReleases = dataObject["items"].toArray();
                }
                if (!isValid) {
                    setSynchronizationEnabled(false);
                    emit errorWhileReleaseSynchronization();
                    return false;
                }
            } else {
                jsonReleases = jsonDocument.array();
            }

            auto newReleasesCount = m_releaseChanges->newReleases()->count();
            auto newOnlineSeriesCount = m_releaseChanges->newOnlineSeries()->count();
            auto newTorrentsCount = m_releaseChanges->newTorrents()->count();
            auto newTorrentSeriesCount = m_releaseChanges->newTorrentSeries()->count();
            auto newReleases = m_releaseChanges->newReleases();
            auto newOnlineSeries = m_releaseChanges->newOnlineSeries();
            auto newTorrents = m_releaseChanges->newTorrents();
            auto newTorrentSeries = m_releaseChanges->newTorrentSeries();
            bool isEmptyReleases = m_releases->count() == 0;

            foreach (QJsonValue jsonRelease, jsonReleases) {
                ReleaseModel releaseModel;
                releaseModel.readFromApiModel(jsonRelease.toObject());

                FullReleaseModel* currentReleaseCacheModel = getReleaseById(releaseModel.id());

                FullReleaseModel* newReleaseModel = mapToFullReleaseModel(releaseModel);

                if (currentReleaseCacheModel != nullptr) {
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
                    if (currentReleaseCacheModel->poster() != newReleaseModel->poster()) m_localStorage->invalidateReleasePoster(currentReleaseCacheModel->id());

                    m_releases->removeOne(currentReleaseCacheModel);

                } else {
                    if (!isEmptyReleases) {
                        int newReleaseId = newReleaseModel->id();
                        if (!newReleases->contains(newReleaseId)) newReleases->append(newReleaseId);
                    }
                }
                m_releases->append(newReleaseModel);
            }

            saveReleasesFromMemoryToFile();
            saveChanges();

            //TODO: make check based on favorites
            QString newEntities;
            if (newReleases->count() > newReleasesCount) newEntities += "Новых релизов " + QString::number(newReleases->count() - newReleasesCount) + " ";
            if (newOnlineSeries->count() > newOnlineSeriesCount) newEntities += "Новых серий " + QString::number(newOnlineSeries->count() - newOnlineSeriesCount) + " ";
            if (newTorrents->count() > newTorrentsCount) newEntities += "Новых торрентов " + QString::number(newTorrents->count() - newTorrentsCount) + " ";
            if (newTorrentSeries->count() > newTorrentSeriesCount) newEntities += "Новых серий в торрентах " + QString::number(newTorrentSeries->count() - newTorrentSeriesCount);
            setNewEntities(newEntities);

            return true;
        }
    );
    m_releasesUpdateWatcher->setFuture(future);
}

void ReleasesViewModel::openInExternalPlayer(const QString &url)
{
    if (url.isEmpty()) return;

    QDesktopServices::openUrl(QUrl(url));
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

void ReleasesViewModel::clearAccountFavorites()
{
    clearFavorites();
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

    updateAllReleases(json, false);

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
    m_localStorage->addToCinemahall(items);
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

void ReleasesViewModel::resetReleaseChanges(int releaseId) noexcept
{
    if (m_releaseChanges->newReleases()->contains(releaseId)) m_releaseChanges->newReleases()->removeOne(releaseId);
    if (m_releaseChanges->newOnlineSeries()->contains(releaseId)) m_releaseChanges->newOnlineSeries()->removeOne(releaseId);
    if (m_releaseChanges->newTorrents()->contains(releaseId)) m_releaseChanges->newTorrents()->removeOne(releaseId);
    if (m_releaseChanges->newTorrentSeries()->contains(releaseId)) m_releaseChanges->newTorrentSeries()->removeOne(releaseId);

    saveChanges();
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

FullReleaseModel *ReleasesViewModel::mapToFullReleaseModel(ReleaseModel &releaseModel)
{
    FullReleaseModel* model = new FullReleaseModel();

    auto torrents = releaseModel.torrents();
    auto torrentJson = torrentsToJson(torrents);

    auto videos = releaseModel.videos();
    auto videosJson = videosToJson(videos);

    auto voices = releaseModel.voices().join(", ");
    if (voices.length() == 0) voices = "Не указано";

    auto genres = releaseModel.genres().join(", ");
    if (genres.length() == 0) genres = "Не указано";

    model->setId(releaseModel.id());
    model->setTitle(releaseModel.title());
    model->setCode(releaseModel.code());
    auto names = releaseModel.names();
    auto last = names.last();
    model->setOriginalName(last);
    model->setRating(releaseModel.rating());
    model->setSeries(releaseModel.series());
    model->setStatus(releaseModel.status());
    model->setType(releaseModel.type());
    model->setTimestamp(releaseModel.timestamp().toInt());
    model->setYear(releaseModel.year());
    model->setSeason(releaseModel.season());
    model->setCountTorrents(torrents.count());
    model->setCountOnlineVideos(videos.count());
    model->setDescription(releaseModel.description());
    model->setAnnounce(releaseModel.announce());
    model->setVoicers(voices);
    model->setGenres(genres);
    model->setVideos(videosJson);
    model->setTorrents(torrentJson);
    model->setPoster(releaseModel.poster());

    return model;
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

void ReleasesViewModel::releasesUpdated()
{
    if (!m_releasesUpdateWatcher->result()) return;

    reloadReleases();

    m_synchronizationService->synchronizeSchedule();
    if (!m_applicationSettings->userToken().isEmpty()) m_synchronizationService->synchronizeUserFavorites(m_applicationSettings->userToken());

    setSynchronizationEnabled(false);

    emit afterSynchronizedReleases();
}

void ReleasesViewModel::synchronizedReleases()
{
    updateAllReleases(m_synchronizationService->getSynchronizedReleases(), true);
}

void ReleasesViewModel::synchronizedFromDL(const QString &data)
{
    updateAllReleases(data, false);
}

void ReleasesViewModel::synchronizedSchedule(const QString &data)
{
    saveSchedule(data);
}

void ReleasesViewModel::userFavoritesReceived(const QString &data)
{
    saveFavoritesFromJson(data);
    loadFavorites();
}
