#ifndef RELEASESERIESMODEL_H
#define RELEASESERIESMODEL_H

#include <QObject>
#include <QJsonObject>
#include <QVariant>

class ReleaseSeriesModel
{

private:
    QList<QVariant>* m_releaseIds;
    QList<QVariant>* m_posters;
    QScopedPointer<QStringList> m_titles { new QStringList() };
    QScopedPointer<QStringList> m_genres { new QStringList() };
    qint32 m_countReleases;

public:
    explicit ReleaseSeriesModel();
    ~ReleaseSeriesModel();

    QList<QVariant>* releaseIds() const { return m_releaseIds; }
    QList<QVariant>* posters() const { return m_posters; }
    QStringList* titles() const { return m_titles.get(); }
    qint32 countReleases() const { return m_countReleases; }
    QStringList* genres() const { return m_genres.get(); }
    QString genresAsString() const { return m_genres->join(", "); }

    void recalculateCountReleases();
    bool appendReleaseId(const int id);
    void appendPoster(const QString& poster);
    void appendTitle(const QString& title);
    void appendGenre(const QString& genre);

    void readFromJson(const QJsonObject &jsonObject) noexcept;
    void writeToJson(QJsonObject &json) const noexcept;
};

#endif // RELEASESERIESMODEL_H
