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

#ifndef APPLICATIONTHEMEVIEWMODEL_H
#define APPLICATIONTHEMEVIEWMODEL_H

#include <QObject>
#include <QMap>
#include "../../globalconstants.h"
#include "../Services/thememanagerservice.h"
#include "../ListModels/externalthemeslistmodel.h"
#include "../Models/themeitemmodel.h"
#include "../ListModels/themefieldlistmodel.h"
#include "../ListModels/localthemeslistmodel.h"

class ApplicationThemeViewModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString selectedTheme READ selectedTheme WRITE setSelectedTheme NOTIFY selectedThemeChanged)
    Q_PROPERTY(QString plainTextColor READ plainTextColor NOTIFY plainTextColorChanged)
    Q_PROPERTY(QString headerTextColor READ headerTextColor NOTIFY headerTextColorChanged)
    Q_PROPERTY(QString linkTextColor READ linkTextColor NOTIFY linkTextColorChanged)
    Q_PROPERTY(QString pageVerticalPanel READ pageVerticalPanel NOTIFY pageVerticalPanelChanged)
    Q_PROPERTY(QString pageBackground READ pageBackground NOTIFY pageBackgroundChanged)
    Q_PROPERTY(QString pageUpperPanel READ pageUpperPanel NOTIFY pageUpperPanelChanged)
    Q_PROPERTY(QString panelBackground READ panelBackground NOTIFY panelBackgroundChanged)
    Q_PROPERTY(QString panelBackgroundShadow READ panelBackgroundShadow NOTIFY panelBackgroundShadowChanged)
    Q_PROPERTY(QString roundedButtonBackground READ roundedButtonBackground NOTIFY roundedButtonBackgroundChanged)
    Q_PROPERTY(QString roundedButtonBackgroundDisabled READ roundedButtonBackgroundDisabled NOTIFY roundedButtonBackgroundDisabledChanged)
    Q_PROPERTY(QString roundedButtonForeground READ roundedButtonForeground NOTIFY roundedButtonForegroundChanged)
    Q_PROPERTY(QString roundedButtonHovered READ roundedButtonHovered NOTIFY roundedButtonHoveredChanged)
    Q_PROPERTY(QString drawerGradiendStep0 READ drawerGradiendStep0 NOTIFY drawerGradiendStep0Changed)
    Q_PROPERTY(QString drawerGradiendStep1 READ drawerGradiendStep1 NOTIFY drawerGradiendStep1Changed)
    Q_PROPERTY(QString drawerGradiendStep2 READ drawerGradiendStep2 NOTIFY drawerGradiendStep2Changed)
    Q_PROPERTY(QString drawerGradiendStep3 READ drawerGradiendStep3 NOTIFY drawerGradiendStep3Changed)
    Q_PROPERTY(QString drawerGradiendStep4 READ drawerGradiendStep4 NOTIFY drawerGradiendStep4Changed)
    Q_PROPERTY(QString filterIconButtonColor READ filterIconButtonColor NOTIFY filterIconButtonColorChanged)
    Q_PROPERTY(QString filterIconButtonGreenColor READ filterIconButtonGreenColor NOTIFY filterIconButtonGreenColorChanged)
    Q_PROPERTY(QString filterIconButtonHoverColor READ filterIconButtonHoverColor NOTIFY filterIconButtonHoverColorChanged)
    Q_PROPERTY(QString selectedItem READ selectedItem NOTIFY selectedItemChanged)
    Q_PROPERTY(QString selectedFavorite READ selectedFavorite NOTIFY selectedFavoriteChanged)
    Q_PROPERTY(QString playerControlBackground READ playerControlBackground NOTIFY playerControlBackgroundChanged)
    Q_PROPERTY(QString notificationCenterBackground READ notificationCenterBackground NOTIFY notificationCenterBackgroundChanged)
    Q_PROPERTY(QString notificationCenterPanelBackground READ notificationCenterPanelBackground NOTIFY notificationCenterPanelBackgroundChanged)
    Q_PROPERTY(QString notificationCenterItemBackground READ notificationCenterItemBackground NOTIFY notificationCenterItemBackgroundChanged)
    Q_PROPERTY(QString playlistSelectedBackground READ playlistSelectedBackground NOTIFY playlistSelectedBackgroundChanged)
    Q_PROPERTY(QString playlistBackground READ playlistBackground NOTIFY playlistBackgroundChanged)
    Q_PROPERTY(QString playlistSelectedText READ playlistSelectedText NOTIFY playlistSelectedTextChanged)
    Q_PROPERTY(QString playlistText READ playlistText NOTIFY playlistTextChanged)
    Q_PROPERTY(QString toggleButtonHovered READ toggleButtonHovered NOTIFY toggleButtonHoveredChanged)
    Q_PROPERTY(QString toggleButtonChecked READ toggleButtonChecked NOTIFY toggleButtonCheckedChanged)
    Q_PROPERTY(QString toggleButtonNotChecked READ toggleButtonNotChecked NOTIFY toggleButtonNotCheckedChanged)
    Q_PROPERTY(QStringList themes READ themes NOTIFY themesChanged)
    Q_PROPERTY(bool basedOnDark READ basedOnDark NOTIFY basedOnDarkChanged)
    Q_PROPERTY(ThemeManagerService* service READ service NOTIFY serviceChanged)
    Q_PROPERTY(ExternalThemesListModel* externalThemes READ externalThemes NOTIFY externalThemesChanged)
    Q_PROPERTY(QStringList menuItems READ menuItems NOTIFY menuItemsChanged)
    Q_PROPERTY(int selectedMenuItem READ selectedMenuItem WRITE setSelectedMenuItem NOTIFY selectedMenuItemChanged)
    Q_PROPERTY(QString selectedMenuItemName READ selectedMenuItemName NOTIFY selectedMenuItemNameChanged)
    Q_PROPERTY(ThemeFieldListModel* fieldList READ fieldList NOTIFY fieldListChanged)
    Q_PROPERTY(QVariantMap previewItems READ previewItems NOTIFY previewItemsChanged)
    Q_PROPERTY(LocalThemesListModel* localThemes READ localThemes NOTIFY localThemesChanged)

