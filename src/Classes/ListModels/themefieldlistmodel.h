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

#ifndef THEMEFIELDLISTMODEL_H
#define THEMEFIELDLISTMODEL_H

#include <QAbstractListModel>
#include <QMap>

class ThemeFieldListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(QString themeName READ themeName WRITE setThemeName NOTIFY themeNameChanged)
    Q_PROPERTY(QString basedOnTheme READ basedOnTheme WRITE setBasedOnTheme NOTIFY basedOnThemeChanged)
    Q_PROPERTY(QString editMode READ editMode NOTIFY editModeChanged)
    Q_PROPERTY(int selectedIndex READ selectedIndex WRITE setSelectedIndex NOTIFY selectedIndexChanged)
    Q_PROPERTY(bool hasValues READ hasValues NOTIFY hasValuesChanged)
    Q_PROPERTY(QStringList saveMenuItems READ saveMenuItems NOTIFY saveMenuItemsChanged)
    Q_PROPERTY(QString filter READ filter WRITE setFilter NOTIFY filterChanged)

private:
    enum ThemeFieldRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        TitleRole,
        DescriptionRole,
        ValueRole,
        IsDefinedRole,
        FieldTypeRole,
        TooltipOverrideRole
    };
    QList<QString> m_colorFields { QList<QString>() };
    QList<QString> m_filteredColorFields { QList<QString>() };
    QString m_themeName { "" };
    QString m_basedOnTheme { "" };
    bool m_isBlankTheme { true };
    bool m_isEditTheme { false };
    bool m_isCopyTheme { false };
    QMap<QString, QString> m_values { QMap<QString, QString>() };
    QMap<QString, QString> m_descriptions { QMap<QString, QString>() };
    QStringList m_saveMenuItems { QStringList() };
    int m_selectedIndex { -1 };
    QString m_filter { "" };

public:
    explicit ThemeFieldListModel(QObject *parent = nullptr);

    void fillFields(const QList<QString>& colors);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int,QByteArray> roleNames() const override;

    QString themeName() const noexcept { return m_themeName; }
    void setThemeName(const QString& themeName) noexcept;

    QString basedOnTheme() const noexcept { return m_basedOnTheme; }
    void setBasedOnTheme(const QString& basedOnTheme) noexcept;

    int selectedIndex() const noexcept { return m_selectedIndex; }
    void setSelectedIndex(int selectedIndex) noexcept;

    QString editMode() const noexcept;

    bool hasValues() const noexcept { return !m_values.isEmpty() && !m_themeName.isEmpty() && !m_basedOnTheme.isEmpty(); }

    QMap<QString, QString> getValues() const noexcept { return m_values; }

    QStringList saveMenuItems() const noexcept { return m_saveMenuItems; }

    QString filter() const noexcept { return m_filter; }
    void setFilter(const QString& filter) noexcept;

    void setValues(QMap<QString, QString>&& values, const QString& name, const QString& basedTheme) noexcept;

    void refresh() noexcept;

    Q_INVOKABLE void createBlankTheme() noexcept;
    Q_INVOKABLE void setValueToItem(QString value) noexcept;
    Q_INVOKABLE void setValueToItemByIndex(int index, QString value) noexcept;
    Q_INVOKABLE void undefineField(int itemIndex) noexcept;
    Q_INVOKABLE void defineField(int itemIndex) noexcept;
    Q_INVOKABLE void toggleDefinedField(int itemIndex) noexcept;
    Q_INVOKABLE void saveThemeToFile(const QString& path) noexcept;
    Q_INVOKABLE void addIconFromFile(const QString& path, int itemIndex) noexcept;

private:
    void refreshRowByField(const QString& field) noexcept;

signals:
    void themeNameChanged();
    void basedOnThemeChanged();
    void editModeChanged();
    void selectedIndexChanged();
    void hasValuesChanged();
    void saveMenuItemsChanged();
    void filterChanged();
    void errorMessage(const QString& message);

};

#endif // THEMEFIELDLISTMODEL_H
