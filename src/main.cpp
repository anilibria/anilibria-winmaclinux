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
#include <QtSvg>
#include <QQmlContext>
#include <QString>
#include <QQuickStyle>
#include <QQmlFileSelector>
#include <QQuickWindow>
#include "Classes/Services/localstorageservice.h"
#include "Classes/Services/applicationsettings.h"
#include "Classes/Services/analyticsservice.h"
#include "Classes/Services/versionchecker.h"
#include "Classes/Services/downloadmanager.h"
#include "Classes/Services/apiserviceconfigurator.h"
#include "Classes/Services/proxyconfigurator.h"
#include "Classes/RemotePlayer/remoteplayer.h"
#include "Classes/ListModels/alphabetlistmodel.h"
#include "globalconstants.h"
#include "Classes/Services/releaselinkedseries.h"
#include "Classes/ViewModels/onlineplayerviewmodel.h"
#include "Classes/ViewModels/youtubeviewmodel.h"
#include "Classes/ViewModels/authorizationviewmodel.h"
#include "Classes/ViewModels/notificationviewmodel.h"
#include "Classes/ViewModels/onlineplayerwindowviewmodel.h"
#include "Classes/ViewModels/releasesviewmodel.h"
#include "Classes/ViewModels/useractivityviewmodel.h"
#include "Classes/Services/osextras.h"
#include "Classes/ViewModels/userconfigurationviewmodel.h"
#include "Classes/ViewModels/imagebackgroundviewmodel.h"
#include "Classes/ListModels/commoncomboboxlistmodel.h"
#include "Classes/ListModels/mainmenulistmodel.h"
#include "Classes/ViewModels/mainviewmodel.h"
#include "Classes/ListModels/releaseslistmodel.h"
#include "Classes/ListModels/cinemahalllistmodel.h"
#include "Classes/ListModels/releasetorrentcommonlist.h"
#include "Classes/ViewModels/myanilibriaviewmodel.h"
#include "Classes/ListModels/releasesimplelistmodel.h"
#include "Classes/ListModels/myanilibrialistmodel.h"
#include "Classes/ListModels/releaseserieslistmodel.h"
#include "Classes/ListModels/releasecardmenulistmodel.h"
#include "Classes/ListModels/releaseonlineserieslistmodel.h"
#include "Classes/ViewModels/applicationthemeviewmodel.h"
#include "Classes/ListModels/themefieldlistmodel.h"
#include "Classes/ListModels/localthemeslistmodel.h"
#include "Classes/ListModels/myanilibriasearchlistmodel.h"
#include "Classes/ViewModels/torrentnotifierviewmodel.h"
#include "Classes/ViewModels/globaleventtrackerviewmodel.h"
#include "Classes/ListModels/releaseserieslistmodel.h"
#include "Classes/ViewModels/externalplayerviewmodel.h"
#include "Classes/ViewModels/filterdictionariesviewmodel.h"
#include "Classes/ViewModels/releasecustomgroupsviewmodel.h"
#include "Classes/ViewModels/applicationsviewmodel.h"
#include "Classes/ViewModels/extensionsviewmodel.h"
#include "Classes/Services/synchronizev2service.h"
#include "Classes/ViewModels/synchronizationhub.h"
#include "Classes/customstyle.h"
#ifdef USE_VLC_PLAYER
#include "vlc-qt/qml/VlcQmlPlayer.h"
#include "vlc-qt/qml/VlcQmlVideoOutput.h"
#include "vlc-qt/core/TrackModel.h"
#include "vlc-qt/core/Common.h"
#endif
#ifdef USE_MPV_PLAYER
#include "PlayerMpv/mpvobject.h"
#endif
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#include <QQuickWindow>
#endif

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    Q_UNUSED(context);

    QString txt;
    switch (type) {
        case QtDebugMsg:
        case QtInfoMsg:
            txt = QString("Common: %1").arg(msg);
            break;
        case QtWarningMsg:
            txt = QString("Warning: %1").arg(msg);
            break;
        case QtCriticalMsg:
            txt = QString("Critical: %1").arg(msg);
            break;
        case QtFatalMsg:
            txt = QString("Fatal: %1").arg(msg);
            abort();
    }

    QFile outFile("qt.log");
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << txt << Qt::endl;
}

