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

#ifndef SYNCHRONIZATIONSERVICE_H
#define SYNCHRONIZATIONSERVICE_H

#include <QQmlListProperty>
#include <QObject>
#include "anilibriaapiservice.h"
#include "dlservice.h"
#include "../Models/releasemodel.h"

class SynchronizationService : public QObject
{
    Q_OBJECT

private:
    AnilibriaApiService* m_AnilibriaApiService;
    DLService* m_dlService;
public:    
    explicit SynchronizationService(QObject *parent = nullptr);

    Q_INVOKABLE void synchronizeReleases();
    Q_INVOKABLE void synchronizeSchedule();
    Q_INVOKABLE void authorize(QString email, QString password, QString fa2code);
    Q_INVOKABLE void signout(QString token);
    Q_INVOKABLE void getUserData(QString token);
    Q_INVOKABLE void synchronizeUserFavorites(QString token);
    Q_INVOKABLE void addUserFavorites(QString token, QString ids);
    Q_INVOKABLE void removeUserFavorites(QString token, QString ids);
    Q_INVOKABLE QString combineWithWebSiteUrl(QString path);
    Q_INVOKABLE void downloadTorrent(QString torrentPath);
    Q_INVOKABLE void synchronizeYoutube();
    Q_INVOKABLE void synchronizeDL();
signals:
    void synchronizedReleases(QString data);
    void synchronizationCompleted();
    void synchronizationFailed();
    void synchronizedSchedule(QString data);
    void synchronizedYoutube(QString data);
    void userCompleteAuthentificated(QString token);
    void userFailedAuthentificated(QString errorMessage);
    void userSignouted();
    void userDataReceived(QString data);
    void userFavoritesReceived(QString data);
    void userFavoritesEdited();
    void torrentDownloaded(QString torrentPath);
    void synchronizedFromDL(QString data);

public slots:
    void saveReleasesToCache(QString data);
    void saveReleasesFromDLToCache();
    void saveScheduleToCache(QString data);
    void saveYoutubeToCache(QString data);
    void handleSignout();
    void handleUserData(QString data);
    void handleUserFavorites(QString data);
    void handleEditUserFavorites();
    void handleTorrentDownloaded(QString torrentPath);
};

#endif // SYNCHRONIZATIONSERVICE_H
