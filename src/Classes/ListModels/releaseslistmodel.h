#ifndef RELEASESLISTMODEL_H
#define RELEASESLISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QSet>
#include "../Models/fullreleasemodel.h"
#include "../Models/changesmodel.h"
#include "../Models/historymodel.h"

class ReleasesListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString titleFilter READ titleFilter WRITE setTitleFilter NOTIFY titleFilterChanged)
    Q_PROPERTY(QString descriptionFilter READ descriptionFilter WRITE setDescriptionFilter NOTIFY descriptionFilterChanged)
    Q_PROPERTY(QString typeFilter READ typeFilter WRITE setTypeFilter NOTIFY typeFilterChanged)
    Q_PROPERTY(QString yearsFilter READ yearsFilter WRITE setYearsFilter NOTIFY yearsFilterChanged)
    Q_PROPERTY(QString statusesFilter READ statusesFilter WRITE setStatusesFilter NOTIFY statusesFilterChanged)
    Q_PROPERTY(QString seasonesFilter READ seasonesFilter WRITE setSeasonesFilter NOTIFY seasonesFilterChanged)
    Q_PROPERTY(QString genresFilter READ genresFilter WRITE setGenresFilter NOTIFY genresFilterChanged)
    Q_PROPERTY(bool genresFilterOr READ genresFilterOr WRITE setGenresFilterOr NOTIFY genresFilterOrChanged)
    Q_PROPERTY(QString voicesFilter READ voicesFilter WRITE setVoicesFilter NOTIFY voicesFilterChanged)
    Q_PROPERTY(bool voicesFilterOr READ voicesFilterOr WRITE setVoicesFilterOr NOTIFY voicesFilterOrChanged)
    Q_PROPERTY(QString alphabetsFilter READ alphabetsFilter WRITE setAlphabetsFilter NOTIFY alphabetsFilterChanged)
    Q_PROPERTY(int favoriteMarkFilter READ favoriteMarkFilter WRITE setFavoriteMarkFilter NOTIFY favoriteMarkFilterChanged)
    Q_PROPERTY(int seenMarkFilter READ seenMarkFilter WRITE setSeenMarkFilter NOTIFY seenMarkFilterChanged)
    Q_PROPERTY(int section READ section WRITE setSection NOTIFY sectionChanged)
    Q_PROPERTY(int sortingField READ sortingField WRITE setSortingField NOTIFY sortingFieldChanged)
    Q_PROPERTY(bool sortingDescending READ sortingDescending WRITE setSortingDescending NOTIFY sortingDescendingChanged)
    Q_PROPERTY(bool isHasReleases READ isHasReleases NOTIFY isHasReleasesChanged)

private:
    QSharedPointer<QList<FullReleaseModel*>> m_releases;
    QScopedPointer<QList<FullReleaseModel*>> m_filteredReleases { new QList<FullReleaseModel*>() };
    QVector<int>* m_userFavorites { nullptr };
    QHash<QString, bool>* m_seenMarkModels { nullptr };
    QVector<int>* m_hiddenReleases { nullptr };
    QSharedPointer<ChangesModel> m_changesModel { nullptr };
    QMap<int, int>* m_scheduleReleases { nullptr };
    QSharedPointer<QHash<int, HistoryModel*>> m_historyModels { nullptr };
    QString m_titleFilter { "" };
    QString m_descriptionFilter { "" };
    QString m_typeFilter { "" };
    QString m_yearsFilter { "" };
    QString m_statusesFilter { "" };
    QString m_seasonesFilter { "" };
    QString m_genresFilter { "" };
    bool m_genresFilterOr { false };
    QString m_voicesFilter { "" };
    bool m_voicesFilterOr { false };
    QString m_alphabetsFilter { "" };
    int m_favoriteMarkFilter { 0 };
    int m_seenMarkFilter { 0 };
    int m_section { 0 };
    int m_sortingField { 0 };
    bool m_sortingDescending { true };
    bool m_isHasReleases { false };
    QScopedPointer<QSet<int>> m_selectedReleases { new QSet<int>() };
    enum FullReleaseRoles {
        ReleaseIdRole = Qt::UserRole + 1,
        TitleRole,
        StatusRole,
        SeasonRole,
        YearRole,
        TypeRole,
        GenresRole,
        VoicesRole,
        CountVideosRole,
        CountSeensMarksRole,
        DescriptionRole,
        PosterRole,
        CountTorrentRole,
        VideosRole,
        RatingRole,
        InFavoritesRole,
        SelectedRole,
        InScheduleRole,
        ScheduledDayRole
    };

    enum FilterSortingField {
        ReleaseTimestamp = 0,
        ReleaseScheduleDay,
        ReleaseTitle,
        ReleaseYear,
        ReleaseRating,
        ReleaseStatus,
        ReleaseOriginalName,
        ReleaseHistory,
        ReleaseWatchHistory,
        ReleaseSeason,
        ReleaseFavoriteMark,
        ReleaseSeenMark
    };

