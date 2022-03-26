#include "releasesimplelistmodel.h"

ReleaseSimpleListModel::ReleaseSimpleListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    auto now = QDateTime::currentDateTimeUtc();
}

int ReleaseSimpleListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_releases->size();
}

QVariant ReleaseSimpleListModel::data(const QModelIndex &index, int role) const
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
    }

    return QVariant();
}

QHash<int, QByteArray> ReleaseSimpleListModel::roleNames() const
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
        }
    };
}

void ReleaseSimpleListModel::setReleases(const ReleasesViewModel *viewModel) noexcept
{
    if (m_releasesViewModel == viewModel) return;

    m_releasesViewModel = const_cast<ReleasesViewModel*>(viewModel);
    emit releasesChanged();
    refresh();
}

void ReleaseSimpleListModel::setFilterMode(const QString &filterMode) noexcept
{
    if (m_filterMode == filterMode) return;

    m_filterMode = filterMode;
    emit filterModeChanged();
    refresh();
}

static bool compareTimeStampDescending(const FullReleaseModel* first, const FullReleaseModel* second)
{
    return first->timestamp() > second->timestamp();
}

void ReleaseSimpleListModel::refresh()
{
    if (m_releasesViewModel == nullptr) return;

    beginResetModel();

    m_releases->clear();

    if (m_filterMode == "newinfavorites") {
        m_releasesViewModel->fillNewInFavorites(m_releases.get());

        std::sort(m_releases->begin(), m_releases->end(), compareTimeStampDescending);
    }

    if (m_filterMode == "newfromstart") {
        m_releasesViewModel->fillNewFromStart(m_releases.get());

        std::sort(m_releases->begin(), m_releases->end(), compareTimeStampDescending);
    }

    endResetModel();

    emit hasItemsChanged();
}
