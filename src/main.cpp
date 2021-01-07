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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebView>
#include <QtSvg>
#include <QQmlContext>
#include <QString>
#include "Classes/Services/synchronizationservice.h"
#include "Classes/Services/localstorageservice.h"
#include "Classes/Services/applicationsettings.h"
#include "Classes/Services/analyticsservice.h"
#include "Classes/Services/versionchecker.h"
#include "Classes/Services/downloadmanager.h"
#include "Classes/Services/apiserviceconfigurator.h"
#include "Classes/Services/proxyconfigurator.h"
#include "Classes/RemotePlayer/remoteplayer.h"
#include "globalconstants.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);

#ifdef Q_OS_WIN
    if (argc == 2 && QString(argv[1]) == "outputlog") {
        FreeConsole();
        AllocConsole();
        AttachConsole(GetCurrentProcessId());

        // reopen the std I/O streams to redirect I/O to the new console
        freopen("CON", "w", stdout);
        freopen("CON", "w", stderr);
        freopen("CON", "r", stdin);
    }
#endif

    QGuiApplication app(argc, argv);
    QtWebView::initialize();

    qmlRegisterType<SynchronizationService>("Anilibria.Services", 1, 0, "SynchronizationService");
    qmlRegisterType<LocalStorageService>("Anilibria.Services", 1, 0, "LocalStorage");
    qmlRegisterType<ApplicationSettings>("Anilibria.Services", 1, 0, "ApplicationSettings");    
    qmlRegisterType<AnalyticsService>("Anilibria.Services", 1, 0, "AnalyticsService");
    qmlRegisterType<VersionChecker>("Anilibria.Services", 1, 0, "VersionChecker");
    qmlRegisterType<DownloadManager>("Anilibria.Services", 1, 0, "DownloadManager");
    qmlRegisterType<ApiServiceConfigurator>("Anilibria.Services", 1, 0, "ApiServiceConfigurator");
    qmlRegisterType<ProxyConfigurator>("Anilibria.Services", 1, 0, "ProxyConfigurator");
    qmlRegisterType<RemotePlayer>("Anilibria.RemotePlayer", 1, 0, "RemotePlayer");

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty("ApplicationVersion", ApplicationVersion);

    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
    );
    engine.load(url);

    return app.exec();
}
