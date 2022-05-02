/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2021 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MYANILIBRIALISTMODEL_H
#define MYANILIBRIALISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QMap>
#include <QList>
#include <QSet>
#include "../ViewModels/userconfigurationviewmodel.h"

class MyAnilibriaListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(UserConfigurationViewModel* userConfiguration READ userConfiguration WRITE setUserConfiguration NOTIFY userConfigurationChanged)
private:
    QList<QString> m_sections { QList<QString>() };
    QSet<QString> m_fullSections { QSet<QString>() };
    QMap<QString, QString> m_sectionTitles { QMap<QString, QString>() };
    UserConfigurationViewModel* m_userConfiguration { nullptr };
    enum MyAnilibriaSectionRole {
        SectionIdRole = Qt::UserRole + 1,
        SectionTitleRole,
        ComponentIdRole,
        HeaderVisibleRole
    };

public:
    explicit MyAnilibriaListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const override;

    UserConfigurationViewModel* userConfiguration() const noexcept { return m_userConfiguration; }
    void setUserConfiguration(const UserConfigurationViewModel* userConfiguration) noexcept;

    void fillSections(const QSet<QString>& sections) noexcept;
    QStringList getSections() const noexcept;

    void setSectionHideMark(const QString& section, const bool notVisible) noexcept;

    Q_INVOKABLE void toggleSectionHideMark(const QString& section);
    Q_INVOKABLE void setNotVisibleAllMarks(const bool visible);

private:
    bool getHideSection(const QString& section) const noexcept;

signals:
    void userConfigurationChanged();
    void needSaveSections();

};

#endif // MYANILIBRIALISTMODEL_H
