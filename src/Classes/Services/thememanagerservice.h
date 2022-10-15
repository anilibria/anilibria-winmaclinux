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

private:
    QNetworkAccessManager m_networkManager { new QNetworkAccessManager(this) };
    const QString m_baseUrl { "https://raw.githubusercontent.com/anilibria/anilibria-winmaclinux/master/themes/" };
    bool m_loading { false };
    QList<ThemeItemModel*> m_themeItems { QList<ThemeItemModel*>() };

public:
    explicit ThemeManagerService(QObject *parent = nullptr);

    bool loading() const noexcept { return m_loading; }


    QList<ThemeItemModel*> getItems() noexcept;

    Q_INVOKABLE void loadThemes(bool isDark);

private slots:
    void getThemes(QNetworkReply *reply);

signals:
    void loadingChanged();

};

#endif // THEMEMANAGERSERVICE_H
