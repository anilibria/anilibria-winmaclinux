#include "downloaditemmodel.h"

DownloadItemModel::DownloadItemModel()
    : m_ReleaseId(0)
    ,  m_VideoId(0)
    ,  m_Quality(0)
    ,  m_Downloaded(false)
    ,  m_DownloadedUrl("")
{

}

void DownloadItemModel::setReleaseId(int releaseId) noexcept
{
    if (m_ReleaseId == releaseId) return;

    m_ReleaseId = releaseId;
}

void DownloadItemModel::setVideoId(int videoId) noexcept
{
    if (m_VideoId == videoId) return;

    m_VideoId = videoId;

}

void DownloadItemModel::setQuality(int quality) noexcept
{
    if (m_Quality == quality) return;

    m_Quality = quality;

}

void DownloadItemModel::setDownloaded(bool downloaded) noexcept
{
    if (m_Downloaded == downloaded) return;

    m_Downloaded = downloaded;
}

void DownloadItemModel::setDownloadedUrl(const QString &downloadedUrl) noexcept
{
    if (m_DownloadedUrl == downloadedUrl) return;

    m_DownloadedUrl = downloadedUrl;
}

void DownloadItemModel::readFromJsonObject(const QJsonObject &jsonObject) noexcept
{
    setReleaseId(jsonObject.value("releaseId").toInt());
    setVideoId(jsonObject.value("videoId").toInt());
    setQuality(jsonObject.value("quality").toInt());
    setDownloaded(jsonObject.value("downloaded").toBool());
    setDownloadedUrl(jsonObject.value("downloadedUrl").toString());
}

void DownloadItemModel::writeToJsonObject(const QJsonObject &jsonObject) noexcept
{
    jsonObject["releaseId"] = m_ReleaseId;
    jsonObject["videoId"] = m_VideoId;
    jsonObject["quality"] = m_Quality;
    jsonObject["downloaded"] = m_Downloaded;
    jsonObject["downloadedUrl"] = m_DownloadedUrl;
}
