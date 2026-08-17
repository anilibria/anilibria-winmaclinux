## Сборка Flatpak

Манифест `tv.anilibria.app.yml` собирает приложение из Qt6/CMake и отдельно
встраивает `libmpv` вместе с его библиотечными зависимостями. Зависимость на
другое Flatpak-приложение `io.mpv.Mpv` не используется: содержимое приложений
не доступно друг другу внутри sandbox.

### Установка инструментов (Fedora)

```bash
sudo dnf install flatpak flatpak-builder
flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo
flatpak install flathub org.kde.Platform//6.11 org.kde.Sdk//6.11
```

### Сборка и установка

```bash
flatpak-builder --force-clean build tv.anilibria.app.yml
flatpak-builder --user --install --force-clean build tv.anilibria.app.yml
flatpak run tv.anilibria.anilibria
```

Для повторных сборок можно убрать `--force-clean`; flatpak-builder использует
локальный кэш модулей.

### Bundle

```bash
flatpak build-bundle \
  ~/.local/share/flatpak/repo \
  anilibria.flatpak \
  tv.anilibria.anilibria
```

### CI

`.github/workflows/flatpak.yml` использует контейнер KDE 6.11 и тот же манифест
для `x86_64` и `aarch64`.
