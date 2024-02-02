#include <QString>
#include <QJsonArray>

QString getCachePath(const QString& filename) noexcept;

void createIfNotExistsFile(const QString& path, const QString& defaultContent) noexcept;

void createIfNotExistsCacheFile(const QString& fileName, const QString& defaultContent) noexcept;

QString removeFileProtocol(QString& path) noexcept;

bool isRutubeHasVideos(const QString& videos) noexcept;

QString getJsonContentFromFile(const QString& path) noexcept;

void saveJsonArrayToFile(const QString& path, const QJsonArray& array) noexcept;

QString getLeadingZeroDigit(int number) noexcept;
