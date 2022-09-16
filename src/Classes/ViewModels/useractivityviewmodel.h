#ifndef USERACTIVITYVIEWMODEL_H
#define USERACTIVITYVIEWMODEL_H

#include <QObject>
#include <QDateTime>
#include "../../globalconstants.h"

class UserActivityViewModel : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString watchDuration READ watchDuration NOTIFY watchDurationChanged)
    Q_PROPERTY(QString watchUsingApplication READ watchUsingApplication NOTIFY watchUsingApplicationChanged)
    Q_PROPERTY(int countOpenedReleaseCard READ countOpenedReleaseCard NOTIFY countOpenedReleaseCardChanged)
    Q_PROPERTY(int countDownloadTorrent READ countDownloadTorrent NOTIFY countDownloadTorrentChanged)
    Q_PROPERTY(int countAddedToCinemahall READ countAddedToCinemahall NOTIFY countAddedToCinemahallChanged)
    Q_PROPERTY(int previousApplicationStart READ previousApplicationStart NOTIFY previousApplicationStartChanged)

private:
    uint64_t m_watchDurationMinutes { 0 };
    uint64_t m_watchUsingApplicationMinutes { 0 };
    int m_countOpenedReleaseCard { 0 };
    int m_countDownloadTorrent { 0 };
    QDateTime m_watchUsingApplicationStart { QDateTime::currentDateTime() };
    int m_countAddedToCinemahall { 0 };
    int m_previousApplicationStart { 0 };

public:
    explicit UserActivityViewModel(QObject *parent = nullptr);

    QString watchDuration() const noexcept;
    Q_INVOKABLE void addWatchDurationMinute() noexcept;

    QString watchUsingApplication() const noexcept;
    Q_INVOKABLE void saveUsingApplication() noexcept;

    int countOpenedReleaseCard() const noexcept { return m_countOpenedReleaseCard; }
    Q_INVOKABLE void addOpenedCardToCounter() noexcept;

    int countDownloadTorrent() const noexcept { return m_countDownloadTorrent; }
    Q_INVOKABLE void addDownloadedTorrentToCounter() noexcept;

    int countAddedToCinemahall() const noexcept { return m_countAddedToCinemahall; }
    Q_INVOKABLE void addCinemahallMarkToCounter(const int count) noexcept;

    int previousApplicationStart() const noexcept { return m_previousApplicationStart; }

    Q_INVOKABLE void refreshUserActivity() noexcept;

private:
    void createIfNotExistsFile(const QString& path, const QString& defaultContent) const noexcept;
    QString getCachePath() const noexcept;
    void readUserActivity() noexcept;
    void writeUserActivity() noexcept;
    std::tuple<int, int, int> getDataFromMinutes(uint64_t minutes) const;
    QString getDisplayTime(int days, int hours, int minutes) const;

signals:
    void watchDurationChanged();
    void watchUsingApplicationChanged();
    void countOpenedReleaseCardChanged();
    void countDownloadTorrentChanged();
    void countAddedToCinemahallChanged();
    void previousApplicationStartChanged();

};

#endif // USERACTIVITYVIEWMODEL_H
