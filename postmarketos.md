## Сборка из исходников на PostmarketOs (Alpine)

*Данный набор команд написан для полностью чистой системы.*  
**Внимание! Если найдете ошибки в командах или будут предложения по улучшению то создавайте ишью!**

**Этап 1. Устанавливаем зависимости необходимые для сборки**
```shell
sudo apk add qt5-qtbase-devel qt5-qtwebview-devel qt5-qtwebengine-devel qt5-qtmultimedia-devel qt5-qtsvg-devel qt5-qtwebsockets-devel qt5-qtdeclarative-devel qt5-qtquickcontrols2-devel qt5-qtquickcontrols 
sudo apk add vlc vlc-qt vlc-dev g++ make git 
```
**Этап 2. Создаем папку для проекта, переходим в нее и извлекаем исходники (предполагается что Вы находитесь в домашней папке)**
```shell
mkdir anilibria
cd anilibria/
git clone https://github.com/anilibria/anilibria-winmaclinux.git
cd anilibria-winmaclinux/src/
```
**Этап 3. Редактируем файлы для мобильной адаптации**

Необходимо заменить в файле src/main.qml следующие строчки:
```shell
ApplicationWindow {
    id: window
    visible: true
    width: 800
    minimumWidth: 800
    minimumHeight: 600
    height: 600
```
на эти
```shell
ApplicationWindow {
    id: window
    visible: true
    width: 300
    minimumWidth: 300
    minimumHeight: 300
    height: 300
```

Так же заменить в файле src/AniLibria.pro следующие строчки:
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
qmake-qt5
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
