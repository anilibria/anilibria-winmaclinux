# Flatpak: что сделано

## Причина изменений

Проект перешёл на Qt6 и CMake, а старый Flatpak-манифест всё ещё использовал
Qt5 и qmake. Такой манифест не соответствует текущему `src/CMakeLists.txt`.

## Реализовано

### Манифест

Файл: `tv.anilibria.app.yml`

- runtime и SDK: `org.kde.Platform` / `org.kde.Sdk` версии `6.11`;
- сборка AniLiberty через `cmake-ninja`;
- исходники приложения зафиксированы на commit текущего `master`;
- Qt6 QML-модули берутся из KDE runtime;
- добавлена сборка `libmpv` через Meson;
- добавлены зависимости `libplacebo` и `libass`;
- все внешние архивы имеют SHA-256 checksum;
- MPV собирается как библиотека (`libmpv`), а не как отдельное приложение.

Отдельное Flatpak-приложение `io.mpv.Mpv` не используется: файлы одного
Flatpak-приложения нельзя использовать как библиотеки другого приложения.

### Desktop и metadata

CMake upstream устанавливает следующие имена:

```text
 tv.aniliberty.AniLiberty.desktop
 aniliberty.png
```

Post-install манифеста приводит их к app-id приложения:

```text
tv.anilibria.anilibria.desktop
tv.anilibria.anilibria.png
```

Также устанавливается:

```text
/app/bin/AniLiberty
/app/share/metainfo/tv.anilibria.anilibria.metainfo.xml
```

AppStream metadata была исправлена и проходит проверку:

```bash
appstreamcli validate --explain tv.anilibria.anilibria.appdata.xml
```

### Sandbox permissions

Добавлены необходимые разрешения для:

- Wayland;
- fallback X11;
- PulseAudio;
- GPU/DRI;
- сети;
- загрузок и видео;
- предотвращения блокировки экрана во время воспроизведения.

Host-библиотеки и `--filesystem=host` не используются.

## Локальная проверка

Сборка:

```bash
flatpak-builder --force-clean --keep-build-dirs build tv.anilibria.app.yml
```

Установка:

```bash
flatpak-builder --user --install --force-clean build tv.anilibria.app.yml
```

Запуск:

```bash
flatpak run tv.anilibria.anilibria
```

Создание bundle:

```bash
flatpak build-bundle \
  ~/.local/share/flatpak/repo \
  anilibria.flatpak \
  tv.anilibria.anilibria
```

## Результат проверки

Локально подтверждено:

- Flatpak manifest успешно разобран;
- Qt6/CMake-сборка завершилась без ошибок;
- `libmpv.so.2` присутствует внутри приложения;
- `mpv.pc` присутствует внутри приложения;
- `ldd /app/bin/AniLiberty` не показывает отсутствующих библиотек;
- desktop-файл, иконки и AppStream metadata экспортируются;
- приложение устанавливается в user Flatpak installation;
- процесс запускается через `flatpak run`.

В headless WSL-среде визуальное Qt-окно не проверялось, но запуск и загрузка
динамических библиотек проверены.

## Источники

- Flatpak Builder: https://docs.flatpak.org/en/latest/flatpak-builder.html
- Sandbox permissions: https://docs.flatpak.org/en/latest/sandbox-permissions.html
- Flathub application requirements: https://docs.flathub.org/docs/for-app-authors/requirements/
- Official MPV manifest: https://raw.githubusercontent.com/flathub/io.mpv.Mpv/master/io.mpv.Mpv.yml
- Official libmpv packaging pattern: https://raw.githubusercontent.com/flathub/com.github.KRTirtho.Spotube/master/com.github.KRTirtho.Spotube.yml

## Файлы изменений

```text
tv.anilibria.app.yml
tv.anilibria.anilibria.appdata.xml
flatpak-build.md
docs/FLATPAK.md
```

Workflow CI намеренно не изменялся в этом PR.

