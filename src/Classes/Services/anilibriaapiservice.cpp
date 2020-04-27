#include "anilibriaapiservice.h"
#include <QtNetwork>
#include <QtDebug>
#include <QFile>
#include <QUuid>
#include <QJsonDocument>

AnilibriaApiService::AnilibriaApiService(QObject *parent) : QObject(parent),
    m_QueuedAddedFavorites(new QQueue<int>()),
    m_QueuedDeletedFavorites(new QQueue<int>()),
    m_FavoriteToken("")
{
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation));
    if (dir.exists()) {
        QFile settingsFile(QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/api.settings");
        if (settingsFile.exists()) {
            if (settingsFile.open(QFile::ReadOnly | QFile::Text)) {
                auto settingsJson = settingsFile.readAll();
                settingsFile.close();
                auto settingsObject = QJsonDocument::fromJson(settingsJson).object();
                auto apiPath = settingsObject.value("apiPath").toString();
                if (!apiPath.isEmpty()) AnilibriaApiPath = apiPath;
                auto staticPath = settingsObject.value("staticPath").toString();
                if (!staticPath.isEmpty()) AnilibriaImagesPath = staticPath;
            }
        }
    }
}

void AnilibriaApiService::getAllReleases()
{
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiPath + "public/api/index.php"));
    request.setRawHeader("User-Agent", "Anilibria CP Client");
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));

    QUrlQuery params;
    params.addQueryItem("query", "list");
    params.addQueryItem("page", "1");
    params.addQueryItem("perPage", "1000");

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(getAllReleasesResponse(QNetworkReply*)));

    networkManager->post(request, params.query(QUrl::FullyEncoded).toUtf8());
}

void AnilibriaApiService::getYoutubeVideos()
{
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiPath + "public/api/index.php"));
    request.setRawHeader("User-Agent", "Anilibria CP Client");
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));

    QUrlQuery params;
    params.addQueryItem("query", "youtube");
    params.addQueryItem("page", "1");
    params.addQueryItem("perPage", "1000");

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(getAllYoutubeItemsResponse(QNetworkReply*)));

    networkManager->post(request, params.query(QUrl::FullyEncoded).toUtf8());
}

void AnilibriaApiService::getSchedule()
{
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiPath + "public/api/index.php"));
    request.setRawHeader("User-Agent", "Anilibria CP Client");
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("application/x-www-form-urlencoded"));
    QUrlQuery params;
    params.addQueryItem("query", "schedule");
    params.addQueryItem("filter", "id");

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(getScheduleResponse(QNetworkReply*)));

    networkManager->post(request, params.query(QUrl::FullyEncoded).toUtf8());
}

void AnilibriaApiService::signin(QString email, QString password, QString fa2code)
{
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiPath + "public/login.php"));

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(signinResponse(QNetworkReply*)));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("mail", email);
    params.addQueryItem("passwd", password);
    params.addQueryItem("fa2code", fa2code);

    networkManager->post(request, params.query().toUtf8());
}

void AnilibriaApiService::signout(QString token)
{
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiPath + "public/logout.php"));

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(signoutResponse(QNetworkReply*)));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    auto cookie = "PHPSESSID=" + token;
    request.setRawHeader("Cookie", cookie.toUtf8());

    QUrlQuery params;
    params.addQueryItem("query", "-");

    networkManager->post(request, params.query().toUtf8());

}

void AnilibriaApiService::getUserData(QString token)
{
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiPath + "public/api/index.php"));

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(getUserDataResponse(QNetworkReply*)));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    auto cookie = "PHPSESSID=" + token;
    request.setRawHeader("Cookie", cookie.toUtf8());

    QUrlQuery params;
    params.addQueryItem("query", "user");

    networkManager->post(request, params.query().toUtf8());
}

void AnilibriaApiService::getFavorites(QString token)
{
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiPath + "public/api/index.php"));

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(getUserFavoritesResponse(QNetworkReply*)));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");
    auto cookie = "PHPSESSID=" + token;
    request.setRawHeader("Cookie", cookie.toUtf8());

    QUrlQuery params;
    params.addQueryItem("query", "favorites");
    params.addQueryItem("filter", "id");
    params.addQueryItem("page", "1");
    params.addQueryItem("perPage", "1000");

    networkManager->post(request, params.query().toUtf8());
}

void AnilibriaApiService::addMultiFavorites(QString token, QString ids)
{
    auto idsArray = ids.split(",");
    foreach (auto idAsString, idsArray) {
        int id = idAsString.toInt();
        if (!m_QueuedAddedFavorites->contains(id)) m_QueuedAddedFavorites->enqueue(id);
    }

    auto firstFavoriteId = m_QueuedAddedFavorites->dequeue();
    performAddFavorite(token, firstFavoriteId);
}

