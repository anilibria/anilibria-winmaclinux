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

#include <QFile>
#include <QStandardPaths>
#include "downloadmanager.h"

DownloadManager::DownloadManager(QObject *parent) : QObject(parent),
    m_CurrentAccessManager(nullptr),
    m_CurrentNetworkReply(nullptr),
    m_DownloadSpeedTimer(new QTimer(this)),
    m_Url(""),
    m_Running(false),
    m_Progress(0),
    m_Destination(""),
    m_DisplayBytesInSeconds(""),
    m_BytesInSeconds(0)
{
    m_Destination = QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
    connect(m_DownloadSpeedTimer, &QTimer::timeout, this, &DownloadManager::onTimerTimeout);
}

void DownloadManager::setUrl(QUrl url) noexcept
{
    if (url == m_Url) return;

    m_Url = url;
    emit urlChanged(m_Url);
}

void DownloadManager::setDestination(QString destination) noexcept
{
    if (destination == m_Destination) return;

    m_Destination = destination;
    emit destinationChanged(m_Destination);
}

void DownloadManager::setSaveFileName(QString saveFileName) noexcept
{
    if (saveFileName == m_SaveFileName) return;

    m_SaveFileName = saveFileName;
    emit saveFileNameChanged();
}

void DownloadManager::setDisplayBytesInSeconds(QString displayBytesInSeconds)
{
    m_DisplayBytesInSeconds = displayBytesInSeconds;
    emit displayBytesInSecondsChanged(m_DisplayBytesInSeconds);
}

void DownloadManager::setRunning(bool running)
{
    m_Running = running;
    emit runningChanged(m_Running);
}

void DownloadManager::setProgress(qreal progress)
{
    if (progress == m_Progress) return;

    m_Progress = progress;
    emit progressChanged(progress);
}

void DownloadManager::start()
{
    if (m_Running) return;

    if (m_CurrentAccessManager == nullptr) m_CurrentAccessManager = new QNetworkAccessManager(this);

    setRunning(true);

    m_CurrentNetworkReply = m_CurrentAccessManager->get(QNetworkRequest(m_Url));
    connect(m_CurrentNetworkReply, &QNetworkReply::finished, this, &DownloadManager::onFinished);
    connect(m_CurrentNetworkReply, &QNetworkReply::downloadProgress, this, &DownloadManager::onDownloadProgress);

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
    m_DownloadSpeedTimer->stop();
    setRunning(false);
    setDisplayBytesInSeconds("");

    if (m_CurrentNetworkReply->error()) {
        emit error((int)m_CurrentNetworkReply->error(), m_CurrentNetworkReply->errorString());
        return;
    }

    auto bufferSize = 1024 * 10;
    auto filePath = m_Destination + "/" + m_SaveFileName;
    QFile file(filePath);
    file.open(QFile::WriteOnly);

    while (true) {
        auto buffer = m_CurrentNetworkReply->read(bufferSize);
        if (buffer.length() == 0) break;

        file.write(buffer);

        if (buffer.length() < bufferSize) break;
    }

    file.close();

    emit finished(filePath);
}

void DownloadManager::onDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    m_BytesInSeconds = m_BytesInSeconds + (bytesTotal - bytesReceived);
    double percent = (double)bytesReceived / (double)bytesTotal;
    setProgress(percent * 100);
}

void DownloadManager::onTimerTimeout()
{
    QVector<QString> sizes(5);
    sizes[0] = "B";
    sizes[1] = "KB";
    sizes[2] = "MB";
    sizes[3] = "GB";
    sizes[4] = "TB";
    double len = m_BytesInSeconds;
    int order = 0;
    while (len >= 1024 && order < 4) {
        order++;
        len = len / 1024;
    }
    setDisplayBytesInSeconds(QString::number((int)len) + " " + sizes[order] + "/s");
    m_BytesInSeconds = 0;
}
