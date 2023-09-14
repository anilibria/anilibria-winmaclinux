#include "globalhelpers.h"
#include <QString>
#include <QDir>
#include <QStandardPaths>
#include <QRegularExpression>
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

QString removeFileProtocol(QString &path) noexcept
{
    return path.replace("file:///", "").replace("file://", "");
}

bool isRutubeHasVideos(const QString& videos) noexcept {
    static QRegularExpression re(R"arg("rutube_id\": \")arg");
    QRegularExpressionMatch match = re.match(videos);

    return match.hasMatch();
}

QString getJsonContentFromFile(const QString& path) noexcept {
    QFile releasesCacheFile(getCachePath(path));

    if (!releasesCacheFile.open(QFile::ReadOnly | QFile::Text)) return "";

    QString releasesJson = releasesCacheFile.readAll();
    releasesCacheFile.close();

    return releasesJson;
}

