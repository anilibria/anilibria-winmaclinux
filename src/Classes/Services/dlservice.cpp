#include "dlservice.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonArray>
#include "../Models/releasemodel.h"
#include "../Models/onlinevideomodel.h"

DLService::DLService(QObject *parent) : QObject(parent),
    m_basedUrl("https://darklibria.it/exclusive?&page="),
    m_page(1),
    m_releasesUrls(new QList<QString>()),
    m_loadedReleases(new QList<ReleaseModel*>()),
    m_detailsIndex(0)
{

}

void DLService::synchronize()
{
    downloadReleasesPage(1);
}

void DLService::downloadReleasesPage(int number)
{
    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl(m_basedUrl + QString::number(number)));

    connect(networkManager,&QNetworkAccessManager::finished,this,&DLService::downloadedReleasePage);

    networkManager->get(request);
}

void DLService::downloadDetailsPage()
{
    if (m_detailsIndex >= m_releasesUrls->length()) {
        allSynchronized();
        return;
    }

    auto networkManager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("https://" + m_releasesUrls->value(m_detailsIndex)));

    qDebug() << m_releasesUrls->value(m_detailsIndex);

    connect(networkManager,&QNetworkAccessManager::finished,this,&DLService::downloadedDetailsPage);

    networkManager->get(request);
}

QString DLService::removeTags(const QString &source, const QString &header) const
{
    auto string = source;
    auto &cleanString = string
            .replace("<ul class=\"list-unstyled\">", "")
            .replace("<li>", "")
            .replace("</li>", "")
            .replace("<span>", "")
            .replace("</span>", "")
            .replace("<a>", "")
            .replace("</a>", "")
            .replace("<span class=\"text-justify\">", "")
            .replace("<span class=\"font-weight-bold\">", "")
            .replace(header, "");

    while (true) {
        auto startIndex = cleanString.indexOf("<");
        if (startIndex == -1) break;

        auto endIndex = cleanString.indexOf(">");

        auto tempString = cleanString.mid(0, startIndex) + cleanString.mid(endIndex + 1);
        cleanString = tempString;
    }

    return cleanString;
}

void DLService::downloadedReleasePage(QNetworkReply *reply)
{
    QString data = reply->readAll();

    QVariant statusCodeAttribute = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    auto statusCode = statusCodeAttribute.toInt();

    if (data.length() == 0 || statusCode != 200 || data.indexOf("ничего не найдено") > -1) {
        m_page = 1;
        downloadDetailsPage();
        return;
    }

    data = data.replace("\n", "");

    QRegExp releasePageRegexp("tv1.darklibria.it\\/release\\/[a-z0-9\\-]{0,}");
    int pos = 0;

    while ((pos = releasePageRegexp.indexIn(data, pos)) != -1) {
        auto link = releasePageRegexp.cap(0);
        if (!m_releasesUrls->contains(link)) m_releasesUrls->append(link);
        pos += releasePageRegexp.matchedLength();
    }

    m_page += 1;
    downloadReleasesPage(m_page);
}

