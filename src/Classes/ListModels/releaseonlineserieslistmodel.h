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

#ifndef RELEASEONLINESERIESLISTMODEL_H
#define RELEASEONLINESERIESLISTMODEL_H

#include <QAbstractListModel>
#include "../Models/onlinevideomodel.h"
#include "../ViewModels/releasesviewmodel.h"
#include "../ViewModels/onlineplayerviewmodel.h"

class ReleaseOnlineSeriesListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int releaseId READ releaseId WRITE setReleaseId NOTIFY releaseIdChanged)
    Q_PROPERTY(ReleasesViewModel* releases READ releases WRITE setReleases NOTIFY releasesChanged)
    Q_PROPERTY(OnlinePlayerViewModel* onlinePlayer READ onlinePlayer WRITE setOnlinePlayer NOTIFY onlinePlayerChanged)
    Q_PROPERTY(bool isEmpty READ isEmpty NOTIFY isEmptyChanged)

private:
    QList<OnlineVideoModel*> m_items { QList<OnlineVideoModel*>() };
    int m_releaseId { 0 };
    ReleasesViewModel* m_releases { nullptr };
    OnlinePlayerViewModel* m_onlinePlayer { nullptr };
    enum ReleaseOnlineSeriaRoles {
        IdRole = Qt::UserRole + 1,
        IndexRole,
        PosterRole,
        IsSeensRole,
        IsCurrentVideoRole,
        CurrentTimeVideoRole,
        IsEmptyPosterRole
    };

public:
    explicit ReleaseOnlineSeriesListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    int releaseId() const noexcept { return m_releaseId; }
    void setReleaseId(int releaseId) noexcept;

    ReleasesViewModel* releases() const noexcept { return m_releases; }
    void setReleases(const ReleasesViewModel* releases) noexcept;

    OnlinePlayerViewModel* onlinePlayer() const noexcept { return m_onlinePlayer; }
    void setOnlinePlayer(const OnlinePlayerViewModel* onlinePlayer) noexcept;

    bool isEmpty() const noexcept { return m_items.isEmpty(); }

    void refresh();

private:
    QString getDisplayTime(int milliseconds) const;
    QString getZeroValue(int value) const;

signals:
    void releaseIdChanged();
    void releasesChanged();
    void onlinePlayerChanged();
    void isEmptyChanged();

};

#endif // RELEASEONLINESERIESLISTMODEL_H