int main(int argc, char *argv[])
{
    auto isNeedLogging = argc == 2 && QString(argv[1]) == "outputlog";
    if (isNeedLogging) qInstallMessageHandler(myMessageOutput);

    qputenv("QML_DISABLE_DISK_CACHE", "1");

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
#else
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_ShareOpenGLContexts);
#endif

#ifdef Q_OS_WIN
    if (isNeedLogging) freopen("output.log", "w", stdout); //redirect output to file
#endif

    if (argc >= 2) {
        auto parameter = QString(argv[1]);
        if (parameter == "portable" || parameter == "-portable") {
            IsPortable = true;
        }
    }

    QGuiApplication app(argc, argv);

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    QQuickStyle::setStyle("aniliberty");
#else
    QQuickStyle::setStyle("CustomStyle");
#endif
    QQuickStyle::setFallbackStyle("Material");

    qmlRegisterType<LocalStorageService>("Anilibria.Services", 1, 0, "LocalStorage");
    qmlRegisterType<ApplicationSettings>("Anilibria.Services", 1, 0, "ApplicationSettings");    
    qmlRegisterType<AnalyticsService>("Anilibria.Services", 1, 0, "AnalyticsService");
    qmlRegisterType<VersionChecker>("Anilibria.Services", 1, 0, "VersionChecker");
    qmlRegisterType<DownloadManager>("Anilibria.Services", 1, 0, "DownloadManager");
    qmlRegisterType<ApiServiceConfigurator>("Anilibria.Services", 1, 0, "ApiServiceConfigurator");
    qmlRegisterType<ProxyConfigurator>("Anilibria.Services", 1, 0, "ProxyConfigurator");
    qmlRegisterType<RemotePlayer>("Anilibria.RemotePlayer", 1, 0, "RemotePlayer");
    qmlRegisterType<ReleaseLinkedSeries>("Anilibria.Services", 1, 0, "ReleaseLinkedSeries");
    qmlRegisterType<OsExtras>("Anilibria.Services", 1, 0, "OsExtras");
    qmlRegisterType<AlphabetListModel>("Anilibria.ListModels", 1, 0, "AlphabetListModel");
    qmlRegisterType<OnlinePlayerViewModel>("Anilibria.ViewModels", 1, 0, "OnlinePlayerViewModel");
    qmlRegisterType<YoutubeViewModel>("Anilibria.ViewModels", 1, 0, "YoutubeViewModel");
    qmlRegisterType<AuthorizationViewModel>("Anilibria.ViewModels", 1, 0, "AuthorizationViewModel");
    qmlRegisterType<NotificationViewModel>("Anilibria.ViewModels", 1, 0, "NotificationViewModel");
    qmlRegisterType<OnlinePlayerWindowViewModel>("Anilibria.ViewModels", 1, 0, "OnlinePlayerWindowViewModel");
    qmlRegisterType<ReleasesViewModel>("Anilibria.ViewModels", 1, 0, "ReleasesViewModel");
    qmlRegisterType<UserActivityViewModel>("Anilibria.ViewModels", 1, 0, "UserActivityViewModel");
    qmlRegisterType<UserConfigurationViewModel>("Anilibria.ViewModels", 1, 0, "UserConfigurationViewModel");
    qmlRegisterType<ImageBackgroundViewModel>("Anilibria.ViewModels", 1, 0, "ImageBackgroundViewModel");
    qmlRegisterType<CommonComboBoxListModel>("Anilibria.ListModels", 1, 0, "CommonComboBoxListModel");
    qmlRegisterType<MainViewModel>("Anilibria.ViewModels", 1, 0, "MainViewModel");
    qmlRegisterType<MainMenuListModel>("Anilibria.ListModels", 1, 0, "MainMenuListModel");
    qmlRegisterType<ReleasesListModel>("Anilibria.ListModels", 1, 0, "ReleasesListModel");
    qmlRegisterType<CinemahallListModel>("Anilibria.ListModels", 1, 0, "CinemahallListModel");
    qmlRegisterType<ReleaseTorrentCommonList>("Anilibria.ListModels", 1, 0, "ReleaseTorrentCommonList");
    qmlRegisterType<MyAnilibriaViewModel>("Anilibria.ViewModels", 1, 0, "MyAnilibriaViewModel");
    qmlRegisterType<ReleaseSimpleListModel>("Anilibria.ListModels", 1, 0, "ReleaseSimpleListModel");
    qmlRegisterType<MyAnilibriaListModel>("Anilibria.ListModels", 1, 0, "MyAnilibriaListModel");
    qmlRegisterType<ReleaseSeriesListModel>("Anilibria.ListModels", 1, 0, "ReleaseSeriesListModel");
    qmlRegisterType<ReleaseCardMenuListModel>("Anilibria.ListModels", 1, 0, "ReleaseCardMenuListModel");
    qmlRegisterType<ReleaseOnlineSeriesListModel>("Anilibria.ListModels", 1, 0, "ReleaseOnlineSeriesListModel");
    qmlRegisterType<ApplicationThemeViewModel>("Anilibria.ViewModels", 1, 0, "ApplicationThemeViewModel");
    qmlRegisterType<ThemeFieldListModel>("Anilibria.ListModels", 1, 0, "ThemeFieldListModel");
    qmlRegisterType<LocalThemesListModel>("Anilibria.ListModels", 1, 0, "LocalThemesListModel");
    qmlRegisterType<MyAnilibriaSearchListModel>("Anilibria.ListModels", 1, 0, "MyAnilibriaSearchListModel");
    qmlRegisterType<TorrentNotifierViewModel>("Anilibria.ViewModels", 1, 0, "TorrentNotifierViewModel");
    qmlRegisterType<GlobalEventTrackerViewModel>("Anilibria.ViewModels", 1, 0 , "GlobalEventTrackerViewModel");
    qmlRegisterType<ReleaseSeriesListModel>("Anilibria.ListModels", 1, 0 , "ReleaseSeriesListModel");
    qmlRegisterType<ExternalPlayerViewModel>("Anilibria.ViewModels", 1, 0 , "ExternalPlayerViewModel");
    qmlRegisterType<FilterDictionariesViewModel>("Anilibria.ViewModels", 1, 0 , "FilterDictionariesViewModel");
    qmlRegisterType<ReleaseCustomGroupsViewModel>("Anilibria.ViewModels", 1, 0 , "ReleaseCustomGroupsViewModel");
    qmlRegisterType<ApplicationsViewModel>("Anilibria.ViewModels", 1, 0 , "ApplicationsViewModel");
    qmlRegisterType<ExtensionsViewModel>("Anilibria.ViewModels", 1, 0 , "ExtensionsViewModel");
    qmlRegisterType<Synchronizev2Service>("Anilibria.Services", 1, 0 , "Synchronizev2Service");
    qmlRegisterType<SynchronizationHub>("Anilibria.Services", 1, 0 , "SynchronizationHub");

    qmlRegisterUncreatableType<CustomStyle>("CustomStyle", 1, 0, "CustomStyle", "CustomStyle is an attached property");

