#ifndef MAINMENULISTMODEL_H
#define MAINMENULISTMODEL_H

#include <QObject>
#include <QList>
#include <QModelIndex>
#include "../Models/mainmenuitemmodel.h"

class MainMenuListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit MainMenuListModel(QObject *parent = nullptr);

    QList<MainMenuItemModel*>* m_items { new QList<MainMenuItemModel*>() };

    int m_selectedItem { 0 };

    enum MainMenuRoles {
        IndexRole = Qt::UserRole + 1,
        TitleRole,
        PageNameRole,
        IconRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const override;

    void setAuthorizeItemVisible(bool visible);

    Q_INVOKABLE void selectItem(int index);

signals:    
    void selectedItem(int index, QString pageName);

};

#endif // MAINMENULISTMODEL_H
