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

#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QQueue>
#include "../Models/downloadqueueitemmodel.h"

class DownloadManager : public QObject
{
    Q_OBJECT

private:
    QNetworkAccessManager* m_CurrentAccessManager;
    DownloadQueueItemModel* m_CurrentDownloadingItem;
    QQueue<DownloadQueueItemModel*>* m_QueuedItems;

public:
    explicit DownloadManager(QObject *parent = nullptr);

    void startDownload();

private:
    void takeNextDownload();

signals:
    void seriaDownloaded(int id, int seriaId);

public slots:
    void seriaFinished(QNetworkReply* reply);

};

#endif // DOWNLOADMANAGER_H
