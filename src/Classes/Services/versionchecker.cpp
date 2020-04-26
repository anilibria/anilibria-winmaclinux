#include "versionchecker.h"

VersionChecker::VersionChecker(QObject *parent) : QObject(parent)
{
#ifdef USE_VERSION_CHECK
    auto networkManager = new QNetworkAccessManager(this);
    auto url = QUrl("https://api.github.com/repos/anilibria/anilibria-winmaclinux/releases/latest");
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Anilibria CP Client");

    connect(networkManager,&QNetworkAccessManager::finished,this,&VersionChecker::latestDownloaded);

    networkManager->get(request);
#endif
}

void VersionChecker::latestDownloaded(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    QByteArray data = reply->readAll();

    if (data.length() == 0) return;

    QJsonParseError jsonError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data, &jsonError);
    if (jsonError.error != 0) return;

    auto latestRelease = jsonDocument.object();

    if (latestRelease.contains("message")) {
        auto message = latestRelease.value("message").toString();
        if (message == "Not Found") return;
    }

    auto version = latestRelease.value("tag_name").toString();
    auto url = latestRelease.value("html_url").toString();

    if (version == ApplicationVersion) return;

    emit newVersionAvailable(version, url);
}
