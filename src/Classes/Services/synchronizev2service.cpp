#include <QUuid>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDesktopServices>
#include <QFile>
#include <QDir>
#include "synchronizev2service.h"
#include "../../globalhelpers.h"

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

void Synchronizev2Service::setTorrentDownloadMode(int torrentDownloadMode) noexcept
{
    if (m_torrentDownloadMode == torrentDownloadMode) return;

    m_torrentDownloadMode = torrentDownloadMode;
    emit torrentDownloadModeChanged();
}

void Synchronizev2Service::setTorrentStreamPort(int torrentStreamPort) noexcept
{
    if (m_torrentStreamPort == torrentStreamPort) return;

    m_torrentStreamPort = torrentStreamPort;
    emit torrentStreamPortChanged();
}

void Synchronizev2Service::setCacheFolder(const QString &cacheFolder) noexcept
{
    if (m_cacheFolder == cacheFolder) return;

    m_cacheFolder = cacheFolder;
    emit cacheFolderChanged();
}

QMap<int, QString> &&Synchronizev2Service::getLocalCollections()
{
    return std::move(m_synchronizedCollection);
}

void Synchronizev2Service::authorize(QString login, QString password)
{
    QNetworkRequest request(QUrl(m_apiv2host + "/api/v1/accounts/users/auth/login"));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto body = QString(R"({"login": "%1","password": "%2"})");
    body = body.arg(login);
    body = body.arg(password);

    auto reply = m_networkManager->post(request, body.toUtf8());
    adjustIdentifier(reply, m_pairLogin);
}

void Synchronizev2Service::authorizeSocial(const QString &provider)
{
    QNetworkRequest request(QUrl(m_apiv2host + "/api/v1/accounts/users/auth/social/" + provider + "/login?host=https:%2F%2Fanilibria.top"));

    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_socialRequest);
}

void Synchronizev2Service::logout()
{
    if (!m_isAuhorized) return;

    QNetworkRequest request(QUrl(m_apiv2host + "/api/v1/accounts/users/auth/logout"));
    adjustRequestToken(request);

    auto reply = m_networkManager->post(request, "");
    adjustIdentifier(reply, m_logoutRequest);
}

void Synchronizev2Service::getUserData()
{
    QNetworkRequest request(QUrl(m_apiv2host + "/api/v1/accounts/users/me/profile"));
    adjustRequestToken(request);

    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_userProfileRequest);
}

void Synchronizev2Service::getUserFavorites()
{
    if (!m_isAuhorized) return;

    QNetworkRequest request(QUrl(m_apiv2host + "/api/v1/accounts/users/me/favorites/ids"));
    adjustRequestToken(request);

    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_getFavoritesRequest);
}

