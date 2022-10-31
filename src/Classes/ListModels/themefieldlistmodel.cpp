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

#include "themefieldlistmodel.h"
#include "../../globalconstants.h"

ThemeFieldListModel::ThemeFieldListModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_descriptions.insert(plainTextColorField, "Цвет текста:Цвет текста для обычных описаний");
    m_descriptions.insert(headerTextColorField, "Цвет текста заголовка:Цвет текста для акцентированных мест вроде заголовков");
    m_descriptions.insert(linkTextColorField, "Цвет текста ссылки:Цвет текста для гипер ссылок");
    m_descriptions.insert(pageVerticalPanelField, "Цвет боковой панели:Фон для вертикальной боковой панели с кнопками на всех страницах");
    m_descriptions.insert(pageBackgroundField, "Цвет фона страницы:Фон для всех страниц в приложении");
    m_descriptions.insert(pageUpperPanelField, "Цвет верхней панели:Фон для верхней горизонтальной панели");
    m_descriptions.insert(panelBackgroundField, "Цвет панели элемента:Фон для прямоугольной области элемента например релиза в каталоге релизов");
    m_descriptions.insert(panelBackgroundShadowField, "Цвет тени панели элемента:Цвет тени панели элемента рядом с прямоугольной областью элемента");
    m_descriptions.insert(roundedButtonBackgroundField, "Цвет фона экшен кнопки 1:Заливка фона кнопки в состоянии Активно");
    m_descriptions.insert(roundedButtonBackgroundDisabledField, "Цвет фона экшен кнопки 2:Заливка фона кнопки в состоянии Не Активно");
    m_descriptions.insert(roundedButtonHoveredField, "Цвет фона экшен кнопки 3:Заливка фона кнопки в состоянии Мышка поверх элемента");
    m_descriptions.insert(roundedButtonForegroundField, "Цвет текста экшен кнопки:Цвет текста внутри экшен кнопки");
    m_descriptions.insert(drawerGradiendStep0Field, "Цвет фона главного меню 1:Первый цвет для определения градиента");
    m_descriptions.insert(drawerGradiendStep1Field, "Цвет фона главного меню 2:Второй цвет для определения градиента");
    m_descriptions.insert(drawerGradiendStep2Field, "Цвет фона главного меню 3:Третий цвет для определения градиента");
    m_descriptions.insert(drawerGradiendStep3Field, "Цвет фона главного меню 4:Четвертый цвет для определения градиента");
    m_descriptions.insert(drawerGradiendStep4Field, "Цвет фона главного меню 5:Пятый цвет для определения градиента");
    m_descriptions.insert(filterIconButtonColorField, "Цвет фона кнопки иконки 1:Заливка фона кнопки в состоянии Активно");
    m_descriptions.insert(filterIconButtonGreenColorField, "Цвет фона кнопки иконки 2:Заливка фона кнопки в состоянии Активировано");
    m_descriptions.insert(filterIconButtonHoverColorField, "Цвет фона кнопки иконки 3:Заливка фона кнопки в состоянии Мышка поверх элемента");
    m_descriptions.insert(selectedItemField, "Цвет выбранного элемента:Цвет рамки выбранных или акцентированных элементов");
    m_descriptions.insert(selectedFavoriteField, "Цвет выбранного элемента в избранном:Цвет рамки выбранных или акцентированных элементов в избранном");
    m_descriptions.insert(playerControlBackgroundField, "Цвет фона контрольных элементов плеера:Цвет фона интерактивных элементов плеера (контрольной и других панелей)");
    m_descriptions.insert(notificationCenterBackgroundField, "Цвет фона панелей для навигации:Цвет фона верхней (там где кнопки страниц) и нижней панелей окна (там где статистика)");
    m_descriptions.insert(notificationCenterPanelBackgroundField, "Цвет фона панелей для уведомлений:Цвен фона боковой панели уведомленй");
    m_descriptions.insert(notificationCenterItemBackgroundField, "Цвет фона панелей для элементов уведомлений:Фон сообщений в панели уведомлений");
    m_descriptions.insert(playlistSelectedBackgroundField, "Цвет фона панелей для элементов плейлиста 1: Цвет элемента плейлиста в состоянии Выделен");
    m_descriptions.insert(playlistBackgroundField, "Цвет фона панелей для элементов плейлиста 2: Цвет элемента плейлиста в состоянии Не Выделен");
    m_descriptions.insert(playlistSelectedTextField, "Цвет текста в плейлисте 1: Цвет текста в плейлисте в состоянии Выделен");
    m_descriptions.insert(playlistTextField, "Цвет текста в плейлисте 2: Цвет текста в плейлисте в состоянии Не Выделен");
    m_descriptions.insert(toggleButtonHoveredField, "Цвет кнопки переключателя 1: Цвет фона кнопки переключателя в состоянии Мышка поверх элемента");
    m_descriptions.insert(toggleButtonCheckedField, "Цвет кнопки переключателя 2: Цвет фона кнопки переключателя в состоянии Выбрано");
    m_descriptions.insert(toggleButtonNotCheckedField, "Цвет кнопки переключателя 3: Цвет фона кнопки переключателя в состоянии Не Выбрано");

    m_saveMenuItems.append("Сохранить и добавить тему в список");
    m_saveMenuItems.append("Сохранить в файл");
}

