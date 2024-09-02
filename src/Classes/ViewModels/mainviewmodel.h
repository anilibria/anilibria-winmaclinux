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
    Q_PROPERTY(bool isApplicationsPageVisible READ isApplicationsPageVisible NOTIFY isApplicationsPageVisibleChanged)
    Q_PROPERTY(bool isExtensionsPageVisible READ isExtensionsPageVisible NOTIFY isExtensionsPageVisibleChanged)
    Q_PROPERTY(bool hasBackHistory READ hasBackHistory NOTIFY hasBackHistoryChanged)
    Q_PROPERTY(bool hasForwardHistory READ hasForwardHistory NOTIFY hasForwardHistoryChanged)
    Q_PROPERTY(QString startPage READ startPage WRITE setStartPage NOTIFY startPageChanged)
    Q_PROPERTY(QString pageParameters READ pageParameters NOTIFY pageParametersChanged)
    Q_PROPERTY(QVariantList leftToolbar READ leftToolbar NOTIFY leftToolbarChanged)
    Q_PROPERTY(bool editLeftToolbar READ editLeftToolbar NOTIFY editLeftToolbarChanged)
    Q_PROPERTY(QVariantList otherLeftToolbar  READ otherLeftToolbar NOTIFY otherLeftToolbarChanged)
    Q_PROPERTY(QString dropIndex READ dropIndex WRITE setDropIndex NOTIFY dropIndexChanged)
    Q_PROPERTY(QString dragIndex READ dragIndex WRITE setDragIndex NOTIFY dragIndexChanged)
    Q_PROPERTY(QString globalTextFont READ globalTextFont WRITE setGlobalTextFont NOTIFY globalTextFontChanged)
    Q_PROPERTY(QStringList fontFamilies READ fontFamilies NOTIFY fontFamiliesChanged FINAL)

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
    bool m_editLeftToolbar { false };
    QVariantList m_otherLeftToolbar { QVariantList() };
    QString m_dropIndex { "" };
    QString m_dragIndex { "" };
    QString m_globalTextFont { "" };
    QStringList m_fontFamilies { QStringList() };
    const QString addItemButton { "additem" };
    const QString removeItemButton { "removeitem" };

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
    bool isApplicationsPageVisible() const noexcept { return m_currentPageId == "applications"; }
    bool isExtensionsPageVisible() const noexcept { return m_currentPageId == "extensions"; }
    QVariantList leftToolbar() const noexcept { return m_leftToolbar; }
    bool editLeftToolbar() const noexcept { return m_editLeftToolbar; }
    QVariantList otherLeftToolbar() const noexcept { return m_otherLeftToolbar; }

    QString startPage() const noexcept { return m_startPage; }
    void setStartPage(const QString& startPage) noexcept;

    QString pageParameters() const noexcept { return m_pageParameters; }

    QString dropIndex() const noexcept { return m_dropIndex; }
    void setDropIndex(const QString& dropIndex) noexcept;

    QString dragIndex() const noexcept { return m_dragIndex; }
    void setDragIndex(const QString& dragIndex) noexcept;

    QString globalTextFont() const noexcept { return m_globalTextFont; }
    void setGlobalTextFont(const QString& globalTextFont) noexcept;

    QStringList fontFamilies() const noexcept { return m_fontFamilies; }

    Q_INVOKABLE void selectPage(const QString& pageId) noexcept;
    Q_INVOKABLE void backToPage() noexcept;
    Q_INVOKABLE void forwardToPage() noexcept;
    Q_INVOKABLE void toggleEditToolBarMode() noexcept;
    Q_INVOKABLE void addOptionToToolbar(int index) noexcept;
    Q_INVOKABLE void saveState() noexcept;
    Q_INVOKABLE void reorderMenu() noexcept;
    Q_INVOKABLE bool isControlButton(const QString& id) noexcept;

private:
    void setPageDisplayName(const QString& pageId) noexcept;
    void refreshPageVisible() noexcept;
    void selectToPage(const QString& pageId);
    void loadLeftToolbar();
    void saveLeftToolbar();
    void addOptionsToOtherToolbar(const QString& id);

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
    void changeExtensionsParameters(QString parameters);
    void leftToolbarChanged();
    void editLeftToolbarChanged();
    void otherLeftToolbarChanged();
    void dropIndexChanged();
    void dragIndexChanged();
    void globalTextFontChanged();
    void fontFamiliesChanged();
    void isApplicationsPageVisibleChanged();
    void isExtensionsPageVisibleChanged();

};

#endif // MAINVIEWMODEL_H
