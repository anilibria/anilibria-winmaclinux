#ifndef FULLRELEASEMODEL_H
#define FULLRELEASEMODEL_H

#include <QObject>
#include "releasemodel.h"

class FullReleaseModel
{

private:
    int m_Id;
    QString m_Code;
    QString m_Series;
    QString m_Poster;
    int m_Timestamp;
    QString m_Status;
    QString m_Type;
    QString m_Year;
    QString m_Description;
    QString m_Genres;
    QString m_Voices;
    QString m_OriginalName;
    QString m_Videos;
    int m_CountVideos;
    QString m_Torrents;
    int m_CountTorrents;
    QString m_Title;
    QString m_Season;
    QString m_Announce;
    int m_Rating;

public:
    QString title() const { return m_Title; }
    void setTitle(const QString &title) noexcept;

    QString status() const { return m_Status; }
    void setStatus(const QString &status) noexcept;

    QString year() const { return m_Year; }
    void setYear(const QString &year) noexcept;

    QString poster() const { return m_Poster; }
    void setPoster(const QString &poster) noexcept;

    QString description() const { return m_Description; }
    void setDescription(const QString &description) noexcept;

    QString releaseType() const { return m_Type; }
    void setReleaseType(const QString &releaseType) noexcept;

    QString genres() const { return m_Genres; }
    void setGenres(const QString &genres) noexcept;

    QString voicers() const { return m_Voices; }
    void setVoicers(const QString &voicers) noexcept;

    QString season() const { return m_Season; }
    void setSeason(const QString &season) noexcept;

    QString series() const { return m_Series; }
    void setSeries(const QString &series) noexcept;

    QString code() const { return m_Code; }
    void setCode(const QString &code) noexcept;

    int id() const { return m_Id; }
    void setId(const int id) noexcept;

    int countOnlineVideos() const { return m_CountVideos; }
    void setCountOnlineVideos(const int countOnlineVideos) noexcept;

    int countTorrents() const { return m_CountTorrents; }
    void setCountTorrents(const int countTorrents) noexcept;

    QString announce() const { return m_Announce; }
    void setAnnounce(const QString& announce) noexcept;

    QString originalName() const { return m_OriginalName; }
    void setOriginalName(const QString& originalName) noexcept;

    int rating() const { return m_Rating; }
    void setRating(const int rating) noexcept;

    QString torrents() const { return m_Torrents; }
    void setTorrents(const QString& torrents) noexcept;

    QString videos() const { return m_Videos; }
    void setVideos(const QString& videos) noexcept;

    int timestamp() const noexcept { return m_Timestamp; }
    void setTimestamp(const int timestamp) noexcept;

    QString type() const { return m_Type; }
    void setType(const QString& type) noexcept;

    void writeToJson(QJsonObject &json) const noexcept;

    void readFromJson(QJsonValue &json);

    bool operator== (const FullReleaseModel &comparedModel) noexcept;

};

#endif // FULLRELEASEMODEL_H
