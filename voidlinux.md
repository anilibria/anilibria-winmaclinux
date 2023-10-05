## Сборка из исходников на Void Linux

*Данный набор команд написан для полностью чистой системы.*  
**Внимание! Если найдете ошибки в командах или будут предложения по улучшению то создавайте ишью!**

**Этап 1. Устанавливаем зависимости необходимые для сборки**
```shell
sudo xbps-install -S gcc git qt5-declarative-devel qt5-webview-devel qt5-webengine-devel qt5-multimedia-devel qt5-svg-devel qt5-websockets-devel qt5-declarative-devel qt5-quickcontrols2-devel qt5-quickcontrols  vlc vlc-devel gstreamer1
```
**Этап 2. Создаем папку для проекта, переходим в нее и извлекаем исходники (предполагается что Вы находитесь в домашней папке)**
```shell
mkdir anilibria
cd anilibria/
git clone https://github.com/anilibria/anilibria-winmaclinux.git
cd anilibria-winmaclinux/src/
```
**Этап 3. (опциональный). Сборка с плеером VLC**  
Необходимо заменить в файле src/AniLibria.pro следующие строчки
```shell
#unix {
#    LIBS += -lvlc

#    INCLUDEPATH += /usr/include/
#    DEPENDPATH += /usr/include/

#    INCLUDEPATH += /usr/include/vlc/plugins
#    DEPENDPATH += /usr/include/vlc/plugins

#    CONFIG += buildwithvlc
#}
```
на эти
```shell
unix {
    LIBS += -lvlc

    INCLUDEPATH += /usr/include/
    DEPENDPATH += /usr/include/

    INCLUDEPATH += /usr/include/vlc/plugins
    DEPENDPATH += /usr/include/vlc/plugins

    CONFIG += buildwithvlc
}
```
**Этап 4. Выполняем сборку**
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
