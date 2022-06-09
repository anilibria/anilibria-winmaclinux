/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2021 Roman Vladimirov

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

#ifndef ONLINEPLAYERVIDEOLIST_H
#define ONLINEPLAYERVIDEOLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QVariant>
#include <QHash>
#include <QSet>
#include <QVector>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "../Models/onlinevideomodel.h"
#include "../ViewModels/releasesviewmodel.h"

class OnlinePlayerVideoList : public QAbstractListModel
{
    Q_OBJECT

private:
    QVector<OnlineVideoModel*>* m_videos;
    int m_selectedReleaseId;
    int m_selectedVideoId;
    ReleasesViewModel* m_releaseViewModel { nullptr };

    enum PlayListRoles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        LinkSdRole,
        LinkHdRole,
        LinkFullHdRole,
        LinkSourceSdRole,
        LinkSourceHdRole,
        IsSeenRole,
        IsGroupRole,
        ReleaseIdRole,
        ReleasePosterRole,
        SelectedRole,
        OrderRole,
        PosterRole,
        EndingStartRole,
        EndingEndRole,
        OpeningStartRole,
        OpeningEndRole,
    };

public:
    explicit OnlinePlayerVideoList(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const override;

    OnlineVideoModel* getVideoAtIndex(int index) const noexcept;
    int getReleaseVideosCount(int releaseId) const noexcept;
    OnlineVideoModel* getFirstReleaseWithPredicate(std::function<bool(OnlineVideoModel*)> callback, bool isEndDirection = false) const noexcept;

    void setVideosFromSingleList(const QString& json, int releaseId, const QString& poster) noexcept;
    void setVideosFromCinemahall(QList<FullReleaseModel*>&& releases) noexcept;
    void selectVideo(int releaseId, int videoId) noexcept;
    int getVideoIndex(OnlineVideoModel* video) noexcept;
    QList<int> getReleaseIds() noexcept;
    void setup(ReleasesViewModel* releaseViewModel);
    void refreshSingleVideo(int releaseId, int videoId) noexcept;

signals:    

};

#endif // ONLINEPLAYERVIDEOLIST_H
