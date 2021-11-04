#ifndef MAINVIEWMODEL_H
#define MAINVIEWMODEL_H

#include <QObject>
#include "../ListModels/mainmenulistmodel.h"

class MainViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(MainMenuListModel* mainMenuListModel READ mainMenuListModel NOTIFY mainMenuListModelChanged)
    Q_PROPERTY(bool notVisibleSignin READ notVisibleSignin WRITE setNotVisibleSignin NOTIFY notVisibleSigninChanged)

private:
    MainMenuListModel* m_mainMenuListModel { new MainMenuListModel(this) };
    bool m_notVisibleSignin { false };

public:
    explicit MainViewModel(QObject *parent = nullptr);
    MainMenuListModel* mainMenuListModel() const noexcept { return m_mainMenuListModel; };

    bool notVisibleSignin() const noexcept { return m_notVisibleSignin; }
    void setNotVisibleSignin(bool notVisibleSignin);

public slots:
    void selectedItemInMainMenu(int index, QString pageName);

signals:
    void mainMenuListModelChanged();
    void pageShowed(const QString& pageName);
    void notVisibleSigninChanged();

};

#endif // MAINVIEWMODEL_H