void Synchronizev2Service::addUserFavorites(const QList<int> ids)
{
    if (!m_isAuhorized) return;

    QNetworkRequest request(QUrl(m_apiv2host + "/api/v1/accounts/users/me/favorites"));
    adjustRequestToken(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QStringList releases;
    foreach (auto id, ids) {
        auto item = QString(R"({"release_id": %1})").arg(id);
        releases.append(item);
    }
    QString result = "[" + releases.join(",") + "]";
    auto reply = m_networkManager->post(request, result.toUtf8());
    adjustIdentifier(reply, m_addFavoritesRequest);
}

void Synchronizev2Service::removeUserFavorites(const QList<int> ids)
{
    if (!m_isAuhorized) return;

    QNetworkRequest request(QUrl(m_apiv2host + "/api/v1/accounts/users/me/favorites"));
    adjustRequestToken(request);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QStringList releases;
    foreach (auto id, ids) {
        auto item = QString(R"({"release_id": %1})").arg(id);
        releases.append(item);
    }
    QString result = "[" + releases.join(",") + "]";
    auto reply = m_networkManager->sendCustomRequest(request, "DELETE", result.toUtf8());
    adjustIdentifier(reply, m_removeFavoritesRequest);
}

void Synchronizev2Service::downloadTorrent(QString torrentPath, int releaseId, const QString magnet)
{
    if (m_torrentDownloadMode == 3) {
        QDesktopServices::openUrl(QUrl(magnet));
        return;
    }

    QUrl url;
    //if use torrent stream
    if (m_torrentDownloadMode == 2) {
        url = QUrl("http://localhost:" + QString::number(m_torrentStreamPort) + "/fulldownload?id=" + QString::number(releaseId) + "&path=" + torrentPath);
    } else {
        url = QUrl(torrentPath);
    }

    qDebug() << "Download torrent url: " << url;

    QNetworkRequest request(url);
    auto reply = m_networkManager->get(request);
    auto identifier = adjustIdentifier(reply, m_downloadTorrentRequest);
    m_pendingTorrentReleases.insert(identifier, releaseId);
}

void Synchronizev2Service::synchronizeFullCache()
{
    if (m_synchronizeCacheActived) return;

    m_synchronizeCacheActived = true;
    emit synchronizeCacheActivedChanged();

    if (m_cacheHostIsFolder) {
        QDir cacheDir(m_cacheFolder);
        if (!cacheDir.exists()) {
            emit synchronizeCacheFailed("Папка с кешем не найдена или не доступна " + cacheDir.absolutePath());
            return;
        }
        cacheFolderHandler(cacheDir.absolutePath());
    } else {
        QNetworkRequest request(QUrl(m_cachehost + "/metadata"));
        auto reply = m_networkManager->get(request);
        adjustIdentifier(reply, m_cacheMetadataRequest);
    }
}

QString Synchronizev2Service::checkFolderAvailability(const QString &folder)
{
    QDir dir(folder);
    if (!dir.exists(folder)) return "Папка кеша не существует!";

    if (!QFile::exists(folder + "/metadata")) return "Файл метаданных кеша не найден!";

    return "";
}

void Synchronizev2Service::checkNetworkAvailability(const QString &address)
{
    auto url = QUrl(address);
    QNetworkRequest request(url);
    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_checkNetworkAvailability);
}

void Synchronizev2Service::getUserSeens()
{
    QNetworkRequest request(QUrl(m_apiv2host + "/api/v1/accounts/users/me/views/timecodes"));
    adjustRequestToken(request);

    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_userSeenRequest);
}

QVariantMap Synchronizev2Service::getUserSynchronizedSeens()
{
    QVariantMap result;
    auto keys = m_synchronizedSeens.keys();
    foreach (auto key, keys) {
        auto item = m_synchronizedSeens[key];
        QVariantMap map;
        map["mark"] = std::get<0>(item);
        map["time"] = std::get<1>(item);
        result[key] = map;
    }

    return result;
}

void Synchronizev2Service::clearUserSynchronizedSeens()
{
    m_synchronizedSeens.clear();
}

void Synchronizev2Service::addSeenMarks(QList<QString> videoIds, bool seenMark)
{
    QNetworkRequest request(QUrl(m_apiv2host + "/api/v1/accounts/users/me/views/timecodes"));
    adjustRequestToken(request);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonArray array;

    foreach (auto videoId, videoIds) {
        QJsonObject item;
        item["time"] = 0;
        item["is_watched"] = seenMark;
        item["release_episode_id"] = videoId;

        array.append(item);
    }

    auto body = QJsonDocument(array).toJson();

    auto reply = m_networkManager->post(request, body);
    adjustIdentifier(reply, m_addUserSeenRequest);
}

void Synchronizev2Service::getCollections()
{
    QNetworkRequest request(QUrl(m_apiv2host + "/api/v1/accounts/users/me/collections/ids"));
    adjustRequestToken(request);
    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_collectionsRequest);
}

