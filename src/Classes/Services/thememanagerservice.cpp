#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include "thememanagerservice.h"

ThemeManagerService::ThemeManagerService(QObject *parent)
    : QObject{parent}
{

}

QList<ThemeItemModel *> ThemeManagerService::getItems() noexcept
{
    return m_themeItems;
}

void ThemeManagerService::loadThemes(bool isDark)
{
    auto url = m_baseUrl + (isDark ? "darkthemes.json" : "lightthemes.json" );
    QNetworkRequest request(QUrl(url.toUtf8()));

    connect(&m_networkManager, &QNetworkAccessManager::finished, this, &ThemeManagerService::getThemes);

    m_networkManager.get(request);

}

void ThemeManagerService::getThemes(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::TimeoutError) return;
    if (reply->error() == QNetworkReply::ProtocolFailure) return;
    if (reply->error() == QNetworkReply::HostNotFoundError) return;

    QString data = reply->readAll();

    m_themeItems.clear();

    auto document = QJsonDocument::fromJson(data.toUtf8());
    auto items = document.array();
    foreach (auto item, items) {
        auto model = new ThemeItemModel();
        model->readFromJson(item.toObject());
        m_themeItems.append(model);
    }

    m_loading = false;
    emit loadingChanged();
}
