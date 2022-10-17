#include "externalthemeslistmodel.h"

ExternalThemesListModel::ExternalThemesListModel(QObject *parent)
    : QAbstractListModel{parent}
{

}

void ExternalThemesListModel::setItems(const QList<ThemeItemModel *> &items) noexcept
{
    beginResetModel();

    m_items = items;

    endResetModel();

    emit listIsEmptyChanged();
}

int ExternalThemesListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_items.size();

}

QVariant ExternalThemesListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto itemIndex = index.row();
    auto item = m_items.at(itemIndex);

    switch (role) {
        case IndexRole: {
            return QVariant(itemIndex);
        }
        case TitleRole: {
            return QVariant(item->name());
        }
        case ScreenshootRole: {
            return QVariant("data:image/png;base64," + item->screenshoot());
        }
        case AuthorRole: {
            return QVariant(item->author());
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ExternalThemesListModel::roleNames() const
{
    return {
        {
            IndexRole,
            "pageIndex"
        },
        {
            TitleRole,
            "title"
        },
        {
            ScreenshootRole,
            "screenshoots"
        },
        {
            AuthorRole,
            "author"
        }
    };
}