private:
    QString m_cachePathName { "applicationtheme.cache" };
    QString m_lightTheme { "Светлая" };
    QString m_darkTheme { "Темная" };
    QString m_selectedTheme { m_lightTheme };
    QMap<QString, QMap<QString, QString>*> m_themes { QMap<QString, QMap<QString, QString>*>() };
    QList<QString> m_fields { QList<QString>() };
    bool m_basedOnDark { false };
    ThemeManagerService* m_service { new ThemeManagerService(this) };
    ExternalThemesListModel* m_externalThemes { new ExternalThemesListModel(this) };
    QStringList m_menuItems { QStringList() };
    int m_selectedMenuItem { 0 };
    ThemeFieldListModel* m_fieldList { new ThemeFieldListModel(this) };
    QList<QString>* m_externalIds { new QList<QString>() };
    QList<QString>* m_localIds { new QList<QString>() };
    LocalThemesListModel* m_localThemes { new LocalThemesListModel(this) };

public:
    explicit ApplicationThemeViewModel(QObject *parent = nullptr);

    QString selectedTheme() const noexcept { return m_selectedTheme; }
    void setSelectedTheme(const QString& selectedTheme) noexcept;

    QString plainTextColor() const noexcept { return m_themes.value(m_selectedTheme)->value(plainTextColorField); }
    QString headerTextColor() const noexcept { return m_themes.value(m_selectedTheme)->value(headerTextColorField); }
    QString linkTextColor() const noexcept { return m_themes.value(m_selectedTheme)->value(linkTextColorField); }
    QString pageVerticalPanel() const noexcept { return m_themes.value(m_selectedTheme)->value(pageVerticalPanelField); }
    QString pageBackground() const noexcept { return m_themes.value(m_selectedTheme)->value(pageBackgroundField); }
    QString pageUpperPanel() const noexcept { return m_themes.value(m_selectedTheme)->value(pageUpperPanelField); }
    QString panelBackground() const noexcept { return m_themes.value(m_selectedTheme)->value(panelBackgroundField); }
    QString panelBackgroundShadow() const noexcept { return m_themes.value(m_selectedTheme)->value(panelBackgroundShadowField); }
    QString roundedButtonBackground() const noexcept { return m_themes.value(m_selectedTheme)->value(roundedButtonBackgroundField); }
    QString roundedButtonBackgroundDisabled() const noexcept { return m_themes.value(m_selectedTheme)->value(roundedButtonBackgroundDisabledField); }
    QString roundedButtonForeground() const noexcept { return m_themes.value(m_selectedTheme)->value(roundedButtonForegroundField); }
    QString roundedButtonHovered() const noexcept { return m_themes.value(m_selectedTheme)->value(roundedButtonHoveredField); }
    QString drawerGradiendStep0() const noexcept { return m_themes.value(m_selectedTheme)->value(drawerGradiendStep0Field); }
    QString drawerGradiendStep1() const noexcept { return m_themes.value(m_selectedTheme)->value(drawerGradiendStep1Field); }
    QString drawerGradiendStep2() const noexcept { return m_themes.value(m_selectedTheme)->value(drawerGradiendStep2Field); }
    QString drawerGradiendStep3() const noexcept { return m_themes.value(m_selectedTheme)->value(drawerGradiendStep3Field); }
    QString drawerGradiendStep4() const noexcept { return m_themes.value(m_selectedTheme)->value(drawerGradiendStep4Field); }
    QString filterIconButtonColor() const noexcept { return m_themes.value(m_selectedTheme)->value(filterIconButtonColorField); }
    QString filterIconButtonGreenColor() const noexcept { return m_themes.value(m_selectedTheme)->value(filterIconButtonGreenColorField); }
    QString filterIconButtonHoverColor() const noexcept { return m_themes.value(m_selectedTheme)->value(filterIconButtonHoverColorField); }
    QString selectedItem() const noexcept { return m_themes.value(m_selectedTheme)->value(selectedItemField); }
    QString selectedFavorite() const noexcept { return m_themes.value(m_selectedTheme)->value(selectedFavoriteField); }
    QString playerControlBackground() const noexcept { return m_themes.value(m_selectedTheme)->value(playerControlBackgroundField); }
    QString notificationCenterBackground() const noexcept { return m_themes.value(m_selectedTheme)->value(notificationCenterBackgroundField); }
    QString notificationCenterPanelBackground() const noexcept { return m_themes.value(m_selectedTheme)->value(notificationCenterPanelBackgroundField); }
    QString notificationCenterItemBackground() const noexcept { return m_themes.value(m_selectedTheme)->value(notificationCenterItemBackgroundField); }
    QString playlistSelectedBackground() const noexcept { return m_themes.value(m_selectedTheme)->value(playlistSelectedBackgroundField); }
    QString playlistBackground() const noexcept { return m_themes.value(m_selectedTheme)->value(playlistBackgroundField); }
    QString playlistSelectedText() const noexcept { return m_themes.value(m_selectedTheme)->value(playlistSelectedTextField); }
    QString playlistText() const noexcept { return m_themes.value(m_selectedTheme)->value(playlistTextField); }
    QString toggleButtonHovered() const noexcept { return m_themes.value(m_selectedTheme)->value(toggleButtonHoveredField); }
    QString toggleButtonChecked() const noexcept { return m_themes.value(m_selectedTheme)->value(toggleButtonCheckedField); }
    QString toggleButtonNotChecked() const noexcept { return m_themes.value(m_selectedTheme)->value(toggleButtonNotCheckedField); }
    QStringList themes() const noexcept { return m_themes.keys(); }
    bool basedOnDark() const noexcept { return m_basedOnDark; }

    ThemeManagerService* service() const noexcept { return m_service; }
    ExternalThemesListModel* externalThemes() const noexcept { return m_externalThemes; }

    ThemeFieldListModel* fieldList() const noexcept { return m_fieldList; }

    QStringList menuItems() const noexcept { return m_menuItems; }

    int selectedMenuItem() const noexcept { return m_selectedMenuItem; }
    void setSelectedMenuItem(int selectedMenuItem) noexcept;

    QString selectedMenuItemName() const noexcept { return m_menuItems.value(m_selectedMenuItem); }

    QVariantMap previewItems() const noexcept;

    LocalThemesListModel* localThemes() const noexcept { return m_localThemes; }

    Q_INVOKABLE void saveCurrentState();
    Q_INVOKABLE void reloadThemes();
    Q_INVOKABLE void importTheme(const QString& content);
    Q_INVOKABLE void importThemeFromFile(const QString& content);
    Q_INVOKABLE void importThemeFromExternal(int themeIndex);
    Q_INVOKABLE void saveThemeAndApply() noexcept;
    Q_INVOKABLE void preparePreviewItems() noexcept;
    Q_INVOKABLE void deleteThemeByExternalId(const QString& externalId) noexcept;
    Q_INVOKABLE void deleteThemeFromLocal(const QString& name) noexcept;

