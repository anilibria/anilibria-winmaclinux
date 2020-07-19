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

#ifndef YOUTUBEVIDEOMODEL_H
#define YOUTUBEVIDEOMODEL_H

#include <QString>
#include <QJsonObject>
#include <QJsonValue>

class YoutubeVideoModel
{

private:
    int m_Id;
    QString m_Title;
    QString m_Image;
    QString m_VideoIdentifier;
    int m_Timestamp;

public:
    YoutubeVideoModel();

    int id() const { return m_Id; }
    QString title() const { return m_Title; }
    QString image() const { return m_Image; }
    QString videoIdentifier() const { return  m_VideoIdentifier; }
    int timestamp() const { return m_Timestamp; }

    void setId(const int id) noexcept;
    void setTitle(const QString& title) noexcept;
    void setImage(const QString& image) noexcept;
    void setVideoIdentifier(const QString& videoIdentifier) noexcept;
    void setTimestamp(const int timestamp) noexcept;

    void writeToJson(QJsonObject &json) const noexcept;
    void readFromJson(QJsonValue &json);

};

#endif // YOUTUBEVIDEOMODEL_H
