## Сборка из исходников на PostmarketOs (Alpine)

*Данный набор команд написан для полностью чистой системы.*  
**Внимание! Если найдете ошибки в командах или будут предложения по улучшению то создавайте ишью!**

**Этап 1. Устанавливаем зависимости необходимые для сборки**
```shell
sudo apk add cmake ninja git qt6-qttools qt6-qtbase-dev qt6-qtdeclarative-dev qt6-qtwebengine-dev qt6-qtsvg-dev qt6-qtmultimedia-dev mpv-dev qt6-qtshadertools-dev qt6-qtdeclarative-private-dev
sudo apk add mpv-dev g++ make git pkgconf
```
**Этап 2. Создаем папку для проекта, переходим в нее и извлекаем исходники (предполагается что Вы находитесь в домашней папке)**
```shell
mkdir anilibria
cd anilibria/
git clone https://github.com/anilibria/anilibria-winmaclinux.git
cd anilibria-winmaclinux/src/
```

**Этап 3. Настройка сборки**

```shell
cmake -S src -B build -DCMAKE_BUILD_TYPE=Release -DANILIBERTY_LESS_68_QT=On
```

**Этап 4. Выполняем сборку и установку**
```shell
sudo cmake --build build --parallel --target install
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


