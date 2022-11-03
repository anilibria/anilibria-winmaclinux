#include "osextras.h"
#include <QString>
#ifdef Q_OS_WIN
#include <windows.h>
#endif
#ifdef Q_OS_LINUX
#include <QtDBus/QDBusConnection>
#include <QtDBus/QDBusInterface>
#include <QtDBus/QDBusReply>
#endif

OsExtras::OsExtras(QObject *parent) : QObject(parent)
{
#ifdef Q_OS_LINUX
    m_dbusServices.append("org.freedesktop.ScreenSaver");
    m_dbusServices.append("org.gnome.SessionManager");

    m_dbusPaths.append("org.freedesktop.ScreenSaver");
    m_dbusPaths.append("/org/gnome/SessionManager");
#endif
}

void OsExtras::startPreventSleepMode()
{
#ifdef Q_OS_LINUX
    m_dbusReplies.clear();

    QDBusConnection bus = QDBusConnection::sessionBus();
    if(!bus.isConnected()) return;

    for(int i = 0; i < m_dbusServices.count() ; i++)
    {
        QDBusInterface screenSaverInterface( m_dbusServices[i], m_dbusPaths[i], m_dbusServices[i], bus);

        if (!screenSaverInterface.isValid()) continue;

        QDBusReply<uint> reply = screenSaverInterface.call("Inhibit", "AniLibria", "REASON");
        m_dbusReplies[i] = reply.isValid() ? reply.value() : 0;
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
        QDBusInterface screenSaverInterface( m_dbusServices[i], m_dbusPaths[i], m_dbusServices[i], bus);

        if (!screenSaverInterface.isValid()) continue;
        auto reply = m_dbusReplies[i];

        if (reply > 0) screenSaverInterface.call("UnInhibit", reply);
    }
    m_dbusReplies.clear();
#endif
#ifdef Q_OS_WIN
    SetThreadExecutionState(ES_CONTINUOUS);
#endif
}