void Synchronizev2Service::addReleasesToCollection(QList<int> releaseIds, const QString& collection)
{
    QNetworkRequest request(QUrl(m_apiv2host + "/api/v1/accounts/users/me/collections"));
    adjustRequestToken(request);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonArray array;

    foreach (auto releaseId, releaseIds) {
        QJsonObject item;
        item["release_id"] = releaseId;
        item["type_of_collection"] = collection;

        array.append(item);
    }

    auto body = QJsonDocument(array).toJson();
    auto reply = m_networkManager->post(request, body);
    adjustIdentifier(reply, m_addToCollectionRequest);
 }

void Synchronizev2Service::removeReleasesFromCollection(QList<int> releaseIds)
{
    QNetworkRequest request(QUrl(m_apiv2host + "/api/v1/accounts/users/me/collections"));
    adjustRequestToken(request);

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QJsonArray array;

    foreach (auto releaseId, releaseIds) {
        QJsonObject item;
        item["release_id"] = releaseId;

        array.append(item);
    }

    auto body = QJsonDocument(array).toJson();
    auto reply = m_networkManager->sendCustomRequest(request, "DELETE", body);
    adjustIdentifier(reply, m_removeFromCollectionRequest);
}

void Synchronizev2Service::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    m_socialCheckTimerIterator++;

    if (m_socialState.isEmpty() || m_socialCheckTimerIterator == 20) {
        killTimer(m_socialCheckTimer);
        m_socialCheckTimer = 0;
        emit isSocialAuthentificationChanged();
        return;
    }

    QNetworkRequest request(QUrl(m_apiv2host + "/api/v1/accounts/users/auth/social/authenticate?state=" + m_socialState));

    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_socialRequestResponse);
}

void Synchronizev2Service::downloadReleaseFile() noexcept
{
    QNetworkRequest request(QUrl(m_cachehost + "/releases" + QString::number(m_currentSynchronizationItem) +".json"));
    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_cacheReleaseRequest);
}

void Synchronizev2Service::downloadEpisodesFile() noexcept
{
    QNetworkRequest request(QUrl(m_cachehost + "/episodes" + QString::number(m_currentSynchronizationItem) +".json"));
    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_cacheEpisodesRequest);
}

void Synchronizev2Service::downloadTorrentsFile() noexcept
{
    QNetworkRequest request(QUrl(m_cachehost + "/torrents.json"));
    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_cacheTorrentRequest);
}

void Synchronizev2Service::downloadTypesFile() noexcept
{
    QNetworkRequest request(QUrl(m_cachehost + "/types.json"));
    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_cacheTypesRequest);
}

void Synchronizev2Service::downloadSchedulesFile() noexcept
{
    QNetworkRequest request(QUrl(m_cachehost + "/schedule.json"));
    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_cacheScheduleRequest);
}

void Synchronizev2Service::downloadReleaseSeriesFile() noexcept
{
    QNetworkRequest request(QUrl(m_cachehost + "/releaseseries.json"));
    auto reply = m_networkManager->get(request);
    adjustIdentifier(reply, m_cacheReleaseSerieRequest);
}

void Synchronizev2Service::logoutHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) return;

    m_isAuhorized = false;
    m_token = "";
    m_nickName = "";
    m_avatar = "";

    emit tokenChanged();
    emit isAuhorizedChanged();
    emit userSignouted();
    emit nickNameChanged();
    emit userAvatarChanged();
}

void Synchronizev2Service::userProfileHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) {
        emit getUserFailed("Контент ответа пустой");
        return;
    }

    QJsonParseError* error = nullptr;
    auto json = QJsonDocument::fromJson(content, error);
    if (error != nullptr) return;

    auto response = json.object();
    if (response.contains("nickname")) {
        m_nickName = response.value("nickname").toString();
        emit nickNameChanged();
    }
    if (m_nickName.isEmpty()) {
        if (response.contains("login")) {
            m_nickName = response.value("login").toString();
            emit nickNameChanged();
        }
    }
    //if (response.contains("created_at")) m_nickName = response.value("created_at").toString();
    if (response.contains("avatar")) {
        auto avatarObject = response.value("avatar").toObject();
        if (avatarObject.contains("preview")) {
            m_avatar = avatarObject.value("preview").toString();
            emit userAvatarChanged();
        }
    }
    if (response.contains("torrents")) {
        auto torrentsObject = response.value("torrents").toObject();
        if (torrentsObject.contains("uploaded")) m_userTorrentsUploaded = torrentsObject.value("uploaded").toInt();
        if (torrentsObject.contains("downloaded")) m_userTorrentsDownloaded = torrentsObject.value("downloaded").toInt();
        if (torrentsObject.contains("passkey")) m_userTorrentsPasskey = torrentsObject.value("passkey").toString();
    }

    m_isAuhorized = true;
    emit isAuhorizedChanged();

    getUserFavorites();
    getUserSeens();
    getCollections();
}

