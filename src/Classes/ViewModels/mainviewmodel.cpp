#include <QDesktopServices>
#include <QUrl>
#include <QDebug>
#include "mainviewmodel.h"

MainViewModel::MainViewModel(QObject *parent) : QObject(parent)
{
    connect(m_mainMenuListModel, &MainMenuListModel::selectedItem, this, &MainViewModel::selectedItemInMainMenu);
    m_mainMenuListModel->setAuthorizeItemVisible(false);
}

void MainViewModel::setNotVisibleSignin(bool notVisibleSignin)
{
    if (m_notVisibleSignin == notVisibleSignin) return;

    m_notVisibleSignin = notVisibleSignin;
    emit notVisibleSigninChanged();

    m_mainMenuListModel->setAuthorizeItemVisible(notVisibleSignin);
}

void MainViewModel::selectedItemInMainMenu(int index, QString pageName)
{
    int supportProject = 7;
    if (index != supportProject) {
        emit pageShowed(pageName);
    }

    if (index == supportProject) QDesktopServices::openUrl(QUrl("https://www.anilibria.tv/pages/donate.php"));
}
