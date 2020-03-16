#include "anilibriaapiservice.h"
#include <QtNetwork>
#include <QtDebug>
#include <QFile>
#include <QUuid>

const QString AnilibriaApiService::apiAddress = "https://anilibriasmartservice.azurewebsites.net/";
const QString AnilibriaApiService::newApiAddress = "https://wwnd.space/";

AnilibriaApiService::AnilibriaApiService(QObject *parent) : QObject(parent)
{
}

void AnilibriaApiService::getAllReleases()
{
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiService::newApiAddress + "public/api/index.php"));
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
    QNetworkRequest request(QUrl(AnilibriaApiService::newApiAddress + "public/api/index.php"));
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
    QNetworkRequest request(QUrl(AnilibriaApiService::newApiAddress + "public/api/index.php"));
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
    QNetworkRequest request(QUrl(AnilibriaApiService::apiAddress + "api/auth/signin" ));

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(signinResponse(QNetworkReply*)));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    QUrlQuery params;
    params.addQueryItem("mail", email);
    params.addQueryItem("password", password);
    params.addQueryItem("fa2code", fa2code);

    networkManager->post(request, params.query().toUtf8());
}

void AnilibriaApiService::signout(QString token)
{
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiService::apiAddress + "api/auth/signout?token=" + token ));

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(signoutResponse(QNetworkReply*)));

    networkManager->get(request);
}

void AnilibriaApiService::getUserData(QString token)
{
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiService::apiAddress + "api/auth/getuserdata?token=" + token ));

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(getUserDataResponse(QNetworkReply*)));

    networkManager->get(request);
}

void AnilibriaApiService::getFavorites(QString token)
{
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiService::apiAddress + "api/auth/getuserfavorites?token=" + token ));

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(getUserFavoritesResponse(QNetworkReply*)));

    networkManager->get(request);
}

void AnilibriaApiService::addMultiFavorites(QString token, QString ids)
{
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiService::apiAddress + "api/auth/addmultifavorites?token=" + token + "&ids=" + ids ));

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(editFavoritesResponse(QNetworkReply*)));

    networkManager->get(request);

}

void AnilibriaApiService::removeMultiFavorites(QString token, QString ids)
{
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(AnilibriaApiService::apiAddress + "api/auth/removemultifavorites?token=" + token + "&ids=" + ids ));

    connect(networkManager,SIGNAL(finished(QNetworkReply*)),this,SLOT(editFavoritesResponse(QNetworkReply*)));

    networkManager->get(request);
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

    emit signinReceived(reply->readAll());
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

    emit userFavoritesUpdated();
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
