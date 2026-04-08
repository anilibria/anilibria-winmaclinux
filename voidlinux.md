## Сборка из исходников на Void Linux

*Данный набор команд написан для полностью чистой системы.*  
**Внимание! Если найдете ошибки в командах или будут предложения по улучшению то создавайте ишью!**

**Этап 1. Устанавливаем зависимости необходимые для сборки**
```shell
sudo xbps-install -S cmake ninja git qt6-tools qt6-base-devel qt6-declarative-devel qt6-websockets-devel qt6-svg-devel qt6-dbus qt6-multimedia-devel mpv-devel gstreamer1-devel
```
**Этап 2. Создаем папку для проекта, переходим в нее и извлекаем исходники (предполагается что Вы находитесь в домашней папке)**
```shell
mkdir anilibria
cd anilibria/
git clone https://github.com/anilibria/anilibria-winmaclinux.git
cd anilibria-winmaclinux
```
**Этап 3. Настройка сборки**

```shell
cmake -S src -B build -DCMAKE_BUILD_TYPE=Release -DANILIBERTY_LESS_68_QT=On
```

**Этап 4. Выполняем сборку и установку**
```shell
sudo cmake --build build -t install
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
