#include "externalplaylistvideo.h"

ExternalPlaylistVideo::ExternalPlaylistVideo() : m_Order(0), m_Address(""), m_Name("")
{

}

int ExternalPlaylistVideo::order() const
{
    return m_Order;
}

QString ExternalPlaylistVideo::address() const
{
    return m_Address;
}

QString ExternalPlaylistVideo::name() const
{
    return m_Name;
}

void ExternalPlaylistVideo::setOrder(int order)
{
    m_Order = order;
}

void ExternalPlaylistVideo::setAddress(const QString &address)
{
    m_Address = address;
}

void ExternalPlaylistVideo::setName(const QString &name)
{
    m_Name = name;
}

QString ExternalPlaylistVideo::exportToM3U()
{
    return "# EXTINF:-1, " + m_Name + "\n" + m_Address + "\n\n";
}

QString ExternalPlaylistVideo::exportToMPCPL()
{
    QString order = QString::number(m_Order);
    return order + ",type,0\n" + order + ",label," + m_Name + "\n" + order + ",filename," + m_Address + "\n";
}

bool ExternalPlaylistVideo::operator==(const ExternalPlaylistVideo &comparedModel)
{
    return m_Order == comparedModel.order();
}
