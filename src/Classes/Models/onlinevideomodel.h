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

#ifndef ONLINEVIDEOMODEL_H
#define ONLINEVIDEOMODEL_H

#include <QtCore>

class OnlineVideoModel
{
private:
    int m_Id;
    QString m_Title;
    QString m_SD;
    QString m_HD;
    QString m_FullHD;
    QString m_SourceSD;
    QString m_SourceHD;
    int m_releaseId;
    int m_order;
    QString m_releasePoster;
    bool m_isGroup;
    QString m_videoPoster { "" };
    int m_endingStartSeconds { -1 };
    int m_endingEndSeconds { -1 };
    int m_openingStartSeconds { -1 };
    int m_openingEndSeconds { -1 };

public:
    OnlineVideoModel();

    void readFromApiModel(const QJsonObject &jsonObject);

    void writeToJson(QJsonObject &json) const noexcept;

    int id() const { return m_Id; }
    QString title() const { return m_Title; }
    QString sd() const { return m_SD; }
    QString hd() const noexcept { return m_HD; }
    QString fullhd() const { return m_FullHD; }
    QString sourcesd() const { return m_SourceSD; }
    QString sourcehd() const { return m_SourceHD; }
    int releaseId() const { return m_releaseId; }
    int order() const { return m_order; }
    QString releasePoster() const { return m_releasePoster; }
    bool isGroup() const { return m_isGroup; }
    QString videoPoster() const { return m_videoPoster; }
    int endingStartSeconds() const noexcept { return m_endingStartSeconds; }
    int endingEndSeconds() const noexcept { return m_endingEndSeconds; };
    int openingStartSeconds() const noexcept { return m_openingStartSeconds; };
    int openingEndSeconds() const noexcept { return m_openingEndSeconds; };

    void setId(const int id) noexcept;
    void setTitle(const QString &title) noexcept;
    void setSd(const QString &sd) noexcept;
    void setHd(const QString &hd) noexcept;
    void setFullHd(const QString &fullhd) noexcept;
    void setSourceSd(const QString &sourcesd) noexcept;
    void setSourceHd(const QString &sourcehd) noexcept;
    void setReleaseId(int releaseId) noexcept;
    void setOrder(int order) noexcept;
    void setReleasePoster(QString releasePoster) noexcept;
    void setIsGroup(bool isGroup) noexcept;
    void setVideoPoster(const QString& poster) noexcept;
};

#endif // ONLINEVIDEOMODEL_H
