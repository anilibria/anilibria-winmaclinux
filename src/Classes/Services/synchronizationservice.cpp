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

#include <QtCore>
#include <QQmlListProperty>
#include <QJsonDocument>
#include "synchronizationservice.h"
#include "anilibriaapiservice.h"
#include "../Models/releasemodel.h"
#include "../../globalconstants.h"

SynchronizationService::SynchronizationService(QObject *parent) : QObject(parent),
    m_AnilibriaApiService(new AnilibriaApiService(this)),
    m_dlService(new DLService(this))
{

    connect(m_AnilibriaApiService,&AnilibriaApiService::allReleasesReceived,this,&SynchronizationService::saveReleasesToCache);
    connect(m_AnilibriaApiService,&AnilibriaApiService::scheduleReceived,this,&SynchronizationService::saveScheduleToCache);
    connect(m_AnilibriaApiService,&AnilibriaApiService::signoutReceived,this,&SynchronizationService::handleSignout);
    connect(m_AnilibriaApiService,&AnilibriaApiService::userDataReceived,this,&SynchronizationService::handleUserData);
    connect(m_AnilibriaApiService,&AnilibriaApiService::userFavoritesReceived,this,&SynchronizationService::handleUserFavorites);
    connect(m_AnilibriaApiService,&AnilibriaApiService::userFavoritesUpdated,this,&SynchronizationService::handleEditUserFavorites);
    connect(m_AnilibriaApiService,&AnilibriaApiService::torrentDownloaded,this,&SynchronizationService::handleTorrentDownloaded);
    connect(m_AnilibriaApiService,&AnilibriaApiService::allYoutubeItemReceived,this,&SynchronizationService::saveYoutubeToCache);
    connect(m_dlService, &DLService::allSynchronized, this, &SynchronizationService::saveReleasesFromDLToCache);
}

void SynchronizationService::synchronizeReleases()
{
    m_AnilibriaApiService->getAllReleases();
}

void SynchronizationService::synchronizeSchedule()
{
    m_AnilibriaApiService->getSchedule();
}

void SynchronizationService::authorize(QString email, QString password, QString fa2code)
{
    m_AnilibriaApiService->signin(email, password, fa2code);
}

void SynchronizationService::signout(QString token)
{
    m_AnilibriaApiService->signout(token);
}

void SynchronizationService::getUserData(QString token)
{
    m_AnilibriaApiService->getUserData(token);
}

void SynchronizationService::synchronizeUserFavorites(QString token)
{
    m_AnilibriaApiService->getFavorites(token);
}

void SynchronizationService::addUserFavorites(QString token, QString ids)
{
    m_AnilibriaApiService->addMultiFavorites(token,ids);
}

void SynchronizationService::removeUserFavorites(QString token, QString ids)
{
    m_AnilibriaApiService->removeMultiFavorites(token,ids);
}

QString SynchronizationService::combineWithWebSiteUrl(QString path)
{
    return AnilibriaImagesPath + path;
}

void SynchronizationService::downloadTorrent(QString torrentPath)
{
    m_AnilibriaApiService->downloadTorrent(torrentPath);
}

void SynchronizationService::synchronizeYoutube()
{
    m_AnilibriaApiService->getYoutubeVideos();
}

void SynchronizationService::synchronizeDL()
{
    m_dlService->synchronize();
}

void SynchronizationService::saveReleasesToCache(QString data)
{
    emit synchronizedReleases(data);
}

void SynchronizationService::saveReleasesFromDLToCache()
{
    auto loadedReleases = m_dlService->getLoadedReleases();
    if (loadedReleases->count() == 0) return;

    QJsonArray releases;

    foreach(auto loadedRelease, *loadedReleases) {
        QJsonObject releaseObject;
        loadedRelease->writeToApiModel(releaseObject);
        releases.append(releaseObject);
    }

    QJsonDocument document(releases);

    emit synchronizedFromDL(document.toJson());
}

void SynchronizationService::saveScheduleToCache(QString data)
{
    emit synchronizedSchedule(data);
}

void SynchronizationService::saveYoutubeToCache(QString data)
{
    emit synchronizedYoutube(data);
}

void SynchronizationService::handleSignout()
{
    emit userSignouted();
}

void SynchronizationService::handleUserData(QString data)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8(), &jsonError);
    auto object = jsonDocument.object();

    if (object.contains("status") && object.value("status").toBool()) {
        auto dataObject = object.value("data").toObject();
        auto avatar = dataObject.value("avatar").toString();
        dataObject["avatar"] = AnilibriaImagesPath + avatar;
        QJsonDocument resultJson(dataObject);
        emit userDataReceived(resultJson.toJson(QJsonDocument::Compact));
    }
}

void SynchronizationService::handleUserFavorites(QString data)
{
    emit userFavoritesReceived(data);
}

void SynchronizationService::handleEditUserFavorites()
{
    emit userFavoritesEdited();
}

void SynchronizationService::handleTorrentDownloaded(QString torrentPath)
{
    emit torrentDownloaded(torrentPath);
}
