#ifndef SYNCHRONIZEV2SERVICE_H
#define SYNCHRONIZEV2SERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Synchronizev2Service : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString apiv2host READ apiv2host WRITE setApiv2host NOTIFY apiv2hostChanged FINAL)
    Q_PROPERTY(QString cachehost READ cachehost WRITE setCachehost NOTIFY cachehostChanged FINAL)
    Q_PROPERTY(bool cacheHostIsFolder READ cacheHostIsFolder WRITE setCacheHostIsFolder NOTIFY cacheHostIsFolderChanged FINAL)
    Q_PROPERTY(QString token READ token WRITE setToken NOTIFY tokenChanged FINAL)
    Q_PROPERTY(bool isAuhorized READ isAuhorized NOTIFY isAuhorizedChanged FINAL)
    Q_PROPERTY(QString userAvatar READ userAvatar NOTIFY userAvatarChanged FINAL)
    Q_PROPERTY(QString nickName READ nickName NOTIFY nickNameChanged FINAL)
    Q_PROPERTY(int torrentDownloadMode READ torrentDownloadMode WRITE setTorrentDownloadMode NOTIFY torrentDownloadModeChanged FINAL)
    Q_PROPERTY(bool synchronizeCacheActived READ synchronizeCacheActived NOTIFY synchronizeCacheActivedChanged FINAL)
    Q_PROPERTY(int torrentStreamPort READ torrentStreamPort WRITE setTorrentStreamPort NOTIFY torrentStreamPortChanged FINAL)
    Q_PROPERTY(QString mainGithubCacheServer READ mainGithubCacheServer NOTIFY mainGithubCacheServerChanged FINAL)
    Q_PROPERTY(QString mainNextAPIServer READ mainNextAPIServer NOTIFY mainNextAPIServerChanged FINAL)
    Q_PROPERTY(QString appMirrorNextAPIServer READ appMirrorNextAPIServer NOTIFY appMirrorNextAPIServerChanged FINAL)
    Q_PROPERTY(QString appMirrorWtfNextAPIServer READ appMirrorWtfNextAPIServer NOTIFY appMirrorWtfNextAPIServerChanged FINAL)
    Q_PROPERTY(QString cacheFolder READ cacheFolder WRITE setCacheFolder NOTIFY cacheFolderChanged FINAL)
    Q_PROPERTY(bool isSocialAuthentification READ isSocialAuthentification NOTIFY isSocialAuthentificationChanged FINAL)
    Q_PROPERTY(bool useTorrentStreamAsLibrary READ useTorrentStreamAsLibrary WRITE setUseTorrentStreamAsLibrary NOTIFY useTorrentStreamAsLibraryChanged FINAL)
    Q_PROPERTY(QString pathToTSLibrary READ pathToTSLibrary NOTIFY pathToTSLibraryChanged FINAL)
    Q_PROPERTY(QString pathToTSContent READ pathToTSContent NOTIFY pathToTSContentChanged FINAL)

private:
    QString m_apiv2host { "" };
    QString m_cachehost { "" };
    QString m_libraryCacheHost { "" };
    QString m_newVersionFileHost { "" };
    bool m_cacheHostIsFolder { false };
    QNetworkAccessManager* m_networkManager { new QNetworkAccessManager(this) };
    QMap<QString, QString> m_pendingRequests { QMap<QString, QString>() };
    QMap<QString, int> m_pendingTorrentReleases { QMap<QString, int>() };
    const QString m_pairLogin { "pairsignin" };
    const QString m_logoutRequest { "logout" };
    const QString m_userProfileRequest { "userprofile" };
    const QString m_addFavoritesRequest { "addfavorites" };
    const QString m_removeFavoritesRequest { "removefavorites" };
    const QString m_getFavoritesRequest { "getfavorites" };
    const QString m_downloadTorrentRequest { "downloadtorrent" };
    const QString m_cacheMetadataRequest { "cachemetadata" };
    const QString m_cacheReleaseRequest { "cacherelease" };
    const QString m_cacheTorrentRequest { "cachetorrent" };
    const QString m_cacheEpisodesRequest { "cacheepisode" };
    const QString m_cacheTypesRequest { "cachetypes" };
    const QString m_cacheScheduleRequest { "cacheschedule" };
    const QString m_cacheReleaseSerieRequest { "releaseseries" };
    const QString m_socialRequest { "socialrequest" };
    const QString m_socialRequestResponse { "socialrequestresponse" };
    const QString m_checkNetworkAvailability { "checknetworkavailability" };
    const QString m_userSeenRequest { "userseen" };
    const QString m_addUserSeenRequest { "adduserseen" };
    const QString m_collectionsRequest { "collections" };
    const QString m_addToCollectionRequest { "addtocollection" };
    const QString m_removeFromCollectionRequest { "removefromcollection" };
    const QString m_checkVersionTorrentStream { "checkVersionTorrentStream" };
    const QString m_downloadTorrentStreamLibraryRequest { "downloadTorrentStreamLibrary" };
    bool m_isAuhorized { false };
    QString m_nickName { "" };
    QString m_avatar { "" };
    int m_userTorrentsUploaded { 0 };
    int m_userTorrentsDownloaded { 0 };
    QString m_userTorrentsPasskey { "" };
    QString m_token { "" };
    int m_torrentDownloadMode { 0 };
    bool m_synchronizeCacheActived { false };
    bool m_synchronizeCacheForce { false };
    int m_countEpisodes { 0 };
    int m_countReleases { 0 };
    int m_currentSynchronizationItem { 0 };
    int m_lastReleaseTimeStamp { 0 };
    int m_previousLastTimeStamp { 0 };
    int m_torrentStreamPort { 0 };
    QString m_cacheFolder { "" };
    QString m_socialState { "" };
    int m_socialCheckTimer { 0 };
    int m_socialCheckTimerIterator { 0 };
    bool m_useTorrentStreamAsLibrary { false };
    QMap<QString, std::tuple<bool, int>> m_synchronizedSeens { QMap<QString, std::tuple<bool, int>>() };
    QMap<int, QString> m_synchronizedCollection { QMap<int, QString>() };
    QString m_savedTorrentStreamVersion { "" };
    QString m_savedTorrentStreamNewVersion { "" };
    QString m_pathToTSLibrary { "" };
    QString m_pathToTSContent { "" };