void Synchronizev2Service::adjustRequestToken(QNetworkRequest &request) noexcept
{
    auto tokenAndBearer = "Bearer " + m_token;
    request.setRawHeader("Authorization", tokenAndBearer.toUtf8());
}

QString Synchronizev2Service::adjustIdentifier(QNetworkReply *reply, const QString& type) noexcept
{
    auto identifier = QUuid::createUuid().toString();
    m_pendingRequests.insert(identifier, type);

    reply->setProperty("identifier", identifier);

    return identifier;
}

void Synchronizev2Service::favoritesSynchronizedHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) {
        emit synchronizeFavoritesFailed("Контент ответа пустой");
        return;
    }

    QJsonParseError* error = nullptr;
    auto json = QJsonDocument::fromJson(content, error);
    if (error != nullptr) {
        emit synchronizeFavoritesFailed("Ответ не содержит корректный JSON");
        return;
    }

    auto response = json.array();
    if (response.empty()) return;

    QList<int> ids;
    foreach(auto id, response) {
        ids.append(id.toInt(0));
    }
    emit userFavoritesReceivedV2(ids);
}

void Synchronizev2Service::downloadTorrentHandler(QNetworkReply *reply, const QString& identifier) noexcept
{
    auto byteArray = reply->readAll();

    auto uuid = QUuid::createUuid();
    auto randomName = uuid.toString().replace("-", "").replace("{", "").replace("}", "").replace("[", "").replace("]", "");

    auto randomTorrentPath = getCachePath(randomName + ".torrent");
    QFile torrentFile(randomTorrentPath);
    torrentFile.open(QFile::WriteOnly);
    torrentFile.write(byteArray);
    torrentFile.close();

    if (m_torrentDownloadMode == 0) { //Открыть в торрент клиенте
#ifdef Q_OS_WIN
        auto protocol = "file:///";
#else
        auto protocol = "file://";
#endif

        QDesktopServices::openUrl(QUrl(protocol + randomTorrentPath));
    }

    if (m_torrentDownloadMode == 1) { // Сохранить файл
        emit saveDownloadedTorrent(randomTorrentPath);
    }

    if (m_torrentDownloadMode == 2 && m_pendingTorrentReleases.contains(identifier)) { // Использовать TorrentStream
        emit downloadInTorrentStream(m_pendingTorrentReleases[identifier]);
    }

    emit torrentDownloaded(randomTorrentPath);
}

