## Сборка из исходников на Linux Mint 20

*Данный набор команд написан для полностью чистой системы.*  
**Внимание! Если найдете ошибки в командах или будут предложения по улучшению то создавайте ишью!**

**Этап 1. Устанавливаем зависимости необходимые для сборки**
```shell
sudo apt install git
sudo apt install build-essential
sudo apt install qt5-default
sudo apt install qtdeclarative5-dev
sudo apt install qtmultimedia5-dev libqt5multimedia5-plugins
sudo apt install libqt5svg5-dev
sudo apt install libqt5webview5-dev
sudo apt install qml-module-qtgraphicaleffects
sudo apt install qml-module-qtquick-layouts
sudo apt install qml-module-qtquick-dialogs
sudo apt install qml-module-qtquick-controls
sudo apt install qml-module-qtquick-controls2
sudo apt install qml-module-qt-labs-folderlistmodel
sudo apt install qml-module-qt-labs-settings
sudo apt install qml-module-qtmultimedia
```
**Этап 2. Создаем папку для проекта, переходим в нее и извлекаем исходники (предполагается что Вы находитесь в домашней папке)**
```shell
mkdir anilibria
cd anilibria/
git clone https://github.com/anilibria/anilibria-winmaclinux.git
cd anilibria-winmaclinux/src/
```
**Этап 3. Выполняем сборку**
```shell
qmake
sudo make install
```
**Необязательный последний этап. После всех манипуляций можно удалить созданную папку командой**
```shell
cd ~
sudo rm -rf anilibria/
```

### Как найти приложение?
Приложение будет доступно из меню по имени AniLibria.

### Как обновить приложение?
Для того чтобы обновить приложение можно выполнить шаги начиная с этапа 2. Первый этап в таком случае выполнять не надо!