#ifdef USE_VLC_PLAYER
    VlcCommon::setPluginPath(app.applicationDirPath() + "/plugins");

    qmlRegisterUncreatableType<Vlc>("VLCQt", 1, 1, "Vlc", QStringLiteral("Vlc cannot be instantiated directly"));
    qmlRegisterUncreatableType<VlcQmlSource>("VLCQt", 1, 1, "VlcSource", QStringLiteral("VlcQmlSource cannot be instantiated directly"));
    qmlRegisterUncreatableType<VlcTrackModel>("VLCQt", 1, 1, "VlcTrackModel", QStringLiteral("VlcTrackModel cannot be instantiated directly"));

    qmlRegisterType<VlcQmlPlayer>("VLCQt", 1, 1, "VlcPlayer");
    qmlRegisterType<VlcQmlVideoOutput>("VLCQt", 1, 1, "VlcVideoOutput");

#endif

#ifdef USE_MPV_PLAYER
    qmlRegisterType<MpvObject>("MpvIntegration", 1, 0 , "MpvObject");
#endif

    QCoreApplication::setOrganizationDomain("anilibria.tv");
    QCoreApplication::setOrganizationName("EmptyFlow");
    QCoreApplication::setApplicationName("AnilibriaDesktopClient");

    QQmlApplicationEngine engine;    
    engine.rootContext()->setContextProperty("ApplicationVersion", ApplicationVersion);

    const QUrl url(QStringLiteral("qrc:/Main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl) QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection
    );

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
    engine.loadFromModule("aniliberty", "Main");
#else
    engine.load(url);
#endif

    return app.exec();
}
