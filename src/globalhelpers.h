#include <QString>

QString getCachePath(const QString& filename) noexcept;

void createIfNotExistsFile(const QString& path, const QString& defaultContent) noexcept;

QString removeFileProtocol(QString& path) noexcept;

bool isRutubeHasVideos(const QString& videos) noexcept;

QString getJsonContentFromFile(const QString& path) noexcept;
