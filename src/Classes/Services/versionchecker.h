/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2020 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

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
