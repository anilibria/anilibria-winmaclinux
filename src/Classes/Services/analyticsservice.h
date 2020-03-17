#ifndef ANALYTICSSERVICE_H
#define ANALYTICSSERVICE_H

#include <QObject>
#include <QtNetwork>
#include <QUuid>
#include <QSysInfo>
#include "../../globalconstants.h"

class AnalyticsService : public QObject
{
    Q_OBJECT

private:
    void sendPostEvent(QString type, QString category, QString message, QString page);

public:
    explicit AnalyticsService(QObject *parent = nullptr);

    const static QString googleAnalyticsAddress;

    Q_INVOKABLE void sendEvent(QString category, QString message);
    Q_INVOKABLE void sendView(QString category, QString message, QString page);

signals:

};

#endif // ANALYTICSSERVICE_H