void Synchronizev2Service::metadataCacheHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) {
        emit synchronizeCacheFailed("Metadata: Контент ответа пустой");
        return;
    }

    auto updatedMetadataJson = QJsonDocument::fromJson(content);
    auto updatedMetadata = updatedMetadataJson.object();
    m_lastReleaseTimeStamp = 0;
    m_countEpisodes = 0;
    m_countReleases = 0;
    if (updatedMetadata.contains("lastReleaseTimeStamp")) m_lastReleaseTimeStamp = updatedMetadata.value("lastReleaseTimeStamp").toInt(0);
    if (updatedMetadata.contains("countEpisodes")) m_countEpisodes = updatedMetadata.value("countEpisodes").toInt(0);
    if (updatedMetadata.contains("countReleases")) m_countReleases = updatedMetadata.value("countReleases").toInt(0);

    auto metadataPath = getCachePath("metadata");
    auto lastTimeStamp = 0;
    if (QFile::exists(metadataPath)) {
        QFile file(metadataPath);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            auto loadedContent = file.readAll();
            file.close();

            auto metadataJson = QJsonDocument::fromJson(loadedContent);
            auto metadata = metadataJson.object();
            if (metadata.contains("lastReleaseTimeStamp")) {
                lastTimeStamp = metadata.value("lastReleaseTimeStamp").toInt(0);
            }
        }
    }

    if (lastTimeStamp > 0 && m_lastReleaseTimeStamp <= lastTimeStamp) {
        m_synchronizeCacheActived = false;
        emit synchronizeCacheActivedChanged();
        emit synchronizationCompletedNoChanges();
        return;
    }

    m_previousLastTimeStamp = lastTimeStamp;

    if (QFile::exists(metadataPath)) QFile::remove(metadataPath);

    QFile file(metadataPath);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        file.write(content);
        file.close();
    }

    m_currentSynchronizationItem = 0;
    downloadReleaseFile();
}

void Synchronizev2Service::releaseCacheHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) {
        emit synchronizeCacheFailed("Release" + QString::number(m_currentSynchronizationItem) + ": Контент ответа пустой");
        m_synchronizeCacheActived = false;
        emit synchronizeCacheActivedChanged();
        return;
    }

    auto cacheFilePath = getCachePath("releases" + QString::number(m_currentSynchronizationItem) + ".cache");

    if (QFile::exists(cacheFilePath)) QFile::remove(cacheFilePath);

    QFile file(cacheFilePath);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        file.write(content);
        file.close();
    }

    if (m_currentSynchronizationItem == m_countReleases - 1) {
        m_currentSynchronizationItem = 0;
        downloadEpisodesFile();
        return;
    }

    m_currentSynchronizationItem++;
    downloadReleaseFile();
}

void Synchronizev2Service::episodeCacheHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) {
        emit synchronizeCacheFailed("Episode" + QString::number(m_currentSynchronizationItem) + ": Контент ответа пустой");
        m_synchronizeCacheActived = false;
        emit synchronizeCacheActivedChanged();
        return;
    }

    auto cacheFilePath = getCachePath("episodes" + QString::number(m_currentSynchronizationItem) + ".cache");

    if (QFile::exists(cacheFilePath)) QFile::remove(cacheFilePath);

    QFile file(cacheFilePath);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        file.write(content);
        file.close();
    }

    if (m_currentSynchronizationItem == m_countEpisodes - 1) {
        downloadTorrentsFile();
        return;
    }

    m_currentSynchronizationItem++;
    downloadEpisodesFile();
}

void Synchronizev2Service::torrentCacheHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) {
        emit synchronizeCacheFailed("Torrent: Контент ответа пустой");
        m_synchronizeCacheActived = false;
        emit synchronizeCacheActivedChanged();
        return;
    }

    auto cacheFilePath = getCachePath("torrents.cache");

    if (QFile::exists(cacheFilePath)) QFile::remove(cacheFilePath);

    QFile file(cacheFilePath);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        file.write(content);
        file.close();
    }

    downloadSchedulesFile();
}

void Synchronizev2Service::scheduleCacheHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) {
        emit synchronizeCacheFailed("Schedule: Контент ответа пустой");
        m_synchronizeCacheActived = false;
        emit synchronizeCacheActivedChanged();
        return;
    }

    auto cacheFilePath = getCachePath("schedule.cache");

    if (QFile::exists(cacheFilePath)) QFile::remove(cacheFilePath);

    QFile file(cacheFilePath);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        file.write(content);
        file.close();
    }

    downloadReleaseSeriesFile();
}

