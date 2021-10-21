#include <QFile>
#include <QDir>
#include <QStandardPaths>
#include <QJsonDocument>
#include <QJsonObject>
#include <QVariant>
#include <QDateTime>
#include <QTime>
#include <math.h>
#include "useractivityviewmodel.h"

UserActivityViewModel::UserActivityViewModel(QObject *parent) : QObject(parent)
{
    createIfNotExistsFile(getCachePath(), "{}");

    readUserActivity();
}

QString UserActivityViewModel::watchDuration() const noexcept
{
    if (m_watchDurationMinutes == 0) return "Нет данных";
    auto data = getDataFromMinutes(m_watchDurationMinutes);

    return getDisplayTime(std::get<0>(data), std::get<1>(data), std::get<2>(data));
}

void UserActivityViewModel::addWatchDurationMinute() noexcept
{
    m_watchDurationMinutes += 1;
    emit watchDurationChanged();

    writeUserActivity();
}

QString UserActivityViewModel::watchUsingApplication() const noexcept
{
    if (m_watchUsingApplicationMinutes == 0) return "Нет данных";

    auto data = getDataFromMinutes(m_watchUsingApplicationMinutes);

    return getDisplayTime(std::get<0>(data), std::get<1>(data), std::get<2>(data));
}

void UserActivityViewModel::saveUsingApplication() noexcept
{
    auto now = QDateTime::currentDateTime();
    auto difference = m_watchUsingApplicationStart.secsTo(now);
    m_watchUsingApplicationMinutes += difference / 60;
    writeUserActivity();
}

void UserActivityViewModel::createIfNotExistsFile(const QString& path, const QString& defaultContent) const noexcept
{
    if (QFile::exists(path)) return;

    QFile file(path);
    file.open(QFile::WriteOnly | QFile::Text);
    file.write(defaultContent.toUtf8());
    file.close();
}

QString UserActivityViewModel::getCachePath() const noexcept
{
    QString fileName = "/useractivity.cache";
    if (IsPortable) {
        return QDir::currentPath() + fileName;
    } else {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + fileName;
    }
}

void UserActivityViewModel::readUserActivity() noexcept
{
    QFile file(getCachePath());
    file.open(QFile::ReadOnly | QFile::Text);
    auto json = file.readAll();
    file.close();

    auto document = QJsonDocument::fromJson(json);
    auto object = document.object();
    auto watchDuration = object.contains("watchDuration") ? object["watchDuration"].toVariant().toULongLong() : 0;
    m_watchDurationMinutes = static_cast<uint64_t>(watchDuration);
    auto watchUsingApplication = object.contains("watchUsingApplication") ? object["watchUsingApplication"].toVariant().toULongLong() : 0;
    m_watchUsingApplicationMinutes = static_cast<uint64_t>(watchUsingApplication);
}

void UserActivityViewModel::writeUserActivity() noexcept
{
    QJsonObject object;
    object["watchDuration"] = static_cast<long long>(m_watchDurationMinutes);
    object["watchUsingApplication"] = static_cast<long long>(m_watchUsingApplicationMinutes);

    QFile file(getCachePath());
    file.open(QFile::WriteOnly | QFile::Text);
    file.write(QJsonDocument(object).toJson());
    file.close();
}

std::tuple<int, int, int> UserActivityViewModel::getDataFromMinutes(uint64_t minutes) const
{
    auto seconds = minutes * 60;
    int days = floor(seconds / (3600 * 24));
    seconds -= days * 3600 * 24;
    int hours = floor(seconds / 3600);
    seconds -= hours * 3600;
    int innerMinutes = floor(seconds / 60);

    return std::make_tuple(days, hours, innerMinutes);
}

QString UserActivityViewModel::getDisplayTime(int days, int hours, int minutes) const
{
    QString result = "";
    if (days > 0) result += "дней " + QString::number(days) + " ";
    if (hours > 0) result += "часов " + QString::number(hours) + " ";
    if (minutes > 0) result += "минут " + QString::number(minutes);

    return result;
}