public:
    explicit ReleasesListModel(QObject *parent = nullptr);

    void setup(QSharedPointer<QList<FullReleaseModel*>> releases, QMap<int, int>* schedules, QVector<int>* userFavorites, QVector<int>* hidedReleases, QHash<QString, bool>* seenMarks, QSharedPointer<QHash<int, HistoryModel*>> historyItems, QSharedPointer<ChangesModel> changes);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const override;

    QString titleFilter() const noexcept { return m_titleFilter; }
    void setTitleFilter(const QString& titleFilter) noexcept;

    QString descriptionFilter() const noexcept { return m_descriptionFilter; }
    void setDescriptionFilter(const QString& descriptionFilter) noexcept;

    QString typeFilter() const noexcept { return m_typeFilter; }
    void setTypeFilter(const QString& typeFilter) noexcept;

    QString yearsFilter() const noexcept { return m_yearsFilter; }
    void setYearsFilter(const QString& yearsFilter) noexcept;

    QString statusesFilter() const noexcept { return m_statusesFilter; }
    void setStatusesFilter(const QString& statusesFilter) noexcept;

    QString seasonesFilter() const noexcept { return m_seasonesFilter; }
    void setSeasonesFilter(const QString& seasonesFilter) noexcept;

    QString genresFilter() const noexcept { return m_genresFilter; }
    void setGenresFilter(const QString& genresFilter) noexcept;

    bool genresFilterOr() const noexcept { return m_genresFilterOr; }
    void setGenresFilterOr(bool genresFilterOr) noexcept;

    QString voicesFilter() const noexcept { return m_voicesFilter; }
    void setVoicesFilter(const QString& voicesFilter) noexcept;

    bool voicesFilterOr() const noexcept { return m_voicesFilterOr; }
    void setVoicesFilterOr(bool voicesFilterOr) noexcept;

    QString alphabetsFilter() const noexcept { return m_alphabetsFilter; }
    void setAlphabetsFilter(QString alphabetsFilter) noexcept;

    int favoriteMarkFilter() const noexcept { return m_favoriteMarkFilter; }
    void setFavoriteMarkFilter(int favoriteMarkFilter) noexcept;

    int seenMarkFilter() const noexcept { return m_seenMarkFilter; }
    void setSeenMarkFilter(int seenMarkFilter) noexcept;

    int section() const noexcept { return m_section; }
    void setSection(int section) noexcept;

    int sortingField() const noexcept { return m_sortingField; }
    void setSortingField(int sortingField) noexcept;

    int sortingDescending() const noexcept { return m_sortingDescending; }
    void setSortingDescending(bool sortingDescending) noexcept;

    bool isHasReleases() const noexcept { return m_isHasReleases; }
    void setIsHasReleases(const bool& isHasReleases) noexcept;

    void refreshItem(int id);
    int getReleaseSeenMarkCount(int releaseId) const noexcept;

    QString getScheduleDay(int dayNumber) const noexcept;

    Q_INVOKABLE void refresh();
    Q_INVOKABLE void selectItem(int id);
    Q_INVOKABLE void deselectItem(int id);
    Q_INVOKABLE void clearSelected();

private:
    void removeTrimsInStringCollection(QStringList& list);
    bool checkOrCondition(const QStringList& source, const QStringList& target);
    bool checkAllCondition(const QStringList& source, const QStringList& target);
    QHash<int, int>&& getAllSeenMarkCount(QHash<int, int>&& result) noexcept;
    void sortingFilteringReleases(QHash<int, int>&& seenMarks);
    void refreshFilteredReleaseById(int id);

signals:
    void titleFilterChanged();
    void descriptionFilterChanged();
    void typeFilterChanged();
    void yearsFilterChanged();
    void statusesFilterChanged();
    void seasonesFilterChanged();
    void genresFilterChanged();
    void genresFilterOrChanged();
    void voicesFilterChanged();
    void voicesFilterOrChanged();
    void alphabetsFilterChanged();
    void favoriteMarkFilterChanged();
    void seenMarkFilterChanged();
    void sectionChanged();
    void sortingFieldChanged();
    void sortingDescendingChanged();
    void isHasReleasesChanged();

};

#endif // RELEASESLISTMODEL_H
