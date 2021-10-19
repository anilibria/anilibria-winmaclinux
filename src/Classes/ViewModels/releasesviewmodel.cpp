#include <QClipboard>
#include <QGuiApplication>
#include <QPixmap>
#include <QImageReader>
#include "releasesviewmodel.h"

ReleasesViewModel::ReleasesViewModel(QObject *parent) : QObject(parent)
{

}

QString ReleasesViewModel::getScheduleDay(const QString &dayNumber) const noexcept
{
    if (dayNumber.isEmpty()) return "";

    auto day = dayNumber.toInt();
    switch (day){
        case 1: return QString("понедельник");
        case 2: return QString("вторник");
        case 3: return QString("среда");
        case 4: return QString("четверг");
        case 5: return QString("пятница");
        case 6: return QString("суббота");
        case 7: return QString("воскресенье");
        default: return "";
    }
}

void ReleasesViewModel::copyToClipboard(const QString &text) const noexcept
{
    if (text.isEmpty()) return;

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(text);
}

void ReleasesViewModel::copyImageToClipboard(const QString &imagePath) const
{
    if (imagePath.isEmpty()) return;
    auto pathToImage = imagePath;
#ifdef Q_OS_WIN
    pathToImage = pathToImage.replace("file:///", "");
#else
    pathToImage = pathToImage.replace("file://", "");
#endif

    QPixmap pixmap;
    if (!pixmap.load(pathToImage, "jpg")) return;

    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setPixmap(pixmap);
}
