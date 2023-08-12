#ifndef RELEASESERIESCARDLISTMODEL_H
#define RELEASESERIESCARDLISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include "../Models/fullreleasemodel.h"

class ReleaseSeriesCardListModel : public QAbstractListModel
{
    Q_OBJECT

private:
    QList<FullReleaseModel*> m_items { QList<FullReleaseModel*>() };
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

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    void setup(QList<FullReleaseModel*> releases);
    void clear();

signals:

};

#endif // RELEASESERIESCARDLISTMODEL_H