void DLService::downloadedDetailsPage(QNetworkReply *reply)
{
    QString data = reply->readAll();

    QVariant statusCodeAttribute = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    auto statusCode = statusCodeAttribute.toInt();

    if (data.length() == 0 || statusCode != 200) {
        m_detailsIndex++;
        return;
    }

    data = data.replace("\n", "");

    QString nameKey("Название: ");
    QString originalKey("Оригинальное название: ");
    QString releaseKindKey("Вид релиза: ");
    QString seasonKey("Сезон: ");
    QString voicesKey("Озвучка: ");
    QString genresKey("Жанры: ");
    QString descriptionKey("Описание: ");

    auto releaseModel = new ReleaseModel();

    auto nameIndex = data.indexOf(nameKey);
    auto originalNameIndex = data.indexOf(originalKey);
    auto releaseKindIndex = data.indexOf(releaseKindKey);
    auto seasonIndex = data.indexOf(seasonKey);
    auto voicesIndex = data.indexOf(voicesKey);
    auto genresIndex = data.indexOf(genresKey);
    auto descriptionIndex = data.indexOf(descriptionKey);
    auto endDescriptionIndex = data.indexOf("</li>", descriptionIndex) + 5;
    auto startSeriesIndex = data.indexOf("file: ", descriptionIndex) + 7;
    auto endSeriesIndex = data.indexOf("}]", startSeriesIndex);

    releaseModel->setTitle(removeTags(data.midRef(nameIndex, originalNameIndex - nameIndex).toString(), nameKey));
    releaseModel->addToNames(releaseModel->title());
    releaseModel->addToNames(removeTags(data.midRef(originalNameIndex, releaseKindIndex - originalNameIndex).toString(), originalKey));
    releaseModel->setType(removeTags(data.midRef(releaseKindIndex, seasonIndex - releaseKindIndex).toString(), releaseKindKey));
    auto seasonAndYear = removeTags(data.midRef(seasonIndex, voicesIndex - seasonIndex).toString(), seasonKey);
    auto seasonParts = seasonAndYear.split(" ");
    if (seasonParts.length() == 2) {
        releaseModel->setSeason(seasonParts[0]);
        releaseModel->setYear(seasonParts[1]);
    }
    auto voices = removeTags(data.midRef(voicesIndex, genresIndex - voicesIndex).toString(), voicesKey);
    auto voicesParts = voices.split(",");
    foreach (auto voice, voicesParts)
    {
        releaseModel->addToVoices(voice);
    }

    auto genres = removeTags(data.midRef(genresIndex, descriptionIndex - genresIndex).toString(), genresKey);
    auto genreParts = genres.split(",");
    foreach (auto genre, genreParts)
    {
        releaseModel->addToGenres(genre);
    }

    auto description = removeTags(data.midRef(descriptionIndex, endDescriptionIndex - descriptionIndex).toString(), descriptionKey);
    releaseModel->setDescription(description);

    auto url = reply->request().url().toString();
    releaseModel->setCode(url.mid(url.lastIndexOf("/") + 1));

    QRegExp releasePageRegexp("release\\/350x500\\/[\\d]{0,}\\.jpg");
    if (releasePageRegexp.indexIn(data, 0) != -1) {
        auto posterLink = releasePageRegexp.cap(0);
        releaseModel->setPoster("/upload/" + posterLink);
        auto id = posterLink.replace("release/350x500/", "").replace(".jpg", "").toInt();
        releaseModel->setId(id);
    }

    auto seriesJson = data.mid(startSeriesIndex, endSeriesIndex).toUtf8();
    //WORAROUND: need run it once again, I don't know why it don't work for first time :(
    auto endSeriesIndexForScript = seriesJson.lastIndexOf("}]");//, startSeriesIndex);
    seriesJson = (seriesJson.mid(0, endSeriesIndexForScript) + "}]");
    seriesJson = seriesJson.replace("\\\"", "\"");

    auto seriesDocument = QJsonDocument::fromJson(seriesJson);
    auto seriesArray = seriesDocument.array();
    foreach (auto item, seriesArray) {
        OnlineVideoModel video;
        video.setTitle(item["title"].toString());
        video.setId(item["id"].toString().toInt());
        auto file = item["file"].toString();
        file = file.replace("[Среднее]", "").replace("[Низкое]", "");
        auto qualities = file.split(",");
        video.setHd(qualities[0]);
        video.setSd(qualities[1]);
        video.setSourceHd("");
        video.setSourceSd("");
        video.setOrder(0);

        releaseModel->addToVideos(video);
    }

    releaseModel->setStatus("Завершен");

    m_loadedReleases->append(releaseModel);

    m_detailsIndex++;

    downloadDetailsPage();
}
