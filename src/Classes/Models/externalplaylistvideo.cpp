#include "externalplaylistvideo.h"

ExternalPlaylistVideo::ExternalPlaylistVideo()
    : m_Order(0)
    , m_Address("")
    , m_Name("")
{
}

void ExternalPlaylistVideo::setOrder(int order) noexcept
{
    m_Order = order;
}

void ExternalPlaylistVideo::setAddress(const QString &address) noexcept
{
    m_Address = address;
}

void ExternalPlaylistVideo::setName(const QString &name) noexcept
{
    m_Name = name;
}

QString ExternalPlaylistVideo::exportToM3U() noexcept
{
    return "# EXTINF:-1, " + m_Name + "\n" + m_Address + "\n\n";
}

QString ExternalPlaylistVideo::exportToMPCPL() noexcept
{
    QString order = QString::number(m_Order);
    return order + ",type,0\n" + order + ",label," + m_Name + "\n" + order + ",filename," + m_Address + "\n";
}

bool ExternalPlaylistVideo::operator==(const ExternalPlaylistVideo &comparedModel) noexcept
{
    return m_Order == comparedModel.order();
}
