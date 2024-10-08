#include "globalhelpers.h"
#include <QString>
#include <QDir>
#include <QStandardPaths>
#include <QRegularExpression>
#include <QJsonDocument>
#include "globalconstants.h"

QString getCachePath(const QString& filename) noexcept
{
    QString cacheFileName = "/" + filename;
    if (IsPortable) {
        return QDir::currentPath() + cacheFileName;
    } else {
        return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation) + cacheFileName;
    }
}

void createIfNotExistsFile(const QString& path, const QString& defaultContent) noexcept
{
    if (QFile::exists(path)) return;

    QFile file(path);
    file.open(QFile::WriteOnly | QFile::Text);
    file.write(defaultContent.toUtf8());
    file.close();
}

void createIfNotExistsCacheFile(const QString& fileName, const QString& defaultContent) noexcept
{
    auto fullPath = getCachePath(fileName);
    if (QFile::exists(fullPath)) return;

    QFile file(fullPath);
    file.open(QFile::WriteOnly | QFile::Text);
    file.write(defaultContent.toUtf8());
    file.close();
}


QString removeFileProtocol(QString &path) noexcept
{
#ifdef Q_OS_WIN
    return path.replace("file:///", "");
#else
    return path.replace("file://", "");
#endif
}

QString addFileProtocol(QString& path) noexcept
{
#ifdef Q_OS_WIN
    return "file:///" + path;
#else
    return "file://" + path;
#endif
}

QString getJsonContentFromFile(const QString& path) noexcept {
    QFile releasesCacheFile(getCachePath(path));

    if (!releasesCacheFile.open(QFile::ReadOnly | QFile::Text)) return "";

    QString releasesJson = releasesCacheFile.readAll();
    releasesCacheFile.close();

    return releasesJson;
}


void saveJsonArrayToFile(const QString &path, const QJsonArray &array) noexcept
{
    QJsonDocument document(array);

    QFile scheduleCacheFile(getCachePath(path));
    scheduleCacheFile.open(QFile::WriteOnly | QFile::Text);
    scheduleCacheFile.write(document.toJson());
    scheduleCacheFile.close();
}

QString getLeadingZeroDigit(int number) noexcept
{
    auto result = QString::number(number);
    if (number < 10) result.insert(0, "0");

    return result;
}

void saveJsonObjectToFile(const QString &path, const QJsonObject &object) noexcept
{
    QJsonDocument document(object);

    QFile scheduleCacheFile(getCachePath(path));
    scheduleCacheFile.open(QFile::WriteOnly | QFile::Text);
    scheduleCacheFile.write(document.toJson());
    scheduleCacheFile.close();
}

bool readJsonObjectFromFile(const QString &path, QJsonObject &object) noexcept
{
    QFile file(path);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        auto content = file.readAll();
        file.close();

        object = QJsonDocument::fromJson(content).object();
        return true;
    }

    return false;
}
