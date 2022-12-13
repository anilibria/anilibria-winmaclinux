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

#include <QFile>
#include <QJsonObject>
#include <QJsonDocument>
#include "themefieldlistmodel.h"
#include "../../globalconstants.h"
#include "../../globalhelpers.h"

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
    m_descriptions.insert(roundedButtonForegroundField, "Цвет текста экшен кнопки:Цвет текста внутри экшен кнопки в состоянии Активно");
    m_descriptions.insert(roundedButtonDisableForegroundField, "Цвет текста экшен кнопки:Цвет текста внутри экшен кнопки в состоянии Не Активно");
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
    m_descriptions.insert(posterBorderField, "Цвет рамки постера:Цвет рамки вокруг постера в элементе релиза в различных списках");
    m_descriptions.insert(materialAccentField, "Цвет акцента для Material:Цвет акцента в Material элементах (переключатель, текстовое поле)");
    m_descriptions.insert(colorBorderInPanelField, "Цвет рамки заголовка регионов:Цвет рамки заголовков регионов на странице Моя Анилибирия");
    m_descriptions.insert(iconMainMenuField, "Иконка главного меню:Иконка находящаяся на всех страницах и предназначена для открытия главного меню");
    m_descriptions.insert(iconMainMenuReleasesField, "Иконка страницы Каталог релизов:Иконка находящаяся в главном меню и на кастомном тулбаре");
    m_descriptions.insert(iconMainMenuCinemahallField, "Иконка страницы Кинозал:Иконка находящаяся в главном меню и на кастомном тулбаре");
    m_descriptions.insert(iconMainMenuReleasesSeriesField, "Иконка страницы Связанные релизы:Иконка находящаяся в главном меню и на кастомном тулбаре");
    m_descriptions.insert(iconMainMenuMaintenanceField, "Иконка страницы Обслуживание:Иконка находящаяся в главном меню и на кастомном тулбаре");
    m_descriptions.insert(iconMainMenuMyAnilibriaField, "Иконка страницы Моя Анилибрия:Иконка находящаяся в главном меню и на кастомном тулбаре");
    m_descriptions.insert(iconMainMenuVideoplayerField, "Иконка страницы Видеоплеер:Иконка находящаяся в главном меню и на кастомном тулбаре");
    m_descriptions.insert(iconMainMenuDonateField, "Иконка страницы Поддержать проект:Иконка находящаяся в главном меню и на кастомном тулбаре");
    m_descriptions.insert(iconMainMenuThemeManagerField, "Иконка страницы Менеджер тем:Иконка находящаяся в главном меню и на кастомном тулбаре");
    m_descriptions.insert(iconMainMenuYoutubeField, "Иконка страницы Youtube:Иконка находящаяся в главном меню и на кастомном тулбаре");
    m_descriptions.insert(iconMainMenuAuthorizationField, "Иконка страницы Авторизация:Иконка находящаяся в главном меню и на кастомном тулбаре");
    m_descriptions.insert(iconLeftHalfScreenField, "Иконка кнопки Левая половина окна:Иконка находящаяся в правой стороне кастомного тулбара");
    m_descriptions.insert(iconRightHalfScreenField, "Иконка кнопки Правая половина окна:Иконка находящаяся в правой стороне кастомного тулбара");
    m_descriptions.insert(iconMinimizeWindowField, "Иконка кнопки Минимизировать окно:Иконка находящаяся в правой стороне кастомного тулбара");
    m_descriptions.insert(iconMaximizeWindowField, "Иконка кнопки Максимизировать окно:Иконка находящаяся в правой стороне кастомного тулбара");
    m_descriptions.insert(iconCloseWindowField, "Иконка кнопки Закрыть окно:Иконка находящаяся в правой стороне кастомного тулбара");
    m_descriptions.insert(iconNormalWindowField, "Иконка кнопки Нормальный размер окна:Иконка находящаяся в правой стороне кастомного тулбара");

    m_saveMenuItems.append("Сохранить и добавить тему в список");
    m_saveMenuItems.append("Сохранить в файл");
}

void ThemeFieldListModel::fillFields(const QList<QString> &colors)
{
    m_colorFields.append(colors);
    m_colorFields.removeOne(externalIdField); // remove service field

    m_filteredColorFields.append(m_colorFields);
}

int ThemeFieldListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_filteredColorFields.size();
}

QVariant ThemeFieldListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    auto itemIndex = index.row();
    auto field = m_filteredColorFields.at(itemIndex);
    auto isDefined = m_values.contains(field);

    switch (role) {
        case IdRole: {
            return QVariant(m_colorFields.indexOf(field));
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
            if (field.startsWith("icon")) {
                return QVariant("icon");
            }
            if (field.startsWith("sizes")) {
                return QVariant("size");
            }
            if (field.startsWith("position")) {
                return QVariant("position");
            }
            if (field.startsWith("visibility")) {
                return QVariant("visibility");
            }
            return QVariant("color");
        }
        case TooltipOverrideRole: {
            if (field.startsWith("icon")) {
                return QVariant(isDefined ? "Брать иконку из базовой темы" : "Переопределить иконку");
            }
            if (field.startsWith("size")) {
                return QVariant(isDefined ? "Брать размер из базовой темы" : "Переопределить размер");
            }
            if (field.startsWith("position")) {
                return QVariant(isDefined ? "Брать позицию из базовой темы" : "Переопределить позицию");
            }
            if (field.startsWith("visibility")) {
                return QVariant(isDefined ? "Брать видимость из базовой темы" : "Переопределить видимость");
            }
            return QVariant(isDefined ? "Брать цвет из базовой темы" : "Переопределить цвет");
        }
    }

    return QVariant();
}

