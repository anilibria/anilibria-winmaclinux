#include "analyticsservice.h"

const QString AnalyticsService::googleAnalyticsAddress = "https://www.google-analytics.com/collect";

void AnalyticsService::sendPostEvent(QString type, QString category, QString message, QString page)
{
    auto networkManager = new QNetworkAccessManager(this);
    auto url = QUrl(AnalyticsService::googleAnalyticsAddress);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", "Anilibria CP Client");
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("plain/text"));

    QByteArray postData;
    postData.append("v=1&");
    postData.append("tid=UA-152944623-1&");
    auto uniqueName = QSysInfo::machineUniqueId();//TODO: check if null and replace on something
    postData.append("cid=" + uniqueName + "&");
    postData.append("t=" + type + "&");
    postData.append("an=AniLibriaDesktop&");
    postData.append("av=" + ApplicationVersion + "&");
    if (!page.isEmpty()) postData.append("dp=" + page + "&");
    postData.append("ec=" + category + "&");
    postData.append("ea=" + message);


    networkManager->post(request, postData);
}

AnalyticsService::AnalyticsService(QObject *parent) : QObject(parent)
{

}

void AnalyticsService::sendEvent(QString category, QString message)
{
    sendPostEvent("event", category, message, "");
}

void AnalyticsService::sendView(QString category, QString message, QString page)
{
    sendPostEvent("pageview", category, message, page);
}
