#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include <QObject>
#include <QMap>
#include "../ListModels/mainmenulistmodel.h"
#include "../Services/analyticsservice.h"

class MainViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MainMenuListModel* mainMenuListModel READ mainMenuListModel NOTIFY mainMenuListModelChanged)
    Q_PROPERTY(AnalyticsService* analyticsService READ analyticsService WRITE setAnalyticsService NOTIFY analyticsServiceChanged)
    Q_PROPERTY(bool notVisibleSignin READ notVisibleSignin WRITE setNotVisibleSignin NOTIFY notVisibleSigninChanged)
    Q_PROPERTY(QString currentPageId READ currentPageId WRITE setCurrentPageId NOTIFY currentPageIdChanged)
    Q_PROPERTY(QString currentPageDisplayName READ currentPageDisplayName WRITE setCurrentPageDisplayName NOTIFY currentPageDisplayNameChanged)
    Q_PROPERTY(bool isReleasesPageVisible READ isReleasesPageVisible NOTIFY isReleasesPageVisibleChanged)
    Q_PROPERTY(bool isOnlinePlayerPageVisible READ isOnlinePlayerPageVisible NOTIFY isOnlinePlayerPageVisibleChanged)
    Q_PROPERTY(bool isYoutubePageVisible READ isYoutubePageVisible NOTIFY isYoutubePageVisibleChanged)
    Q_PROPERTY(bool isAboutPageVisible READ isAboutPageVisible NOTIFY isAboutPageVisibleChanged)
    Q_PROPERTY(bool isCinemahallPageVisible READ isCinemahallPageVisible NOTIFY isCinemahallPageVisibleChanged)
    Q_PROPERTY(bool isDownloadPageVisible READ isDownloadPageVisible NOTIFY isDownloadPageVisibleChanged)
    Q_PROPERTY(bool isMaintenancePageVisible READ isMaintenancePageVisible NOTIFY isMaintenancePageVisibleChanged)
    Q_PROPERTY(bool isReleasesSeriesPageVisible READ isReleasesSeriesPageVisible NOTIFY isReleasesSeriesPageVisibleChanged)
    Q_PROPERTY(bool isMyAnilibriaPageVisible READ isMyAnilibriaPageVisible NOTIFY isMyAnilibriaPageVisibleChanged)
    Q_PROPERTY(bool isAuthorizationPageVisible READ isAuthorizationPageVisible NOTIFY isAuthorizationPageVisibleChanged)
    Q_PROPERTY(bool isThemeManagerVisible READ isThemeManagerVisible NOTIFY isThemeManagerVisibleChanged)
    Q_PROPERTY(bool isSmallSizeMode READ isSmallSizeMode WRITE setIsSmallSizeMode NOTIFY isSmallSizeModeChanged)
    Q_PROPERTY(bool isTorrentStreamPageVisible READ isTorrentStreamPageVisible NOTIFY isTorrentStreamPageVisibleChanged)
    Q_PROPERTY(bool hasBackHistory READ hasBackHistory NOTIFY hasBackHistoryChanged)
    Q_PROPERTY(bool hasForwardHistory READ hasForwardHistory NOTIFY hasForwardHistoryChanged)
    Q_PROPERTY(QString startPage READ startPage WRITE setStartPage NOTIFY startPageChanged)
    Q_PROPERTY(QString pageParameters READ pageParameters NOTIFY pageParametersChanged)
    Q_PROPERTY(QVariantList leftToolbar READ leftToolbar NOTIFY leftToolbarChanged)

private:
    MainMenuListModel* m_mainMenuListModel { new MainMenuListModel(this) };
    bool m_notVisibleSignin { false };
    QString m_currentPageId { "release" };
    QString m_currentPageDisplayName { "" };
    QMap<QString, QString>* m_displayNames { new QMap<QString, QString>() };
    AnalyticsService* m_analyticsService { nullptr };
    bool m_isSmallSizeMode { false };
    QStringList m_history { QStringList() };
    int m_historyPosition { -1 };
    QString m_startPage { "" };
    bool m_startPageFilled { false };
    QString m_pageParameters { "" };
    const QString toolbarItemCacheFileName { "lefttoolbar.cache" };
    QVariantList m_leftToolbar { QVariantList() };

