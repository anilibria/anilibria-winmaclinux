#ifndef RELEASEONLINEVIDEOMODEL_H
#define RELEASEONLINEVIDEOMODEL_H

#include <QObject>
#include <QJsonObject>

class ReleaseOnlineVideoModel
{
private:
    QString m_uniqueId;
    QString m_title { "" };
    QString m_sd  { "" };
    QString m_hd { "" };
    QString m_fullhd  { "" };
    int m_releaseId;
    int m_order;
    QString m_videoPoster { "" };
    int m_endingStartSeconds { -1 };
    int m_endingEndSeconds { -1 };
    int m_openingStartSeconds { -1 };
    int m_openingEndSeconds { -1 };
    QString m_rutubeId { "" };
    QString m_description { "" };
    QString m_posterHost { "" };

public:
    ReleaseOnlineVideoModel();

    QString uniqueId() const noexcept { return m_uniqueId; }
    QString title() const noexcept { return m_title; }
    QString sd() const noexcept { return m_sd; }
    QString hd() const noexcept { return m_hd; }
    QString fullhd() const noexcept { return m_fullhd; }
    int releaseId() const noexcept { return m_releaseId; }
    int order() const noexcept { return m_order; }
    QString videoPoster() const noexcept { return m_posterHost + m_videoPoster; }
    int endingStartSeconds() const noexcept { return m_endingStartSeconds; }
    int endingEndSeconds() const noexcept { return m_endingEndSeconds; }
    int openingStartSeconds() const noexcept { return m_openingStartSeconds; }
    int openingEndSeconds() const noexcept { return m_openingEndSeconds; }
    QString rutubeId() const noexcept { return m_rutubeId; }
    QString description() const noexcept { return m_description; }
    QString posterHost() const { return m_posterHost; }

    void setUniqueId(const QString& uniqueId) noexcept { m_uniqueId = uniqueId; }
    void setTitle(const QString& title) noexcept { m_title = title; }
    void setSD(const QString& sd) noexcept { m_sd = sd; }
    void setHD(const QString& hd) noexcept { m_hd = hd; }
    void setFullHD(const QString& fullhd) noexcept { m_fullhd = fullhd; }
    void setReleaseId(int releaseId) noexcept { m_releaseId = releaseId; }
    void setOrder(int order) noexcept { m_order = order; }
    void setVideoPoster(const QString& videoPoster) noexcept { m_videoPoster = videoPoster; }
    void setEndingStartSeconds(int endingStartSeconds) noexcept { m_endingStartSeconds = endingStartSeconds; }
    void setEndingEndSeconds(int endingEndSeconds) noexcept { m_endingEndSeconds = endingEndSeconds; }
    void setOpeningStartSeconds(int openingStartSeconds) noexcept { m_openingStartSeconds = openingStartSeconds; }
    void setOpeningEndSeconds(int openingEndSeconds) noexcept { m_openingEndSeconds = openingEndSeconds; }
    void setRutubeId(const QString& rutubeId) noexcept { m_rutubeId = rutubeId; }
    void setDescription(const QString& description) noexcept { m_description = description; }   
    void setPosterHost(const QString& posterHost) noexcept { m_posterHost = posterHost; }


    void readFromApiModel(const QJsonObject &jsonObject, int releaseId);

signals:
};

#endif // RELEASEONLINEVIDEOMODEL_H
