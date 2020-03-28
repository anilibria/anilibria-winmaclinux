#ifndef IMAGELOADER_H
#define IMAGELOADER_H

#include <QObject>
#include <QStandardPaths>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFileInfo>

class ImageLoader : public QObject
{
    Q_OBJECT

private:
    int m_Id;
    QString m_OriginalPath;
    QString m_Path;

public:
    explicit ImageLoader(QObject *parent = nullptr, QString path = "");

    void loadImage(int id, const QString& originalPath);

signals:
    void imageLoaded(QString loadedPath, int id);

public slots:
    void imageDownloaded(QNetworkReply* reply);

};

#endif // IMAGELOADER_H