public:
    explicit MainViewModel(QObject *parent = nullptr);
    MainMenuListModel* mainMenuListModel() const noexcept { return m_mainMenuListModel; };

    bool notVisibleSignin() const noexcept { return m_notVisibleSignin; }
    void setNotVisibleSignin(bool notVisibleSignin);

    QString currentPageId() const noexcept { return m_currentPageId; }
    void setCurrentPageId(const QString& currentPageId) noexcept;

    QString currentPageDisplayName() const noexcept { return m_currentPageDisplayName; }
    void setCurrentPageDisplayName(const QString& currentPageDisplayName) noexcept;

    AnalyticsService* analyticsService() const noexcept { return m_analyticsService; }
    void setAnalyticsService(const AnalyticsService* service) noexcept;

    bool isSmallSizeMode() const noexcept { return m_isSmallSizeMode; }
    void setIsSmallSizeMode(bool isSmallSizeMode) noexcept;

    bool hasBackHistory() const noexcept { return !m_history.isEmpty() && m_historyPosition > 0; }
    bool hasForwardHistory() const noexcept { return !m_history.isEmpty() && m_historyPosition < m_history.count() - 1; }

    bool isReleasesPageVisible() const noexcept { return m_currentPageId == "release"; }
    bool isOnlinePlayerPageVisible() const noexcept { return m_currentPageId == "videoplayer"; }
    bool isYoutubePageVisible() const noexcept { return m_currentPageId == "youtube"; }
    bool isAboutPageVisible() const noexcept { return m_currentPageId == "about"; }
    bool isCinemahallPageVisible() const noexcept { return m_currentPageId == "cinemahall"; }
    bool isDownloadPageVisible() const noexcept { return m_currentPageId == "download"; }
    bool isMaintenancePageVisible() const noexcept { return m_currentPageId == "maintenance"; }
    bool isReleasesSeriesPageVisible() const noexcept { return m_currentPageId == "releaseseries"; }
    bool isMyAnilibriaPageVisible() const noexcept { return m_currentPageId == "myanilibria"; }
    bool isAuthorizationPageVisible() const noexcept { return m_currentPageId == "authorization"; }
    bool isThemeManagerVisible() const noexcept { return m_currentPageId == "thememanager"; }
    bool isTorrentStreamPageVisible() const noexcept { return m_currentPageId == "torrentstream"; }
    QVariantList leftToolbar() const noexcept { return m_leftToolbar; }

    QString startPage() const noexcept { return m_startPage; }
    void setStartPage(const QString& startPage) noexcept;

    QString pageParameters() const noexcept { return m_pageParameters; }

    Q_INVOKABLE void selectPage(const QString& pageId);
    Q_INVOKABLE void backToPage();
    Q_INVOKABLE void forwardToPage();

private:
    void setPageDisplayName(const QString& pageId) noexcept;
    void refreshPageVisible() noexcept;
    void selectToPage(const QString& pageId);
    void loadLeftToolbar();

public slots:
    void selectedItemInMainMenu(QString pageName);

signals:
    void mainMenuListModelChanged();
    void pageShowed(const QString& pageName);
    void notVisibleSigninChanged();
    void currentPageIdChanged();
    void currentPageDisplayNameChanged();
    void isReleasesPageVisibleChanged();
    void isOnlinePlayerPageVisibleChanged();
    void isYoutubePageVisibleChanged();
    void isCinemahallPageVisibleChanged();
    void isAboutPageVisibleChanged();
    void isDownloadPageVisibleChanged();
    void isMaintenancePageVisibleChanged();
    void isReleasesSeriesPageVisibleChanged();
    void isMyAnilibriaPageVisibleChanged();
    void onlinePlayerPageNavigated();
    void onlinePlayerPageFromNavigated();
    void isAuthorizationPageVisibleChanged();
    void analyticsServiceChanged();
    void releasesPageToNavigated();
    void isThemeManagerVisibleChanged();
    void isSmallSizeModeChanged();
    void isTorrentStreamPageVisibleChanged();
    void hasBackHistoryChanged();
    void hasForwardHistoryChanged();
    void startPageChanged();
    void pageParametersChanged();
    void changeReleasesParameters(QString parameters);
    void releasesSeriesPageToNavigated();
    void changeReleaseSeriesParameters(QString parameters);
    void leftToolbarChanged();

};

#endif // MAINVIEWMODEL_H
