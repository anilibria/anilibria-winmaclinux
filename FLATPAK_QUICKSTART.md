# Быстрый старт Flatpak

## Требования

```bash
flatpak --version
flatpak-builder --version
flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
flatpak install flathub org.kde.Platform//6.11 org.kde.Sdk//6.11
```

## Проверка metadata

```bash
appstreamcli validate --explain tv.anilibria.anilibria.appdata.xml
flatpak-builder --show-manifest tv.anilibria.app.yml >/dev/null
```

## Сборка

```bash
flatpak-builder --force-clean --keep-build-dirs build tv.anilibria.app.yml
```

Манифест сам собирает `libmpv`; отдельно установленное приложение
`io.mpv.Mpv` для сборки не требуется.

## Установка и запуск

```bash
flatpak-builder --user --install --force-clean build tv.anilibria.app.yml
flatpak run tv.anilibria.anilibria
```

## Bundle

```bash
flatpak build-bundle \
  ~/.local/share/flatpak/repo \
  anilibria.flatpak \
  tv.anilibria.anilibria
```

## Очистка

```bash
flatpak uninstall --user tv.anilibria.anilibria
rm -rf build .flatpak-builder
```

## Известное ограничение локальной проверки

В headless WSL-сеансе можно проверить старт процесса и отсутствие linker/runtime
ошибок, но для визуальной проверки Qt окна нужен рабочий X11/Wayland display.
