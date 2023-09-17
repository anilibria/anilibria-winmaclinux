#include <QDesktopServices>
#include <QUrl>
#include "mainviewmodel.h"
#include "../../globalhelpers.h"

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

    createIfNotExistsFile(getCachePath(toolbarItemCacheFileName), "[\"release\",\"videoplayer\",\"cinemahall\",\"releaseseries\",\"myanilibria\"]");

    loadLeftToolbar();

    m_currentPageDisplayName = m_displayNames->value(m_currentPageId);

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

void MainViewModel::setStartPage(const QString &startPage) noexcept
{
    if (m_startPage == startPage) return;

    m_startPage = startPage;
    emit startPageChanged();

    if (!m_startPageFilled) {
        m_history.append(startPage);
        m_startPageFilled = true;
    }
}

void MainViewModel::selectPage(const QString& pageId) noexcept
{
    auto pageIdentifier = QString(pageId);
    auto parameters = QString("");
    if (pageIdentifier.contains(":")) {
        pageIdentifier = pageIdentifier.mid(0, pageIdentifier.indexOf(":"));
        parameters = pageId.mid(pageId.indexOf(":") + 1);
    } else {
        parameters = "";
    }

    //fix duplicates for changing page
    if (pageIdentifier == m_currentPageId && parameters == m_pageParameters) return;

    if (pageIdentifier != m_currentPageId) selectToPage(pageIdentifier);

    if (!m_history.isEmpty() && m_historyPosition != m_history.count() - 1) m_history.erase(m_history.begin() + (m_historyPosition + 1), m_history.end());

    m_history.append(pageId);
    m_historyPosition += 1;
    emit hasBackHistoryChanged();
    emit hasForwardHistoryChanged();

    m_pageParameters = parameters;
    if (pageIdentifier == "release") emit changeReleasesParameters(m_pageParameters);
    if (pageIdentifier == "releaseseries") emit changeReleaseSeriesParameters(m_pageParameters);

    m_analyticsService->sendView("page", "view", "%2F" + pageIdentifier);
}

void MainViewModel::backToPage() noexcept
{
    if (m_historyPosition <= 0) return;

    m_historyPosition--;

    selectToPage(m_history.value(m_historyPosition));
    emit hasBackHistoryChanged();
    emit hasForwardHistoryChanged();
}

void MainViewModel::forwardToPage() noexcept
{
    if (m_historyPosition >= m_history.count() - 1) return;

    m_historyPosition++;

    selectToPage(m_history.value(m_historyPosition));
    emit hasBackHistoryChanged();
    emit hasForwardHistoryChanged();
}

void MainViewModel::toggleEditToolBarMode() noexcept
{
    m_editLeftToolbar = !m_editLeftToolbar;
    emit editLeftToolbarChanged();

    if (m_editLeftToolbar) {
        QVariantMap map;
        map.insert("identifier", "additem");
        map.insert("title", "Добавить страницу в тулбар");
        map.insert("itemIcon", "iconEditThemeFieldPlus");
        m_leftToolbar.append(map);
    } else {
        m_leftToolbar.removeAt(m_leftToolbar.count() - 1);
    }

    emit leftToolbarChanged();
}

void MainViewModel::addOptionToToolbar(int index) noexcept
{
    auto item = m_otherLeftToolbar.value(index).toMap();

    auto id = item.value("value").toString();
    QVariantMap map;
    map.insert("identifier", id);
    map.insert("title", item.value("key"));
    map.insert("itemIcon", m_mainMenuListModel->getIcon(id));
    m_leftToolbar.insert(m_leftToolbar.count() - 1, map);
    m_otherLeftToolbar.removeAt(index);
    emit leftToolbarChanged();
    emit otherLeftToolbarChanged();
}

void MainViewModel::removeOptionFromToolbar(const QString &id) noexcept
{
    QVariantMap toolbarItem;
    foreach (auto item, m_leftToolbar) {
        auto map = item.toMap();
        if (map.value("identifier") == id) {
            toolbarItem = map;
        }
    }
    m_leftToolbar.removeOne(toolbarItem);

    QVariantMap map;
    map.insert("key", m_displayNames->value(id));
    map.insert("value", id);
    m_otherLeftToolbar.append(map);

    emit leftToolbarChanged();
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
    auto pageIdentifier = QString(pageId);
    auto parameters = QString("");
    if (pageIdentifier.contains(":")) {
        pageIdentifier = pageIdentifier.mid(0, pageIdentifier.indexOf(":"));
        parameters = pageId.mid(pageId.indexOf(":") + 1);
    } else {
        parameters = "";
    }

    //fix duplicates for changing page
    if (pageIdentifier == m_currentPageId && parameters == m_pageParameters) return;

    auto alreadyOnVideoPlayer = m_currentPageId == "videoplayer";

    setCurrentPageId(pageIdentifier);

    refreshPageVisible();

    if (pageIdentifier == "videoplayer"){
        emit onlinePlayerPageNavigated();
    }
    if (alreadyOnVideoPlayer) {
        emit onlinePlayerPageFromNavigated();
    }

    if (pageIdentifier == "release") {
        emit releasesPageToNavigated();
        if (m_pageParameters != parameters) {
            m_pageParameters = parameters;
            emit changeReleasesParameters(m_pageParameters);
        }
    }
    if (pageIdentifier == "releaseseries") {
        emit releasesSeriesPageToNavigated();
        if (m_pageParameters != parameters) {
            m_pageParameters = parameters;
            emit changeReleaseSeriesParameters(m_pageParameters);
        }
    }
}

void MainViewModel::loadLeftToolbar()
{
    m_leftToolbar.clear();

    auto content = getJsonContentFromFile(toolbarItemCacheFileName);
    auto itemsArray = QJsonDocument::fromJson(content.toUtf8()).array();

    foreach (auto item, itemsArray) {
        if (!item.isString()) continue;

        auto id = item.toString();
        QVariantMap map;
        map.insert("identifier", id);
        map.insert("title", m_displayNames->value(id));
        map.insert("itemIcon", m_mainMenuListModel->getIcon(id));
        m_leftToolbar.append(map);
    }

    auto keys = m_displayNames->keys();
    foreach (auto displayItem, keys) {
        if (displayItem == "about" || displayItem == "authorization") continue;

        auto haveInMenu = std::find_if(
            m_leftToolbar.cbegin(),
            m_leftToolbar.cend(),
            [displayItem](QVariant variant) {
                return displayItem == variant.toMap().value("identifier");
            }
        );
        if (haveInMenu != m_leftToolbar.cend()) continue;

        QVariantMap map;
        map.insert("key", m_displayNames->value(displayItem));
        map.insert("value", displayItem);
        m_otherLeftToolbar.append(map);
    }
}

void MainViewModel::selectedItemInMainMenu(QString pageName)
{
    if (!pageName.isEmpty()) {
        selectPage(pageName);
        return;
    }

    QDesktopServices::openUrl(QUrl("https://www.anilibria.tv/pages/donate.php"));
}
