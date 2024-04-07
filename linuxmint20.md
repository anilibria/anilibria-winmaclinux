## Сборка из исходников на Ubuntu/Linux Mint 20

*Данный набор команд написан для полностью чистой системы.*  
**Внимание! Если найдете ошибки в командах или будут предложения по улучшению то создавайте ишью!**

**Этап 1. Устанавливаем зависимости необходимые для сборки**
```shell
sudo apt install git build-essential qtdeclarative5-dev qtmultimedia5-dev libqt5multimedia5-plugins libqt5svg5-dev libqt5webview5-dev qtwebengine5-dev qml-module-qtgraphicaleffects  qml-module-qtquick-layouts qml-module-qtquick-dialogs qml-module-qtquick-controls qml-module-qtquick-controls2 qml-module-qt-labs-folderlistmodel qml-module-qt-labs-settings qml-module-qtmultimedia qml-module-qtwebview libqt5websockets5-dev qdbus-qt5 qdbus qtquickcontrols2-5-dev qml-module-qtquick-particles2 libvlccore-dev libvlc-dev vlc pkg-config
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
qmake CONFIG+=unixvlc
```
Без VLC:

```shell
qmake
```

**Этап 4. Выполняем сборку и установку**

```shell
make
sudo make install
````

**Необязательный последний этап. После всех манипуляций можно удалить созданную папку командой**
```shell
cd ~
rm -rf anilibria/
```

### Как найти приложение?
Приложение будет доступно из меню по имени AniLibria.

### Как обновить приложение?
Для того чтобы обновить приложение можно выполнить шаги начиная с этапа 2. Первый этап в таком случае выполнять не надо!
