#ifndef OFFLINEIMAGECACHESERVICE_H
#define OFFLINEIMAGECACHESERVICE_H

#include <QObject>
#include <QHash>
#include <QDir>
#include <QStandardPaths>
#include "imageloader.h"

class OfflineImageCacheService : public QObject
{
    Q_OBJECT
private:
    QHash<int, QString>* m_Images;
    QHash<int, ImageLoader*>* m_RunningLoading;
    QString m_CachePath;
    QString m_Protocol;

private:
    void loadCache();
    void imageLoaded(QString loadedPath, int id);

public:
    explicit OfflineImageCacheService(QObject *parent = nullptr);

    void invalidateReleasePoster(int id);

    Q_INVOKABLE QString getReleasePath(int id, QString posterPath);

signals:

};

#endif // OFFLINEIMAGECACHESERVICE_H
