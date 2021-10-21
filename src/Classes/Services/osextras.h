#ifndef OSEXTRAS_H
#define OSEXTRAS_H

#include <QObject>

class OsExtras : public QObject
{
    Q_OBJECT
public:
    explicit OsExtras(QObject *parent = nullptr);

    Q_INVOKABLE void startPreventSleepMode();
    Q_INVOKABLE void stopPreventSleepMode();

signals:

};

#endif // OSEXTRAS_H
