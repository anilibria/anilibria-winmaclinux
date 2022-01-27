#ifndef CINEMAHALLLISTMODEL_H
#define CINEMAHALLLISTMODEL_H

#include <QObject>
#include <QList>
#include <QSet>
#include "../Models/fullreleasemodel.h"

class CinemahallListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int countCinemahall READ countCinemahall NOTIFY countCinemahallChanged)
    Q_PROPERTY(bool hasItems READ hasItems NOTIFY hasItemsChanged)
    Q_PROPERTY(int dragRelease READ dragRelease WRITE setDragRelease NOTIFY dragReleaseChanged)
    Q_PROPERTY(int dropRelease READ dropRelease WRITE setDropRelease NOTIFY dropReleaseChanged)
    Q_PROPERTY(bool hasSelectedItems READ hasSelectedItems NOTIFY hasSelectedItemsChanged)

private:
    const QString m_cacheFileName { "cinemahall.cache" };
    QSharedPointer<QList<FullReleaseModel*>> m_releases { nullptr };
    QScopedPointer<QSet<int>> m_selectedItems { new QSet<int>() };
    QScopedPointer<QList<int>> m_items { new QList<int>() };
    QHash<QString, bool>* m_seenMarks { nullptr };
    int m_dragRelease { -1 };
    int m_dropRelease { -1 };

    enum CinemaHallRoles {
        IdRole = Qt::UserRole + 1,
        TitleRole,
        PosterRole,
        SelectedRole,
    };

    void loadItems() noexcept;
    void saveItems() noexcept;
    FullReleaseModel* getRelease(int releaseId) const noexcept;
    void clearDraggingState() noexcept;
    void refreshItems() noexcept;
    FullReleaseModel* getReleaseById(int id) const noexcept;
    int getReleaseSeenMarkCount(int releaseId) const noexcept;

public:
    explicit CinemahallListModel(QObject *parent = nullptr);
    void setup(QSharedPointer<QList<FullReleaseModel*>> releases, QHash<QString, bool>* seenMarks);
    int countCinemahall() const noexcept { return m_items->count(); }
    bool hasItems() const noexcept { return !m_items->isEmpty(); }

    int dragRelease() const noexcept { return m_dragRelease; }
    void setDragRelease(const int dragRelease) noexcept;

    int dropRelease() const noexcept { return m_dropRelease; }
    void setDropRelease(const int dropRelease) noexcept;

    bool hasSelectedItems() const noexcept { return !m_selectedItems->isEmpty(); }

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

signals:
    void countCinemahallChanged();
    void hasItemsChanged();
    void dragReleaseChanged();
    void dropReleaseChanged();
    void hasSelectedItemsChanged();
};

#endif // CINEMAHALLLISTMODEL_H
