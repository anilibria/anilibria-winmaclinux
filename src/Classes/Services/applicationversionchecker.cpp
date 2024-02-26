#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "applicationversionchecker.h"


ApplicationVersionChecker::ApplicationVersionChecker(QObject *parent)
    : QObject{parent}
{
    connect(m_manager, &QNetworkAccessManager::finished, this, &ApplicationVersionChecker::downloaded);
}

void ApplicationVersionChecker::checkNewVersionAvailable(QString appIdentifier, QString repository, QString currentVersion)
{
    m_currentChecks.insert(appIdentifier, currentVersion);

    auto url = QUrl("https://api.github.com/repos/" + repository + "/releases/latest");
    QNetworkRequest request(url);

    auto query = m_manager->get(request);
    query->setProperty("identifier", QVariant(appIdentifier));
}

void ApplicationVersionChecker::downloaded(QNetworkReply *reply)
{
    auto identifier = reply->property("identifier").toString();
    if (reply->error() != QNetworkReply::NoError) {
        m_currentChecks.remove(identifier);
        emit noVersionAvailable();
        return;
    }

    auto currentVersion = m_currentChecks.value(identifier);
    m_currentChecks.remove(identifier);

    QByteArray data = reply->readAll();
    if (data.length() == 0) {
        emit noVersionAvailable();
        return;
    }

    QJsonParseError jsonError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data, &jsonError);
    if (jsonError.error != 0) {
        emit noVersionAvailable();
        return;
    }

    auto latestRelease = jsonDocument.object();

    if (latestRelease.contains("message")) {
        auto message = latestRelease.value("message").toString();
        if (message == "Not Found") {
            emit noVersionAvailable();
            return;
        }
    }

    auto version = latestRelease.value("tag_name").toString();
    auto url = latestRelease.value("html_url").toString();

    if (version == currentVersion) {
        emit noVersionAvailable();
        return;
    }

    emit newVersionAvailable(version, url, identifier);
}