private:
    void readCacheFile();
    void setThemeValue(QMap<QString, QString>* theme, const QJsonObject& themeItem, const QMap<QString, QString>* baseTheme, const QString& name);

private slots:
    void themesLoaded();
    void themeLoaded(const QString& theme, bool isDark, const ThemeItemModel* externalTheme);

signals:
    void selectedThemeChanged();
    void plainTextColorChanged();
    void headerTextColorChanged();
    void linkTextColorChanged();
    void pageVerticalPanelChanged();
    void pageBackgroundChanged();
    void pageUpperPanelChanged();
    void panelBackgroundChanged();
    void panelBackgroundShadowChanged();
    void roundedButtonBackgroundChanged();
    void roundedButtonBackgroundDisabledChanged();
    void roundedButtonForegroundChanged();
    void roundedButtonHoveredChanged();
    void drawerGradiendStep0Changed();
    void drawerGradiendStep1Changed();
    void drawerGradiendStep2Changed();
    void drawerGradiendStep3Changed();
    void drawerGradiendStep4Changed();
    void filterIconButtonColorChanged();
    void filterIconButtonGreenColorChanged();
    void filterIconButtonHoverColorChanged();
    void selectedItemChanged();
    void selectedFavoriteChanged();
    void playerControlBackgroundChanged();
    void notificationCenterBackgroundChanged();
    void notificationCenterPanelBackgroundChanged();
    void notificationCenterItemBackgroundChanged();
    void playlistSelectedBackgroundChanged();
    void playlistBackgroundChanged();
    void playlistSelectedTextChanged();
    void playlistTextChanged();
    void themesChanged();
    void basedOnDarkChanged();
    void errorImportTheme(const QString& message);
    void errorExportTheme(const QString& message);
    void serviceChanged();
    void externalThemesChanged();
    void menuItemsChanged();
    void selectedMenuItemChanged();
    void selectedMenuItemNameChanged();
    void fieldListChanged();
    void isHasValuesChanged();
    void previewItemsChanged();
    void toggleButtonHoveredChanged();
    void toggleButtonCheckedChanged();
    void toggleButtonNotCheckedChanged();
    void localThemesChanged();

};

#endif // APPLICATIONTHEMEVIEWMODEL_H
