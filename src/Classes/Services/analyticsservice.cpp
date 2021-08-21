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

    QString returnData;
    returnData.append("v=1&");
    returnData.append("tid=UA-152944623-1&");
    auto uniqueName = QSysInfo::machineUniqueId();//TODO: check if null and replace on something
    returnData.append("cid=" + uniqueName + "&");
    returnData.append("t=" + type + "&");
    returnData.append("an=AniLibriaDesktop&");
    returnData.append("av=" + ApplicationVersion + "&");
    if (!page.isEmpty()) returnData.append("dp=" + page + "&");
    returnData.append("ec=" + category + "&");
    returnData.append("ea=" + message);

    networkManager->post(request, returnData.toUtf8());
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
