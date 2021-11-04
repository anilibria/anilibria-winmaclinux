#include "mainmenuitemmodel.h"

MainMenuItemModel::MainMenuItemModel(int index, QString title, QString pageName, QString icon)
{
    m_index = index;
    m_title = title;
    m_pageName = pageName;
    m_icon = icon;
}

void MainMenuItemModel::setIndex(const int &index) noexcept
{
    m_index = index;
}

void MainMenuItemModel::setTitle(const QString &title) noexcept
{
    m_title = title;
}

void MainMenuItemModel::setPageName(const QString &pageName) noexcept
{
    m_pageName = pageName;
}

void MainMenuItemModel::setIcon(const QString &icon) noexcept
{
    m_icon = icon;
}
