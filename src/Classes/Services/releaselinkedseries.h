#ifndef RELEASELINKEDSERIES_H
#define RELEASELINKEDSERIES_H


#include <QObject>
#include <QAbstractListModel>
#include "../Models/fullreleasemodel.h"
#include "../Models/releaseseriesmodel.h"

class ReleaseLinkedSeries : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(QString nameFilter READ nameFilter WRITE setNameFilter NOTIFY nameFilterChanged)
private:
    QString m_nameFilter;
    QVector<ReleaseSeriesModel*>* m_series;
    QVector<ReleaseSeriesModel*>* m_filteredSeries;
    bool m_filtering = false;

    enum ItemRoles {
        CountReleasesRole = Qt::UserRole + 1,
        FirstNameRole,
        SecondNameRole,
        ThirdNameRole,
        ReleaseIds,
        Posters,
        FirstPosterRole,
        SecondPosterRole,
        ThirdPosterRole,
        OtherReleasesRole
    };

public:
    explicit ReleaseLinkedSeries(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    QString nameFilter() const { return m_nameFilter; }
    void setNameFilter(const QString& nameFilter) noexcept;
    Q_INVOKABLE void refreshSeries();
    Q_INVOKABLE bool isReleaseInSeries(int id);
    Q_INVOKABLE void filterSeries();
    Q_INVOKABLE void clearFilters();

private:
    QString getSeriesCachePath() const noexcept;
    QString getReleasesCachePath() const noexcept;
    void loadSeries();
    void createCacheFileIfNotExists() const noexcept;
    void processReleasesFromDescription(const QString& description, const QMap<QString, FullReleaseModel*>& releases, int currentRelease, const QString currentReleaseTitle, const QString& poster) noexcept;
    void saveSeries();

signals:
    void nameFilterChanged();

};

#endif // RELEASELINKEDSERIES_H
