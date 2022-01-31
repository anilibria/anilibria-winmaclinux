#include "commonmenulistmodel.h"

CommonMenuListModel::CommonMenuListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void CommonMenuListModel::setup(const QStringList&& items) noexcept
{
    foreach (auto item, items) m_items->append(item);

    refresh();
}

void CommonMenuListModel::refresh() noexcept
{
    beginResetModel();
    m_filteredItems->clear();

    for (auto i = 0; i < m_items->count(); i++) {
        if (m_notVisibleItems->contains(i)) continue;

        m_filteredItems->append(i);
    }
    endResetModel();
}

int CommonMenuListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_filteredItems->size();
}

QVariant CommonMenuListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto itemIndex = m_filteredItems->at(index.row());
    auto item = m_items->at(itemIndex);

    switch (role) {
        case IndexRole: {
            return QVariant(itemIndex);
        }
        case TitleRole: {
            return QVariant(item);
        }
        case DisableRole: {
            return QVariant(m_disabledItems->contains(itemIndex));
        }
        default: return QVariant();
    }
}

QHash<int, QByteArray> CommonMenuListModel::roleNames() const
{
    return {
        {
            IndexRole,
            "itemIndex"
        },
        {
            TitleRole,
            "title"
        },
        {
            DisableRole,
            "itemDisabled"
        }
    };
}

void CommonMenuListModel::disableItem(const int index)
{
    if (m_disabledItems->contains(index)) return;

    m_disabledItems->insert(index);

    refresh();
}

void CommonMenuListModel::enableItem(const int index)
{
    if (!m_disabledItems->contains(index)) return;

    m_disabledItems->remove(index);

    refresh();
}

void CommonMenuListModel::hideItem(const int index)
{
    if (m_notVisibleItems->contains(index)) return;

    m_notVisibleItems->insert(index);

    refresh();
}

void CommonMenuListModel::showItem(const int index)
{
    if (!m_notVisibleItems->contains(index)) return;

    m_notVisibleItems->remove(index);

    refresh();
}

void CommonMenuListModel::selectItem(const int index)
{
    emit itemSelected(index);
}
