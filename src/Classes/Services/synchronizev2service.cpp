#include <QUuid>
#include <QJsonDocument>
#include <QJsonObject>
#include "synchronizev2service.h"

Synchronizev2Service::Synchronizev2Service(QObject *parent)
    : QObject{parent}
{
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &Synchronizev2Service::requestFinished);
}

void Synchronizev2Service::setApiv2host(const QString &apiv2host) noexcept
{
    if (m_apiv2host == apiv2host) return;

    m_apiv2host = apiv2host;
    emit apiv2hostChanged();
}

void Synchronizev2Service::setCachehost(const QString &cachehost) noexcept
{
    if (m_cachehost == cachehost) return;

    m_cachehost = cachehost;
    emit cachehostChanged();
}

void Synchronizev2Service::setCacheHostIsFolder(bool cacheHostIsFolder) noexcept
{
    if (m_cacheHostIsFolder == cacheHostIsFolder) return;

    m_cacheHostIsFolder = cacheHostIsFolder;
    emit cacheHostIsFolderChanged();
}

void Synchronizev2Service::setToken(QString token) noexcept
{
    if (m_token == token) return;

    m_token = token;
    emit tokenChanged();
}

void Synchronizev2Service::authorize(QString email, QString password)
{
    QNetworkRequest request(QUrl(m_apiv2host + "api/v1/accounts/users/auth/login"));

    auto body = QString("{\"login\": \"%1\",\"password\": \"%2\"}");
    body.arg(email);
    body.arg(password);

    auto reply = m_networkManager->post(request, body.toUtf8());
    adjustIdentifier(reply, m_pairLogin);
}

void Synchronizev2Service::logout()
{
    if (!m_isAuhorized) return;

    QNetworkRequest request(QUrl(m_apiv2host + "api/v1/accounts/users/auth/logout"));
    adjustRequestToken(request);

    auto reply = m_networkManager->post(request, "");
    adjustIdentifier(reply, m_logoutRequest);
}

void Synchronizev2Service::getUserData()
{
    if (!m_isAuhorized) return;

    QNetworkRequest request(QUrl(m_apiv2host + "api/v1/accounts/users/me/profile"));
    adjustRequestToken(request);

    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_userProfileRequest);
}

void Synchronizev2Service::synchronizeUserFavorites()
{
    if (!m_isAuhorized) return;

    QNetworkRequest request(QUrl(m_apiv2host + "api/v1/accounts/users/me/favorites/ids"));
    adjustRequestToken(request);

    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_getFavoritesRequest);
}

void Synchronizev2Service::addUserFavorites(QStringList ids)
{
    if (!m_isAuhorized) return;

    QNetworkRequest request(QUrl(m_apiv2host + "api/v1/accounts/users/me/favorites"));
    adjustRequestToken(request);
    QStringList releases;
    foreach (auto id, ids) {
        auto item = QString("{\"release_id\": %1}").arg(id);
        releases.append(item);
    }
    QString result = "[" + releases.join(",") + "]";
    auto reply = m_networkManager->post(request, result.toUtf8());
    adjustIdentifier(reply, m_addFavoritesRequest);
}

void Synchronizev2Service::removeUserFavorites(QStringList ids)
{
    if (!m_isAuhorized) return;

    QNetworkRequest request(QUrl(m_apiv2host + "api/v1/accounts/users/me/favorites"));
    adjustRequestToken(request);
    QStringList releases;
    foreach (auto id, ids) {
        auto item = QString("{\"release_id\": %1}").arg(id);
        releases.append(item);
    }
    QString result = "[" + releases.join(",") + "]";
    auto reply = m_networkManager->sendCustomRequest(request, "DELETE", result.toUtf8());
    adjustIdentifier(reply, m_removeFavoritesRequest);
}

void Synchronizev2Service::downloadTorrent(QString torrentPath)
{

}

void Synchronizev2Service::synchronizeFullCache()
{

}

void Synchronizev2Service::logoutHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) return;

    emit userSignouted();
}

void Synchronizev2Service::userProfileHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) {
        emit userFailedAuthentificated("Контент ответа пустой");
        return;
    }

    QJsonParseError* error = nullptr;
    auto json = QJsonDocument::fromJson(content, error);
    if (error != nullptr) return;

    auto response = json.object();
    if (response.contains("nickname")) m_nickName = response.value("nickname").toString();
    //if (response.contains("created_at")) m_nickName = response.value("created_at").toString();
    if (response.contains("avatar")) {
        auto avatarObject = response.value("avatar").toObject();
        if (avatarObject.contains("preview")) m_avatar = avatarObject.value("preview").toString();
    }
    if (response.contains("torrents")) {
        auto torrentsObject = response.value("torrents").toObject();
        if (torrentsObject.contains("uploaded")) m_userTorrentsUploaded = torrentsObject.value("uploaded").toInt();
        if (torrentsObject.contains("downloaded")) m_userTorrentsDownloaded = torrentsObject.value("downloaded").toInt();
        if (torrentsObject.contains("passkey")) m_userTorrentsPasskey = torrentsObject.value("passkey").toString();
    }

    m_isAuhorized = true;
    emit isAuhorizedChanged();
}

void Synchronizev2Service::adjustRequestToken(QNetworkRequest &request) noexcept
{
    auto tokenAndBearer = "bearer " + m_token;
    request.setRawHeader("Authorization", tokenAndBearer.toUtf8());
}

void Synchronizev2Service::adjustIdentifier(QNetworkReply *reply, const QString& type) noexcept
{
    auto identifier = QUuid::createUuid().toString();
    m_pendingRequests.insert(identifier, type);

    reply->setProperty("identifier", identifier);
}

void Synchronizev2Service::loginHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) {
        emit userFailedAuthentificated("Контент ответа пустой");
        return;
    }

    QJsonParseError* error = nullptr;
    auto json = QJsonDocument::fromJson(content, error);
    if (error != nullptr) {
        emit userFailedAuthentificated("Ответ не содержит корректный JSON");
        return;
    }

    auto response = json.object();
    if (response.contains("token")) {
        m_token = response.value("token").toString();
        emit userCompleteAuthentificated();
    }

    emit userFailedAuthentificated("Ответ не token");
}

void Synchronizev2Service::requestFinished(QNetworkReply *reply)
{
    auto identifierVariant = reply->property("identifier");
    if (!identifierVariant.isValid()) return;

    auto identifier = identifierVariant.toString();

    if (!m_pendingRequests.contains(identifier)) return;

    auto requestType = m_pendingRequests[identifier];
    if (requestType == m_pairLogin) {
        loginHandler(reply);
        return;
    }
    if (requestType == m_logoutRequest) {
        logoutHandler(reply);
        return;
    }
}
