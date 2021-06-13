#ifndef ONLINEPLAYERVIDEOLIST_H
#define ONLINEPLAYERVIDEOLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QVariant>
#include <QHash>
#include <QVector>
#include <QStringList>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>
#include "../Models/onlinevideomodel.h"

class OnlinePlayerVideoList : public QAbstractListModel
{
    Q_OBJECT

private:
    QVector<OnlineVideoModel*>* m_videos;

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
        ReleasePosterRole
    };

public:
    explicit OnlinePlayerVideoList(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const override;

    OnlineVideoModel* getVideoAtIndex(int index) const noexcept;
    int getReleaseVideosCount(int releaseId) const noexcept;
    OnlineVideoModel* getFirstReleaseWithPredicate(std::function<bool(OnlineVideoModel*)> callback) const noexcept;

    void setVideosFromSingleList(const QString& json, int releaseId, const QString& poster) noexcept;
    void setVideosFromMultipleList(const QStringList& json) noexcept;

signals:    

};

#endif // ONLINEPLAYERVIDEOLIST_H
