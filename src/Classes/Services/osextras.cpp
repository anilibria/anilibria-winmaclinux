#include "osextras.h"
#include "globalhelpers.h"
#include <QString>
#include <QDebug>
#include <QThread>
#include <QFile>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
#ifdef Q_OS_LINUX
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <dlfcn.h>
#endif
#ifdef Q_OS_MACOS
#include <dlfcn.h>
#endif

#include "localcachechecker.h"

OsExtras* OsExtras::instance = nullptr;

void routineCallBack(bool completed) {
    if (OsExtras::instance == nullptr) return;

    OsExtras::instance->rountineFinished(completed);
}

void releasesCallBack(bool completed) {
    if (OsExtras::instance == nullptr) return;

    OsExtras::instance->synchronizationFinished(completed);
}

void latestChangesCallback(int32_t percent, int32_t processesReleases) {
    if (OsExtras::instance == nullptr) return;

    OsExtras::instance->synchronizationLatestChanges(percent, processesReleases);
}

OsExtras::OsExtras(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_LINUX
    m_dbusServices.append("org.gnome.ScreenSaver");
    m_dbusServices.append("org.freedesktop.ScreenSaver");
    m_dbusServices.append("org.kde.ScreenSaver");
    m_dbusServices.append("org.freedesktop.PowerManagement");

    m_dbusPaths.append("/org/gnome/ScreenSaver");
    m_dbusPaths.append("/ScreenSaver");
    m_dbusPaths.append("/org/kde/ScreenSaver");
    m_dbusPaths.append("/org/freedesktop/PowerManagement");
#endif
    instance = this;
}

void OsExtras::startPreventSleepMode()
{
#ifdef Q_OS_LINUX

    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.isConnected()) return;

    for(int i = 0; i < m_dbusServices.count() ; i++)
    {
        try {
            QDBusInterface screenSaverInterface( m_dbusServices[i], m_dbusPaths[i], m_dbusServices[i], bus);

            if (!screenSaverInterface.isValid()) continue;

            QDBusReply<uint> reply = screenSaverInterface.call("Inhibit", "AniLibria", "REASON");
            if (reply.isValid()) {
                //qDebug() << "valid value" << reply.value();
                m_dbusReply = reply.value();
                break;
            }
        } catch (std::exception& e) {
            qDebug() << e.what();
        }
    }
#endif
#ifdef Q_OS_WIN
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED | ES_DISPLAY_REQUIRED);
#endif
}

void OsExtras::stopPreventSleepMode()
{
#ifdef Q_OS_LINUX
    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.isConnected()) return;

    for(int i = 0; i < m_dbusServices.count() ; i++)
    {
        try {
            QDBusInterface screenSaverInterface( m_dbusServices[i], m_dbusPaths[i], m_dbusServices[i], bus);

            if (!screenSaverInterface.isValid()) continue;

            screenSaverInterface.call("UnInhibit", m_dbusReply);
        } catch (std::exception& e) {
            qDebug() << e.what();
        }
    }
#endif
#ifdef Q_OS_WIN
    SetThreadExecutionState(ES_CONTINUOUS);
#endif
}

bool OsExtras::initializeTorrentStream(int port, const QString& pathToLibrary, const QString& downloadPath, const QString& listenAddress, bool showui)
{
    void *lib = loadLibrary(pathToLibrary);
    torrentStreamInitialize = (torrentstreaminitializemethod)getExport(lib, "initializetorrentstream");
    torrentStreamStop = (stoptorrentstreammethod)getExport(lib, "stoptorrentstream");
    torrentStreamClearAll = (torrentstreamclearallmethod)getExport(lib, "torrentstreamclearall");
    torrentStreamClearOnlyTorrent = (torrentstreamclearonlytorrentmethod)getExport(lib, "torrentstreamclearonlytorrent");
    torrentStreamClearTorrentAndData = (torrentstreamcleartorrentanddatamethod)getExport(lib, "torrentstreamclearonlytorrentanddata");
    torrentStreamSaveState = (savestatemethod)getExport(lib, "torrentstreamsavestate");
    torrentstreamGetAll = (torrentstreamgetallmethod)getExport(lib, "torrentstreamgetall");
    torrentstreamStartDownload = (torrentstreamstartdownload)getExport(lib, "torrentstreamstartdownload");

    if (torrentStreamInitialize == nullptr) {
        qDebug() << "Can't initialize torrentStreamInitialize method";
        return false;
    }
    if (torrentStreamStop == nullptr) {
        qDebug() << "Can't initialize torrentStreamInitialize method";
        return false;
    }
    auto wDownloadPath = downloadPath.toStdWString();
    auto cDownloadPath = const_cast<wchar_t*>(wDownloadPath.c_str());

    auto wListenAddress = listenAddress.toStdWString();
    auto cListenAddress = const_cast<wchar_t*>(wListenAddress.c_str());

    auto result = torrentStreamInitialize(port, cDownloadPath, cListenAddress, showui, &callbackConnectedExternal);
    if (result != 0) qDebug() << "Can't initialize torrentStream inside library code is " << result;

    return result == 0;
}