void Synchronizev2Service::releaseSeriesCacheHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) {
        emit synchronizeCacheFailed("ReleaseSeries: Контент ответа пустой");
        m_synchronizeCacheActived = false;
        emit synchronizeCacheActivedChanged();
        return;
    }

    auto cacheFilePath = getCachePath("releaseseries.cache");

    if (QFile::exists(cacheFilePath)) QFile::remove(cacheFilePath);

    QFile file(cacheFilePath);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        file.write(content);
        file.close();
    }

    downloadTypesFile();
}

void Synchronizev2Service::typesCacheHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) {
        emit synchronizeCacheFailed("Types: Контент ответа пустой");
        m_synchronizeCacheActived = false;
        emit synchronizeCacheActivedChanged();
        return;
    }

    auto cacheFilePath = getCachePath("types.cache");

    if (QFile::exists(cacheFilePath)) QFile::remove(cacheFilePath);

    QFile file(cacheFilePath);
    if (file.open(QFile::WriteOnly | QFile::Text)) {
        file.write(content);
        file.close();
    }

    m_synchronizeCacheActived = false;
    emit synchronizeCacheActivedChanged();
    emit synchronizationCompleted(m_previousLastTimeStamp);
}

void Synchronizev2Service::cacheFolderHandler(const QString &fullPath) noexcept
{
    auto metadataPath = fullPath + "/metadata";
    if (!QFile::exists(metadataPath)){
        emit synchronizeCacheFailed("Файл метаданных не найден в папке с кешем " + metadataPath);
        return;
    }

    QJsonObject metadataObject;
    if (!readJsonObjectFromFile(metadataPath, metadataObject)) {
        emit synchronizeCacheFailed("Не удалось прочитать файл метаданных в папке с кешем " + metadataPath);
        return;
    }

    m_countEpisodes = 0;
    m_countReleases = 0;
    m_lastReleaseTimeStamp = 0;
    if (metadataObject.contains("countEpisodes")) m_countEpisodes = metadataObject.value("countEpisodes").toInt(0);
    if (metadataObject.contains("countReleases")) m_countReleases = metadataObject.value("countReleases").toInt(0);
    if (metadataObject.contains("lastReleaseTimeStamp")) m_lastReleaseTimeStamp = metadataObject.value("lastReleaseTimeStamp").toInt(0);

    auto localMetadataPath = getCachePath("metadata");
    auto lastTimeStamp = 0;
    if (QFile::exists(localMetadataPath)) {
        QFile file(localMetadataPath);
        if (file.open(QFile::ReadOnly | QFile::Text)) {
            auto loadedContent = file.readAll();
            file.close();

            auto metadataJson = QJsonDocument::fromJson(loadedContent);
            auto metadata = metadataJson.object();
            if (metadata.contains("lastReleaseTimeStamp")) {
                lastTimeStamp = metadata.value("lastReleaseTimeStamp").toInt(0);
            }
        }
    }

    if (lastTimeStamp > 0 && m_lastReleaseTimeStamp > 0 && m_lastReleaseTimeStamp <= lastTimeStamp) {
        m_synchronizeCacheActived = false;
        emit synchronizeCacheActivedChanged();
        emit synchronizationCompletedNoChanges();
        return;
    }

    auto typesPath = fullPath + "/types.json";
    auto schedulePath = fullPath + "/schedule.json";
    auto releaseseriesPath = fullPath + "/releaseseries.json";
    auto torrentsPath = fullPath + "/torrents.json";
    if (!QFile::exists(typesPath)) {
        emit synchronizeCacheFailed("Файл types.json не найден в папке с кешем " + typesPath);
        return;
    }
    if (!QFile::exists(schedulePath)) {
        emit synchronizeCacheFailed("Файл schedule.json не найден в папке с кешем " + schedulePath);
        return;
    }
    if (!QFile::exists(releaseseriesPath)) {
        emit synchronizeCacheFailed("Файл releaseseries.json не найден в папке с кешем " + releaseseriesPath);
        return;
    }
    if (!QFile::exists(torrentsPath)) {
        emit synchronizeCacheFailed("Файл torrents.json не найден в папке с кешем " + torrentsPath);
        return;
    }

    QStringList releases;
    for (auto i = 0; i < m_countReleases; i++) {
        auto releasesPathName = "releases" + QString::number(i);
        releases.append(releasesPathName);
        auto releasePartName = fullPath + "/" + releasesPathName + ".json";
        if (!QFile::exists(releasePartName)) {
            emit synchronizeCacheFailed("Файл releases не найден в папке с кешем " + releasePartName);
            return;
        }
    }

    QStringList episodes;
    for (auto i = 0; i < m_countEpisodes; i++) {
        auto episodePathName = "episodes" + QString::number(i);
        episodes.append(episodePathName);
        auto episodePartName = fullPath + "/" + episodePathName + ".json";
        if (!QFile::exists(fullPath + "/" + episodePathName + ".json")) {
            emit synchronizeCacheFailed("Файл episodes не найден в папке с кешем " + episodePartName);
            return;
        }
    }

    copyFile(typesPath, "types.cache");
    copyFile(schedulePath, "schedule.cache");
    copyFile(releaseseriesPath, "releaseseries.cache");
    copyFile(torrentsPath, "torrents.cache");

    foreach (auto release, releases) {
        auto releasesPath = fullPath + "/" + release + ".json";
        copyFile(releasesPath, release + ".cache");
    }

    foreach (auto episode, episodes) {
        auto episodePath = fullPath + "/" + episode + ".json";
        copyFile(episodePath, episode + ".cache");
    }

    copyFile(metadataPath, "metadata");

    m_synchronizeCacheActived = false;
    emit synchronizeCacheActivedChanged();
    emit synchronizationCompleted(0);
}

