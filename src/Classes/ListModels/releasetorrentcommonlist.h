#ifndef RELEASETORRENTCOMMONLIST_H
#define RELEASETORRENTCOMMONLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "../Models/releasetorrentmodel.h"

class ReleaseTorrentCommonList : public QAbstractListModel
{
    Q_OBJECT
private:
    enum CommonReleaseTorrentRoles {
        IdRole = Qt::UserRole + 1,
        SizeRole,
        QualityRole,
        SeriesRole,
        UrlRole,
        IndexRole
    };
    QScopedPointer<QList<ReleaseTorrentModel*>> m_torrents { new QList<ReleaseTorrentModel*>() };
    QScopedPointer<QNetworkAccessManager> m_networkManager { new QNetworkAccessManager() };

public:
    explicit ReleaseTorrentCommonList(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    void loadFromJson(const QString& json);

    Q_INVOKABLE void downloadTorrent(const int id);

private:
    QString getReadableSize(long long size) const noexcept;

private slots:
    void downloadTorrentResponse(QNetworkReply *reply);

signals:
    void failureDownloadTorrent();

};

#endif // RELEASETORRENTCOMMONLIST_H
