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

private:
    enum ThemeFieldRoles {
        IdRole = Qt::UserRole + 1,
        NameRole,
        TitleRole,
        DescriptionRole,
        ValueRole,
        IsDefinedRole,
        FieldTypeRole
    };
    QList<QString> m_colorFields { QList<QString>() };
    QString m_themeName { "" };
    QString m_basedOnTheme { "" };
    bool m_isBlankTheme { true };
    bool m_isEditTheme { false };
    bool m_isCopyTheme { false };
    QMap<QString, QString> m_values { QMap<QString, QString>() };
    QMap<QString, QString> m_descriptions { QMap<QString, QString>() };

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

    QString editMode() const noexcept;

    Q_INVOKABLE void createBlankTheme() noexcept;

signals:
    void themeNameChanged();
    void basedOnThemeChanged();
    void editModeChanged();

};

#endif // THEMEFIELDLISTMODEL_H
