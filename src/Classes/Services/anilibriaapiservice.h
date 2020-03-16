#ifndef ANILIBRIAAPISERVICE_H
#define ANILIBRIAAPISERVICE_H

#include <QObject>
#include <QtNetwork>

class AnilibriaApiService : public QObject
{
    Q_OBJECT

public:
    explicit AnilibriaApiService(QObject *parent = nullptr);

    const static QString apiAddress;
    const static QString newApiAddress;

    void getAllReleases();
    void getYoutubeVideos();
    void getSchedule();
    void signin(QString email, QString password, QString fa2code);
    void signout(QString token);
    void getUserData(QString token);
    void getFavorites(QString token);
    void addMultiFavorites(QString token, QString ids);
    void removeMultiFavorites(QString token, QString ids);
    void downloadTorrent(QString path);

signals:
    void allReleasesReceived(QString data);
    void allYoutubeItemReceived(QString data);
    void scheduleReceived(QString data);
    void signinReceived(QString data);
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
    void downloadTorrentResponse(QNetworkReply* reply);
};

#endif // ANILIBRIAAPISERVICE_H
