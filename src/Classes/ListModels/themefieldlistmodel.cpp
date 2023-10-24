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
    m_descriptions.insert(colorMaterialTextField, "Цвет текста для Material:Цвет текста в Material элементах (текстовое поле, выпадающее поле)");
    m_descriptions.insert(colorBorderInPanelField, "Цвет рамки заголовка регионов:Цвет рамки заголовков регионов на странице Моя Анилибирия");
    m_descriptions.insert(colorBackgroundNavigationButtonField, "Цвет фона кнопки навигации:Цвет фона кнопок со стрелками навигации в горизонтальных списках");
    m_descriptions.insert(colorPageIndexTextField, "Цвет текста индекса релиза:Цвет текста индекса релиза на странице Кинозал и в Карточке релиза");
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
    m_descriptions.insert(iconLeftHalfScreenField, "Иконка кнопки Предыдущая страница:Иконка находящаяся в правой стороне кастомного тулбара");
    m_descriptions.insert(iconRightHalfScreenField, "Иконка кнопки Следующая страница:Иконка находящаяся в правой стороне кастомного тулбара");
    m_descriptions.insert(iconMinimizeWindowField, "Иконка кнопки Предыдущая страница Неактивная:Иконка находящаяся в правой стороне кастомного тулбара");
    m_descriptions.insert(iconMaximizeWindowField, "Иконка кнопки Следующая страница Неактивная:Иконка находящаяся в правой стороне кастомного тулбара");
    m_descriptions.insert(iconCloseWindowField, "Иконка кнопки Закрыть окно:Иконка находящаяся в правой стороне кастомного тулбара");
    m_descriptions.insert(iconNormalWindowField, "Иконка кнопки Нормальный размер окна:Иконка находящаяся в правой стороне кастомного тулбара");
    m_descriptions.insert(iconFavoritesField, "Иконка Избранное:Иконка для кнопок на разных страницах с действиями с Избранным");
    m_descriptions.insert(iconSeenField, "Иконка Просмотрено:Иконка для кнопок на разных страницах с действиями над (Не)Просмотренными(мыеми) релизами");
    m_descriptions.insert(iconInfoField, "Иконка Информация:Иконка для кнопок содержащих справочные материалы");
    m_descriptions.insert(iconClearTextControlField, "Иконка Очистка:Иконка для кнопки внутри текстовых полей для очистки текста");
    m_descriptions.insert(iconBackToTopField, "Иконка Вернуть наверх:Иконка для кнопки возврата в начало скролируемой области");
    m_descriptions.insert(iconReleaseCatalogSynchronizationField, "Иконка Синхронизация:Иконка для кнопки начала синхронизации в Каталоге релизов");
    m_descriptions.insert(iconReleaseCatalogSearchField, "Иконка Поиск:Иконка для кнопки открытия поисковых полей в Каталоге релизов");
    m_descriptions.insert(iconReleaseCatalogNotificationField, "Иконка Уведомления:Иконка для кнопки открытия уведомлений в Каталоге релизов");
    m_descriptions.insert(iconReleaseCatalogRandomField, "Иконка Случайный релиз:Иконка для кнопки открытия случайного релиза в Каталоге релизов");
    m_descriptions.insert(iconReleaseCatalogSettingsField, "Иконка Настройки:Иконка для кнопки открытия настроек");
    m_descriptions.insert(iconReleaseCatalogHidedButtonsField, "Иконка Настройки:Иконка для кнопки установки скрываемых кнопок");
    m_descriptions.insert(iconReleaseCatalogOnlineField, "Иконка Онлайн:Иконка для кнопки открытия плеера в списке релизов в Каталоге релизов");
    m_descriptions.insert(iconReleaseCatalogTorrentField, "Иконка Торрент:Иконка для кнопки открытия меню торрентов в списке релизов в Каталоге релизов");
    m_descriptions.insert(iconReleaseCatalogScheduleField, "Иконка Расписание:Иконка для кнопки расписание в Каталоге релизов");
    m_descriptions.insert(iconReleaseCatalogHistoryField, "Иконка История:Иконка для кнопки с историей в Каталоге релизов");
    m_descriptions.insert(iconReleaseCatalogAlphabetField, "Иконка Алфавит:Иконка для кнопки с фильтром Алфавит в Каталоге релизов");
    m_descriptions.insert(iconReleaseCatalogAllReleasesField, "Иконка Все релизы:Иконка для кнопки Все релизы в Каталоге релизов");
    m_descriptions.insert(iconReleaseCatalogCompilationField, "Иконка Подборки:Иконка для кнопки Подборки в Каталоге релизов");
    m_descriptions.insert(iconReleaseCatalogSortAscField, "Иконка Сортировка вниз:Иконка для кнопки Сортировка вниз в Каталоге релизов");
    m_descriptions.insert(iconReleaseCatalogSortDescField, "Иконка Сортировка вверх:Иконка для кнопки Сортировка вверх в Каталоге релизов");
    m_descriptions.insert(iconReleaseCardListMenuField, "Иконка Меню Списков:Иконка для кнопки Меню Списков в Карточке релиза");
    m_descriptions.insert(iconReleaseCardCloseField, "Иконка Закрыть карточку:Иконка для кнопки Закрыть карточку в Карточке релиза");
    m_descriptions.insert(iconReleaseCardCopyField, "Иконка Копирование:Иконка для кнопки Копирование в Карточке релиза");
    m_descriptions.insert(iconReleaseCardExternalField, "Иконка Внешние источники:Иконка для кнопки Открыть во внешних плеерах в Карточке релиза");
    m_descriptions.insert(iconMainMenuTorrentStreamField, "Иконка страницы TorrentStream:Иконка находящаяся в главном меню");
    m_descriptions.insert(iconNavigationButtonLeftField, "Иконка Навигацинная стрелка влево:Иконка для кнопки прокручивающей некоторое расстояние влево в списках");
    m_descriptions.insert(iconNavigationButtonRightField, "Иконка Навигацинная стрелка вправо:Иконка для кнопки прокручивающей некоторое расстояние вправо в списках");
    m_descriptions.insert(iconMyAnilibriaButtonSortUpField, "Иконка Переместить вверх:Иконка для кнопки перемещения на позицию вверх для секции страницы Моя Анилибрия");
    m_descriptions.insert(iconMyAnilibriaButtonSortBottomField, "Иконка Переместить вниз:Иконка для кнопки перемещения на позицию вниз для секции страницы Моя Анилибрия");
    m_descriptions.insert(iconMyAnilibriaButtonPlusField, "Иконка Плюс:Иконка для кнопки открыть все секции на странице Моя Анилибрия");
    m_descriptions.insert(iconMyAnilibriaButtonMinusField, "Иконка Минус:Иконка для кнопки закрыть все секции на странице Моя Анилибрия");
    m_descriptions.insert(iconMyAnilibriaButtonSortSectionsField, "Иконка Сортировки:Иконка для кнопки выбора видимости секций страницы Моя Анилибрия");
    m_descriptions.insert(iconDeleteItemField, "Иконка Удалить:Иконка для кнопок удаления на странцах Менеджер Тем и TorrentStream");
    m_descriptions.insert(iconDownloadThemeField, "Иконка Скачать:Иконка для кнопоки скачивания темы на странце Менеджер Тем");
    m_descriptions.insert(iconEditThemeFieldPlusField, "Иконка Перекрыть поле:Иконка для кнопки перекрытия полей на странце Менеджер Тем");
    m_descriptions.insert(iconEmptyField, "Иконка Пустой список:Иконка для состяние пусто в различных списках");
    m_descriptions.insert(iconPlayerFullscreenField, "Иконка На весь экран (плеер):Иконка для кнопки переключения режимов полный экран/оконный");
    m_descriptions.insert(iconPlayerCropModeField, "Иконка Подогнать размер (плеер):Иконка для кнопки подгонки размера видео под высоту окна");
    m_descriptions.insert(iconPlayerSettingsField, "Иконка Настройки (плеер):Иконка для кнопки настройки");
    m_descriptions.insert(iconPlayerRemoteField, "Иконка Удаленный плеер (плеер):Иконка для кнопки удаленный плеер");
    m_descriptions.insert(iconPlayerTopMostField, "Иконка Поверх всех окон (плеер):Иконка для кнопки поверх всех окон");
    m_descriptions.insert(iconPlayerRefreshField, "Иконка Перезапустить (плеер):Иконка для кнопки перезапустить текущее видео");
    m_descriptions.insert(iconPlayerInfoField, "Иконка Информация (плеер):Иконка для кнопки с информацией");
    m_descriptions.insert(iconPlayerMuteField, "Иконка Mute (плеер):Иконка для кнопки mute");
    m_descriptions.insert(iconPlayerUnMuteField, "Иконка Mute off (плеер):Иконка для кнопки mute off");
    m_descriptions.insert(iconPlayerSeenField, "Иконка Просмотрено (плеер):Иконка для кнопки просмотра, состояние просмотрено");
    m_descriptions.insert(iconPlayerUnseenField, "Иконка Не просмотрено (плеер):Иконка для кнопки просмотра, состояние не просмотрено");
    m_descriptions.insert(iconPlayerExternalField, "Иконка Внешний просмотр (плеер):Иконка для кнопки просмотра во внешнем плеере или браузере");
    m_descriptions.insert(iconPlayerPlayField, "Иконка Воспроизведение (плеер):Иконка для кнопки воспроизведение");
    m_descriptions.insert(iconPlayerPauseField, "Иконка Пауза (плеер):Иконка для кнопки пауза");
    m_descriptions.insert(iconPlayerJumpLeftField, "Иконка Прыжок назад (плеер):Иконка для кнопки Прыжок назад");
    m_descriptions.insert(iconPlayerJumpRightField, "Иконка Прыжок вперед (плеер):Иконка для кнопки Прыжок вперед");
    m_descriptions.insert(iconPlayerNextSeriaField, "Иконка Пауза (плеер):Иконка для кнопки Следующая серия");
    m_descriptions.insert(iconPlayerPreviousSeriaField, "Иконка Пауза (плеер):Иконка для кнопки Предыдущая серия");
    m_descriptions.insert(iconPlayerDrawerField, "Иконка Пауза (плеер):Иконка для кнопки Меню");
    m_descriptions.insert(iconNotificationField, "Иконка Уведомления (тулбар):Иконка для кнопки уведомлений в тулбаре");
    m_descriptions.insert(iconNotificationSelectedField, "Иконка Есть уведомления (тулбар):Иконка для кнопки уведомлений в тулбаре");
    m_descriptions.insert(colorTooltipTextField, "Цвет текста в подсказке:Цвет текста во всех подсказках");
    m_descriptions.insert(colorSeenReleaseItemTextField, "Цвет текста Просмотренной серии:Цвет текста для количества посмотренных серий в списке релизов");
    m_descriptions.insert(colorTooltipBackgroundField, "Цвет фона в подсказке:Цвет фона во всех подсказках");
    m_descriptions.insert(colorDrawerItemTextField, "Цвет текста в главном меню:Цвет текста для пунктов страниц, имени пользователя и версии в главном меню");
    m_descriptions.insert(colorDrawerItemHoveredField, "Цвет фона пункта главного меню:Цвет фона пункта главного меню в состоянии Мышка поверх элемента");
    m_descriptions.insert(colorPosterFilterField, "Цвет фильтра постеров:Цвет фильтра над постерами в списках и в карточках");
    m_descriptions.insert(colorPopupBackgroundField, "Цвет фона попапов:Цвет фона всех всплывающих окон в приложении");
    m_descriptions.insert(colorPopupBackgroundShadowField, "Цвет тени попапов:Цвет тени для всех всплывающих окон в приложении");

    m_descriptions.insert(colorSwitchHighlightedRippleField, "Цвет переключателя 1:Цвет подсветки переключателя основного элемента контрола");
    m_descriptions.insert(colorSwitchRippleField, "Цвет переключателя 2:Цвет переключателя основного элемента контрола");
    m_descriptions.insert(colorSwitchCheckedTrackField, "Цвет переключателя 3:Цвет ползунка в состянии выбрано");
    m_descriptions.insert(colorSwitchUncheckedTrackField, "Цвет переключателя 4:Цвет ползунка в состянии не выбрано");
    m_descriptions.insert(colorSwitchDisabledTrackField, "Цвет переключателя 5:Цвет ползунка в состянии не активно");
    m_descriptions.insert(colorSwitchCheckedHandleField, "Цвет переключателя 6:Цвет бара в состоянии выбрано");
    m_descriptions.insert(colorSwitchUncheckedHandleField, "Цвет переключателя 7:Цвет бара в состоянии не выбрано");
    m_descriptions.insert(colorSwitchDisabledHandleField, "Цвет переключателя 8:Цвет бара в состоянии не активно");

    m_descriptions.insert(colorTextFieldAccentField, "Цвет текстового поля 1: Цвет акцента в текстовых полях в фокусе");
    m_descriptions.insert(colorTextFieldForegroundField, "Цвет текстового поля 2: Цвет текста в текстовом поле в состоянии Активно");
    m_descriptions.insert(colorTextFieldHintField, "Цвет текстового поля 2: Цвет текста подсказки");
    m_descriptions.insert(colorTextFieldHighlightField, "Цвет текстового поля 2: Цвет выделенного текста в текстовом поле");
    m_descriptions.insert(colorTextFieldTextField, "Цвет текстового поля 2: Цвет текста в текстовом поле");

    m_descriptions.insert(colorMenuDialogBackgroundField, "Цвет меню 1: Цвет фона попапа меню");
    m_descriptions.insert(colorMenuOverlayBackgroundField, "Цвет меню 2: Цвет фона оверлея меню");
    m_descriptions.insert(colorMenuForegroundField, "Цвет меню 3: Цвет текста в пунктах меню");
    m_descriptions.insert(colorMenuItemHintField, "Цвет меню 4: Цвет подсказки");
    m_descriptions.insert(colorMenuRippleField, "Цвет меню 5: Цвет рассеивания");
    m_descriptions.insert(colorMenuHighlightField, "Цвет меню 6: Цвет выбранного пункта");

    m_descriptions.insert(colorDrawerDividerColorField, "Цвет главного меню 1:Цвет разделителя");
    m_descriptions.insert(colorDrawerDimColorField, "Цвет главного меню 2:Цвет затенения");
    m_descriptions.insert(colorDrawerDialogColorField, "Цвет главного меню 3:Цвет диалога");

    m_descriptions.insert(iconDisabledNotificationsField, "Иконка Нет уведомлений: Иконка для состояния Нет уведомлений");
    m_descriptions.insert(iconDisabledCinemahallField, "Иконка Кинозал Нет Уведомлений: Иконка для состояния Нет уведомлений");

    m_descriptions.insert(iconLogoutField, "Иконка выхода:Иконка для кнопки выхода");

    m_descriptions.insert(colorComboBoxAccentField, "Цвет выпадающий список 1:Цвет акцента на области элемента");
    m_descriptions.insert(colorComboBoxHintField, "Цвет выпадающий список 2:Цвет подсказки");
    m_descriptions.insert(colorComboBoxBackgroundField, "Цвет выпадающий список 3:Цвет фона области элемента");
    m_descriptions.insert(iconCustomGroupField, "Иконка пользовательских групп:Иконка для кнопки управления пользовательскими группами");
    m_descriptions.insert(iconEditField, "Иконка редактирования:Иконка для кнопки редактирования");

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
