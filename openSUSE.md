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

**Этап 2. Устанавливаем зависимости**
```shell
sudo zypper install libqt5-qtbase-common-devel libqt5-qtmultimedia-devel libqt5-qtsvg-devel libqt5-qtwebsockets-devel libQt5QuickControls2-devel
```

Для сборки с VLC нужно доустановить (возможно, ещё нужен пакет `libvlc5`):
```shell
sudo zypper install vlc-devel pkg-config
```
Zypper должен подцепить все неупомянутые в команде нужные пакеты.

**Этап 3. Настройка сборки**
Для сборки с VLC:
```shell
qmake-qt5 CONFIG+=unixvlc
```

Без VLC:
```shell
qmake-qt5
```
Если нет вывода в консоль, это успех. Если пишет `Project ERROR: Unknown module(s) in QT: ...`, то ищем в zypper соответствующие пакеты.


**Этап 4. Выполняем сборку и установку**

Проверяем данные зависимости:
```shell
sudo zypper install libQt5Concurrent-devel libQt5DBus-devel
```

Пробуем компилировать:
```shell
make
sudo make install
```
или, если хотим выделить X потоков процессора:
```shell
make
sudo make install -jX
```
В случае ошибки в процессе ищем созвучный ей пакет.

**Необязательный последний этап**

После всех манипуляций можно удалить папку со скачанным репозиторием. Например, командами
```shell
cd ~
rm -rf anilibria/
```
Также, командой `make clean` можно очистить только объектные (промежуточные) файлы компиляции.

### Как найти приложение?
Приложение будет доступно из меню по имени AniLibria.

### Как обновить приложение?
Для того, чтобы обновить приложение, нужно выполнить те же шаги, но можно не проверять наличие пакетов.
