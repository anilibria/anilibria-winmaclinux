#ifndef COMMONMENULISTMODEL_H
#define COMMONMENULISTMODEL_H

#include <QObject>
#include <QStringList>
#include <QSet>
#include <QAbstractListModel>

class CommonMenuListModel : public QAbstractListModel
{
    Q_OBJECT

    QScopedPointer<QStringList> m_items { new QStringList() };
    QScopedPointer<QList<int>> m_filteredItems { new QList<int>() };
    QScopedPointer<QSet<int>> m_disabledItems { new QSet<int>() };
    QScopedPointer<QSet<int>> m_notVisibleItems { new QSet<int>() };

private:
    enum CommonMenuItemRoles {
        IndexRole = Qt::UserRole + 1,
        TitleRole,
        DisableRole
    };

public:
    explicit CommonMenuListModel(QObject *parent = nullptr);

    void setup(const QStringList&& items) noexcept;

    void refresh() noexcept;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    Q_INVOKABLE void disableItem(const int index);
    Q_INVOKABLE void enableItem(const int index);
    Q_INVOKABLE void hideItem(const int index);
    Q_INVOKABLE void showItem(const int index);
    Q_INVOKABLE void selectItem(const int index);

signals:
    void itemSelected(const int index);

};

#endif // COMMONMENULISTMODEL_H
