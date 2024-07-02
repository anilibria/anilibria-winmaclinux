#include "releaseonlinevideomodel.h"

ReleaseOnlineVideoModel::ReleaseOnlineVideoModel()
{

}

void ReleaseOnlineVideoModel::readFromApiModel(const QJsonObject &jsonObject, int releaseId)
{
    m_uniqueId = jsonObject.value("id").toString();
    auto ordinal = jsonObject.value("ordinal").toDouble(0);
    m_number = ordinal;
    m_title = "Серия " + QString::number(ordinal);
    m_releaseId = releaseId;
    m_sd = jsonObject.value("hls_480").toString();
    m_hd = jsonObject.value("hls_720").toString();
    m_fullhd = jsonObject.value("hls_1080").toString();
    m_order = jsonObject.value("sortOrder").toInt();

    auto previewObject = jsonObject.value("preview").toObject();
    if (previewObject.contains("src")) m_videoPoster = previewObject.value("src").toString();

    auto openingObject = jsonObject.value("opening").toObject();
    if (openingObject.contains("start")) m_openingStartSeconds = openingObject.value("start").toInt();
    if (openingObject.contains("stop")) m_openingEndSeconds = openingObject.value("stop").toInt();

    auto endingObject = jsonObject.value("opening").toObject();
    if (endingObject.contains("start")) m_endingStartSeconds = endingObject.value("start").toInt();
    if (endingObject.contains("stop")) m_endingEndSeconds = endingObject.value("stop").toInt();

    if (jsonObject.contains("rutubeId")) m_rutubeId = jsonObject.value("rutubeId").toString();
    //if (jsonObject.contains("youtubeId")) m_youtubeId = jsonObject.value("youtubeId").toString();
    m_description = jsonObject.value("name").toString();
}
