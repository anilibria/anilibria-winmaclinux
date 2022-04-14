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
    int m_sessionCounter { 0 };
    QDateTime m_startTime { QDateTime::currentDateTimeUtc() };

public:
    explicit AnalyticsService(QObject *parent = nullptr);

    const static QString googleAnalyticsAddress;

    Q_INVOKABLE void sendView(QString category, QString message, QString page);
    Q_INVOKABLE void sendVersion();

private:
    void sendPostEvent(QString category, QString message, QString page);

signals:

};

#endif // ANALYTICSSERVICE_H
