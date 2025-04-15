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

#ifndef RELEASESVIEWMODEL_H
#define RELEASESVIEWMODEL_H

#include <QObject>
#include <QScopedPointer>
#include <QSharedPointer>
#include <QMultiMap>
#include <QMap>
#include "../ListModels/releasetorrentslist.h"
#include "imagebackgroundviewmodel.h"
#include "../Models/fullreleasemodel.h"
#include "../ListModels/releaseslistmodel.h"
#include "../ListModels/cinemahalllistmodel.h"
#include "../ListModels/releasetorrentcommonlist.h"
#include "../Models/historymodel.h"
#include "../Models/changesmodel.h"
#include "../Models/releaseonlinevideomodel.h"
#include "../Models/apitorrentmodel.h"
#include "../Services/applicationsettings.h"
#include "../Services/localstorageservice.h"
#include "../Services/synchronizev2service.h"
#include "../Models/releaseonlinevideomodel.h"
#include "releasecustomgroupsviewmodel.h"
#include "useractivityviewmodel.h"
#include "../Services/releaselinkedseries.h"

class ReleasesViewModel : public QObject
{
    Q_OBJECT    
    Q_PROPERTY(ReleaseTorrentsList* openedCardTorrents READ openedCardTorrents NOTIFY openedCardTorrentsChanged)
    Q_PROPERTY(ReleaseTorrentCommonList* itemTorrents READ itemTorrents NOTIFY itemTorrentsChanged)
    Q_PROPERTY(ImageBackgroundViewModel* imageBackgroundViewModel READ imageBackgroundViewModel NOTIFY imageBackgroundViewModelChanged)
    Q_PROPERTY(int countReleases READ countReleases WRITE setCountReleases NOTIFY countReleasesChanged)
    Q_PROPERTY(ReleasesListModel* items READ items NOTIFY itemsChanged)
    Q_PROPERTY(CinemahallListModel* cinemahall READ cinemahall NOTIFY cinemahallChanged)
    Q_PROPERTY(UserActivityViewModel* userActivity READ userActivity WRITE setUserActivity NOTIFY userActivityChanged)
    Q_PROPERTY(int countSeens READ countSeens WRITE setCountSeens NOTIFY countSeensChanged)
    Q_PROPERTY(bool isChangesExists READ isChangesExists NOTIFY isChangesExistsChanged)
    Q_PROPERTY(int countFavorites READ countFavorites WRITE setCountFavorites NOTIFY countFavoritesChanged)
    Q_PROPERTY(QStringList sectionNames READ sectionNames NOTIFY sectionNamesChanged)
    Q_PROPERTY(int newReleasesCount READ newReleasesCount  NOTIFY newReleasesCountChanged)
    Q_PROPERTY(int newOnlineSeriesCount READ newOnlineSeriesCount NOTIFY newOnlineSeriesCountChanged)
    Q_PROPERTY(int newTorrentsCount READ newTorrentsCount NOTIFY newTorrentsCountChanged)
    Q_PROPERTY(int newTorrentSeriesCount READ newTorrentSeriesCount NOTIFY newTorrentSeriesCountChanged)
    Q_PROPERTY(bool notificationForFavorites READ notificationForFavorites WRITE setNotificationForFavorites NOTIFY notificationForFavoritesChanged)
    Q_PROPERTY(bool showSidePanel READ showSidePanel WRITE setShowSidePanel NOTIFY showSidePanelChanged)
    Q_PROPERTY(bool selectMode READ selectMode WRITE setSelectMode NOTIFY selectModeChanged)
    Q_PROPERTY(ApplicationSettings* applicationSettings READ applicationSettings WRITE setApplicationSettings NOTIFY applicationSettingsChanged)
    Q_PROPERTY(LocalStorageService* localStorage READ localStorage WRITE setLocalStorage NOTIFY localStorageChanged)
    Q_PROPERTY(bool hasCinemahallNotSeenVideos READ hasCinemahallNotSeenVideos NOTIFY hasCinemahallNotSeenVideosChanged)
    Q_PROPERTY(bool isOpenedCard READ isOpenedCard NOTIFY isOpenedCardChanged)
    Q_PROPERTY(int openedReleaseId READ openedReleaseId NOTIFY openedReleaseIdChanged)
    Q_PROPERTY(QString openedReleasePoster READ openedReleasePoster NOTIFY openedReleasePosterChanged)
    Q_PROPERTY(QString openedReleaseTitle READ openedReleaseTitle NOTIFY openedReleaseTitleChanged)
    Q_PROPERTY(QString openedReleaseOriginalName READ openedReleaseOriginalName NOTIFY openedReleaseOriginalNameChanged)
    Q_PROPERTY(QString openedReleaseStatus READ openedReleaseStatus NOTIFY openedReleaseStatusChanged)
    Q_PROPERTY(QString openedReleaseStatusDisplay READ openedReleaseStatusDisplay NOTIFY openedReleaseStatusDisplayChanged)
    Q_PROPERTY(QString openedReleaseYear READ openedReleaseYear NOTIFY openedReleaseYearChanged)
    Q_PROPERTY(QString openedReleaseYearDisplay READ openedReleaseYearDisplay NOTIFY openedReleaseYearDisplayChanged)
    Q_PROPERTY(bool openedReleaseInSchedule READ openedReleaseInSchedule NOTIFY openedReleaseInScheduleChanged)
    Q_PROPERTY(QString openedReleaseInScheduleDisplay READ openedReleaseInScheduleDisplay NOTIFY openedReleaseInScheduleDisplayChanged)
    Q_PROPERTY(QString openedReleaseSeason READ openedReleaseSeason NOTIFY openedReleaseSeasonChanged)
    Q_PROPERTY(QString openedReleaseSeasonDisplay READ openedReleaseSeasonDisplay NOTIFY openedReleaseSeasonDisplayChanged)
    Q_PROPERTY(QString openedReleaseType READ openedReleaseType NOTIFY openedReleaseTypeChanged)
    Q_PROPERTY(QString openedReleaseTypeDisplay READ openedReleaseTypeDisplay NOTIFY openedReleaseTypeDisplayChanged)
    Q_PROPERTY(QString openedReleaseGenres READ openedReleaseGenres NOTIFY openedReleaseGenresChanged)
    Q_PROPERTY(QString openedReleaseGenresDisplay READ openedReleaseGenresDisplay NOTIFY openedReleaseGenresDisplayChanged)
    Q_PROPERTY(QString openedReleaseVoices READ openedReleaseVoices NOTIFY openedReleaseVoicesChanged)
    Q_PROPERTY(QString openedReleaseVoicesDisplay READ openedReleaseVoicesDisplay NOTIFY openedReleaseVoicesDisplayChanged)
    Q_PROPERTY(bool openedReleaseIsAllSeen READ openedReleaseIsAllSeen NOTIFY openedReleaseIsAllSeenChanged)
    Q_PROPERTY(QString openedReleaseDescription READ openedReleaseDescription NOTIFY openedReleaseDescriptionChanged)
    Q_PROPERTY(QString openedReleaseCode READ openedReleaseCode NOTIFY openedReleaseCodeChanged)
    Q_PROPERTY(int openedReleaseCountTorrents READ openedReleaseCountTorrents NOTIFY openedReleaseCountTorrentsChanged)
    Q_PROPERTY(int openedReleaseCountVideos READ openedReleaseCountVideos NOTIFY openedReleaseCountVideosChanged)
    Q_PROPERTY(int openedReleaseSeenCountVideos READ openedReleaseSeenCountVideos NOTIFY openedReleaseSeenCountVideosChanged)
    Q_PROPERTY(bool openedReleaseInHided READ openedReleaseInHided NOTIFY openedReleaseInHidedChanged)
    Q_PROPERTY(bool openedReleaseInFavorites READ openedReleaseInFavorites NOTIFY openedReleaseInFavoritesChanged)
    Q_PROPERTY(QString openedReleaseAnnounce READ openedReleaseAnnounce NOTIFY openedReleaseAnnounceChanged)
    Q_PROPERTY(bool openedReleaseIsRutube READ openedReleaseIsRutube NOTIFY openedReleaseIsRutubeChanged)
    Q_PROPERTY(bool openedReleaseInCollections READ openedReleaseInCollections NOTIFY openedReleaseInCollectionsChanged)
    Q_PROPERTY(bool synchronizationEnabled READ synchronizationEnabled WRITE setSynchronizationEnabled NOTIFY synchronizationEnabledChanged)
    Q_PROPERTY(QString newEntities READ newEntities WRITE setNewEntities NOTIFY newEntitiesChanged)
    Q_PROPERTY(bool notCloseReleaseCardAfterWatch READ notCloseReleaseCardAfterWatch WRITE setNotCloseReleaseCardAfterWatch NOTIFY notCloseReleaseCardAfterWatchChanged)
    Q_PROPERTY(QList<int> countSections READ countSections NOTIFY countSectionsChanged)
    Q_PROPERTY(ReleaseCustomGroupsViewModel* customGroups READ customGroups NOTIFY customGroupsChanged)
    Q_PROPERTY(Synchronizev2Service* synchronizationServicev2 READ synchronizationServicev2 WRITE setSynchronizationServicev2 NOTIFY synchronizationServicev2Changed)
    Q_PROPERTY(int proxyPort READ proxyPort WRITE setProxyPort NOTIFY proxyPortChanged FINAL)
    Q_PROPERTY(ReleaseLinkedSeries* releaseLinkedSeries READ releaseLinkedSeries WRITE setReleaseLinkedSeries NOTIFY releaseLinkedSeriesChanged)
    Q_PROPERTY(QString openedReleseExtensionContent READ openedReleseExtensionContent WRITE setOpenedReleseExtensionContent NOTIFY openedReleseExtensionContentChanged FINAL)

private:
    const QString releasesCacheFileName { "releases.cache" };
    const QString metadataCacheFileName { "metadata" };
    const QString scheduleCacheFileName { "schedule.cache" };
    const QString favoriteCacheFileName { "favorites.cache" };
    const QString hidedReleasesCacheFileName { "hidedreleases.cache" };
    const QString seenMarkCacheFileName { "seenmark.cache" };
    const QString extendedSeenMarkCacheFileName { "extendedseenmark.cache" };
    const QString historyCacheFileName { "history.cache" };
    const QString notificationCacheFileName { "notification.cache" };
    const QString collectionsCacheFileName { "collections.cache" };
    QStringList m_sectionNames;
    ReleaseTorrentsList* m_releaseTorrentsList { new ReleaseTorrentsList(this) };
    UserActivityViewModel* m_userActivity { nullptr };
    QScopedPointer<ReleaseTorrentCommonList> m_itemTorrents { new ReleaseTorrentCommonList(this) };
    QSharedPointer<CinemahallListModel> m_cinemahall { new CinemahallListModel() };
    ImageBackgroundViewModel* m_imageBackgroundViewModel { new ImageBackgroundViewModel(this) };
    QSharedPointer<QList<FullReleaseModel*>> m_releases { new QList<FullReleaseModel*>() };
    QScopedPointer<QMap<int, FullReleaseModel*>> m_releasesMap { new QMap<int, FullReleaseModel*>() };
    QList<ReleaseOnlineVideoModel*> m_onlineVideos { QList<ReleaseOnlineVideoModel*>() };
    QMap<QString, ReleaseOnlineVideoModel*> m_onlineVideosMap { QMap<QString, ReleaseOnlineVideoModel*>() };
    QList<ApiTorrentModel*> m_torrentItems { QList<ApiTorrentModel*>() };
    ReleaseLinkedSeries* m_releaseLinkedSeries { nullptr };
    ReleaseCustomGroupsViewModel* m_customGroups { new ReleaseCustomGroupsViewModel(this) };
    int m_countReleases { 0 };
    int m_countSeens { 0 };
    int m_countFavorites { 0 };
    ReleasesListModel* m_items;
    QMap<int, int>* m_scheduleReleases { new QMap<int, int>() };
    QList<int>* m_userFavorites { new QList<int>() };
    QList<int>* m_hiddenReleases { new QList<int>() };
    QHash<QString, std::tuple<bool, int>> m_extendedSeenMarks { QHash<QString, std::tuple<bool, int>>() };
    QSharedPointer<QHash<int, HistoryModel*>> m_historyItems { new QHash<int, HistoryModel*>() };
    QSharedPointer<ChangesModel> m_releaseChanges { new ChangesModel() };
    ApplicationSettings* m_applicationSettings { nullptr };
    LocalStorageService* m_localStorage { nullptr };
    bool m_notificationForFavorites { false };
    bool m_showSidePanel { false };
    bool m_selectMode { false };
    FullReleaseModel* m_openedRelease { nullptr };
    QFutureWatcher<bool>* m_releasesUpdateWatcher { new QFutureWatcher<bool>(this) };
    bool m_synchronizationEnabled { false };
    QString m_newEntities { "" };
    QScopedPointer<QList<std::tuple<int, int>>> m_sectionSorting { new QList<std::tuple<int, int>>() };
    bool m_notCloseReleaseCardAfterWatch { false };
    QString m_openedReleaseAnnounce { "" };
    QList<int> m_sectionCounters { QList<int>() };
    QNetworkAccessManager* m_manager { new QNetworkAccessManager(this) };
    Synchronizev2Service* m_synchronizationServicev2 { nullptr };
    int m_proxyPort { 0 };
    QMap<int, int> m_oldReleasesCountVideos { QMap<int, int>() };
    QMap<int, int> m_oldReleasesCountTorrents { QMap<int, int>() };
    QMap<int, QString> m_oldReleasesTorrentsSeries { QMap<int, QString>() };
    QSet<int> m_oldReleasesIds { QSet<int>() };
    QMap<int, QString> m_collections { QMap<int, QString>() };
    QString m_openedReleseExtensionContent { "" };

public:
    explicit ReleasesViewModel(QObject *parent = nullptr);

