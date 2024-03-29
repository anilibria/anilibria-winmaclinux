#ifndef THEMEMANAGERSERVICE_H
#define THEMEMANAGERSERVICE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "../Models/themeitemmodel.h"

class ThemeManagerService : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(bool firstLoaded READ firstLoaded NOTIFY firstLoadedChanged)

private:
    QNetworkAccessManager m_networkManager { new QNetworkAccessManager(this) };
    const QString m_baseUrl { "https://raw.githubusercontent.com/anilibria/anilibria-winmaclinux/master/themes/" };
    bool m_loading { false };
    QList<ThemeItemModel*> m_themeItems { QList<ThemeItemModel*>() };
    bool m_firstLoaded { false };
    bool m_darkLoaded { false };
    ThemeItemModel* m_downloadedTheme { nullptr };

public:
    explicit ThemeManagerService(QObject *parent = nullptr);

    bool loading() const noexcept { return m_loading; }

    bool firstLoaded() const noexcept { return m_firstLoaded; }

    QList<ThemeItemModel*> getItems() noexcept;
    void downloadTheme(ThemeItemModel* model) noexcept;

    Q_INVOKABLE void loadThemes(bool isDark);

private slots:
    void loadedResult(QNetworkReply *reply);

signals:
    void loadingChanged();
    void themesLoaded();
    void firstLoadedChanged();
    void themeLoaded(const QString& theme, bool isDark, const ThemeItemModel* externalTheme);

};

#endif // THEMEMANAGERSERVICE_H
