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
    QString m_posterHost { "" };
    QString m_title { "" };
    int64_t m_totalSeconds { 0 };
    int32_t m_totalEpisodes { 0 };
    double m_totalRating { 0.0 };
    QString m_totalSecondsDisplay { "" };
    int m_countSeens { 0 };

public:
    explicit ReleaseSeriesModel();
    ~ReleaseSeriesModel();

    QList<QVariant>* releaseIds() const { return m_releaseIds; }
    QList<QVariant>* posters() const { return m_posters; }
    QString firstPoster() const { return m_posters->size() > 0 ? m_posterHost + m_posters->first().toString() : ""; }
    QString secondPoster() const { return m_posters->size() > 1 ? m_posterHost + m_posters->value(1).toString() : ""; }
    QString thirdPoster() const { return m_posters->size() > 2 ? m_posterHost + m_posters->value(2).toString() : ""; }
    QStringList* titles() const { return m_titles.get(); }
    QString titleAsString();
    qint32 countReleases() const { return m_countReleases; }
    QStringList* genres() const { return m_genres.get(); }
    QString genresAsString();
    int sumOfRatings() const { return m_sumOfRatings; }
    QString years() const { return m_years; }
    QString seasons() const { return m_seasons; }
    int sumOfSeeds() const { return m_sumOfSeeds; }
    QString posterHost() const { return m_posterHost; }
    QString title() const { return m_title; }
    int64_t totalSeconds() const { return m_totalSeconds; }
    QString totalSecondsDisplay() const { return m_totalSecondsDisplay; }
    int32_t totalEpisodes() const { return m_totalEpisodes; }
    qreal totalRating() const { return m_totalRating; }
    int countSeens() const { return m_countSeens; }

    void recalculateCountReleases();
    bool appendReleaseId(const int id);
    void appendPoster(const QString& poster);
    void appendTitle(const QString& title);
    void appendGenre(const QString& genre);
    void setSumOfRatings(int sumOfRatings) noexcept { m_sumOfRatings = sumOfRatings; }
    void setYears(QString years) noexcept { m_years = years; }
    void setSeasons(QString seasons) noexcept { m_seasons = seasons; }
    void setSumOfSeeds(int sumOfSeeds) noexcept { m_sumOfSeeds = sumOfSeeds; }
    void setPosterHost(const QString& posterHost) noexcept { m_posterHost = posterHost; }
    void setTitle(const QString& title) noexcept { m_title = title; }
    void setTotalSeconds(int64_t value) noexcept { m_totalSeconds = value; }
    void setTotalEpisodes(int32_t value) noexcept { m_totalEpisodes = value; }
    void setTotalRating(double value) noexcept { m_totalRating = value; }
    void setCountSeens(int value) noexcept { m_countSeens = value; }

    void readFromJson(const QJsonObject &jsonObject) noexcept;
};

#endif // RELEASESERIESMODEL_H
