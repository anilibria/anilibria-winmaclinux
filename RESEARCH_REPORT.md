# Исследование Flatpak после миграции на Qt6/CMake

## Вывод

Текущий `src/CMakeLists.txt` собирает приложение `AniLiberty` с Qt6, QML и
`libmpv` через `pkg_check_modules(LIBMPV REQUIRED mpv)`. Поэтому рабочий Flatpak
должен содержать не только Qt runtime, но и библиотеку `libmpv` с `mpv.pc`.
Установка отдельного приложения `io.mpv.Mpv` не является зависимостью сборки:
Flatpak-приложения изолированы, и файлы одного приложения не становятся
библиотеками другого.

## Что было устаревшим

Старый манифест использовал:

- KDE/Qt5 runtime `5.15-24.08`;
- `qmake` вместо CMake;
- старые пути desktop-файла и иконки;
- отсутствие модуля `libmpv`;
- широкие host/system-bus разрешения.

Это несовместимо с текущим Qt6/CMake проектом.

## Реализованное решение

`tv.anilibria.app.yml` теперь:

- использует `org.kde.Platform` и `org.kde.Sdk` ветки `6.11`;
- собирает `libmpv` через Meson с `-Dlibmpv=true` и `-Dcplayer=false`;
- собирает `libplacebo` и `libass` как вложенные модули;
- собирает приложение через `cmake-ninja` из зафиксированного commit;
- добавляет локальный AppStream-файл как manifest source, чтобы metadata была
  синхронизирована с рабочей копией;
- переименовывает установленный desktop-файл и иконки в имена app-id;
- устанавливает AppStream metadata в `share/metainfo`;
- не использует host-библиотеки и не вызывает `flatpak` из sandbox сборки.

Desktop-файл, установленный проектным CMake, называется
`tv.aniliberty.AniLiberty.desktop`; post-install приводит его к
`tv.anilibria.anilibria.desktop` и задаёт `Exec=/app/bin/AniLiberty`.

## Проверки

Локально выполнено:

```text
appstreamcli validate --explain tv.anilibria.anilibria.appdata.xml  # success
flatpak-builder --show-manifest tv.anilibria.app.yml                # parsed
flatpak-builder --force-clean --keep-build-dirs build tv.anilibria.app.yml
flatpak-builder --user --install --force-clean build tv.anilibria.app.yml
flatpak info tv.anilibria.anilibria
```

Результат сборки:

- manifest успешно собран и экспортирован;
- runtime: `org.kde.Platform/x86_64/6.11`;
- приложение установлено в user Flatpak installation;
- `flatpak run tv.anilibria.anilibria` запустил процесс без немедленного
  runtime/linker failure. В headless WSL-сеансе процесс остановлен внешним
  `timeout`, поэтому визуальная проверка окна не выполнялась.

## Источники

- Flatpak Builder documentation:
  https://docs.flatpak.org/en/latest/flatpak-builder.html
- Flatpak sandbox permissions:
  https://docs.flatpak.org/en/latest/sandbox-permissions.html
- Flathub application requirements:
  https://docs.flathub.org/docs/for-app-authors/requirements/
- Official MPV Flatpak manifest:
  https://raw.githubusercontent.com/flathub/io.mpv.Mpv/master/io.mpv.Mpv.yml
- Official libmpv packaging pattern (Spotube):
  https://raw.githubusercontent.com/flathub/com.github.KRTirtho.Spotube/master/com.github.KRTirtho.Spotube.yml
- KDE Flatpak guidance:
  https://community.kde.org/Guidelines_and_HOWTOs/Flatpak
