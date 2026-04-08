## Сборка пакета flatpak
### Для сборки на Fedora
Предварительно надо скопировать файл `tv.anilibria.app.yml` из [корня репозитория](https://github.com/anilibria/anilibria-winmaclinux/blob/master/tv.anilibria.app.yml).
```
sudo dnf install flatpak flatpak-builder
sudo flatpak install flathub org.kde.Platform//5.15-22.08 org.kde.Sdk//5.15-22.08
flatpak-builder --force-clean build tv.anilibria.app.yml
flatpak-builder --user --install --force-clean build tv.anilibria.app.yml
flatpak build-bundle ~/.local/share/flatpak/repo/ anilibria.flatpak tv.anilibria.anilibria
```
