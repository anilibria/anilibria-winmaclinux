#ifndef RELEASESVIEWMODEL_H
#define RELEASESVIEWMODEL_H

#include <QObject>
#include "../ListModels/releasetorrentslist.h"
#include "imagebackgroundviewmodel.h"
#include "../Models/fullreleasemodel.h"
#include "../ListModels/releaseslistmodel.h"
#include "../Models/historymodel.h"
#include "../Models/changesmodel.h"
#include "../Services/synchronizationservice.h"
#include "../Services/applicationsettings.h"

class ReleasesViewModel : public QObject
{
    Q_OBJECT    
    Q_PROPERTY(ReleaseTorrentsList* openedCardTorrents READ openedCardTorrents NOTIFY openedCardTorrentsChanged)
    Q_PROPERTY(ImageBackgroundViewModel* imageBackgroundViewModel READ imageBackgroundViewModel NOTIFY imageBackgroundViewModelChanged)
    Q_PROPERTY(int countReleases READ countReleases WRITE setCountReleases NOTIFY countReleasesChanged)
    Q_PROPERTY(ReleasesListModel* items READ items NOTIFY itemsChanged)
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
    Q_PROPERTY(SynchronizationService* synchronizationService READ synchronizationService WRITE setSynchronizationService NOTIFY synchronizationServiceChanged)
    Q_PROPERTY(ApplicationSettings* applicationSettings READ applicationSettings WRITE setApplicationSettings NOTIFY applicationSettingsChanged)

private:
    const QString releasesCacheFileName { "releases.cache" };
    const QString scheduleCacheFileName { "schedule.cache" };
    const QString favoriteCacheFileName { "favorites.cache" };
    const QString hidedReleasesCacheFileName { "hidedreleases.cache" };
    const QString seenMarkCacheFileName { "seenmark.cache" };
    const QString historyCacheFileName { "history.cache" };
    const QString notificationCacheFileName { "notification.cache" };
    QStringList m_sectionNames;
    ReleaseTorrentsList* m_releaseTorrentsList { new ReleaseTorrentsList(this) };
    ImageBackgroundViewModel* m_imageBackgroundViewModel { new ImageBackgroundViewModel(this) };
    QList<FullReleaseModel*>* m_releases { new QList<FullReleaseModel*>() };
    QMap<int, FullReleaseModel*>* m_releasesMap { new QMap<int, FullReleaseModel*>() };
    int m_countReleases { 0 };
    int m_countSeens { 0 };
    int m_countFavorites { 0 };
    ReleasesListModel* m_items;
    QMap<int, int>* m_scheduleReleases { new QMap<int, int>() };
    QVector<int>* m_userFavorites { new QVector<int>() };
    QVector<int>* m_hiddenReleases { new QVector<int>() };
    QHash<QString, bool>* m_seenMarks { new QHash<QString, bool>() };
    QSharedPointer<QHash<int, HistoryModel*>> m_historyItems { new QHash<int, HistoryModel*>() };
    ChangesModel* m_releaseChanges { new ChangesModel() };
    SynchronizationService* m_synchronizationService { nullptr };
    ApplicationSettings* m_applicationSettings { nullptr };
    bool m_notificationForFavorites { false };
    bool m_showSidePanel { false };
    bool m_selectMode { false };

public:
    explicit ReleasesViewModel(QObject *parent = nullptr);

    ReleaseTorrentsList* openedCardTorrents() const noexcept { return m_releaseTorrentsList; };

    ImageBackgroundViewModel* imageBackgroundViewModel() const noexcept { return m_imageBackgroundViewModel; };

    ReleasesListModel* items() const noexcept { return m_items; }

    int countReleases() const noexcept { return m_countReleases; }
    void setCountReleases(int countReleases) noexcept;

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

    SynchronizationService* synchronizationService() const noexcept { return m_synchronizationService; }
    void setSynchronizationService(SynchronizationService* synchronizationService) noexcept;

    ApplicationSettings* applicationSettings() const noexcept { return m_applicationSettings; }
    void setApplicationSettings(ApplicationSettings* applicationSettings) noexcept;

    Q_INVOKABLE QString getScheduleDay(const QString& dayNumber) const noexcept;
    Q_INVOKABLE void copyToClipboard(const QString& text) const noexcept;
    Q_INVOKABLE void copyImageToClipboard(const QString& imagePath) const;
    Q_INVOKABLE QString getVkontakteCommentPage(const QString& code) const noexcept;
    Q_INVOKABLE void resetAllChanges() noexcept;
    Q_INVOKABLE void selectRelease(int id) noexcept;
    Q_INVOKABLE void clearSelectedReleases() noexcept;
    Q_INVOKABLE void addReleaseToFavorites(int id) noexcept;
    Q_INVOKABLE void removeReleaseFromFavorites(int id) noexcept;

private:
    void loadReleases();

    void loadSchedules();
    void saveSchedule(QString json);

    void saveFavoritesFromJson(QString data);
    void saveFavorites();
    void loadFavorites();
    void clearFavorites();

    void loadHidedReleases();
    void saveHidedReleases();

    void loadSeenMarks();
    void recalculateSeenCounts();

    void loadHistory();
    void saveHistory();

    void loadChanges();
    void saveChanges();
    int getCountOnlyFavorites(QList<int>* changes) const noexcept;

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
    void synchronizationServiceChanged();
    void applicationSettingsChanged();

};

#endif // RELEASESVIEWMODEL_H
