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

private:
    MainMenuListModel* m_mainMenuListModel { new MainMenuListModel(this) };
    bool m_notVisibleSignin { false };
    QString m_currentPageId { "release" };
    QString m_currentPageDisplayName { "" };
    QMap<QString, QString>* m_displayNames { new QMap<QString, QString>() };
    AnalyticsService* m_analyticsService { nullptr };

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

    Q_INVOKABLE void selectPage(const QString& pageId);

private:
    void setPageDisplayName(const QString& pageId) noexcept;
    void refreshPageVisible() noexcept;

public slots:
    void selectedItemInMainMenu(int index, QString pageName);

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

};

#endif // MAINVIEWMODEL_H
