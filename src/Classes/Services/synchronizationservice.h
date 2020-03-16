#ifndef SYNCHRONIZATIONSERVICE_H
#define SYNCHRONIZATIONSERVICE_H

#include <QQmlListProperty>
#include <QObject>
#include "anilibriaapiservice.h"
#include "../Models/releasemodel.h"

class SynchronizationService : public QObject
{
    Q_OBJECT

private:
    AnilibriaApiService* m_AnilibriaApiService;
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

public slots:
    void saveReleasesToCache(QString data);
    void saveScheduleToCache(QString data);
    void saveYoutubeToCache(QString data);
    void handleSignin(QString data);
    void handleSignout();
    void handleUserData(QString data);
    void handleUserFavorites(QString data);
    void handleEditUserFavorites();
    void handleTorrentDownloaded(QString torrentPath);
};

#endif // SYNCHRONIZATIONSERVICE_H
