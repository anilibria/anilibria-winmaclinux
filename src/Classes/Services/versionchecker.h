#ifndef VERSIONCHECKER_H
#define VERSIONCHECKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>

#include "globalconstants.h"

class VersionChecker : public QObject
{
    Q_OBJECT
public:
    explicit VersionChecker(QObject *parent = nullptr);

signals:
    void newVersionAvailable(QString version, QString url);

public slots:
    void latestDownloaded(QNetworkReply* reply);

};

#endif // VERSIONCHECKER_H
