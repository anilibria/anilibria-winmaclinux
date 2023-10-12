#ifndef FilterDictionariesViewModel_H
#define FilterDictionariesViewModel_H

#include <QObject>
#include <QSet>
#include "releasesviewmodel.h"

class FilterDictionariesViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ReleasesViewModel* releases READ releases WRITE setReleases NOTIFY releasesChanged)
    Q_PROPERTY(QStringList years READ years NOTIFY yearsChanged)
    Q_PROPERTY(QStringList genres READ genres NOTIFY genresChanged)
    Q_PROPERTY(QStringList statuses READ statuses NOTIFY statusesChanged)
    Q_PROPERTY(QStringList voices READ voices NOTIFY voicesChanged)
    Q_PROPERTY(QStringList seasons READ seasons NOTIFY seasonsChanged)

private:
    ReleasesViewModel* m_releases { nullptr };
    QStringList m_years { QStringList() };
    QStringList m_genres { QStringList() };
    QStringList m_statuses { QStringList() };
    QStringList m_voices { QStringList() };
    QStringList m_seasons { QStringList() };

public:
    explicit FilterDictionariesViewModel(QObject *parent = nullptr);

    ReleasesViewModel* releases() const noexcept { return m_releases; }
    void setReleases(const ReleasesViewModel* releases) noexcept;

    QStringList years() const noexcept { return m_years; };
    QStringList genres() const noexcept { return m_genres; };
    QStringList statuses() const noexcept { return m_statuses; };
    QStringList voices() const noexcept { return m_voices; };
    QStringList seasons() const noexcept { return m_seasons; };

    Q_INVOKABLE void refreshDictionaries();

private:
    void fillDictionaries();

signals:
    void releasesChanged();
    void yearsChanged();
    void genresChanged();
    void statusesChanged();
    void voicesChanged();
    void seasonsChanged();

};

#endif // FilterDictionariesViewModel_H