void ThemeFieldListModel::fillFields(const QList<QString> &colors)
{
    m_colorFields.append(colors);
    m_colorFields.removeOne(externalIdField); // remove service field
}

int ThemeFieldListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_colorFields.size();
}

QVariant ThemeFieldListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto itemIndex = index.row();
    auto field = m_colorFields.at(itemIndex);
    auto isDefined = m_values.contains(field);

    switch (role) {
        case IdRole: {
            return QVariant(itemIndex);
        }
        case NameRole: {
            return QVariant(field);
        }
        case TitleRole: {
            if (m_descriptions.contains(field)) {
                auto description = m_descriptions.value(field);
                return QVariant(description.mid(0, description.indexOf(":")));
            } else {
                return QVariant("Нет заголовка");
            }
        }
        case DescriptionRole: {
            if (m_descriptions.contains(field)) {
                auto description = m_descriptions.value(field);
                return QVariant(description.mid(description.indexOf(":") + 1));
            } else {
                return QVariant("Нет описания");
            }
        }
        case ValueRole: {
            return QVariant(isDefined ? m_values.value(field) : "");
        }
        case IsDefinedRole: {
            return QVariant(isDefined);
        }
        case FieldTypeRole: {
            if (field.startsWith("icons")) {
                return QVariant("icon");
            }
            if (field.startsWith("sizes")) {
                return QVariant("size");
            }
            if (field.startsWith("positions")) {
                return QVariant("position");
            }
            if (field.startsWith("visibilities")) {
                return QVariant("visibility");
            }
            return QVariant("color");
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ThemeFieldListModel::roleNames() const
{
    return {
        {
            IdRole,
            "id"
        },
        {
            NameRole,
            "fieldName"
        },
        {
            TitleRole,
            "title"
        },
        {
            DescriptionRole,
            "description"
        },
        {
            ValueRole,
            "fieldValue"
        },
        {
            IsDefinedRole,
            "isDefined"
        },
        {
            FieldTypeRole,
            "fieldType"
        }
    };
}

void ThemeFieldListModel::setThemeName(const QString &themeName) noexcept
{
    if (m_themeName == themeName) return;

    m_themeName = themeName;
    emit themeNameChanged();
    emit hasValuesChanged();
}

void ThemeFieldListModel::setBasedOnTheme(const QString &basedOnTheme) noexcept
{
    if (m_basedOnTheme == basedOnTheme) return;

    m_basedOnTheme = basedOnTheme;
    emit basedOnThemeChanged();
    emit hasValuesChanged();
}

void ThemeFieldListModel::setSelectedIndex(int selectedIndex) noexcept
{
    if (m_selectedIndex == selectedIndex) return;

    m_selectedIndex = selectedIndex;
    emit selectedIndexChanged();
}

QString ThemeFieldListModel::editMode() const noexcept
{
     if (m_isBlankTheme) return "Новая тема";
     if (m_isEditTheme) return "Редактирование темы";
     if (m_isCopyTheme) return "Импортированная тема";

     return "";
}

void ThemeFieldListModel::createBlankTheme() noexcept
{
    beginResetModel();

    m_values.clear();

    endResetModel();

    setThemeName("");
    setBasedOnTheme("");
    emit hasValuesChanged();
}

void ThemeFieldListModel::setValueToItem(QString value) noexcept
{
    if (m_selectedIndex < 0 && m_selectedIndex >= m_colorFields.count()) return;
    auto field = m_colorFields.value(m_selectedIndex);

    if (m_values.contains(field)) {
        m_values[field] = value;
    } else {
        m_values.insert(field, value);
    }

    emit dataChanged(index(m_selectedIndex, 0), index(m_selectedIndex, 0));
    setSelectedIndex(-1);
    emit hasValuesChanged();
}

void ThemeFieldListModel::setValueToItemByIndex(int index, QString value) noexcept
{
    auto field = m_colorFields.value(index);

    if (m_values.contains(field)) {
        m_values[field] = value;
    } else {
        m_values.insert(field, value);
    }

    emit hasValuesChanged();
}

void ThemeFieldListModel::undefineField(int itemIndex) noexcept
{
    auto field = m_colorFields.value(itemIndex);

    if (m_values.contains(field)) m_values.remove(field);

    emit dataChanged(index(itemIndex, 0), index(itemIndex, 0));
    emit hasValuesChanged();
}

void ThemeFieldListModel::defineField(int itemIndex) noexcept
{
    auto field = m_colorFields.value(itemIndex);

    if (!m_values.contains(field)) m_values.insert(field, "");

    emit dataChanged(index(itemIndex, 0), index(itemIndex, 0));
    emit hasValuesChanged();
}

void ThemeFieldListModel::saveThemeToFile(const QString &path) noexcept
{

}
