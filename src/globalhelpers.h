#include <QString>
#include "globalconstants.h"

QString getCachePath(const QString& filename) noexcept;

void createIfNotExistsFile(const QString& path, const QString& defaultContent) noexcept;
