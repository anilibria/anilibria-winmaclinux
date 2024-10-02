## Сборка из исходников на PostmarketOs (Alpine)

*Данный набор команд написан для полностью чистой системы.*  
**Внимание! Если найдете ошибки в командах или будут предложения по улучшению то создавайте ишью!**

**Этап 1. Устанавливаем зависимости необходимые для сборки**
```shell
sudo apk add qt5-qtbase-dev qt5-qtwebview-dev qt5-qtwebengine-dev qt5-qtmultimedia-dev qt5-qtsvg-dev qt5-qtwebsockets-dev qt5-qtdeclarative-dev qt5-qtquickcontrols2-dev qt5-qtquickcontrols 
sudo apk add mpv-dev g++ make git pkgconf
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
qmake-qt5 CONFIG+=unixmpv
make
sudo make install
```

**Необязательный последний этап. После всех манипуляций можно удалить созданную папку командой**
```shell
cd ~
rm -rf anilibria/
```

### TorrentStream
Воизбежание проскоков видео, рекомендуюется скачать и настроить [TorrentStream](https://github.com/trueromanus/TorrentStream)

Для этого, скачайте последнюю версию из [releases](https://github.com/trueromanus/TorrentStream/releases), разместите файл в удобной для Вас дерриктории, зайдите в приложение AniLibria и во вкладке TorrentStream укажите путь до файла.

### Как найти приложение?
Приложение будет доступно из меню по имени AniLibria.

### Как обновить приложение?
Для того чтобы обновить приложение можно выполнить шаги начиная с этапа 2. Первый этап в таком случае выполнять не надо!


