#ifndef RELEASESIMPLELISTMODEL_H
#define RELEASESIMPLELISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QList>
#include "../Models/fullreleasemodel.h"
#include "../ViewModels/releasesviewmodel.h"

class ReleaseSimpleListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(ReleasesViewModel* releases READ releases WRITE setReleases NOTIFY releasesChanged)
    Q_PROPERTY(QString filterMode READ filterMode WRITE setFilterMode NOTIFY filterModeChanged)
    Q_PROPERTY(QString filterNewInFavorites READ filterNewInFavorites NOTIFY filterNewInFavoritesChanged)

private:
    QScopedPointer<QList<FullReleaseModel*>> m_releases { new QList<FullReleaseModel*>() };
    ReleasesViewModel* m_releasesViewModel { nullptr };
    QString m_filterMode { "" };

public:

    explicit ReleaseSimpleListModel(QObject *parent = nullptr);

    enum ReleaseSimpleRoles {
        IndexRole = Qt::UserRole + 1,
        TitleRole,
        PosterRole
    };

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const override;

    QString filterNewInFavorites() const noexcept { return "newinfavorites"; };

    ReleasesViewModel* releases() const noexcept { return m_releasesViewModel; }
    void setReleases(const ReleasesViewModel* viewModel) noexcept;

    QString filterMode() const noexcept { return m_filterMode; }
    void setFilterMode(const QString& filterMode) noexcept;

private:
    void refresh();

signals:
    void releasesChanged();
    void filterModeChanged();
    void filterNewInFavoritesChanged();

};

#endif // RELEASESIMPLELISTMODEL_H
