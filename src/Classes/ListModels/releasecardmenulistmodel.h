#ifndef RELEASECARDMENULISTMODEL_H
#define RELEASECARDMENULISTMODEL_H

#include <QAbstractListModel>

class ReleaseCardMenuListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString selectedTitle READ selectedTitle NOTIFY selectedTitleChanged)
    Q_PROPERTY(bool isReleaseSeries READ isReleaseSeries NOTIFY isReleaseSeriesChanged)

private:
    QStringList m_items { QStringList() };
    int m_selectedItem { 0 };
    enum ReleaseCardMenuRoles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        IsSelectedRole
    };

public:
    explicit ReleaseCardMenuListModel(QObject *parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    QString selectedTitle() const noexcept { return m_items.value(m_selectedItem); }
    bool isReleaseSeries() const noexcept { return m_selectedItem == 0; }

    Q_INVOKABLE void select(int id) noexcept;

signals:
    void selectedTitleChanged();
    void isReleaseSeriesChanged();

};

#endif // RELEASECARDMENULISTMODEL_H
