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

#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) : QObject(parent),
    m_CurrentAccessManager(nullptr),
    m_CurrentDownloadingItem(nullptr),
    m_QueuedItems(new QQueue<DownloadQueueItemModel*>())
{
}

void DownloadManager::startDownload()
{
    takeNextDownload();
}

void DownloadManager::takeNextDownload()
{
    if (m_QueuedItems->isEmpty()) return;

    auto item = m_QueuedItems->dequeue();

    m_CurrentDownloadingItem = item;

    auto networkManager = new QNetworkAccessManager(this);
    m_CurrentAccessManager = networkManager;
    auto requestUrl = QUrl(item->url());
    QNetworkRequest request(requestUrl);

    connect(networkManager, &QNetworkAccessManager::finished, this, &DownloadManager::seriaFinished);

    networkManager->get(request);
}

void DownloadManager::seriaFinished(QNetworkReply *reply)
{
    auto data = reply->readAll();
}
