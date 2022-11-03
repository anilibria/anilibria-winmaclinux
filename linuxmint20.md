## Сборка из исходников на Ubuntu/Linux Mint 20

*Данный набор команд написан для полностью чистой системы.*  
**Внимание! Если найдете ошибки в командах или будут предложения по улучшению то создавайте ишью!**

**Этап 1. Устанавливаем зависимости необходимые для сборки**
```shell
sudo apt install git build-essential qt5-default qtdeclarative5-dev qtmultimedia5-dev libqt5multimedia5-plugins libqt5svg5-dev libqt5webview5-dev qtwebengine5-dev qml-module-qtgraphicaleffects  qml-module-qtquick-layouts qml-module-qtquick-dialogs qml-module-qtquick-controls qml-module-qtquick-controls2 qml-module-qt-labs-folderlistmodel qml-module-qt-labs-settings qml-module-qtmultimedia qml-module-qtwebview libqt5websockets5-dev qdbus-qt5 qdbus
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

### Сборка с плеером QtAV
Если у Вас наблюдаются проблемы с штатным плеером gstreamer (например во время загрузки нет ожидания и после возврата воспроизведения он прыгает вперед на минуту а то и дальше) то можно собрать с плеером QtAV. Преимущество такого плеера в том что он не зависит от системы и основан на базе ffmpeg.  
Чтобы это сделать нужно выполнить дополнительные действия после этапов описанных выше.  
**После этапа 1 выполнить:**  
```shell
sudo apt install libqtav-dev qml-module-qtav qtav-players vainfo libass9
```  
**После этапа 2 выполнить:**
```shell
sed -i 's/m_isStandartPlayer(true)/m_isStandartPlayer(false)/g' Classes/ViewModels/onlineplayerwindowviewmodel.cpp
```

