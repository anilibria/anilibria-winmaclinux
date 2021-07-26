/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2020 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
