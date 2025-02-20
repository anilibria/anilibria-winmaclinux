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
