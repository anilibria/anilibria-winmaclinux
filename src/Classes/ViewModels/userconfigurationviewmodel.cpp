#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include "userconfigurationviewmodel.h"
#include "../../globalhelpers.h"

UserConfigurationViewModel::UserConfigurationViewModel(QObject *parent) : QObject(parent)
{
    createIfNotExistsFile(getCachePath(m_cacheFileName), "{}");
    readSettingsFromFile();
}

void UserConfigurationViewModel::setNotCloseReleaseCardAfterWatch(const bool notCloseReleaseCardAfterWatch) noexcept
{
    if (m_notCloseReleaseCardAfterWatch == notCloseReleaseCardAfterWatch) return;

    m_notCloseReleaseCardAfterWatch = notCloseReleaseCardAfterWatch;
    emit notCloseReleaseCardAfterWatchChanged();
}

void UserConfigurationViewModel::setOpacityPlayerPanel(int opacityPlayerPanel) noexcept
{
    if (m_opacityPlayerPanel == opacityPlayerPanel) return;

    m_opacityPlayerPanel = opacityPlayerPanel;
    emit opacityPlayerPanelChanged();
}

void UserConfigurationViewModel::saveSettingsToFile()
{
    QJsonObject object;
    object[m_opacityPlayerPanelField] = m_opacityPlayerPanel;
    object[m_notCloseReleaseCardAfterWatchField] = m_notCloseReleaseCardAfterWatch;

    QFile file(getCachePath(m_cacheFileName));
    file.open(QFile::WriteOnly | QFile::Text);
    file.write(QJsonDocument(object).toJson());
    file.close();
}

void UserConfigurationViewModel::readSettingsFromFile()
{
    QFile file(getCachePath(m_cacheFileName));
    file.open(QFile::ReadOnly | QFile::Text);
    auto json = file.readAll();
    file.close();

    auto document = QJsonDocument::fromJson(json);
    auto object = document.object();
    m_opacityPlayerPanel = object.contains(m_opacityPlayerPanelField) ? object[m_opacityPlayerPanelField].toInt() : 50;
    m_notCloseReleaseCardAfterWatch =  object.contains(m_notCloseReleaseCardAfterWatchField) ? object[m_notCloseReleaseCardAfterWatchField].toBool() : false;
}