    ReleaseTorrentsList* openedCardTorrents() const noexcept { return m_releaseTorrentsList; };

    ReleaseTorrentCommonList* itemTorrents() const noexcept { return m_itemTorrents.get(); }

    ImageBackgroundViewModel* imageBackgroundViewModel() const noexcept { return m_imageBackgroundViewModel; };

    ReleasesListModel* items() const noexcept { return m_items; }

    CinemahallListModel* cinemahall() const noexcept { return m_cinemahall.get(); }

    UserActivityViewModel* userActivity() const noexcept { return m_userActivity; }
    void setUserActivity(const UserActivityViewModel* viewModel) noexcept;

    int countReleases() const noexcept { return m_countReleases; }
    void setCountReleases(const int& countReleases) noexcept;

    bool hasCinemahallNotSeenVideos() const noexcept;

    LocalStorageService* localStorage() const noexcept { return m_localStorage; }
    void setLocalStorage(LocalStorageService* localStorage) noexcept;

    int countSeens() const noexcept { return m_countSeens; }
    void setCountSeens(int countSeens) noexcept;

    bool isChangesExists() const noexcept;

    int countFavorites() const noexcept { return m_countFavorites; }
    void setCountFavorites(int countFavorites) noexcept;

    QStringList sectionNames() const noexcept { return m_sectionNames; }