QHash<int, QByteArray> ThemeFieldListModel::roleNames() const
{
    return {
        {
            IdRole,
            "identifier"
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
        },
        {
            TooltipOverrideRole,
            "tooltipOverride"
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

void ThemeFieldListModel::setFilter(const QString &filter) noexcept
{
    if (m_filter == filter) return;

    m_filter = filter;
    emit filterChanged();

    refresh();
}

void ThemeFieldListModel::setValues(QMap<QString, QString>&& values, const QString& name, const QString& basedTheme) noexcept
{
    beginResetModel();

    m_values.clear();

    QMapIterator<QString, QString> iterator(values);
    while (iterator.hasNext()) {
        iterator.next();

        m_values.insert(iterator.key(), iterator.value());
    }

    endResetModel();

    setThemeName(name);
    setBasedOnTheme(basedTheme);
    emit hasValuesChanged();
}

void ThemeFieldListModel::refresh() noexcept
{
    beginResetModel();

    m_filteredColorFields.clear();

    if (m_filter.isEmpty()) {
        m_filteredColorFields.append(m_colorFields);
    } else {
        foreach (auto colorField, m_colorFields) {
            if (m_descriptions[colorField].toLower().contains(m_filter.toLower())) {
                m_filteredColorFields.append(colorField);
            }
        }
    }

    endResetModel();
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

    refreshRowByField(field);
    setSelectedIndex(-1);
    emit hasValuesChanged();
}

void ThemeFieldListModel::setValueToItemByIndex(int index, QString value) noexcept
{
    auto field = m_colorFields.value(index);

    if (m_values.contains(field)) {
        m_values[field] = value;
    } else {
        if (!value.isEmpty()) m_values.insert(field, value);
    }

    emit hasValuesChanged();
}

void ThemeFieldListModel::undefineField(int itemIndex) noexcept
{
    auto field = m_colorFields.value(itemIndex);

    if (m_values.contains(field)) m_values.remove(field);

    refreshRowByField(field);
    emit hasValuesChanged();
}

void ThemeFieldListModel::defineField(int itemIndex) noexcept
{
    auto field = m_colorFields.value(itemIndex);

    if (!m_values.contains(field)) m_values.insert(field, "");

    refreshRowByField(field);
    emit hasValuesChanged();
}

void ThemeFieldListModel::toggleDefinedField(int itemIndex) noexcept
{
    auto field = m_colorFields.value(itemIndex);

    if (m_values.contains(field)) {
        m_values.remove(field);
    } else {
        m_values.insert(field, "");
    }
    refreshRowByField(field);
    emit hasValuesChanged();
}

void ThemeFieldListModel::saveThemeToFile(const QString &path) noexcept
{
    QJsonObject object;

    QMapIterator<QString, QString> iterator(m_values);

    while (iterator.hasNext()) {
        iterator.next();

        object[iterator.key()] = iterator.value();
    }

    object["name"] = m_themeName;
    object[basedOnThemeField] = m_basedOnTheme;

    auto clearedPath = removeFileProtocol(const_cast<QString&>(path));
    QFile file(clearedPath);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        //TODO: show error message
        return;
    }

    QJsonDocument document(object);
    file.write(document.toJson());
    file.close();
}

void ThemeFieldListModel::addIconFromFile(const QString &path, int itemIndex) noexcept
{
    auto clearedPath = removeFileProtocol(const_cast<QString&>(path));
    if (!QFile::exists(clearedPath)) return;

    QFile file(clearedPath);
    if (!file.open(QIODevice::ReadOnly)) return;

    auto content = file.readAll();

    file.close();

    if (content.length() > 40000) {
        emit errorMessage("Размер файла иконки не должен превышать 40 Кб!");
        return;
    }

    auto iconContent = content.toBase64();
    auto lowerPath = path.toLower();
    if (lowerPath.endsWith(".png")) {
        setValueToItemByIndex(itemIndex, "data:image/png;base64," + iconContent);
    }

    if (lowerPath.endsWith(".jpg") || lowerPath.endsWith(".jpeg")) {
        setValueToItemByIndex(itemIndex, "data:image/jpeg;base64," + iconContent);
    }

    if (lowerPath.endsWith(".svg")) {
        setValueToItemByIndex(itemIndex, "data:image/svg;base64," + iconContent);
    }

    if (lowerPath.endsWith(".gif")) {
        setValueToItemByIndex(itemIndex, "data:image/gif;base64," + iconContent);
    }

    auto field = m_colorFields.value(itemIndex);
    refreshRowByField(field);
}

void ThemeFieldListModel::refreshRowByField(const QString &field) noexcept
{
    if (m_filteredColorFields.contains(field)) {
        auto filteredIndex = m_filteredColorFields.indexOf(field);
        emit dataChanged(index(filteredIndex, 0), index(filteredIndex, 0));
    } else {
        auto itemIndex = m_colorFields.indexOf(field);
        emit dataChanged(index(itemIndex, 0), index(itemIndex, 0));
    }
}
