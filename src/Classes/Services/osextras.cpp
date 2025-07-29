#include "osextras.h"
#include <QString>
#include <QDebug>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
#ifdef Q_OS_LINUX
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#include <dlfcn.h>
#endif

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

    auto result = torrentStreamInitialize(port, cDownloadPath, cListenAddress, showui);
    if (result != 0) qDebug() << "Can't initialize torrentStream inside library code is " << result;

    return result == 0;
}

void* OsExtras::loadLibrary(const QString& path)
{
#ifdef Q_OS_WIN
    auto wstring = path.toStdWString();
    HMODULE h = ::LoadLibraryW(wstring.c_str());
    assert(h != nullptr);
    return (void*)h;
#else
    auto wstring = path.toStdWString();
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