    int newReleasesCount() const noexcept { return m_releaseChanges->newReleases()->count(); }
    int newOnlineSeriesCount() const noexcept;
    int newTorrentsCount() const noexcept;
    int newTorrentSeriesCount() const noexcept;

    bool notificationForFavorites() const noexcept { return m_notificationForFavorites; }
    void setNotificationForFavorites(bool notificationForFavorites) noexcept;

    bool showSidePanel() const noexcept { return m_showSidePanel; }
    void setShowSidePanel(bool showSidePanel) noexcept;

    bool selectMode() const noexcept { return m_selectMode; }
    void setSelectMode(bool selectMode) noexcept;

    bool synchronizationEnabled() const noexcept { return m_synchronizationEnabled; }
    void setSynchronizationEnabled(bool synchronizationEnabled) noexcept;

    QString newEntities() const noexcept { return m_newEntities; }
    void setNewEntities(QString newEntities) noexcept;

    ApplicationSettings* applicationSettings() const noexcept { return m_applicationSettings; }
    void setApplicationSettings(ApplicationSettings* applicationSettings) noexcept;

    bool notCloseReleaseCardAfterWatch() const noexcept { return m_notCloseReleaseCardAfterWatch; }
    void setNotCloseReleaseCardAfterWatch(const bool notCloseReleaseCardAfterWatch) noexcept;

