#include "cinemahalllistmodel.h"
#include "../../globalhelpers.h"

void CinemahallListModel::loadItems() noexcept
{
    QFile file(getCachePath(m_cacheFileName));
    if (!file.open(QFile::ReadOnly | QFile::Text)) return;

    auto json = file.readAll();
    file.close();

    auto document = QJsonDocument::fromJson(json);
    auto jsonArray = document.array();

    m_items->clear();
    foreach (auto item, jsonArray) m_items->append(item.toInt());

    emit countCinemahallChanged();
}

void CinemahallListModel::saveItems() noexcept
{
    QJsonArray cinemahallArray;

    foreach (auto releaseId, *m_items) {
        QJsonValue value(releaseId);
        cinemahallArray.append(value);
    }

    QFile cinemahallFile(getCachePath(m_cacheFileName));
    if (!cinemahallFile.open(QFile::WriteOnly | QFile::Text)) return;

    auto document = QJsonDocument(cinemahallArray);
    cinemahallFile.write(document.toJson());

    cinemahallFile.close();
}

FullReleaseModel *CinemahallListModel::getRelease(int releaseId) const noexcept
{
    auto iterator = std::find_if(
        m_releases->cbegin(),
        m_releases->cend(),
        [releaseId] (FullReleaseModel* release) {
            return release->id() == releaseId;
        }
    );

    if (iterator == m_releases->cend()) return nullptr;

    return *iterator;
}

void CinemahallListModel::clearDraggingState() noexcept
{
    setDragRelease(-1);
    setDropRelease(-1);
}

void CinemahallListModel::refreshItems() noexcept
{
    beginResetModel();
    endResetModel();
}

FullReleaseModel *CinemahallListModel::getReleaseById(int id) const noexcept
{
    auto iterator = std::find_if(
        m_releases->cbegin(),
        m_releases->cend(),
        [id](FullReleaseModel* item)
        {
            return item->id() == id;
        }
    );

    if(iterator == m_releases->cend()) return nullptr;

    return *iterator;

}

CinemahallListModel::CinemahallListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    createIfNotExistsFile(getCachePath(m_cacheFileName), "[]");

    loadItems();
}

void CinemahallListModel::setup(QSharedPointer<QList<FullReleaseModel *> > releases)
{
    m_releases = releases;
}

void CinemahallListModel::setDragRelease(const int dragRelease) noexcept
{
    if (dragRelease == m_dragRelease) return;

    m_dragRelease = dragRelease;
    emit dragReleaseChanged();
}

void CinemahallListModel::setDropRelease(const int dropRelease) noexcept
{
    if (dropRelease == m_dropRelease) return;

    m_dropRelease = dropRelease;
    emit dropReleaseChanged();
}

int CinemahallListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_items->size();
}

QVariant CinemahallListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto releaseId = m_items->at(index.row());
    auto release = getRelease(releaseId);
    if (release == nullptr) return QVariant();

    switch (role) {
        case IdRole: {
            return QVariant(releaseId);
        }
        case TitleRole: {
            return QVariant(release->title());
        }
        case PosterRole: {
            return QVariant(release->poster());
        }
        case SelectedRole: {
            return QVariant(m_selectedItems->contains(releaseId));
        }
        default: return QVariant();
    }
}

QHash<int, QByteArray> CinemahallListModel::roleNames() const
{
    return {
        {
            IdRole,
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
            SelectedRole,
            "isSelected"
        }
    };
}

QList<FullReleaseModel *> CinemahallListModel::getCinemahallReleases() const noexcept
{
    QList<FullReleaseModel*> result;
    foreach (auto releaseId, *m_items) {
        auto fullRelease = getReleaseById(releaseId);
        if (fullRelease == nullptr) continue;

        result.append(fullRelease);
    }

    return result;
}


void CinemahallListModel::addReleases(const QList<int> &ids)
{
    foreach(auto id, ids) {
        if (m_items->contains(id)) continue;

        m_items->append(id);
    }

    saveItems();
    emit countCinemahallChanged();
    emit hasItemsChanged();
    refreshItems();
}

bool CinemahallListModel::hasCinemahallReleases()
{
    return !m_items->isEmpty();
}

void CinemahallListModel::reorderRelease()
{
    if (m_dragRelease == -1 || m_dropRelease == -1) return;

    auto placeIndex = m_items->indexOf(m_dropRelease);
    auto oldIndex = m_items->indexOf(m_dragRelease);

    if (placeIndex < 0 || placeIndex > m_items->count()) return;
    if (oldIndex < 0 || oldIndex > m_items->count()) return;

    m_items->removeAt(oldIndex);
    m_items->insert(placeIndex, m_dragRelease);

    saveItems();
    clearDraggingState();
    refreshItems();
}

void CinemahallListModel::deleteReleases(const QList<int> &ids)
{
    foreach (auto id, ids) {
        auto index = m_items->indexOf(id);
        if (index == -1) continue;

        m_items->removeAt(index);
    }

    saveItems();
    emit countCinemahallChanged();
    emit hasSelectedItemsChanged();
    emit hasItemsChanged();
    refreshItems();
}

void CinemahallListModel::deleteSelectedReleases()
{
    if (m_selectedItems->isEmpty()) return;

    QList<int> ids;
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    ids = m_selectedItems->toList();
#else
    ids = m_selectedItems->values();
#endif

    m_selectedItems->clear();
    deleteReleases(ids);
}

void CinemahallListModel::deleteAllReleases()
{
    m_items->clear();
    m_selectedItems->clear();

    saveItems();
    emit countCinemahallChanged();
    emit hasSelectedItemsChanged();
    emit hasItemsChanged();
    refreshItems();
}

void CinemahallListModel::selectItem(const int releaseId)
{
    if (m_selectedItems->contains(releaseId)) {
        m_selectedItems->remove(releaseId);
    } else {
        m_selectedItems->insert(releaseId);
    }

    const int itemIndex = m_items->indexOf(releaseId);

    emit dataChanged(index(itemIndex), index(itemIndex));
    emit hasSelectedItemsChanged();
}

void CinemahallListModel::deselectItems()
{
    QList<int> ids;
#if QT_VERSION < QT_VERSION_CHECK(5, 15, 0)
    ids = m_selectedItems->toList();
#else
    ids = m_selectedItems->values();
#endif
    m_selectedItems->clear();

    foreach (auto releaseId, ids) {
        const int itemIndex = m_items->indexOf(releaseId);

        emit dataChanged(index(itemIndex), index(itemIndex));
    }

    emit hasSelectedItemsChanged();
}