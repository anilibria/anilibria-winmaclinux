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

#ifndef FULLRELEASEMODEL_H
#define FULLRELEASEMODEL_H

#include <QObject>
#include <QString>

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
    int m_CountVideos;
    int m_CountTorrents;
    QString m_Title;
    QString m_Season;
    QString m_Announce;
    int m_Rating;
    bool m_isOngoing { false };
    QString m_ageRating { "" };
    const QString m_isOngoingField { "isOngoing" };
    const QString m_ageRatingField { "ageRating" };
    QString m_posterHost { "" };
    QString m_groupValue { "" };
    QString m_fullTeam;

public:
    QString title() const { return m_Title; }
    void setTitle(const QString &title) noexcept;

    QString status() const { return m_Status; }
    void setStatus(const QString &status) noexcept;

    QString year() const { return m_Year; }
    void setYear(const QString &year) noexcept;

    QString poster() const { return m_posterHost + m_Poster; }
    void setPoster(const QString &poster) noexcept;

    QString description() const { return m_Description; }
    void setDescription(const QString &description) noexcept;

    QString releaseType() const { return m_Type; }
    void setReleaseType(const QString &releaseType) noexcept;

    QString genres() const { return m_Genres; }
    void setGenres(const QString &genres) noexcept;

    QString voicers() const { return m_Voices; }
    QString limitVoicers();
    void setVoicers(const QString &voicers) noexcept;

    QString fullTeam() const { return m_fullTeam; }
    QString limitFullTeam();
    void setFullTeam(const QString &fullTeam) noexcept { m_fullTeam = fullTeam; }

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

    int timestamp() const noexcept { return m_Timestamp; }
    void setTimestamp(const int timestamp) noexcept;

    QString type() const { return m_Type; }
    void setType(const QString& type) noexcept;

    bool isOngoing() const { return m_isOngoing; }
    void setIsOngoing(bool isOngoing) noexcept { m_isOngoing = isOngoing; }

    QString ageRating() const { return m_ageRating; }
    void setAgeRating(const QString& ageRating) noexcept { m_ageRating = ageRating; }

    QString posterHost() const { return m_posterHost; }
    void setPosterHost(const QString& posterHost) noexcept { m_posterHost = posterHost; }

    QString groupValue() const noexcept { return  m_groupValue; }
    void setGroupValue(const QString& groupValue) noexcept { m_groupValue = groupValue; }

    void readFromJson(const QJsonObject &json);

    bool operator== (const FullReleaseModel &comparedModel) noexcept;

};

#endif // FULLRELEASEMODEL_H
