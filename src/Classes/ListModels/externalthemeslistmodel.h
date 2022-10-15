#ifndef EXTERNALTHEMESLISTMODEL_H
#define EXTERNALTHEMESLISTMODEL_H

#include <QAbstractListModel>
#include "../Models/themeitemmodel.h"

class ExternalThemesListModel : public QAbstractListModel
{
    Q_OBJECT

private:
    enum ExternalThemesRoles {
        IndexRole = Qt::UserRole + 1,
        TitleRole,
        ScreenshootRole,
        AuthorRole
    };
    QList<ThemeItemModel*> m_items { QList<ThemeItemModel*>() };

public:
    explicit ExternalThemesListModel(QObject *parent = nullptr);

    void setItems(const QList<ThemeItemModel*>& items) noexcept;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

signals:

};

#endif // EXTERNALTHEMESLISTMODEL_H
