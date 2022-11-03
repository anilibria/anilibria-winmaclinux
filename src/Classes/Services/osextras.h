#ifndef OSEXTRAS_H
#define OSEXTRAS_H

#include <QObject>
#include <QStringList>

class OsExtras : public QObject
{
    Q_OBJECT
public:
    explicit OsExtras(QObject *parent = nullptr);

#ifdef Q_OS_LINUX
    QStringList m_dbusServices { QStringList() };
    QStringList m_dbusPaths { QStringList() };
    QList<uint> m_dbusReplies { QList<uint>() };
#endif

    Q_INVOKABLE void startPreventSleepMode();
    Q_INVOKABLE void stopPreventSleepMode();

signals:

};

#endif // OSEXTRAS_H
