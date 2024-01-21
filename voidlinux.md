## Сборка из исходников на Void Linux

*Данный набор команд написан для полностью чистой системы.*  
**Внимание! Если найдете ошибки в командах или будут предложения по улучшению то создавайте ишью!**

**Этап 1. Устанавливаем зависимости необходимые для сборки**
```shell
sudo xbps-install -S gcc git qt5-declarative-devel qt5-webview-devel qt5-webengine-devel qt5-multimedia-devel qt5-svg-devel qt5-websockets-devel qt5-declarative-devel qt5-quickcontrols2-devel qt5-quickcontrols gstreamer1
```
**Этап 2. Создаем папку для проекта, переходим в нее и извлекаем исходники (предполагается что Вы находитесь в домашней папке)**
```shell
mkdir anilibria
cd anilibria/
git clone https://github.com/anilibria/anilibria-winmaclinux.git
cd anilibria-winmaclinux/src/
```
**Этап 3. Настройка сборки**

Сборка с VLC:

```shell
sudo xbps-install -S vlc vlc-devel pkg-config
qmake CONFIG+=unixvlc
```

без VLC:

```shell
qmake
```

**Этап 4. Выполняем сборку и установку**
```shell
make
sudo make install
```

**Необязательный последний этап. После всех манипуляций можно удалить созданную папку командой**
```shell
cd ~
rm -rf anilibria/
```

### Как найти приложение?
Приложение будет доступно из меню по имени AniLibria.

### Как обновить приложение?
Для того чтобы обновить приложение можно выполнить шаги начиная с этапа 2. Первый этап в таком случае выполнять не надо!
