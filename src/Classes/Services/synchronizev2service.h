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

private:
    QString m_apiv2host { "" };
    QString m_cachehost { "" };
    bool m_cacheHostIsFolder { false };
    QNetworkAccessManager* m_networkManager { new QNetworkAccessManager(this) };
    QMap<QString, QString> m_pendingRequests { QMap<QString, QString>() };
    const QString m_pairLogin { "pairsignin" };
    const QString m_logoutRequest { "logout" };
    const QString m_userProfileRequest { "userprofile" };
    const QString m_addFavoritesRequest { "addfavorites" };
    const QString m_removeFavoritesRequest { "removefavorites" };
    const QString m_getFavoritesRequest { "getfavorites" };
    bool m_isAuhorized { false };
    QString m_nickName { "" };
    QString m_avatar { "" };
    int m_userTorrentsUploaded { 0 };
    int m_userTorrentsDownloaded { 0 };
    QString m_userTorrentsPasskey { "" };
    QString m_token { "" };

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

    Q_INVOKABLE void authorize(QString email, QString password);
    Q_INVOKABLE void logout();
    Q_INVOKABLE void getUserData();
    Q_INVOKABLE void synchronizeUserFavorites();
    Q_INVOKABLE void addUserFavorites(QStringList ids);
    Q_INVOKABLE void removeUserFavorites(QStringList ids);
    Q_INVOKABLE void downloadTorrent(QString torrentPath);
    Q_INVOKABLE void synchronizeFullCache();

private:
    void loginHandler(QNetworkReply* reply) noexcept;
    void logoutHandler(QNetworkReply* reply) noexcept;
    void userProfileHandler(QNetworkReply* reply) noexcept;
    void adjustRequestToken(QNetworkRequest& request) noexcept;
    void adjustIdentifier(QNetworkReply* reply, const QString& type) noexcept;

private slots:
    void requestFinished(QNetworkReply* reply);

signals:
    void apiv2hostChanged();
    void cachehostChanged();
    void cacheHostIsFolderChanged();
    void userSignouted();
    void userCompleteAuthentificated();
    void userFailedAuthentificated(QString errorMessage);
    void isAuhorizedChanged();
    void tokenChanged();

};

#endif // SYNCHRONIZEV2SERVICE_H
