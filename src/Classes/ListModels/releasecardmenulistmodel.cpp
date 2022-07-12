#include "releasecardmenulistmodel.h"

ReleaseCardMenuListModel::ReleaseCardMenuListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_items.append("Связанные релизы");
    m_items.append("Серии");
}

int ReleaseCardMenuListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_items.size();
}

QVariant ReleaseCardMenuListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto currentIndex = index.row();
    auto menu = m_items.at(currentIndex);

    switch (role) {
        case IdRole: {
            return QVariant(currentIndex);
        }
        case TitleRole: {
            return QVariant(menu);
        }
        case IsSelectedRole: {
            return QVariant(m_selectedItem == currentIndex);
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ReleaseCardMenuListModel::roleNames() const
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
            IsSelectedRole,
            "isSelected"
        }
    };
}

void ReleaseCardMenuListModel::select(int id) noexcept
{
    beginResetModel();

    m_selectedItem = id;

    endResetModel();

    emit selectedTitleChanged();
}
