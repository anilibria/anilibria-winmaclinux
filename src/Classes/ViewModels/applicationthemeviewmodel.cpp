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

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>
#include <QMapIterator>
#include "applicationthemeviewmodel.h"
#include "../../globalhelpers.h"
#include "../../globalconstants.h"

ApplicationThemeViewModel::ApplicationThemeViewModel(QObject *parent)
    : QObject{parent}
{
    auto lightTheme = new QMap<QString, QString>();
    lightTheme->insert(plainTextColorField, "black");
    lightTheme->insert(headerTextColorField, "#a32727");
    lightTheme->insert(linkTextColorField, "#b32121");
    lightTheme->insert(pageVerticalPanelField, "#9e2323");
    lightTheme->insert(pageBackgroundField, "#D3D3D3");
    lightTheme->insert(pageUpperPanelField, "#e6e6e6");
    lightTheme->insert(panelBackgroundField, "#f2f2f2");
    lightTheme->insert(panelBackgroundShadowField, "#78808080");
    lightTheme->insert(roundedButtonBackgroundField, "#b32121");
    lightTheme->insert(roundedButtonBackgroundDisabledField, "#eb9393");
    lightTheme->insert(roundedButtonForegroundField, "white");
    lightTheme->insert(roundedButtonHoveredField, "#881919");
    lightTheme->insert(drawerGradiendStep0Field, "#920710");
    lightTheme->insert(drawerGradiendStep1Field, "#aa0813");
    lightTheme->insert(drawerGradiendStep2Field, "#c30916");
    lightTheme->insert(drawerGradiendStep3Field, "#aa0813");
    lightTheme->insert(drawerGradiendStep4Field, "#920710");
    lightTheme->insert(filterIconButtonColorField, "black");
    lightTheme->insert(filterIconButtonGreenColorField, "green");
    lightTheme->insert(filterIconButtonHoverColorField, "white");
    lightTheme->insert(selectedItemField, "red");
    lightTheme->insert(selectedFavoriteField, "red");
    lightTheme->insert(playerControlBackgroundField, "#82ffffff");
    lightTheme->insert(notificationCenterBackgroundField, "#e6e6e6");
    lightTheme->insert(notificationCenterPanelBackgroundField, "white");
    lightTheme->insert(notificationCenterItemBackgroundField, "white");
    lightTheme->insert(playlistSelectedBackgroundField, "#64c25656");
    lightTheme->insert(playlistBackgroundField, "#C8ffffff");
    lightTheme->insert(playlistSelectedTextField, "white");
    lightTheme->insert(playlistTextField, "black");
    lightTheme->insert(basedOnThemeField, m_lightTheme);
    m_themes.insert(m_lightTheme, lightTheme);

    auto darkTheme = new QMap<QString, QString>();
    darkTheme->insert(plainTextColorField, "white");
    darkTheme->insert(headerTextColorField, "white");
    darkTheme->insert(linkTextColorField, "white");
    darkTheme->insert(pageVerticalPanelField, "#262626");
    darkTheme->insert(pageBackgroundField, "#1f1f1f");
    darkTheme->insert(pageUpperPanelField, "#3d3d3d");
    darkTheme->insert(panelBackgroundField, "#474747");
    darkTheme->insert(panelBackgroundShadowField, "#78595959");
    darkTheme->insert(roundedButtonBackgroundField, "white");
    darkTheme->insert(roundedButtonBackgroundDisabledField, "#f2f2f2");
    darkTheme->insert(roundedButtonForegroundField, "black");
    darkTheme->insert(roundedButtonHoveredField, "lightgray");
    darkTheme->insert(drawerGradiendStep0Field, "#1a1a1a");
    darkTheme->insert(drawerGradiendStep1Field, "#1f1f1f");
    darkTheme->insert(drawerGradiendStep2Field, "#2e2e2e");
    darkTheme->insert(drawerGradiendStep3Field, "#404040");
    darkTheme->insert(drawerGradiendStep4Field, "#454341");
    darkTheme->insert(filterIconButtonColorField, "white");
    darkTheme->insert(filterIconButtonGreenColorField, "green");
    darkTheme->insert(filterIconButtonHoverColorField, "#737373");
    darkTheme->insert(selectedItemField, "white");
    darkTheme->insert(selectedFavoriteField, "#b30000");
    darkTheme->insert(playerControlBackgroundField, "#82000000");
    darkTheme->insert(notificationCenterBackgroundField, "#1a1a1a");
    darkTheme->insert(notificationCenterPanelBackgroundField, "#1a1a1a");
    darkTheme->insert(notificationCenterItemBackgroundField, "#1a1a1a");
    darkTheme->insert(playlistSelectedBackgroundField, "#C8ffffff");
    darkTheme->insert(playlistBackgroundField, "#C8000000");
    darkTheme->insert(playlistSelectedTextField, "black");
    darkTheme->insert(playlistTextField, "white");
    darkTheme->insert(basedOnThemeField, m_darkTheme);
    m_themes.insert(m_darkTheme, darkTheme);

    m_cachePathName = getCachePath(m_cachePathName);
    createIfNotExistsFile(m_cachePathName, "{}");
    readCacheFile();
}

void ApplicationThemeViewModel::setSelectedTheme(const QString &selectedTheme) noexcept
{
    if (m_selectedTheme == selectedTheme) return;

    m_selectedTheme = selectedTheme;
    emit selectedThemeChanged();

    auto currentTheme = m_themes.value(m_selectedTheme);
    m_basedOnDark = currentTheme->value(basedOnThemeField) == m_darkTheme;
    emit basedOnDarkChanged();
}