public:
    explicit Synchronizev2Service(QObject *parent = nullptr);

    QString apiv2host() const noexcept { return m_apiv2host; }
    void setApiv2host(const QString& apiv2host) noexcept;

    QString cachehost() const noexcept { return m_cachehost; }
    void setCachehost(const QString& cachehost) noexcept;

    bool cacheHostIsFolder() const noexcept { return m_cacheHostIsFolder; }
    void setCacheHostIsFolder(bool cacheHostIsFolder) noexcept;

    QString token() const noexcept { return m_token; }
    void setToken(QString token) noexcept;

    bool isAuhorized() const noexcept { return m_isAuhorized; }

    QString userAvatar() const noexcept { return m_avatar.isEmpty() ? "" : m_apiv2host + m_avatar; }
    QString nickName() const noexcept { return m_nickName; }

    int torrentDownloadMode() const noexcept { return m_torrentDownloadMode; }
    void setTorrentDownloadMode(int torrentDownloadMode) noexcept;

    int torrentStreamPort() const noexcept { return m_torrentStreamPort; }
    void setTorrentStreamPort(int torrentStreamPort) noexcept;

    QString cacheFolder() const noexcept { return m_cacheFolder; }
    void setCacheFolder(const QString& cacheFolder) noexcept;

    bool synchronizeCacheActived() const noexcept { return m_synchronizeCacheActived; }

    QString mainGithubCacheServer() const noexcept { return "https://raw.githubusercontent.com/trueromanus/LocalCacheChecker/main/cache"; }

    QString mainNextAPIServer() const noexcept { return "https://aniliberty.top"; }

    QString appMirrorNextAPIServer() const noexcept { return "https://api.anilibria.app"; }

    QString appMirrorWtfNextAPIServer() const noexcept { return "https://aniliberty.wtf"; }

    bool isSocialAuthentification() const noexcept { return m_socialCheckTimer > 0; }

    bool useTorrentStreamAsLibrary() const noexcept { return m_useTorrentStreamAsLibrary; }
    void setUseTorrentStreamAsLibrary(bool useTorrentStreamAsLibrary) noexcept;

    QString pathToTSLibrary() const noexcept { return m_pathToTSLibrary; }

    QString pathToTSContent() const noexcept { return m_pathToTSContent; }

    QMap<int, QString>&& getLocalCollections();

    Q_INVOKABLE void authorize(QString login, QString password);
    Q_INVOKABLE void authorizeSocial(const QString& provider);
    Q_INVOKABLE void logout();
    Q_INVOKABLE void getUserData();
    Q_INVOKABLE void getUserFavorites();
    Q_INVOKABLE void addUserFavorites(const QList<int> ids);
    Q_INVOKABLE void removeUserFavorites(const QList<int> ids);
    Q_INVOKABLE void downloadTorrent(QString torrentPath, int releaseId, const QString magnet);
    Q_INVOKABLE void synchronizeFullCache();
    Q_INVOKABLE void synchronizeFullCacheForce();
    Q_INVOKABLE QString checkFolderAvailability(const QString& folder);
    Q_INVOKABLE void checkNetworkAvailability(const QString& address);
    Q_INVOKABLE void getUserSeens();
    Q_INVOKABLE QVariantMap getUserSynchronizedSeens();
    Q_INVOKABLE void clearUserSynchronizedSeens();
    Q_INVOKABLE void addSeenMarks(QList<QString> videoIds, bool seenMark);
    Q_INVOKABLE void getCollections();
    Q_INVOKABLE void addReleasesToCollection(QList<int> releaseIds, const QString& collectionId);
    Q_INVOKABLE void removeReleasesFromCollection(QList<int> releaseIds);
    Q_INVOKABLE void checkVersionTorrentStreamLibrary();
    Q_INVOKABLE void downloadTorrentStreamLibrary(const QString& path);

    void timerEvent(QTimerEvent *event) override;

