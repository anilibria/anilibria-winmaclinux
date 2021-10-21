#include "osextras.h"
#include <QString>
#ifdef Q_OS_WIN
#include <windows.h>
#endif


OsExtras::OsExtras(QObject *parent) : QObject(parent)
{

}

void OsExtras::startPreventSleepMode()
{
#ifdef Q_OS_WIN
    SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_AWAYMODE_REQUIRED | ES_DISPLAY_REQUIRED);
#endif
}

void OsExtras::stopPreventSleepMode()
{
#ifdef Q_OS_WIN
    SetThreadExecutionState(ES_CONTINUOUS);
#endif
}
