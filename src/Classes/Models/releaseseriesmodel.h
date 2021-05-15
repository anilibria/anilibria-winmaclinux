#ifndef RELEASESERIESMODEL_H
#define RELEASESERIESMODEL_H

#include <QObject>
#include <QJsonObject>

class ReleaseSeriesModel
{

private:
    QList<QVariant>* m_releaseIds;
    QList<QVariant>* m_posters;
    QList<QVariant>* m_titles;
    qint32 m_countReleases;

public:
    explicit ReleaseSeriesModel();

    QList<QVariant>* releaseIds() const { return m_releaseIds; }
    QList<QVariant>* posters() const { return m_posters; }
    QList<QVariant>* titles() const { return m_titles; }
    qint32 countReleases() const { return m_countReleases; }

    void recalculateCountReleases();
    bool appendReleaseId(const int id);
    void appendPoster(const QString& poster);
    void appendTitle(const QString& title);

    void readFromJson(const QJsonObject &jsonObject) noexcept;
    void writeToJson(QJsonObject &json) const noexcept;
};

#endif // RELEASESERIESMODEL_H
