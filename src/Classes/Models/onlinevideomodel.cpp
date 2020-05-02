#include "onlinevideomodel.h"

OnlineVideoModel::OnlineVideoModel()
{

}

void OnlineVideoModel::readFromApiModel(const QJsonObject &jsonObject)
{
    m_Id = jsonObject.value("id").toInt();
    m_Title = jsonObject.value("title").toString();
    m_SD = jsonObject.value("sd").toString();
    m_HD = jsonObject.value("hd").toString();
    m_FullHD = jsonObject.value("fullhd").toString();
    m_SourceSD = jsonObject.value("srcSd").toString();
    m_SourceHD = jsonObject.value("srcHd").toString();
}

void OnlineVideoModel::writeToJson(QJsonObject &json) const noexcept
{
    json["id"] = m_Id;
    json["title"] = m_Title;
    json["sd"] = m_SD;
    json["hd"] = m_HD;
    json["fullhd"] = m_FullHD;
    json["srcSd"] = m_SourceSD;
    json["srcHd"] = m_SourceHD;
}

void OnlineVideoModel::setId(const int id) noexcept
{
    m_Id = id;
}

void OnlineVideoModel::setTitle(const QString &title) noexcept
{
    m_Title = title;
}

void OnlineVideoModel::setSd(const QString &sd) noexcept
{
    m_SD = sd;
}

void OnlineVideoModel::setHd(const QString &hd) noexcept
{
    m_HD = hd;
}

void OnlineVideoModel::setFullHd(const QString &fullhd) noexcept
{
    m_FullHD = fullhd;
}

void OnlineVideoModel::setSourceSd(const QString &sourcesd) noexcept
{
    m_SourceSD = sourcesd;
}

void OnlineVideoModel::setSourceHd(const QString &sourcehd) noexcept
{
    m_SourceHD = sourcehd;
}