bool Synchronizev2Service::copyFile(const QString &fullPath, const QString &cacheFileName) noexcept
{
    QFile file(fullPath);
    if (!file.open(QFile::ReadOnly)) {
        emit synchronizeCacheFailed("Не удалось открыть файл(оригинал): " + fullPath);
        return false;
    }

    auto cacheFilePath = getCachePath(cacheFileName);
    QFile cachefile(cacheFilePath);
    if (!cachefile.open(QFile::WriteOnly | QFile::Truncate)) {
        emit synchronizeCacheFailed("Не удалось открыть файл(кеш): " + cacheFilePath);
        return false;
    }

    auto fullData = file.readAll();
    file.close();

    cachefile.write(fullData);
    cachefile.close();

    return true;
}

void Synchronizev2Service::socialRequestHandler(QNetworkReply *reply) noexcept
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
    QString socialUrl = "";
    if (response.contains("state")) m_socialState = response.value("state").toString();
    if (response.contains("url")) socialUrl = response.value("url").toString();

    if (socialUrl.isEmpty() && m_socialState.isEmpty()) {
        emit userFailedAuthentificated("Ответ не содержит state и url");
        return;
    }

    m_socialCheckTimerIterator = 0;
    m_socialCheckTimer = startTimer(1000 * 5);

    emit isSocialAuthentificationChanged();

    QDesktopServices::openUrl(QUrl(socialUrl));
}

void Synchronizev2Service::socialRequestTokenHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) return;

    QJsonParseError* error = nullptr;
    auto json = QJsonDocument::fromJson(content, error);
    if (error != nullptr) return;

    auto response = json.object();
    if (response.contains("token")) {
        m_token = response.value("token").toString();
        if (m_token.isEmpty()) return;

        m_socialState = "";
        killTimer(m_socialCheckTimer);
        m_socialCheckTimer = 0;
        emit isSocialAuthentificationChanged();
        emit userCompleteAuthentificated();
        emit tokenChanged();
    }
}

void Synchronizev2Service::checkNetworkAvailabilityHandler(QNetworkReply *reply) noexcept
{
    if (reply->error() != QNetworkReply::NoError) {
        emit checkNetworkAvailibilityFailedChanged("Проверка не удалась: " + reply->errorString());
        return;
    }

    emit checkNetworkAvailibilityCompletedChanged();
}