    ReleaseCustomGroupsViewModel* customGroups() const noexcept { return m_customGroups; }

    Synchronizev2Service* synchronizationServicev2() const noexcept { return m_synchronizationServicev2; }
    void setSynchronizationServicev2(const Synchronizev2Service* synchronizationServicev2) noexcept;

    int proxyPort() const noexcept { return m_proxyPort; }
    void setProxyPort(int proxyPort) noexcept;

    ReleaseLinkedSeries* releaseLinkedSeries() const noexcept { return m_releaseLinkedSeries; }
    void setReleaseLinkedSeries(ReleaseLinkedSeries* releaseLinkedSeries) noexcept;

    QString openedReleseExtensionContent() const noexcept { return m_openedReleseExtensionContent; }
    void setOpenedReleseExtensionContent(const QString& openedReleseExtensionContent) noexcept;

    bool isOpenedCard() const noexcept { return m_openedRelease != nullptr; }
    int openedReleaseId() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->id() : 0; }
    QString openedReleasePoster() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->poster() : ""; }
    QString openedReleaseTitle() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->title() : ""; }
    QString openedReleaseOriginalName() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->originalName() : ""; }
    QString openedReleaseStatus() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->status() : ""; }
    QString openedReleaseStatusDisplay() const noexcept;
    QString openedReleaseYear() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->year() : ""; }
    QString openedReleaseYearDisplay() const noexcept;
    bool openedReleaseInSchedule() const noexcept { return m_openedRelease != nullptr ? m_scheduleReleases->contains(m_openedRelease->id()) : false; }
    QString openedReleaseInScheduleDisplay() const noexcept;
    QString openedReleaseSeason() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->season() : ""; }
    QString openedReleaseSeasonDisplay() const noexcept;
    QString openedReleaseType() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->type() : ""; }
    QString openedReleaseTypeDisplay() const noexcept;
    QString openedReleaseGenres() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->genres() : ""; }
    QString openedReleaseGenresDisplay() const noexcept;
    QString openedReleaseVoices() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->voicers() : ""; }
    QString openedReleaseVoicesDisplay() const noexcept;
    bool openedReleaseIsAllSeen() const noexcept;
    QString openedReleaseDescription() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->description() : ""; }
    QString openedReleaseCode() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->code() : ""; }
    int openedReleaseCountTorrents() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->countTorrents() : 0; }
    int openedReleaseCountVideos() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->countOnlineVideos() : 0; }
    int openedReleaseSeenCountVideos() const noexcept { return m_openedRelease != nullptr ? m_items->getReleaseSeenMarkCount(m_openedRelease->id()) : 0; }
    bool openedReleaseInHided() const noexcept { return m_openedRelease != nullptr ? m_hiddenReleases->contains(m_openedRelease->id()) : false; }
    bool openedReleaseInFavorites() const noexcept { return m_openedRelease != nullptr ? m_userFavorites->contains(m_openedRelease->id()) : false; }
    QString openedReleaseAnnounce() const noexcept { return m_openedRelease != nullptr ? m_openedRelease->announce() : ""; }
    bool openedReleaseIsRutube() const noexcept;
    bool openedReleaseInCollections() const noexcept;
    QStringList getMostPopularGenres() const noexcept;
    QStringList getMostPopularVoices() const noexcept;
    QList<int> countSections() const noexcept { return m_sectionCounters; };
    void fillNewInFavorites(QList<FullReleaseModel*>* list) const noexcept;
    void fillNewFromStart(QList<FullReleaseModel*>* list) const noexcept;
    void fillNewFromLastTwoDays(QList<FullReleaseModel*>* list) const noexcept;
    void fillAbandonedSeens(QList<FullReleaseModel*>* list) const noexcept;
    void fillRecommendsByGenres(QList<FullReleaseModel*>* list) noexcept;
    void fillWillWatch(QList<FullReleaseModel*>* list) noexcept;
    void fillNextInReleaseSeries(QList<FullReleaseModel*>* list) noexcept;
    void fillCurrentSeason(QList<FullReleaseModel*>* list) noexcept;
    void fillRecommendedByVoices(QList<FullReleaseModel*>* list) noexcept;
    void getFavoritesReleases(QList<FullReleaseModel*>* list) const noexcept;
    void getFavoritesIds(QList<int>* list) const noexcept;
    QString getReleaseCodeFromUrl(const QString& url) const noexcept;
    void fillFullSearch(QList<FullReleaseModel*>& list, const QString& filter) noexcept;
    bool fullSearchCheck(const QString& word, const FullReleaseModel* release) noexcept;
    void iterateOnReleases(std::function<void (FullReleaseModel *)> func) noexcept;
    QList<ReleaseOnlineVideoModel*> getReleaseVideos(int releaseId) noexcept;
    QList<ApiTorrentModel*> getReleaseTorrents(int releaseId) noexcept;
    void getSeenIds(QList<int>* list);
    void synchronizeCollections(QMap<int, QString>&& items) noexcept;
    void setReleasesToCollection(QList<int> ids, const QString& collection) noexcept;
    void removeReleasesFromCollections(QList<int> ids) noexcept;

    Q_INVOKABLE void copyToClipboard(const QString& text) const noexcept;
    Q_INVOKABLE void copyImageToClipboard(const QString& imagePath) const;
    Q_INVOKABLE QString getVkontakteCommentPage(const QString& code) const noexcept;
    Q_INVOKABLE void resetAllChanges() noexcept;
    Q_INVOKABLE void selectRelease(int id) noexcept;
    Q_INVOKABLE void toggleRelease(int id) noexcept;
    Q_INVOKABLE void clearSelectedReleases() noexcept;
    Q_INVOKABLE void addReleaseToFavorites(int id) noexcept;
    Q_INVOKABLE void removeReleaseFromFavorites(int id) noexcept;
    Q_INVOKABLE void addSelectedReleaseToFavorites() noexcept;
    Q_INVOKABLE void removeSelectedReleaseFromFavorites() noexcept;
    Q_INVOKABLE void closeReleaseCard() noexcept;
    Q_INVOKABLE void openDescriptionLink(const QString& link) noexcept;
    Q_INVOKABLE void showReleaseCard(int id, bool needHandle = true) noexcept;
    Q_INVOKABLE void showRandomRelease() noexcept;
    Q_INVOKABLE void showRandomReleaseFromFavorites() noexcept;
    Q_INVOKABLE void showRandomReleaseInFiltered() noexcept;
    Q_INVOKABLE void showRandomReleaseInSeen() noexcept;
    Q_INVOKABLE void hideReleaseCard() noexcept;
    Q_INVOKABLE void setSeenMarkAllSeries(int id, int countSeries, bool marked);
    Q_INVOKABLE void setSeenMarkAllSeriesSelectedReleases(bool marked);
    Q_INVOKABLE void setSeenMarkForSingleRelease(int id, bool marked);
    Q_INVOKABLE void synchronizeSeenMarkForRelease(const QList<QString>& ids, bool marked);
    Q_INVOKABLE void synchronizeSeenMarkForRelease(int id, int countSeries, bool marked);
    Q_INVOKABLE void synchronizeSeenMarkForSingleSeria(const QString& id, bool marked);
    Q_INVOKABLE void refreshOpenedReleaseCard();
    Q_INVOKABLE void removeAllSeenMark();
    Q_INVOKABLE void reloadReleases();
    Q_INVOKABLE void setToReleaseHistory(int id, int type) noexcept;
    Q_INVOKABLE QString getReleasePoster(int id) const noexcept;
    Q_INVOKABLE QString getReleaseTitle(int id) const noexcept;
    Q_INVOKABLE void addToHidedReleases(const QList<int>& ids) noexcept;
    Q_INVOKABLE void addToHidedSelectedReleases() noexcept;
    Q_INVOKABLE void removeFromHidedReleases(const QList<int>& ids) noexcept;
    Q_INVOKABLE void removeFromHidedSelectedReleases() noexcept;
    Q_INVOKABLE void removeAllHidedReleases() noexcept;
    Q_INVOKABLE void addToCinemahallSelectedReleases();
    Q_INVOKABLE void setupSortingForSection() const noexcept;
    Q_INVOKABLE bool getSeriaSeenMark(int id, const QString& seriaId) const noexcept;
    Q_INVOKABLE void toggleSeenMark(int id, const QString& seriaId) noexcept;
    Q_INVOKABLE void setSeenMark(int id, const QString& seriaId, bool marked);
    QHash<QString, std::tuple<bool, int>>& getSeenMarks();
    QMap<QString, ReleaseOnlineVideoModel*>& getVideosMap();
    void updateAllReleases(const QList<QString> &releases, bool insideData);
    uint32_t getCountFromChanges(const QList<int> *releases, bool filterByFavorites);
    Q_INVOKABLE void openInExternalPlayer(const QString& url);
    Q_INVOKABLE void prepareTorrentsForListItem(const int id);
    Q_INVOKABLE QString packAsM3UAndOpen(int id, QString quality);
    Q_INVOKABLE QString packAsMPCPLAndOpen(int id, QString quality);
    Q_INVOKABLE void savePreviousReleases(int previousLastTimeStamp);
    Q_INVOKABLE void refreshApiHost();
    Q_INVOKABLE void copyOpenedReleaseMagnetTorrent(int identifier) noexcept;
    Q_INVOKABLE void openOpenedReleaseMagnetTorrent(int identifier) noexcept;
    Q_INVOKABLE void synchronizeSeens(const QVariantMap items) noexcept;
    Q_INVOKABLE void synchronizeLocalSeensToExternal() noexcept;
    Q_INVOKABLE void recheckOpenedReleaseInCollections() noexcept;
    FullReleaseModel* getReleaseById(int id) const noexcept;
    void resetReleaseChanges(int releaseId) noexcept;
    quint32 m_seedValue { 0 };

