#include "releaseseriescardlistmodel.h"

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
            return QVariant(m_inFavorites.contains(item->id()));
        }
        case DescriptionRole: {
            return QVariant(item->description());
        }
        case ReleaseIdRole:{
            return QVariant(item->id());
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

void ReleaseSeriesCardListModel::setup(QList<FullReleaseModel *> releases, QSet<int> inFavorites)
{
    beginResetModel();

    m_items.clear();

    m_items.append(releases);

    m_releaseIds.clear();
    foreach (auto item, m_items) {
        m_releaseIds.append(item->id());
    }

    m_inFavorites.clear();
    foreach (auto inFavorite, inFavorites) {
        m_inFavorites.insert(inFavorite);
    }

    endResetModel();

    emit releaseIdsChanged();
}

void ReleaseSeriesCardListModel::clear()
{
    beginResetModel();

    m_items.clear();

    endResetModel();

    m_releaseIds.clear();
    emit releaseIdsChanged();
}
