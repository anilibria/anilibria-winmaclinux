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

#ifndef RELEASETORRENTSLIST_H
#define RELEASETORRENTSLIST_H

#include <QObject>
#include "../Models/releasetorrentmodel.h"

class ReleaseTorrentsList : public QAbstractListModel
{
    Q_OBJECT

private:
    enum ReleaseTorrentRoles {
        IdRole = Qt::UserRole + 1,
        SizeRole,
        QualityRole,
        SeriesRole,
        UrlRole,
    };
    QList<ReleaseTorrentModel*>* m_torrents { new QList<ReleaseTorrentModel*>() };

public:
    explicit ReleaseTorrentsList(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE void loadTorrentsFromJson(const QString& json);

private:
    QString getReadableSize(long long size) const noexcept;

signals:

};

#endif // RELEASETORRENTSLIST_H