private:
    void setSeenMarkInternal(const QString& id, bool marked);
    void loadReleases();
    void loadNextReleasesWithoutReactive();
    void reloadApiHostInItems();

    void loadSchedules();
    void saveSchedule(QString json);

    void saveFavoritesFromJson(QString data);
    void saveFavoritesFromArray(const QList<int> ids);
    void saveFavorites();
    void loadFavorites();
    void clearFavorites();

    void loadHidedReleases();
    void saveHidedReleases();

    void loadSeenMarks();
    void recalculateSeenCounts();
    void saveSeenMarks();

    void loadCollections();
    void saveCollections();

    void loadHistory();
    void saveHistory();

    void loadChanges();
    void saveChanges();

    void loadGroups();
    void saveGroups();
    void refreshGroups();
    int getCountOnlyFavorites(QList<int>* changes) const noexcept;
    QString getMultipleLinks(QString text) const noexcept;
    FullReleaseModel* getReleaseByCode(QString code) const noexcept;
    int randomBetween(int low, int high) const noexcept;
    void mapToFullReleaseModel(QJsonObject &&jsonObject, const bool isFirstStart, QSharedPointer<QSet<int>> hittedIds);
    QHash<int, int> getAllSeenMarkCount() noexcept;

private slots:
    void synchronizedSchedule(const QString& data);
    void userFavoritesReceived(const QString& data);
    void userFavoritesReceivedV2(const QList<int>& data);
    void cinemahallItemsChanged();
    void needDeleteFavorites(const QList<int>& ids);

