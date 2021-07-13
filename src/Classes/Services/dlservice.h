#ifndef DLSERVICE_H
#define DLSERVICE_H

#include <QObject>
#include <QNetworkReply>
#include "../Models/releasemodel.h"

class DLService : public QObject
{
    Q_OBJECT
private:
    QString m_basedUrl;
    int m_page;
    QList<QString>* m_releasesUrls;
    QList<ReleaseModel*>* m_loadedReleases;
    int m_detailsIndex;

public:
    explicit DLService(QObject *parent = nullptr);

    void synchronize();
    QList<ReleaseModel*>* getLoadedReleases() const { return m_loadedReleases; };

private:
    void downloadReleasesPage(int number);
    void downloadDetailsPage();
    QString removeTags(const QString& source, const QString &header) const;

signals:
    void downloadDetails();
    void allSynchronized();

private slots:
    void downloadedReleasePage(QNetworkReply* reply);
    void downloadedDetailsPage(QNetworkReply* reply);

};

#endif // DLSERVICE_H