private:
    void downloadReleaseFile() noexcept;
    void downloadEpisodesFile() noexcept;
    void downloadTorrentsFile() noexcept;
    void downloadTypesFile() noexcept;
    void downloadSchedulesFile() noexcept;
    void downloadReleaseSeriesFile() noexcept;
    void loginHandler(QNetworkReply* reply) noexcept;
    void logoutHandler(QNetworkReply* reply) noexcept;
    void userProfileHandler(QNetworkReply* reply) noexcept;
    void adjustRequestToken(QNetworkRequest& request) noexcept;
    QString adjustIdentifier(QNetworkReply* reply, const QString& type) noexcept;
    void favoritesSynchronizedHandler(QNetworkReply* reply) noexcept;
    void downloadTorrentHandler(QNetworkReply* reply, const QString& identifier) noexcept;
    void metadataCacheHandler(QNetworkReply* reply) noexcept;
    void releaseCacheHandler(QNetworkReply* reply) noexcept;
    void episodeCacheHandler(QNetworkReply* reply) noexcept;
    void torrentCacheHandler(QNetworkReply* reply) noexcept;
    void scheduleCacheHandler(QNetworkReply* reply) noexcept;
    void releaseSeriesCacheHandler(QNetworkReply* reply) noexcept;
    void typesCacheHandler(QNetworkReply* reply) noexcept;
    void cacheFolderHandler(const QString& fullPath) noexcept;
    bool copyFile(const QString& fullPath, const QString& cacheFileName) noexcept;
    void socialRequestHandler(QNetworkReply* reply) noexcept;
    void socialRequestTokenHandler(QNetworkReply* reply) noexcept;
    void checkNetworkAvailabilityHandler(QNetworkReply* reply) noexcept;
    void userSeenSynchronizationHandler(QNetworkReply* reply) noexcept;
    void userCollectionSynchronizeHandler(QNetworkReply* reply) noexcept;
    void torrentStreamNewVersionHandler(QNetworkReply* reply) noexcept;
    void downloadTorrentStreamLibraryHandler(QNetworkReply* reply) noexcept;
    void loadLibraryData() noexcept;
    void saveLibraryData() noexcept;
    void installTorrentStreamNewVersion() noexcept;
    QString getTorrentStreamFileName() noexcept;

private slots:
    void requestFinished(QNetworkReply* reply);

signals:
    void apiv2hostChanged();
    void cachehostChanged();
    void cacheHostIsFolderChanged();
    void userSignouted();
    void userCompleteAuthentificated();
    void userFailedAuthentificated(QString errorMessage);
    void synchronizeFavoritesFailed(QString errorMessage);
    void synchronizeSeensFailed(QString errorMessage);
    void synchronizeCollectionFailed(QString errorMessage);
    void torrentStreamNewVersionFailed(QString errorMessage);
    void synchronizeSeensCompleted();
    void synchronizeCollectionCompleted();
    void getUserFailed(QString errorMessage);
    void isAuhorizedChanged();
    void tokenChanged();
    void userAvatarChanged();
    void nickNameChanged();
    void userFavoritesReceivedV2(const QList<int>& data);
    void torrentDownloaded(const QString& torrentPath);
    void saveDownloadedTorrent(const QString& torrentPath);
    void torrentDownloadModeChanged();
    void downloadInTorrentStream(int releaseId);
    void synchronizeCacheActivedChanged();
    void synchronizeCacheFailed(QString errorMessage);
    void synchronizationCompletedNoChanges();
    void synchronizationCompleted(int previousLastTimeStamp);
    void torrentStreamPortChanged();
    void mainGithubCacheServerChanged();
    void mainNextAPIServerChanged();
    void cacheFolderChanged();
    void isSocialAuthentificationChanged();
    void checkNetworkAvailibilityFailedChanged(const QString& message);
    void checkNetworkAvailibilityCompletedChanged();
    void appMirrorNextAPIServerChanged();
    void appMirrorWtfNextAPIServerChanged();
    void useTorrentStreamAsLibraryChanged();
    void tsDownloadTorrent(int releaseId, QString downloadPath);
    void pathToTSLibraryChanged();
    void pathToTSContentChanged();

};

#endif // SYNCHRONIZEV2SERVICE_H
