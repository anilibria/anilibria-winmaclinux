#ifndef MAINMENUITEMMODEL_H
#define MAINMENUITEMMODEL_H

#include <QString>

class MainMenuItemModel
{

private:
    int m_index { -1 };
    QString m_title { "" };
    QString m_pageName { "" };
    QString m_icon { "" };

public:
    MainMenuItemModel(int index, QString title, QString pageName, QString icon);

    int index() const noexcept { return m_index; }
    QString title() const noexcept { return m_title; }
    QString pageName() const noexcept { return m_pageName; }
    QString icon() const noexcept { return m_icon; }

    void setIndex(const int& index) noexcept;
    void setTitle(const QString& title) noexcept;
    void setPageName(const QString& pageName) noexcept;
    void setIcon(const QString& icon) noexcept;

};

#endif // MAINMENUITEMMODEL_H
