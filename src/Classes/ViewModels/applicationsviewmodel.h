#ifndef APPLICATIONSVIEWMODEL_H
#define APPLICATIONSVIEWMODEL_H

#include <QObject>
#include <QVariantList>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "../Models/externalapplicationmodel.h"

class ApplicationsViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList items READ items NOTIFY itemsChanged FINAL)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged FINAL)

private:
    const QString m_cacheFileName { "applications.cache" };
    QVariantList m_items { QVariantList() };
    QList<ExternalApplicationModel*> m_applications { QList<ExternalApplicationModel*>() };
    QNetworkAccessManager* m_networkManager { new QNetworkAccessManager(this) };
    ExternalApplicationModel* m_currentApplication { nullptr };
    bool m_loading { false };

public:
    explicit ApplicationsViewModel(QObject *parent = nullptr);

    QVariantList items() const noexcept { return m_items; }

    bool loading() const noexcept { return m_loading; }

    Q_INVOKABLE void refresh();
    Q_INVOKABLE void installByIndex(const QString& name, const QString& path);

private:
    void createApplications();
    void readCache();
    void writeCache();

private slots:
    void versionDownloaded(QNetworkReply* reply);

signals:
    void itemsChanged();
    void loadingChanged();

};

#endif // APPLICATIONSVIEWMODEL_H
