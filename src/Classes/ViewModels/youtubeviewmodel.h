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

#ifndef YOUTUBEVIEWMODEL_H
#define YOUTUBEVIEWMODEL_H

#include <QObject>
#include "../Models/youtubevideomodel.h"
#include "../Services/anilibriaapiservice.h"
#include "../../globalconstants.h"

class YoutubeViewModel : public QAbstractListModel
{
    Q_OBJECT
private:
    AnilibriaApiService* m_apiService;
    QList<YoutubeVideoModel*>* m_youtubeVideos;
    enum PlayListRoles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        ImageRole,
        VideoUrlRole,
        TimestampRole
    };

public:
    explicit YoutubeViewModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE void synchronize();

signals:
    void synchronizeCompleted();

private:
    void createIfNotExistsFile(QString path, QString defaultContent) const;
    QString getYoutubeCachePath() const;
    void saveYoutubeItems(const QString &youtubeItems);
    void readYoutubeItems();
    void refreshYoutubeItems(const QString& json);

private slots:
    void receiveYoutubeVideos(const QString& data);

};

#endif // YOUTUBEVIEWMODEL_H
