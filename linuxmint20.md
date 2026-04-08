## Сборка из исходников на Ubuntu/Linux Mint 23+

*Данный набор команд написан для полностью чистой системы.*  
**Внимание! Если найдете ошибки в командах или будут предложения по улучшению то создавайте ишью!**

**Этап 1. Устанавливаем зависимости необходимые для сборки**
```shell
sudo apt install git build-essential git libmpv-dev pkg-config cmake qt6-base-dev qt6-declarative-dev qt6-websockets-dev qt6-svg-dev libxkbcommon-dev qml6-module-*
```
**Этап 2. Создаем папку для проекта, переходим в нее и извлекаем исходники (предполагается что Вы находитесь в домашней папке)**
```shell
mkdir anilibria
cd anilibria/
git clone https://github.com/anilibria/anilibria-winmaclinux.git
cd anilibria-winmaclinux/src/
```
**Этап 3. Выполняем сборку и установку**
Подготовка сборки:
```shell
cmake -S src -B build -DCMAKE_BUILD_TYPE=Release -DANILIBERTY_LESS_68_QT=On
```
Сборка и установка:
```shell
sudo cmake --build build -t install
```
**Необязательный последний этап. После всех манипуляций можно удалить созданную папку командой**
```shell
cd ~
rm -rf anilibria/
```

### Как найти приложение?
Приложение будет доступно по имени AniLiberty.

### Как обновить приложение?
Для того чтобы обновить приложение можно выполнить шаги начиная с этапа 2. Первый этап в таком случае выполнять не надо!
