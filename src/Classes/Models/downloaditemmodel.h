#ifndef DOWNLOADITEMMODEL_H
#define DOWNLOADITEMMODEL_H

#include <QString>
#include <QJsonObject>

class DownloadItemModel
{

private:
    int m_ReleaseId;
    int m_VideoId;
    int m_Quality;
    bool m_Downloaded;
    QString m_DownloadedUrl;

public:
    DownloadItemModel();

    int releaseId() const { return m_ReleaseId; }
    void setReleaseId(int releaseId) noexcept;

    int videoId() const { return m_VideoId; }
    void setVideoId(int videoId) noexcept;

    int quality() const { return m_Quality; }
    void setQuality(int quality) noexcept;

    bool downloaded() const { return m_Downloaded; }
    void setDownloaded(bool downloaded) noexcept;

    QString downloadedUrl() const { return m_DownloadedUrl; }
    void setDownloadedUrl(const QString& downloadedUrl) noexcept;

    void readFromJsonObject(const QJsonObject &jsonObject) noexcept;
    void writeToJsonObject(const QJsonObject &jsonObject) noexcept;

};

#endif // DOWNLOADITEMMODEL_H
