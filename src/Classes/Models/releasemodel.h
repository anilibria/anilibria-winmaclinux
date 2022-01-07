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

#ifndef RELEASEMODEL_H
#define RELEASEMODEL_H

#include <QtCore>
#include "onlinevideomodel.h"
#include "releasetorrentmodel.h"

class ReleaseModel
{
private:
    int m_Id;
    QString m_Code;
    QString m_Series;
    QString m_Poster;
    QString m_Timestamp;
    QString m_Status;
    QString m_Type;
    QString m_Year;
    QString m_Description;
    QStringList m_Genres;
    QStringList m_Voices;
    QStringList m_Names;
    QList<OnlineVideoModel> m_Videos;
    QList<ReleaseTorrentModel> m_Torrents;
    QString m_Title;
    QString m_Season;
    QString m_Announce;
    int m_Rating;

public:
    ReleaseModel();
    ~ReleaseModel();

    void readFromApiModel(const QJsonObject &jsonObject);
    
    void writeToApiModel(QJsonObject& jsonObject) const noexcept;

    void writeToJson(QJsonObject &json) const noexcept;

    void readFromJson(const QJsonObject &json);

    int id() const { return m_Id; }

    void setId(int id) { m_Id = id; }

    QString code() const { return m_Code; }

    void setCode(const QString& code) { m_Code = code; }

    QString series() const { return m_Series; }

    QString poster() const { return m_Poster; }

    void setPoster(const QString& poster) { m_Poster = poster; }

    QString timestamp() const { return m_Timestamp; }

    QString status() const { return m_Status; }

    void setStatus(const QString& status) { m_Status = status; }

    QString type() const { return m_Type; }

    void setType(const QString& type) { m_Type = type; }

    QString year() const { return m_Year; }

    void setYear(const QString& year) { m_Year = year; }

    QString description() const { return m_Description; }

    void setDescription(const QString& description) { m_Description = description; }

    QString season() const { return m_Season; }

    void setSeason(const QString& season) { m_Season = season; }

    QString announce() const { return m_Announce; }

    QStringList genres() const { return m_Genres; }

    QStringList voices() const { return m_Voices; }

    QStringList names() const { return m_Names; }

    int rating() const { return m_Rating; }

    QString title() const { return m_Title; }

    void setTitle(const QString& title) { m_Title = title; }

    void addToNames(const QString& name) { m_Names.append(name); }

    void addToVoices(const QString& voice) { m_Voices.append(voice); }

    void addToGenres(const QString& genre) { m_Genres.append(genre); }

    void addToVideos(const OnlineVideoModel& video) { m_Videos.append(video); }

    QList<OnlineVideoModel> videos() const { return m_Videos; }

    QList<ReleaseTorrentModel> torrents() const { return m_Torrents; }

};

#endif // RELEASEMODEL_H
