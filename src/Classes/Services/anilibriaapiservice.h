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

#ifndef ANILIBRIAAPISERVICE_H
#define ANILIBRIAAPISERVICE_H

#include <QObject>
#include <QtNetwork>
#include <QSet>
#include "globalconstants.h"

class AnilibriaApiService : public QObject
{
    Q_OBJECT

public:
    explicit AnilibriaApiService(QObject *parent = nullptr);

    QQueue<int>* m_QueuedAddedFavorites;
    QQueue<int>* m_QueuedDeletedFavorites;
    QString m_FavoriteToken;
    QList<QString> m_pages { QList<QString>() };
    QSet<int> m_pageCounter { QSet<int>() };
    QMutex* m_mutex { new QMutex() };

    void getAllReleases(const int countPages, const int perPage);
    void getYoutubeVideos();
    void getSchedule();
    void signin(QString email, QString password, QString fa2code);
    void signout(QString token);
    void getUserData(QString token);
    void getFavorites(QString token);
    void addMultiFavorites(QString token, QString ids);
    void performAddFavorite(QString token, int id);
    void performRemoveFavorite(QString token, int id);
    void removeMultiFavorites(QString token, QString ids);
    QList<QString> getPages() const noexcept { return m_pages; };

signals:
    void allReleasesReceived();
    void allYoutubeItemReceived(QString data);
    void scheduleReceived(QString data);
    void signinReceived(QString token, QString payload);
    void signoutReceived();
    void userDataReceived(QString data);
    void userFavoritesReceived(QString data);
    void userFavoritesUpdated();
    void torrentDownloaded(QString path);

public slots:
    void getAllReleasesResponse(QNetworkReply* reply);
    void getAllYoutubeItemsResponse(QNetworkReply* reply);
    void getScheduleResponse(QNetworkReply* reply);
    void signinResponse(QNetworkReply* reply);
    void signoutResponse(QNetworkReply* reply);
    void getUserDataResponse(QNetworkReply* reply);
    void getUserFavoritesResponse(QNetworkReply* reply);
    void editFavoritesResponse(QNetworkReply* reply);
    void deleteFavoritesResponse(QNetworkReply* reply);
    void downloadTorrentResponse(QNetworkReply* reply);
};

#endif // ANILIBRIAAPISERVICE_H
