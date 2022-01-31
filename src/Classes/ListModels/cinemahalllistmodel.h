#ifndef CINEMAHALLLISTMODEL_H
#define CINEMAHALLLISTMODEL_H

#include <QObject>
#include <QList>
#include <QSet>
#include "../Models/fullreleasemodel.h"
#include "commonmenulistmodel.h"

class CinemahallListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int countCinemahall READ countCinemahall NOTIFY countCinemahallChanged)
    Q_PROPERTY(bool hasItems READ hasItems NOTIFY hasItemsChanged)
    Q_PROPERTY(int dragRelease READ dragRelease WRITE setDragRelease NOTIFY dragReleaseChanged)
    Q_PROPERTY(int dropRelease READ dropRelease WRITE setDropRelease NOTIFY dropReleaseChanged)
    Q_PROPERTY(bool hasSelectedItems READ hasSelectedItems NOTIFY hasSelectedItemsChanged)
    Q_PROPERTY(CommonMenuListModel* itemMenuList READ itemMenuList NOTIFY itemMenuListChanged)
    Q_PROPERTY(int openedItemIndex READ openedItemIndex WRITE setOpenedItemIndex NOTIFY openedItemIndexChanged)

private:
    const QString m_cacheFileName { "cinemahall.cache" };
    QSharedPointer<QList<FullReleaseModel*>> m_releases { nullptr };
    QScopedPointer<QSet<int>> m_selectedItems { new QSet<int>() };
    QScopedPointer<QList<int>> m_items { new QList<int>() };
    QScopedPointer<CommonMenuListModel> m_itemMenuList { new CommonMenuListModel() };
    QHash<QString, bool>* m_seenMarks { nullptr };
    int m_dragRelease { -1 };
    int m_dropRelease { -1 };
    int m_openedItemIndex { -1 };

    enum CinemaHallRoles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        PosterRole,
        SelectedRole,
        NumberRole,
    };

    void loadItems() noexcept;
    void saveItems() noexcept;
    FullReleaseModel* getRelease(int releaseId) const noexcept;
    void clearDraggingState() noexcept;
    void refreshItems() noexcept;
    int getReleaseSeenMarkCount(int releaseId) const noexcept;
    void reorderElements(const int placeIndex, const int oldIndex);

public:
    explicit CinemahallListModel(QObject *parent = nullptr);
    void setup(QSharedPointer<QList<FullReleaseModel*>> releases, QHash<QString, bool>* seenMarks);
    int countCinemahall() const noexcept { return m_items->count(); }
    bool hasItems() const noexcept { return !m_items->isEmpty(); }

    int dragRelease() const noexcept { return m_dragRelease; }
    void setDragRelease(const int dragRelease) noexcept;

    int dropRelease() const noexcept { return m_dropRelease; }
    void setDropRelease(const int dropRelease) noexcept;

    int openedItemIndex() const noexcept { return m_openedItemIndex; }
    void setOpenedItemIndex(const int openedItemIndex) noexcept;

    bool hasSelectedItems() const noexcept { return !m_selectedItems->isEmpty(); }

    CommonMenuListModel* itemMenuList() const noexcept { return m_itemMenuList.get(); }

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;

    QList<FullReleaseModel*> getCinemahallReleases() const noexcept;

    Q_INVOKABLE void addReleases(const QList<int>& ids);
    Q_INVOKABLE bool hasCinemahallReleases();
    Q_INVOKABLE void reorderRelease();
    Q_INVOKABLE void deleteReleases(const QList<int> &ids);
    Q_INVOKABLE void deleteSelectedReleases();
    Q_INVOKABLE void deleteAllReleases();
    Q_INVOKABLE void selectItem(const int releaseId);
    Q_INVOKABLE void deselectItems();
    Q_INVOKABLE void deletedSeenReleases();

private slots:
    void itemMenuSelected(const int index);

signals:
    void countCinemahallChanged();
    void hasItemsChanged();
    void dragReleaseChanged();
    void dropReleaseChanged();
    void hasSelectedItemsChanged();
    void itemMenuListChanged();
    void openedItemIndexChanged();

};

#endif // CINEMAHALLLISTMODEL_H
