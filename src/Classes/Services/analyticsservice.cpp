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

#include <QLocale>
#include <QGuiApplication>
#include <QScreen>
#include "analyticsservice.h"
#include "../../globalconstants.h"

const QString AnalyticsService::googleAnalyticsAddress = "https://www.google-analytics.com/g/collect?v=2&tid=G-DRSN62ELJ5&gtm=2oe460&_p=538594933&_z=ccd.IAB&cid=5573181.376415052&sct=1&seg=1";

AnalyticsService::AnalyticsService(QObject *parent) : QObject(parent)
{

}

void AnalyticsService::sendView(QString category, QString message, QString page)
{
    sendPostEvent(category, message, page);
}

void AnalyticsService::sendVersion()
{
    auto versionNumber = QString(ApplicationVersion);
    sendPostEvent("version", versionNumber.replace(".", "_"), "%2Fcommon");
}

void AnalyticsService::sendPostEvent(QString category, QString message, QString page)
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
    if (category.isEmpty() || message.isEmpty() || page.isEmpty()) {

    }
#else

    auto networkManager = new QNetworkAccessManager(this);

    auto language = "&ul=" + QLocale::system().name().toLower();

    QScreen *screen = QGuiApplication::primaryScreen();
    auto screenGeometry = screen->geometry();
    auto screenResolution = "&sr=" + QString::number(screenGeometry.width()) + "x" + QString::number(screenGeometry.height());

    auto fullPath = QString(page);
    auto cutPath = page.replace("%2F", "");

    auto eventName = "&en=" + category.toLower() + "_" + message.toLower();

    auto pageName = "&dl=http%3A%2F%2Fanilibria.tv" + fullPath;

    m_sessionCounter++;
    auto sessionCounter = "&_s=" + QString::number(m_sessionCounter);

    auto documentTitle = "&dt=Page%20-%20" + cutPath;

    auto sessionStart = "&sid=" + QString::number(m_startTime.toTime_t());

    auto url = QUrl(AnalyticsService::googleAnalyticsAddress + language + screenResolution + eventName + pageName+ sessionCounter + documentTitle + sessionStart);

    QNetworkRequest request(url);
    request.setRawHeader("User-Agent", userAgent.toUtf8());
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("plain/text;charset=UTF-8"));

    networkManager->post(request, eventName.toUtf8());
#endif
}
