#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include "mainviewmodel.h"

MainViewModel::MainViewModel(QObject *parent) : QObject(parent)
{
    connect(m_mainMenuListModel, &MainMenuListModel::selectedItem, this, &MainViewModel::selectedItemInMainMenu);
    m_mainMenuListModel->setAuthorizeItemVisible(false);

    m_displayNames->insert("release", "Каталог релизов");
    m_displayNames->insert("videoplayer", "Видеоплеер");
    m_displayNames->insert("youtube", "Youtube");
    m_displayNames->insert("about", "О Программе");
    m_displayNames->insert("cinemahall", "Кинозал");
    m_displayNames->insert("download", "Менеджер загрузок");
    m_displayNames->insert("maintenance", "Обслуживание");
    m_displayNames->insert("releaseseries", "Связанные релизы");

    m_currentPageDisplayName = m_displayNames->value(m_currentPageId);
}

void MainViewModel::setNotVisibleSignin(bool notVisibleSignin)
{
    if (m_notVisibleSignin == notVisibleSignin) return;

    m_notVisibleSignin = notVisibleSignin;
    emit notVisibleSigninChanged();

    m_mainMenuListModel->setAuthorizeItemVisible(notVisibleSignin);
}

void MainViewModel::setCurrentPageId(const QString &currentPageId) noexcept
{
    if (m_currentPageId == currentPageId) return;

    m_currentPageId = currentPageId;
    emit currentPageIdChanged();
    setPageDisplayName(currentPageId);
}

void MainViewModel::setCurrentPageDisplayName(const QString &currentPageDisplayName) noexcept
{
    if (m_currentPageDisplayName == currentPageDisplayName) return;

    m_currentPageDisplayName = currentPageDisplayName;
    emit currentPageDisplayNameChanged();
}

void MainViewModel::setPageDisplayName(const QString &pageId) noexcept
{
    setCurrentPageDisplayName(m_displayNames->value(pageId, ""));
}

void MainViewModel::selectedItemInMainMenu(int index, QString pageName)
{
    int supportProject = 7;
    if (index != supportProject) {
        emit pageShowed(pageName);
    }

    if (index == supportProject) QDesktopServices::openUrl(QUrl("https://www.anilibria.tv/pages/donate.php"));
}
