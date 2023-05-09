#include <QDesktopServices>
#include <QUrl>
#include "mainviewmodel.h"

MainViewModel::MainViewModel(QObject *parent) : QObject(parent)
{
    connect(m_mainMenuListModel, &MainMenuListModel::selectedItem, this, &MainViewModel::selectedItemInMainMenu);
    m_mainMenuListModel->setAuthorizeItemVisible(false);

    m_displayNames->insert("release", "Каталог релизов");
    m_displayNames->insert("videoplayer", "Видеоплеер");
    m_displayNames->insert("myanilibria", "Моя Анилибрия");
    m_displayNames->insert("youtube", "Youtube");
    m_displayNames->insert("about", "О Программе");
    m_displayNames->insert("cinemahall", "Кинозал");
    m_displayNames->insert("download", "Менеджер загрузок");
    m_displayNames->insert("maintenance", "Обслуживание");
    m_displayNames->insert("releaseseries", "Связанные релизы");
    m_displayNames->insert("thememanager", "Менеджер Тем");
    m_displayNames->insert("authorization", "Авторизация");
    m_displayNames->insert("torrentstream", "TorrentStream");

    m_currentPageDisplayName = m_displayNames->value(m_currentPageId);

    m_history.append("release");
    m_historyPosition = 0;
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

void MainViewModel::setAnalyticsService(const AnalyticsService *service) noexcept
{
    if (m_analyticsService == service) return;

    m_analyticsService = const_cast<AnalyticsService *>(service);
    emit analyticsServiceChanged();
}

void MainViewModel::setIsSmallSizeMode(bool isSmallSizeMode) noexcept
{
    if (m_isSmallSizeMode == isSmallSizeMode) return;

    m_isSmallSizeMode = isSmallSizeMode;
    emit isSmallSizeModeChanged();
}

void MainViewModel::selectPage(const QString& pageId)
{
    if (pageId == m_currentPageId) return;

    selectToPage(pageId);

    if (!m_history.isEmpty() && m_historyPosition != m_history.count() - 1) m_history.erase(m_history.begin() + (m_historyPosition + 1), m_history.end());

    m_history.append(pageId);
    m_historyPosition += 1;
    emit hasBackHistoryChanged();
    emit hasForwardHistoryChanged();
}

void MainViewModel::backToPage()
{
    if (m_historyPosition <= 0) return;

    m_historyPosition--;

    selectToPage(m_history.value(m_historyPosition));
    emit hasBackHistoryChanged();
    emit hasForwardHistoryChanged();
}

void MainViewModel::forwardToPage()
{
    if (m_historyPosition >= m_history.count() - 1) return;

    m_historyPosition++;

    selectToPage(m_history.value(m_historyPosition));
    emit hasBackHistoryChanged();
    emit hasForwardHistoryChanged();
}

void MainViewModel::setPageDisplayName(const QString &pageId) noexcept
{
    setCurrentPageDisplayName(m_displayNames->value(pageId, ""));
}

void MainViewModel::refreshPageVisible() noexcept
{
    emit isReleasesPageVisibleChanged();
    emit isOnlinePlayerPageVisibleChanged();
    emit isYoutubePageVisibleChanged();
    emit isAboutPageVisibleChanged();
    emit isCinemahallPageVisibleChanged();
    emit isDownloadPageVisibleChanged();
    emit isMaintenancePageVisibleChanged();
    emit isReleasesSeriesPageVisibleChanged();
    emit isAuthorizationPageVisibleChanged();
    emit isMyAnilibriaPageVisibleChanged();
    emit isThemeManagerVisibleChanged();
    emit isTorrentStreamPageVisibleChanged();
}

void MainViewModel::selectToPage(const QString &pageId)
{
    if (pageId == m_currentPageId) return;

    auto alreadyOnVideoPlayer = m_currentPageId == "videoplayer";

    setCurrentPageId(pageId);

    refreshPageVisible();

    if (pageId == "videoplayer"){
        emit onlinePlayerPageNavigated();
    }
    if (alreadyOnVideoPlayer) {
        emit onlinePlayerPageFromNavigated();
    }

    if (pageId == "release") {
        emit releasesPageToNavigated();
    }

    m_analyticsService->sendView("page", "view", "%2F" + pageId);
}

void MainViewModel::selectedItemInMainMenu(QString pageName)
{
    if (!pageName.isEmpty()) {
        selectPage(pageName);
        return;
    }

    QDesktopServices::openUrl(QUrl("https://www.anilibria.tv/pages/donate.php"));
}
