#ifndef MYANILIBRIASEARCHLISTMODEL_H
#define MYANILIBRIASEARCHLISTMODEL_H

#include <QAbstractListModel>
#include "../Models/fullreleasemodel.h"
#include "../ViewModels/releasesviewmodel.h"

class MyAnilibriaSearchListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ReleasesViewModel* releases READ releases WRITE setReleases NOTIFY releasesChanged)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(bool hasItems READ hasItems NOTIFY hasItemsChanged)

private:
    QList<FullReleaseModel*> m_findedReleases { QList<FullReleaseModel*>() };
    ReleasesViewModel* m_releases { nullptr };
    QString m_filter { "" };

public:
    explicit MyAnilibriaSearchListModel(QObject *parent = nullptr);

    enum MyAnilibriaSearchRole {
        ReleaseIdRole = Qt::UserRole + 1,
        PosterRole,
        TitleRole,
        YearRole,
        SeasonRole,
        OriginalNameRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    ReleasesViewModel* releases() const noexcept { return m_releases; }
    void setReleases(const ReleasesViewModel* viewModel) noexcept;

    QString filter() const noexcept { return m_filter; }
    void setFilter(const QString& filter) noexcept;

    bool hasItems() const noexcept { return !m_findedReleases.isEmpty(); }

    void refresh() noexcept;

signals:
    void releasesChanged();
    void filterChanged();
    void hasItemsChanged();

};

#endif // MYANILIBRIASEARCHLISTMODEL_H
