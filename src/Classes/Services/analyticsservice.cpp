#include "analyticsservice.h"

const QString AnalyticsService::googleAnalyticsAddress = "https://www.google-analytics.com/collect";

void AnalyticsService::sendPostEvent(QString type, QString category, QString message, QString page)
{
    QString userAgent = "Mozilla/5.0 ";

#ifdef Q_OS_WIN
    userAgent += "(Windows NT 10.0; Win64; x64)";
#endif

#ifdef Q_OS_MACOS
    userAgent += "(Macintosh; Intel Mac OS X 10_10_5)";
#endif

#ifdef Q_OS_LINUX
    userAgent += "(X11; Linux x86_64)";
#endif

#ifdef QT_DEBUG
    if (type.isEmpty() || category.isEmpty() || message.isEmpty() || page.isEmpty()) {

    }
#else
    auto networkManager = new QNetworkAccessManager(this);
    auto url = QUrl(AnalyticsService::googleAnalyticsAddress);
    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", userAgent.toUtf8());
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
#endif
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
