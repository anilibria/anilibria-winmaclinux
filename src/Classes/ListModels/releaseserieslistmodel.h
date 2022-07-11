#ifndef RELEASESERIESLISTMODEL_H
#define RELEASESERIESLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include "../Models/fullreleasemodel.h"
#include "../Services/releaselinkedseries.h"

class ReleaseSeriesListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int releaseId READ releaseId WRITE setReleaseId NOTIFY releaseIdChanged)
    Q_PROPERTY(ReleaseLinkedSeries* linkedSeries READ linkedSeries WRITE setLinkedSeries NOTIFY linkedSeriesChanged)

private:
    enum ReleaseSimpleRoles {
        IndexRole = Qt::UserRole + 1,
        TitleRole,
        PosterRole,
        IsSelected,
        ReleaseNumber
    };
    QScopedPointer<QList<FullReleaseModel*>> m_releases { new QList<FullReleaseModel*>() };
    ReleaseLinkedSeries* m_linkedSeries { nullptr };
    int m_releaseId { -1 };

public:
    explicit ReleaseSeriesListModel(QObject *parent = nullptr);

    void refresh() noexcept;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    ReleaseLinkedSeries* linkedSeries() const noexcept { return m_linkedSeries; }
    void setLinkedSeries(const ReleaseLinkedSeries* linkedSeries) noexcept;

    int releaseId() const noexcept { return m_releaseId; }
    void setReleaseId(int releaseId) noexcept;

signals:
    void releaseIdChanged();
    void linkedSeriesChanged();

};

#endif // RELEASESERIESLISTMODEL_H
