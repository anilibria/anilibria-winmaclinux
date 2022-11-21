#include "myanilibriasearchlistmodel.h"

MyAnilibriaSearchListModel::MyAnilibriaSearchListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

int MyAnilibriaSearchListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_findedReleases.size();
}

QVariant MyAnilibriaSearchListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto release = m_findedReleases.value(index.row());

    switch (role) {
        case ReleaseIdRole: {
            return QVariant(release->id());
        }
        case PosterRole: {
            return QVariant(release->poster());
        }
        case TitleRole: {
            return QVariant(release->title());
        }
        case YearRole: {
            return QVariant(release->year());
        }
        case SeasonRole: {
            return QVariant(release->season());
        }
        case OriginalNameRole: {
            return QVariant(release->originalName());
        }
    }

    return QVariant();
}

QHash<int, QByteArray> MyAnilibriaSearchListModel::roleNames() const
{
    return {
        {
            ReleaseIdRole,
            "releaseId"
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
            YearRole,
            "year"
        },
        {
            SeasonRole,
            "season"
        },
        {
            OriginalNameRole,
            "originalName"
        }
    };
}

void MyAnilibriaSearchListModel::setReleases(const ReleasesViewModel *viewModel) noexcept
{
    if (m_releases == viewModel) return;

    m_releases = const_cast<ReleasesViewModel*>(viewModel);
    emit releasesChanged();

    refresh();
}

void MyAnilibriaSearchListModel::setFilter(const QString &filter) noexcept
{
    if (m_filter == filter) return;

    m_filter = filter;
    emit filterChanged();

    refresh();
}

void MyAnilibriaSearchListModel::refresh() noexcept
{
    beginResetModel();

    m_findedReleases.clear();

    m_releases->fillFullSearch(m_findedReleases, m_filter);

    endResetModel();

    emit hasItemsChanged();
}
