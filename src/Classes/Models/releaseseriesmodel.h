#ifndef RELEASESERIESMODEL_H
#define RELEASESERIESMODEL_H

#include <QObject>
#include <QJsonObject>
#include <QVariant>

class ReleaseSeriesModel
{

private:
    QList<QVariant>* m_releaseIds { new QList<QVariant>() };
    QList<QVariant>* m_posters { new QList<QVariant>() };
    QScopedPointer<QStringList> m_titles { new QStringList() };
    QScopedPointer<QStringList> m_genres { new QStringList() };
    qint32 m_countReleases { 0 };
    QString m_genresAsString { "" };
    QString m_titlesAsString { "" };
    int m_sumOfRatings { 0 };
    QString m_years { "" };
    QString m_seasons { "" };
    int m_sumOfSeeds { 0 };

public:
    explicit ReleaseSeriesModel();
    ~ReleaseSeriesModel();

    QList<QVariant>* releaseIds() const { return m_releaseIds; }
    QList<QVariant>* posters() const { return m_posters; }
    QStringList* titles() const { return m_titles.get(); }
    QString titleAsString();
    qint32 countReleases() const { return m_countReleases; }
    QStringList* genres() const { return m_genres.get(); }
    QString genresAsString();
    int sumOfRatings() const { return m_sumOfRatings; }
    QString years() const { return m_years; }
    QString seasons() const { return m_seasons; }
    int sumOfSeeds() const { return m_sumOfSeeds; }

    void recalculateCountReleases();
    bool appendReleaseId(const int id);
    void appendPoster(const QString& poster);
    void appendTitle(const QString& title);
    void appendGenre(const QString& genre);
    void setSumOfRatings(int sumOfRatings) noexcept { m_sumOfRatings = sumOfRatings; }
    void setYears(QString years) noexcept { m_years = years; }
    void setSeasons(QString seasons) noexcept { m_seasons = seasons; }
    void setSumOfSeeds(int sumOfSeeds) noexcept { m_sumOfSeeds = sumOfSeeds; }

    void readFromJson(const QJsonObject &jsonObject) noexcept;
    void writeToJson(QJsonObject &json) const noexcept;
};

#endif // RELEASESERIESMODEL_H
