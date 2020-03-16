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
    QString title() const;
    void setTitle(const QString &title);

    QString status() const;
    void setStatus(const QString &status);

    QString year() const;
    void setYear(const QString &year);

    QString poster() const;
    void setPoster(const QString &poster);

    QString description() const;
    void setDescription(const QString &description);

    QString releaseType() const;
    void setReleaseType(const QString &releaseType);

    QString genres() const;
    void setGenres(const QString &genres);

    QString voicers() const;
    void setVoicers(const QString &voicers);

    QString season() const;
    void setSeason(const QString &season);

    QString series() const;
    void setSeries(const QString &series);

    QString code() const;
    void setCode(const QString &code);

    int id() const;
    void setId(const int id);

    int countOnlineVideos() const;
    void setCountOnlineVideos(const int countOnlineVideos);

    int countTorrents() const;
    void setCountTorrents(const int countTorrents);

    QString announce() const;
    void setAnnounce(const QString& announce);

    QString originalName() const;
    void setOriginalName(const QString& originalName);

    int rating() const;
    void setRating(const int rating);

    QString torrents() const;
    void setTorrents(const QString& torrents);

    QString videos() const;
    void setVideos(const QString& videos);

    int timestamp() const;
    void setTimestamp(const int timestamp);

    QString type() const;
    void setType(const QString& type);

    void writeToJson(QJsonObject &json) const;

    void readFromJson(QJsonValue &json);

    bool operator== (const FullReleaseModel &comparedModel);

};

#endif // FULLRELEASEMODEL_H