signals:
    void openedCardTorrentsChanged();
    void imageBackgroundViewModelChanged();
    void countReleasesChanged();
    void itemsChanged();
    void countSeensChanged();
    void isChangesExistsChanged();
    void countFavoritesChanged();
    void sectionNamesChanged();
    void newReleasesCountChanged();
    void newOnlineSeriesCountChanged();
    void newTorrentsCountChanged();
    void newTorrentSeriesCountChanged();
    void notificationForFavoritesChanged();
    void showSidePanelChanged();
    void selectModeChanged();
    void applicationSettingsChanged();
    void isOpenedCardChanged();
    void openedReleaseIdChanged();
    void openedReleasePosterChanged();
    void openedReleaseTitleChanged();
    void openedReleaseOriginalNameChanged();
    void openedReleaseStatusChanged();
    void openedReleaseStatusDisplayChanged();
    void openedReleaseYearChanged();
    void openedReleaseYearDisplayChanged();
    void openedReleaseInScheduleChanged();
    void openedReleaseInScheduleDisplayChanged();
    void openedReleaseSeasonChanged();
    void openedReleaseSeasonDisplayChanged();
    void openedReleaseTypeChanged();
    void openedReleaseTypeDisplayChanged();
    void openedReleaseVoicesChanged();
    void openedReleaseVoicesDisplayChanged();
    void openedReleaseIsAllSeenChanged();
    void openedReleaseGenresChanged();
    void openedReleaseGenresDisplayChanged();
    void openedReleaseDescriptionChanged();
    void releaseCardOpened();
    void openedReleaseCodeChanged();
    void openedReleaseCountTorrentsChanged();
    void openedReleaseCountVideosChanged();
    void afterSynchronizedReleases();
    void synchronizationEnabledChanged();
    void newEntitiesChanged();
    void errorWhileReleaseSynchronization();
    void openedReleaseInHidedChanged();
    void localStorageChanged();
    void openedReleaseInFavoritesChanged();
    void notCloseReleaseCardAfterWatchChanged();
    void cinemahallChanged();
    void itemTorrentsChanged();
    void userActivityChanged();
    void hasCinemahallNotSeenVideosChanged();
    void openedReleaseAnnounceChanged();
    void countSectionsChanged();
    void openedReleaseSeenCountVideosChanged();
    void openedReleaseIsRutubeChanged();
    void customGroupsChanged();
    void synchronizationServicev2Changed();
    void proxyPortChanged();
    void releaseLinkedSeriesChanged();
    void releasesFullyLoaded();
    void addedSeenMarks(QList<QString> uniqueIds);
    void removeSeenMarks(QList<QString> uniqueIds);
    void openedReleaseInCollectionsChanged();
    void openedReleseExtensionContentChanged();

};

#endif // RELEASESVIEWMODEL_H
