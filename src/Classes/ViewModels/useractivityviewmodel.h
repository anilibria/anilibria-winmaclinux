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

private:
    uint64_t m_watchDurationMinutes { 0 };
    uint64_t m_watchUsingApplicationMinutes { 0 };
    QDateTime m_watchUsingApplicationStart { QDateTime::currentDateTime() };

public:
    explicit UserActivityViewModel(QObject *parent = nullptr);

    QString watchDuration() const noexcept;
    Q_INVOKABLE void addWatchDurationMinute() noexcept;

    QString watchUsingApplication() const noexcept;
    Q_INVOKABLE void saveUsingApplication() noexcept;

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

};

#endif // USERACTIVITYVIEWMODEL_H
