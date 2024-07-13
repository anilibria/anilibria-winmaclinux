#include <QString>
#include <QJsonArray>
#include <QJsonObject>

QString getCachePath(const QString& filename) noexcept;

void createIfNotExistsFile(const QString& path, const QString& defaultContent) noexcept;

void createIfNotExistsCacheFile(const QString& fileName, const QString& defaultContent) noexcept;

QString removeFileProtocol(QString& path) noexcept;

QString getJsonContentFromFile(const QString& path) noexcept;

void saveJsonArrayToFile(const QString& path, const QJsonArray& array) noexcept;

void saveJsonObjectToFile(const QString& path, const QJsonObject& object) noexcept;

QString getLeadingZeroDigit(int number) noexcept;

bool readJsonObjectFromFile(const QString &path, QJsonObject &object) noexcept;
