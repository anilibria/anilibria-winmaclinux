## Сборка из исходников на openSUSE Tumbleweed

*Данные советы по сборке написаны без тестирования на чистой системе!*

**Внимание! Если найдете ошибки в командах или будут предложения по улучшению, то создавайте ишью!**

Этапы разбиты так, чтобы на каждом шаге при возникновении ошибок можно было, опираясь на них, последовательно скачивать недостающие библиотеки. Пакетный менеджер имеет команды `zypper search` (`se`) для поиска пакета, `sudo zypper install` (`in`) для установки и `sudo zypper remove` (`rm`) для удаления.


**Этап 1. Скачиваем проект и переходим в папку src**

Любым способом. Можно распаковать архив со [страницы релизов](https://github.com/safing/portmaster/releases/tag/latest). В соседних руководствах по сборке рекомендуется клонировать репозиторий:
```shell
mkdir anilibria
cd anilibria/
git clone https://github.com/anilibria/anilibria-winmaclinux.git
cd anilibria-winmaclinux/src/
```

**Этап 2. Формируем make-файл**

Сначала проверяем данные зависимости:
```shell
sudo zypper install libqt5-qtbase-common-devel libqt5-qtmultimedia-devel libqt5-qtsvg-devel libqt5-qtwebsockets-devel libQt5QuickControls2-devel
```
Zypper должен подцепить все неупомянутые в команде нужные пакеты. Затем пробуем:
```shell
qmake-qt5
```
Если нет вывода в консоль, это успех. Если пишет `Project ERROR: Unknown module(s) in QT: ...`, то ищем в zypper соответствующие пакеты.


**Этап 3. Выполняем сборку**

Проверяем данные зависимости:
```shell
sudo zypper install libQt5Concurrent-devel libQt5DBus-devel
```

Пробуем компилировать:
```shell
sudo make install
```
или, если хотим выделить X потоков процессора:
```shell
sudo make install -jX
```
В случае ошибки в процессе ищем созвучный ей пакет.


**Этап 4 (опциональный). Сборка с плеером VLC**

Проверяем зависимости:
```shell
sudo zypper install vlc libvlc5
```
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

**Необязательный последний этап**

После всех манипуляций можно удалить папку со скачанным репозиторием. Например, командами
```shell
cd ~
sudo rm -rf anilibria/
```

### Как найти приложение?
Приложение будет доступно из меню по имени AniLibria.

### Как обновить приложение?
Для того, чтобы обновить приложение, нужно выполнить те же шаги, но можно не проверять наличие пакетов.
