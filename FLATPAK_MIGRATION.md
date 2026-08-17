# Миграция Flatpak на Qt6/CMake

## До изменений

Старый манифест запускал `qmake` на KDE/Qt5 runtime. Текущий проект использует
Qt6, CMake, QML module registration и обязательный `libmpv` через pkg-config.
Старый манифест поэтому не мог собрать текущий `src/CMakeLists.txt`.

## Текущий манифест

`tv.anilibria.app.yml`:

- `org.kde.Platform` / `org.kde.Sdk` `6.11`;
- `cmake-ninja` для AniLiberty;
- `meson` для `libmpv`;
- вложенные `libplacebo` и `libass`;
- фиксированные исходники и хэши архивов;
- desktop/AppStream/icon post-install alignment;
- Wayland, fallback X11, PulseAudio, DRI и network permissions;
- только нужные D-Bus `talk-name` разрешения для inhibit sleep;
- без `--filesystem=host`, host `PKG_CONFIG_PATH` и зависимости на другое
  Flatpak-приложение.

## Metadata alignment

CMake upstream устанавливает `tv.aniliberty.AniLiberty.desktop` и
`aniliberty.png`. App id проекта — `tv.anilibria.anilibria`, поэтому manifest
после установки:

1. меняет `Exec` на `/app/bin/AniLiberty`;
2. меняет `Icon` на `tv.anilibria.anilibria`;
3. переименовывает desktop и PNG в app-id names;
4. устанавливает AppStream metadata в `share/metainfo`.

## Локальная проверка

Сборка и установка:

```bash
flatpak-builder --force-clean --keep-build-dirs build tv.anilibria.app.yml
flatpak-builder --user --install --force-clean build tv.anilibria.app.yml
flatpak run tv.anilibria.anilibria
```

Проверка metadata:

```bash
appstreamcli validate --explain tv.anilibria.anilibria.appdata.xml
flatpak-builder --show-manifest tv.anilibria.app.yml
```

## CI

`.github/workflows/flatpak.yml` переведён на контейнер
`ghcr.io/flathub-infra/flatpak-github-actions:kde-6.11` и использует тот же
manifest для `x86_64` и `aarch64`.
