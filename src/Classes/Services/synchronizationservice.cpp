#include <QtCore>
#include <QQmlListProperty>
#include "synchronizationservice.h"
#include "anilibriaapiservice.h"
#include "../Models/releasemodel.h"
#include "../../globalconstants.h"

SynchronizationService::SynchronizationService(QObject *parent) : QObject(parent)
{
    m_AnilibriaApiService = new AnilibriaApiService(this);
    connect(m_AnilibriaApiService,&AnilibriaApiService::allReleasesReceived,this,&SynchronizationService::saveReleasesToCache);
    connect(m_AnilibriaApiService,&AnilibriaApiService::scheduleReceived,this,&SynchronizationService::saveScheduleToCache);
    connect(m_AnilibriaApiService,&AnilibriaApiService::signinReceived,this,&SynchronizationService::handleSignin);
    connect(m_AnilibriaApiService,&AnilibriaApiService::signoutReceived,this,&SynchronizationService::handleSignout);
    connect(m_AnilibriaApiService,&AnilibriaApiService::userDataReceived,this,&SynchronizationService::handleUserData);
    connect(m_AnilibriaApiService,&AnilibriaApiService::userFavoritesReceived,this,&SynchronizationService::handleUserFavorites);
    connect(m_AnilibriaApiService,&AnilibriaApiService::userFavoritesUpdated,this,&SynchronizationService::handleEditUserFavorites);
    connect(m_AnilibriaApiService,&AnilibriaApiService::torrentDownloaded,this,&SynchronizationService::handleTorrentDownloaded);
    connect(m_AnilibriaApiService,&AnilibriaApiService::allYoutubeItemReceived,this,&SynchronizationService::saveYoutubeToCache);
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

void SynchronizationService::saveReleasesToCache(QString data)
{
    emit synchronizedReleases(data);
}

void SynchronizationService::saveScheduleToCache(QString data)
{
    emit synchronizedSchedule(data);
}

void SynchronizationService::saveYoutubeToCache(QString data)
{
    emit synchronizedYoutube(data);
}

void SynchronizationService::handleSignin(QString data)
{
    QJsonParseError jsonError;
    QJsonDocument jsonDocument = QJsonDocument::fromJson(data.toUtf8(), &jsonError);
    auto object = jsonDocument.object();
    auto token = object.value("token").toString();
    auto errorMessage = object.value("errorMessage").toString();
    if (errorMessage.isEmpty()) {
        emit userCompleteAuthentificated(token);
    } else {
        emit userFailedAuthentificated(errorMessage);
    }
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

    if (object.contains("message")) {
        auto errorMessage = object.value("message").toString();
    } else {
        auto avatar = object.value("avatar").toString();
        object["avatar"] = AnilibriaApiService::apiAddress + avatar;
        QJsonDocument resultJson(object);
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
