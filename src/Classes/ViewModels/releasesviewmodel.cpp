#include <QClipboard>
#include <QGuiApplication>
#include <QPixmap>
#include <QImageReader>
#include "releasesviewmodel.h"
#include "../../globalhelpers.h"

ReleasesViewModel::ReleasesViewModel(QObject *parent) : QObject(parent)
{
    m_items = new ReleasesListModel(m_releases, this);

    m_imageBackgroundViewModel->setOptionFilePath("releasesbackground");

    createIfNotExistsFile(getCachePath(releasesCacheFileName), "[]");
}

void ReleasesViewModel::setCountReleases(int countReleases) noexcept
{
    if (countReleases == m_countReleases) return;

    m_countReleases = countReleases;
    emit countReleasesChanged();
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

QString ReleasesViewModel::getVkontakteCommentPage(const QString &code) const noexcept
{
    return "https://vk.com/widget_comments.php?app=5315207&width=100%&_ver=1&limit=8&norealtime=0&url=https://www.anilibria.tv/release/" + code + ".html";
}

void ReleasesViewModel::loadReleases()
{
    m_releases->clear();

    QFile releasesCacheFile(getCachePath(releasesCacheFileName));

    releasesCacheFile.open(QFile::ReadOnly | QFile::Text);

    QString releasesJson = releasesCacheFile.readAll();
    releasesCacheFile.close();
    auto releasesArray = QJsonDocument::fromJson(releasesJson.toUtf8()).array();

    foreach (auto release, releasesArray) {
        FullReleaseModel* jsonRelease = new FullReleaseModel();
        jsonRelease->readFromJson(release);

        m_releases->append(jsonRelease);
    }

    setCountReleases(m_releases->count());
}
