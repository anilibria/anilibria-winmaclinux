#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include <QObject>
#include <QMap>
#include "../ListModels/mainmenulistmodel.h"

class MainViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MainMenuListModel* mainMenuListModel READ mainMenuListModel NOTIFY mainMenuListModelChanged)
    Q_PROPERTY(bool notVisibleSignin READ notVisibleSignin WRITE setNotVisibleSignin NOTIFY notVisibleSigninChanged)
    Q_PROPERTY(QString currentPageId READ currentPageId WRITE setCurrentPageId NOTIFY currentPageIdChanged)
    Q_PROPERTY(QString currentPageDisplayName READ currentPageDisplayName WRITE setCurrentPageDisplayName NOTIFY currentPageDisplayNameChanged)

private:
    MainMenuListModel* m_mainMenuListModel { new MainMenuListModel(this) };
    bool m_notVisibleSignin { false };
    QString m_currentPageId { "release" };
    QString m_currentPageDisplayName { "" };
    QMap<QString, QString>* m_displayNames { new QMap<QString, QString>() };

public:
    explicit MainViewModel(QObject *parent = nullptr);
    MainMenuListModel* mainMenuListModel() const noexcept { return m_mainMenuListModel; };

    bool notVisibleSignin() const noexcept { return m_notVisibleSignin; }
    void setNotVisibleSignin(bool notVisibleSignin);

    QString currentPageId() const noexcept { return m_currentPageId; }
    void setCurrentPageId(const QString& currentPageId) noexcept;

    QString currentPageDisplayName() const noexcept { return m_currentPageDisplayName; }
    void setCurrentPageDisplayName(const QString& currentPageDisplayName) noexcept;

private:
    void setPageDisplayName(const QString& pageId) noexcept;

public slots:
    void selectedItemInMainMenu(int index, QString pageName);

signals:
    void mainMenuListModelChanged();
    void pageShowed(const QString& pageName);
    void notVisibleSigninChanged();
    void currentPageIdChanged();
    void currentPageDisplayNameChanged();

};

#endif // MAINVIEWMODEL_H
