#include "releaseseriescardlistmodel.h"
#include "../../globalconstants.h"

ReleaseSeriesCardListModel::ReleaseSeriesCardListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

int ReleaseSeriesCardListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_items.size();
}

QVariant ReleaseSeriesCardListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto currentIndex = index.row();

    auto item = m_items.at(currentIndex);

    switch (role) {
        case IndexRole: {
            return QVariant(currentIndex);
        }
        case PosterRole: {
            return QVariant(item->poster());
        }
        case TitleRole: {
            return QVariant(item->title());
        }
        case InFavoritesRole: {
            return QVariant(false);
        }
        case DescriptionRole: {
            return item->description();
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ReleaseSeriesCardListModel::roleNames() const
{
    return {
        {
            IndexRole,
            "identifier"
        },
        {
            PosterRole,
            "poster"
        },
        {
            TitleRole,
            "title"
        },
        {
            InFavoritesRole,
            "inFavorites"
        },
        {
            DescriptionRole,
            "description"
        },
        {
            ReleaseIdRole,
            "releaseId"
        }
    };
}

void ReleaseSeriesCardListModel::setup(QList<FullReleaseModel *> releases)
{
    beginResetModel();

    m_items.clear();

    m_items.append(releases);

    endResetModel();
}

void ReleaseSeriesCardListModel::clear()
{
    beginResetModel();

    m_items.clear();

    endResetModel();
}
