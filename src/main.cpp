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