void Synchronizev2Service::userSeenSynchronizationHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) {
        emit synchronizeSeensFailed("Контент ответа пустой");
        return;
    }

    QJsonParseError* error = nullptr;
    auto json = QJsonDocument::fromJson(content, error);
    if (error != nullptr) {
        emit synchronizeSeensFailed("Ответ не содержит корректный JSON");
        return;
    }

    auto items = json.array();

    m_synchronizedSeens.clear();

    foreach (auto item, items) {
        auto itemArray = item.toArray();
        if (itemArray.size() != 3) continue;

        auto episodeId = itemArray.first().toString();
        auto time = itemArray[1].toInt(0);
        auto isWatched = itemArray.last().toBool();

        auto valueItem = std::make_tuple(isWatched, time);
        m_synchronizedSeens.insert(episodeId, valueItem);
    }

    if (!m_synchronizedSeens.isEmpty()) emit synchronizeSeensCompleted();
}

void Synchronizev2Service::userCollectionSynchronizeHandler(QNetworkReply *reply) noexcept
{
    auto content = reply->readAll();
    if (content.isEmpty()) {
        emit synchronizeCollectionFailed("Контент ответа пустой");
        return;
    }

    QJsonParseError* error = nullptr;
    auto json = QJsonDocument::fromJson(content, error);
    if (error != nullptr) {
        emit synchronizeCollectionFailed("Ответ не содержит корректный JSON");
        return;
    }

    m_synchronizedCollection.clear();

    auto response = json.array();
    foreach (auto item, response) {
        auto itemArray = item.toArray();
        if (itemArray.size() < 2) continue;

        auto releaseId = itemArray.first().toInt();
        auto collection = itemArray[1].toString();

        m_synchronizedCollection.insert(releaseId, collection);
    }

    if (!m_synchronizedCollection.isEmpty()) emit synchronizeCollectionCompleted();
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
        emit tokenChanged();
        return;
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
    m_pendingRequests.remove(identifier);

    if (requestType == m_pairLogin) {
        loginHandler(reply);
        return;
    }
    if (requestType == m_logoutRequest) {
        logoutHandler(reply);
        return;
    }
    if (requestType == m_userProfileRequest) {
        userProfileHandler(reply);
        return;
    }
    if (requestType == m_getFavoritesRequest) {
        favoritesSynchronizedHandler(reply);
        return;
    }
    if (requestType == m_downloadTorrentRequest) {
        downloadTorrentHandler(reply, identifier);
        return;
    }
    if (requestType == m_cacheMetadataRequest) {
        metadataCacheHandler(reply);
        return;
    }
    if (requestType == m_cacheReleaseRequest) {
        releaseCacheHandler(reply);
        return;
    }
    if (requestType == m_cacheTorrentRequest) {
        torrentCacheHandler(reply);
        return;
    }
    if (requestType == m_cacheEpisodesRequest) {
        episodeCacheHandler(reply);
        return;
    }
    if (requestType == m_cacheTypesRequest) {
        typesCacheHandler(reply);
        return;
    }
    if (requestType == m_cacheScheduleRequest) {
        scheduleCacheHandler(reply);
        return;
    }
    if (requestType == m_cacheReleaseSerieRequest) {
        releaseSeriesCacheHandler(reply);
        return;
    }
    if (requestType == m_socialRequest) {
        socialRequestHandler(reply);
        return;
    }
    if (requestType == m_socialRequestResponse) {
        socialRequestTokenHandler(reply);
        return;
    }
    if (requestType == m_checkNetworkAvailability) {
        checkNetworkAvailabilityHandler(reply);
        return;
    }
    if (requestType == m_userSeenRequest) {
        userSeenSynchronizationHandler(reply);
        return;
    }
    if (requestType == m_addUserSeenRequest || requestType == m_addToCollectionRequest || requestType == m_removeFromCollectionRequest) {
        //don't require handle response
        return;
    }
    if (requestType == m_collectionsRequest) {
        userCollectionSynchronizeHandler(reply);
        return;
    }

}
