#ifndef COMMONCOMBOBOXLISTMODEL_H
#define COMMONCOMBOBOXLISTMODEL_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>

class CommonComboBoxListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int selectedItem READ selectedItem WRITE setSelectedItem NOTIFY selectedItemChanged)

private:
    QList<std::tuple<int, QString>>* m_items { new QList<std::tuple<int, QString>>() };
    int m_selectedItem { -1 };

    enum CommonItemRoles {
        IdRole = Qt::UserRole + 1,
        TitleRole
    };

public:
    explicit CommonComboBoxListModel(QObject *parent = nullptr);

    void addItem(int id, QString title) noexcept;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    int selectedItem() const noexcept { return m_selectedItem; }
    void setSelectedItem(int selectedItem) noexcept;

signals:
    void selectedItemChanged();

};

#endif // COMMONCOMBOBOXLISTMODEL_H
