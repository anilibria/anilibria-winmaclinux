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
