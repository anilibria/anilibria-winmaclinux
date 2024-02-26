#ifndef APPLICATIONSVIEWMODEL_H
#define APPLICATIONSVIEWMODEL_H

#include <QObject>
#include <QVariantList>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include "../Models/externalapplicationmodel.h"
#include "../Services/applicationversionchecker.h"

class ApplicationsViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantList items READ items NOTIFY itemsChanged FINAL)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged FINAL)
    Q_PROPERTY(QString installPath READ installPath WRITE setInstallPath NOTIFY installPathChanged FINAL)
    Q_PROPERTY(QString installIndex READ installIndex WRITE setInstallIndex NOTIFY installIndexChanged FINAL)
    Q_PROPERTY(QString deleteIndex READ deleteIndex WRITE setDeleteIndex NOTIFY deleteIndexChanged FINAL)

private:
    const QString m_cacheFileName { "applications.cache" };
    QVariantList m_items { QVariantList() };
    QList<ExternalApplicationModel*> m_applications { QList<ExternalApplicationModel*>() };
    QNetworkAccessManager* m_networkManager { new QNetworkAccessManager(this) };
    ApplicationVersionChecker* m_versionChecker { new ApplicationVersionChecker(this) };
    ExternalApplicationModel* m_currentApplication { nullptr };
    bool m_loading { false };
    QString m_installPath { "" };
    QString m_installIndex { -1 };
    QString m_deleteIndex { -1 };

public:
    explicit ApplicationsViewModel(QObject *parent = nullptr);

    QVariantList items() const noexcept { return m_items; }

    bool loading() const noexcept { return m_loading; }

    QString installPath() const noexcept { return m_installPath; }
    void setInstallPath(const QString& installPath) noexcept;

    QString installIndex() const noexcept { return m_installIndex; }
    void setInstallIndex(const QString& installIndex) noexcept;

    QString deleteIndex() const noexcept { return m_deleteIndex; }
    void setDeleteIndex(const QString& deleteIndex) noexcept;

    Q_INVOKABLE void refresh();
    Q_INVOKABLE void installByIndex();
    Q_INVOKABLE void checkNewVersions();
    Q_INVOKABLE void clearInstallData();
    Q_INVOKABLE void deleteByIndex();

private:
    void createApplications();
    void readCache();
    void writeCache();

private slots:
    void versionDownloaded(QNetworkReply* reply);
    void newVersionAvailable(QString version, QString url, QString appIdentifier);
    void noVersionAvailable();

signals:
    void itemsChanged();
    void loadingChanged();
    void installPathChanged();
    void installIndexChanged();
    void deleteIndexChanged();

};

#endif // APPLICATIONSVIEWMODEL_H
