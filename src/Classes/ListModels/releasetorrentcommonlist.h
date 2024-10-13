#ifndef RELEASETORRENTCOMMONLIST_H
#define RELEASETORRENTCOMMONLIST_H

#include <QObject>
#include <QAbstractListModel>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include "../Models/apitorrentmodel.h"
#include "../Models/apitorrentmodel.h"

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
        IndexRole,
        TimeCreationRole,
    };
    QScopedPointer<QList<ApiTorrentModel*>> m_torrents { new QList<ApiTorrentModel*>() };
    QScopedPointer<QNetworkAccessManager> m_networkManager { new QNetworkAccessManager() };

public:
    explicit ReleaseTorrentCommonList(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    void loadFromJson(const QList<ApiTorrentModel *>& json);

    Q_INVOKABLE QString getDownloadPath(int index);

private:
    QString getReadableSize(int64_t size) const noexcept;

private slots:
    void downloadTorrentResponse(QNetworkReply *reply);

signals:
    void failureDownloadTorrent();

};

#endif // RELEASETORRENTCOMMONLIST_H
