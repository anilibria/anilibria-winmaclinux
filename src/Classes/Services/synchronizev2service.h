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

private:
    QString m_apiv2host { "" };
    QString m_cachehost { "" };
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
    bool m_isAuhorized { false };
    QString m_nickName { "" };
    QString m_avatar { "" };
    int m_userTorrentsUploaded { 0 };
    int m_userTorrentsDownloaded { 0 };
    QString m_userTorrentsPasskey { "" };
    QString m_token { "" };
    int m_torrentDownloadMode { 0 };
    bool m_synchronizeCacheActived { false };
    int m_countEpisodes { 0 };
    int m_countReleases { 0 };
    int m_currentSynchronizationItem { 0 };
    int m_lastReleaseTimeStamp { 0 };

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

    bool synchronizeCacheActived() const noexcept { return m_synchronizeCacheActived; }

    Q_INVOKABLE void authorize(QString login, QString password);
    Q_INVOKABLE void logout();
    Q_INVOKABLE void getUserData();
    Q_INVOKABLE void getUserFavorites();
    Q_INVOKABLE void addUserFavorites(const QList<int> ids);
    Q_INVOKABLE void removeUserFavorites(const QList<int> ids);
    Q_INVOKABLE void downloadTorrent(QString torrentPath, int releaseId);
    Q_INVOKABLE void synchronizeFullCache();

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
    void getUserFailed(QString errorMessage);
    void isAuhorizedChanged();
    void tokenChanged();
    void userAvatarChanged();
    void nickNameChanged();
    void userFavoritesReceivedV2(const QList<int>& data);
    void torrentDownloaded(const QString& torrentPath);
    void saveDownloadedTorrent(const QString& torrentPath);
    void torrentDownloadModeChanged();
    void downloadInTorrentStream(int releaseId, const QString& torrentPath);
    void synchronizeCacheActivedChanged();
    void synchronizeCacheFailed(QString errorMessage);

};

#endif // SYNCHRONIZEV2SERVICE_H
