#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QtWebView>
#include <QtSvg>
#include "Classes/Services/synchronizationservice.h"
#include "Classes/Services/localstorageservice.h"
#include "Classes/Services/applicationsettings.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);
    QtWebView::initialize();

    qmlRegisterType<SynchronizationService>("Anilibria.Services", 1, 0, "SynchronizationService");
    qmlRegisterType<LocalStorageService>("Anilibria.Services", 1, 0, "LocalStorage");
    qmlRegisterType<ApplicationSettings>("Anilibria.Services", 1, 0, "ApplicationSettings");

    QQmlApplicationEngine engine;
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
