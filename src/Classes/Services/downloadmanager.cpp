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
    m_CurrentNetworkReply(nullptr),
    m_DownloadSpeedTimer(new QTimer(this)),
    m_DownloadedBytesInSecond(0),
    m_Url(""),
    m_Running(false),
    m_Progress(0),
    m_Destination("")
{
    connect(m_DownloadSpeedTimer, &QTimer::timeout, this, &DownloadManager::onTimerTimeout);
}

void DownloadManager::setUrl(QUrl url) noexcept
{
    if (url == m_Url) return;

    m_Url = url;
    emit
}

void DownloadManager::setRunning(bool running) noexcept
{
    if (running == m_Running) return;

    m_Running = running;
    emit runningChanged(m_Running);
}

void DownloadManager::setDestination(QUrl destination) noexcept
{
    if (destination == m_Destination) return;

    m_Destination = destination;
    emit destinationChanged(m_Destination);
}

void DownloadManager::start()
{
    if (m_Running) return;

    if (m_CurrentAccessManager == nullptr) m_CurrentAccessManager = new QNetworkAccessManager(this);

    m_Running = true;

    m_CurrentNetworkReply = m_CurrentAccessManager->get(QNetworkRequest(m_Url));
    connect(m_CurrentNetworkReply,&QNetworkReply::finished,this,&DownloadManager::onFinished);
    connect(m_CurrentNetworkReply,&QNetworkReply::downloadProgress,this,&DownloadManager::onDownloadProgress);

    m_DownloadSpeedTimer->start(1000);
}

void DownloadManager::stop()
{
    if (!m_Running) return;

    m_CurrentNetworkReply->abort();
    m_CurrentNetworkReply->deleteLater();

    m_Running = false;
    m_DownloadSpeedTimer->stop();
}

void DownloadManager::onFinished()
{
    if (m_CurrentNetworkReply->error()) {
        emit error((int)m_CurrentNetworkReply->error(), m_CurrentNetworkReply->errorString());
        return;
    }

    m_DownloadSpeedTimer->stop();
}

void DownloadManager::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    if (bytesTotal > 0) {

    }
    m_DownloadedBytesInSecond += (int)bytesReceived;
}

void DownloadManager::onTimerTimeout()
{
    auto downloadedBytesInSecond = (int)m_DownloadedBytesInSecond;
    m_DownloadedBytesInSecond = 0;
    displayBytesInSecondsChanged(downloadedBytesInSecond);

}