void ApplicationThemeViewModel::saveCurrentState()
{
    QJsonArray themes;
    QMapIterator<QString, QMap<QString, QString>*> iterator(m_themes);
    while (iterator.hasNext()) {
        iterator.next();
        if (iterator.key() == m_lightTheme || iterator.key() == m_darkTheme) continue;

        QJsonObject theme;
        theme["name"] = iterator.key();
        QMapIterator<QString, QString> fieldsIterator(*iterator.value());
        while (fieldsIterator.hasNext()) {
            theme[fieldsIterator.key()] = fieldsIterator.value();
        }
        themes.append(theme);
    }

    QJsonObject root;
    root["selectedTheme"] = m_selectedTheme;
    root["themes"] = themes;

    QFile cacheFile(m_cachePathName);
    if (!cacheFile.open(QFile::WriteOnly | QFile::Text)) return;

    auto document = QJsonDocument(root);
    cacheFile.write(document.toJson());
    cacheFile.close();
}

void ApplicationThemeViewModel::readCacheFile()
{
    QFile cacheFile(m_cachePathName);
    if (!cacheFile.open(QFile::ReadOnly | QFile::Text)) {
        return;
    }
    auto data = cacheFile.readAll();
    cacheFile.close();

    auto jsonDocument = QJsonDocument::fromJson(data);
    auto rootObject = jsonDocument.object();

    QString selectedTheme = m_lightTheme;
    if (rootObject.contains("selectedTheme")) selectedTheme = rootObject.value("selectedTheme").toString();

    if (!rootObject.contains("themes")) return;

    auto themes = rootObject.value("themes").toArray();
    foreach (auto theme, themes) {
        auto themeItem = theme.toObject();
        if (!themeItem.contains("name")) continue;
        if (!themeItem.contains("base")) continue;

        auto themeName = themeItem.value("name").toString();
        auto baseName = themeItem.value("base").toString();

        if (baseName != m_lightTheme || baseName != m_darkTheme) continue;

        auto baseTheme = m_themes.value(baseName);

        auto savedTheme = new QMap<QString, QString>();
        setThemeValue(savedTheme, themeItem, baseTheme, plainTextColorField);
        setThemeValue(savedTheme, themeItem, baseTheme, headerTextColorField);
        setThemeValue(savedTheme, themeItem, baseTheme, linkTextColorField);
        setThemeValue(savedTheme, themeItem, baseTheme, pageVerticalPanelField);
        setThemeValue(savedTheme, themeItem, baseTheme, pageBackgroundField);
        setThemeValue(savedTheme, themeItem, baseTheme, pageUpperPanelField);
        setThemeValue(savedTheme, themeItem, baseTheme, panelBackgroundField);
        setThemeValue(savedTheme, themeItem, baseTheme, panelBackgroundShadowField);
        setThemeValue(savedTheme, themeItem, baseTheme, roundedButtonBackgroundField);
        setThemeValue(savedTheme, themeItem, baseTheme, roundedButtonBackgroundDisabledField);
        setThemeValue(savedTheme, themeItem, baseTheme, roundedButtonForegroundField);
        setThemeValue(savedTheme, themeItem, baseTheme, roundedButtonHoveredField);
        setThemeValue(savedTheme, themeItem, baseTheme, drawerGradiendStep0Field);
        setThemeValue(savedTheme, themeItem, baseTheme, drawerGradiendStep1Field);
        setThemeValue(savedTheme, themeItem, baseTheme, drawerGradiendStep2Field);
        setThemeValue(savedTheme, themeItem, baseTheme, drawerGradiendStep3Field);
        setThemeValue(savedTheme, themeItem, baseTheme, drawerGradiendStep4Field);
        setThemeValue(savedTheme, themeItem, baseTheme, filterIconButtonColorField);
        setThemeValue(savedTheme, themeItem, baseTheme, filterIconButtonGreenColorField);
        setThemeValue(savedTheme, themeItem, baseTheme, filterIconButtonHoverColorField);
        setThemeValue(savedTheme, themeItem, baseTheme, selectedItemField);
        setThemeValue(savedTheme, themeItem, baseTheme, selectedFavoriteField);
        setThemeValue(savedTheme, themeItem, baseTheme, playerControlBackgroundField);
        setThemeValue(savedTheme, themeItem, baseTheme, notificationCenterBackgroundField);
        setThemeValue(savedTheme, themeItem, baseTheme, notificationCenterPanelBackgroundField);
        setThemeValue(savedTheme, themeItem, baseTheme, notificationCenterItemBackgroundField);
        setThemeValue(savedTheme, themeItem, baseTheme, playlistSelectedBackgroundField);
        setThemeValue(savedTheme, themeItem, baseTheme, playlistBackgroundField);
        setThemeValue(savedTheme, themeItem, baseTheme, playlistSelectedTextField);
        setThemeValue(savedTheme, themeItem, baseTheme, playlistTextField);
        savedTheme->insert(basedOnThemeField, baseName);
        m_themes.insert(themeName, savedTheme);
    }

    setSelectedTheme(selectedTheme);
}

void ApplicationThemeViewModel::setThemeValue(QMap<QString, QString>* theme,const QJsonObject &themeItem, const QMap<QString, QString> *baseTheme, const QString& name)
{
    theme->insert(name, themeItem.contains(name) ? themeItem.value(name).toString() : baseTheme->value(name));
}
