#ifndef RELEASESERIESCARDLISTMODEL_H
#define RELEASESERIESCARDLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QSet>
#include "../Models/fullreleasemodel.h"

class ReleaseSeriesCardListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QList<int> releaseIds READ releaseIds NOTIFY releaseIdsChanged)

private:
    QList<FullReleaseModel*> m_items { QList<FullReleaseModel*>() };
    QList<int> m_releaseIds { QList<int>() };
    QSet<int> m_inFavorites { QSet<int>() };
    enum ReleaseSeriesCardRoles {
        IndexRole = Qt::UserRole + 1,
        PosterRole,
        TitleRole,
        InFavoritesRole,
        DescriptionRole,
        ReleaseIdRole,
    };

public:
    explicit ReleaseSeriesCardListModel(QObject *parent = nullptr);

    QList<int> releaseIds() const noexcept { return m_releaseIds; }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    void setup(QList<FullReleaseModel*> releases, QSet<int> inFavorites);
    void clear();

signals:
    void releaseIdsChanged();

};

#endif // RELEASESERIESCARDLISTMODEL_H
