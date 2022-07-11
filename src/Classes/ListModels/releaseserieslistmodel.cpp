#include "releaseserieslistmodel.h"

ReleaseSeriesListModel::ReleaseSeriesListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void ReleaseSeriesListModel::refresh() noexcept
{
    if (m_linkedSeries == nullptr) return;

    beginResetModel();

    m_releases->clear();

    if (m_releaseId > 0) {
        auto releases = m_releases.get();

        m_linkedSeries->fillReleaseSeries(releases, m_releaseId);
    }

    endResetModel();
}

int ReleaseSeriesListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_releases->size();
}

QVariant ReleaseSeriesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto release = m_releases->at(index.row());

    switch (role) {
        case IndexRole: {
            return QVariant(release->id());
        }
        case TitleRole: {
            return QVariant(release->title());
        }
        case PosterRole: {
            return QVariant(release->poster());
        }
        case IsSelected: {
            return QVariant(release->id() == m_releaseId);
        }
        case ReleaseNumber: {
            return QVariant(index.row() + 1);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ReleaseSeriesListModel::roleNames() const
{
    return {
        {
            IndexRole,
            "id"
        },
        {
            TitleRole,
            "title"
        },
        {
            PosterRole,
            "poster"
        },
        {
            IsSelected,
            "isSelected",
        },
        {
            ReleaseNumber,
            "releaseNumber"
        }
    };
}

void ReleaseSeriesListModel::setLinkedSeries(const ReleaseLinkedSeries *linkedSeries) noexcept
{
    if (m_linkedSeries == linkedSeries) return;

    m_linkedSeries = const_cast<ReleaseLinkedSeries *>(linkedSeries);
    emit linkedSeriesChanged();
}

void ReleaseSeriesListModel::setReleaseId(int releaseId) noexcept
{
    if (m_releaseId == releaseId) return;

    m_releaseId = releaseId;
    emit releaseIdChanged();

    refresh();
}
