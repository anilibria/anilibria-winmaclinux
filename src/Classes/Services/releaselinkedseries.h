#ifndef RELEASELINKEDSERIES_H
#define RELEASELINKEDSERIES_H


#include <QObject>
#include <QAbstractListModel>
#include "../Models/fullreleasemodel.h"
#include "../Models/releaseseriesmodel.h"

class ReleaseLinkedSeries : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(qint32 countSeries READ countSeries WRITE setCountSeries NOTIFY countSeriesChanged)
private:
    qint32 m_countSeries;
    QVector<ReleaseSeriesModel*>* m_series;

    enum ItemRoles {
        CountReleasesRole = Qt::UserRole + 1,
        FirstNameRole,
        SecondNameRole,
        ThirdNameRole,
        ReleaseIds
    };

public:
    explicit ReleaseLinkedSeries(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    qint32 countSeries() const { return m_countSeries; }
    void setCountSeries(const qint32& countSeries ) noexcept;
    Q_INVOKABLE void refreshSeries();
    Q_INVOKABLE bool isReleaseInSeries(int id);

private:
    QString getSeriesCachePath() const noexcept;
    QString getReleasesCachePath() const noexcept;
    void loadSeries();
    void createCacheFileIfNotExists() const noexcept;
    void setSeriaName(int index, QString name, ReleaseSeriesModel& model);
    void processReleasesFromDescription(const QString& description, const QMap<QString, FullReleaseModel*>& releases, int currentRelease, const QString currentReleaseTitle) noexcept;
    void saveSeries();

signals:
    void countSeriesChanged();

};

#endif // RELEASELINKEDSERIES_H