bool OsExtras::initializeLocalCacheChecker(const QString &pathToLibrary)
{
    if (!QFile::exists(pathToLibrary)) return false;
    if (m_localCacheCheckerConnected) return true;

    try {
        auto importFunctions = new ImportFunctions(pathToLibrary.toStdWString());
        m_LocalCacheChecker = importFunctions;
        m_localCacheCheckerConnected = true;
    } catch (std::exception& e) {
        qDebug() << "initializeLocalCacheChecker: failed to load library " << e.what();
        return false;
    }

    emit localCacheCheckerConnectedChanged();

    return true;
}

void OsExtras::synchronizeRoutine()
{
    if (m_LocalCacheChecker == nullptr) return;

    auto path = getCacheOnlyPath();

    auto checker = (ImportFunctions*)m_LocalCacheChecker;
    auto pathChar = path.toUtf8();
    checker->synchronizeRoutines(true, true, true, pathChar.constData(), &routineCallBack);
}

void OsExtras::synchronizeChanges()
{
    if (m_LocalCacheChecker == nullptr) return;

    m_synchronizationReleases = 0;
    auto path = getCacheOnlyPath();
    auto checker = (ImportFunctions*)m_LocalCacheChecker;
    auto pathChar = path.toUtf8();
    checker->synchronizeChangedReleases(50, pathChar.constData(), &latestChangesCallback, &releasesCallBack);
}

void OsExtras::synchronizeLatest()
{
    if (m_LocalCacheChecker == nullptr) return;

    m_synchronizationReleases = 0;
    auto path = getCacheOnlyPath();
    auto checker = (ImportFunctions*)m_LocalCacheChecker;
    auto pathChar = path.toUtf8();
    checker->synchronizeLatestReleases(50, 3, pathChar.constData(), &latestChangesCallback, &releasesCallBack);
}

void OsExtras::synchronizeAllReleases()
{
    if (m_LocalCacheChecker == nullptr) return;

    m_synchronizationReleases = 0;
    auto path = getCacheOnlyPath();
    auto checker = (ImportFunctions*)m_LocalCacheChecker;
    auto pathChar = path.toUtf8();
    checker->synchronizeLatestReleases(50, 80, pathChar.constData(), &latestChangesCallback, &releasesCallBack);
}

void OsExtras::deinitializeTorrentStream() noexcept
{
    if (torrentStreamStop == nullptr) return;

    torrentStreamStop();
}

void OsExtras::tsClearAll() noexcept
{
    if (torrentStreamClearAll == nullptr) return;

    torrentStreamClearAll(&callbackRefreshExternal);
}

void OsExtras::tsClearOnlyTorrent(const QString& downloadPath) noexcept
{
    if (torrentStreamClearOnlyTorrent == nullptr) return;

    auto wDownloadPath = downloadPath.toStdWString();
    auto cDownloadPath = const_cast<wchar_t*>(wDownloadPath.c_str());

    torrentStreamClearOnlyTorrent(cDownloadPath, &callbackRefreshExternal);
}

void OsExtras::tsClearTorrentAndData(const QString& downloadPath) noexcept
{
    if (torrentStreamClearTorrentAndData == nullptr) return;

    auto wDownloadPath = downloadPath.toStdWString();
    auto cDownloadPath = const_cast<wchar_t*>(wDownloadPath.c_str());

    torrentStreamClearTorrentAndData(cDownloadPath, &callbackRefreshExternal);
}

void OsExtras::tsSaveState() noexcept
{
    if (torrentStreamSaveState == nullptr) return;

    torrentStreamSaveState();
}

QString OsExtras::tsGetAll() noexcept
{
    auto pointer = torrentstreamGetAll();
    return QString::fromWCharArray(pointer);
}

void OsExtras::tsStartFullDownload(int id, QString downloadPath) noexcept
{
    auto wDownloadPath = downloadPath.toStdWString();
    auto cDownloadPath = const_cast<wchar_t*>(wDownloadPath.c_str());

    torrentstreamStartDownload(id, cDownloadPath, &callbackStartDownloadExternal);
}

void* OsExtras::loadLibrary(const QString& path)
{
#ifdef Q_OS_WIN
    auto wstring = path.toStdWString();
    HMODULE h = ::LoadLibraryW(wstring.c_str());
    assert(h != nullptr);
    return (void*)h;
#else
    auto wstring = path.toStdString();
    void *h = dlopen(wstring.c_str(), RTLD_LAZY | RTLD_LOCAL);
    assert(h != nullptr);
    return h;
#endif
}

void *OsExtras::getExport(void *h, const char *name)
{
#ifdef Q_OS_WIN
    void *f = ::GetProcAddress((HMODULE)h, name);
    assert(f != nullptr);
    return f;
#else
    void *f = dlsym(h, name);
    assert(f != nullptr);
    return f;
#endif
}

void OsExtras::callbackConnected()
{
    QThread::msleep(300);
    emit torrentStreamConnected();
}

void OsExtras::callbackRefresh(bool isResult)
{
    emit torrentStreamRefreshed(isResult);
}

void OsExtras::callbackStartDownload(int id, const QString &path, bool isAdded)
{
    emit torrentStreamStartDownload(id, path, isAdded);
}

void OsExtras::synchronizationFinished(bool completed)
{
    if (completed && m_synchronizationReleases > 0) emit needReloadReleases();
}

void OsExtras::rountineFinished(bool completed)
{
    //TODO: reload routines
}

void OsExtras::synchronizationLatestChanges(int32_t percent, int32_t processesReleases)
{
    qDebug() << "Latest changes process: " << percent << "%, releases processed " << processesReleases;
    m_synchronizationReleases = processesReleases;
}
