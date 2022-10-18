#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonArray>
#include "thememanagerservice.h"

ThemeManagerService::ThemeManagerService(QObject *parent)
    : QObject{parent}
{
    connect(&m_networkManager, &QNetworkAccessManager::finished, this, &ThemeManagerService::loadedResult);
}

QList<ThemeItemModel *> ThemeManagerService::getItems() noexcept
{
    return m_themeItems;
}

void ThemeManagerService::downloadTheme(ThemeItemModel* model) noexcept
{
    auto url = m_baseUrl + (m_darkLoaded ? "dark/" : "light/" ) + model->source() + ".theme";
    QNetworkRequest request(QUrl(url.toUtf8()));

    m_networkManager.get(request);
    m_downloadedTheme = model;
}

void ThemeManagerService::loadThemes(bool isDark)
{
    auto url = m_baseUrl + (isDark ? "darkthemes.json" : "lightthemes.json" );
    QNetworkRequest request(QUrl(url.toUtf8()));

    m_networkManager.get(request);
    m_darkLoaded = isDark;
}

void ThemeManagerService::loadedResult(QNetworkReply *reply)
{
    if (reply->request().url().toString().contains(".json")) {
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
        m_firstLoaded = true;
        emit loadingChanged();
        emit themesLoaded();
        emit firstLoadedChanged();
    } else {
        QString data = reply->readAll();

        emit themeLoaded(data, m_darkLoaded, m_downloadedTheme);

    }
}
