#include "commoncomboboxlistmodel.h"

CommonComboBoxListModel::CommonComboBoxListModel(QObject *parent) : QAbstractListModel(parent)
{

}

void CommonComboBoxListModel::addItem(int id, QString title) noexcept
{
    m_items->append(std::make_tuple(id, title));
    if (m_items->count() == 1) setSelectedItem(id);
}

int CommonComboBoxListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;
    return m_items->size();
}

QVariant CommonComboBoxListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto item = m_items->at(index.row());

    switch (role) {
        case IdRole: {
            return QVariant(std::get<0>(item));
        }
        case TitleRole: {
            return QVariant(std::get<1>(item));
        }
        default: return QVariant();
    }
}

QHash<int, QByteArray> CommonComboBoxListModel::roleNames() const
{
    return {
        {
            IdRole,
            "id"
        },
        {
            TitleRole,
            "title"
        }
    };
}

void CommonComboBoxListModel::setSelectedItem(int selectedItem) noexcept
{
    if (m_selectedItem == selectedItem) return;

    m_selectedItem = selectedItem;
    emit selectedItemChanged();
}