void AnilibriaApiService::performAddFavorite(QString token, int id)
{
    m_FavoriteToken = token;
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiPath + "public/api/index.php"));

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(editFavoritesResponse(QNetworkReply*)));

    auto cookie = "PHPSESSID=" + token;
    request.setRawHeader("Cookie", cookie.toUtf8());

    QUrlQuery params;
    params.addQueryItem("query", "favorites");
    params.addQueryItem("action", "add");
    params.addQueryItem("id", QString::number(id));

    networkManager->post(request, params.query().toUtf8());
}

void AnilibriaApiService::performRemoveFavorite(QString token, int id)
{
    m_FavoriteToken = token;
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiPath + "public/api/index.php"));

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(deleteFavoritesResponse(QNetworkReply*)));

    auto cookie = "PHPSESSID=" + token;
    request.setRawHeader("Cookie", cookie.toUtf8());

    QUrlQuery params;
    params.addQueryItem("query", "favorites");
    params.addQueryItem("action", "delete");
    params.addQueryItem("id", QString::number(id));

    networkManager->post(request, params.query().toUtf8());
}

void AnilibriaApiService::removeMultiFavorites(QString token, QString ids)
{
    auto idsArray = ids.split(",");
    foreach (auto idAsString, idsArray) {
        int id = idAsString.toInt();
        if (!m_QueuedDeletedFavorites->contains(id)) m_QueuedDeletedFavorites->enqueue(id);
    }

    auto firstFavoriteId = m_QueuedDeletedFavorites->dequeue();
    performRemoveFavorite(token, firstFavoriteId);
}

void AnilibriaApiService::downloadTorrent(QString path)
{
    auto networkManager = new QNetworkAccessManager(this);
    auto url = QUrl(path);
    QNetworkRequest request(url);

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(downloadTorrentResponse(QNetworkReply*)));

    networkManager->get(request);

}

void AnilibriaApiService::getAllReleasesResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    QString data = reply->readAll();

    emit allReleasesReceived(data);
}

void AnilibriaApiService::getAllYoutubeItemsResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    QString data = reply->readAll();

    emit allYoutubeItemReceived(data);
}

void AnilibriaApiService::getScheduleResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    QString data = reply->readAll();

    emit scheduleReceived(data);
}

void AnilibriaApiService::signinResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    QList<QByteArray> headerList = reply->rawHeaderList();
    QString cookie;
    foreach(QByteArray head, headerList) {
        if (head == "Set-Cookie") {
            cookie = reply->rawHeader(head);
            break;
        }
    }

    if (!cookie.isEmpty()) {
        auto indexKey = cookie.indexOf("PHPSESSID=");
        auto endIndex = cookie.indexOf(";", indexKey);

        auto cookieValue = cookie.mid(indexKey, endIndex).replace("PHPSESSID=", "");
        emit signinReceived(cookieValue, reply->readAll());
    } else {
        emit signinReceived("", reply->readAll());
    }
}

void AnilibriaApiService::signoutResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    emit signoutReceived();
}

void AnilibriaApiService::getUserDataResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    emit userDataReceived(reply->readAll());
}

void AnilibriaApiService::getUserFavoritesResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    emit userFavoritesReceived(reply->readAll());
}

void AnilibriaApiService::editFavoritesResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    if (m_QueuedAddedFavorites->isEmpty()) {
        emit userFavoritesUpdated();
        return;
    }

    auto id = m_QueuedAddedFavorites->dequeue();
    performAddFavorite(m_FavoriteToken, id);
}

void AnilibriaApiService::deleteFavoritesResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    if (m_QueuedDeletedFavorites->isEmpty()) {
        emit userFavoritesUpdated();
        return;
    }

    auto id = m_QueuedDeletedFavorites->dequeue();
    performRemoveFavorite(m_FavoriteToken, id);
}

void AnilibriaApiService::downloadTorrentResponse(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    auto byteArray = reply->readAll();

    auto uuid = QUuid::createUuid();
    auto randomName = uuid.toString().replace("-", "").replace("{", "").replace("}", "").replace("[", "").replace("]", "");
    auto randomTorrentPath = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + "/" + randomName + ".torrent";
    QFile torrentFile(randomTorrentPath);
    torrentFile.open(QFile::WriteOnly);
    torrentFile.write(byteArray);
    torrentFile.close();

    emit torrentDownloaded(randomTorrentPath);
}
