## Сборка из исходников на Linux Mint 20

Данный набор команд написан для полностью чистой системы.

Устанавливаем зависимости необходимые для сборки
```shell
sudo apt install git
sudo apt install build-essential
sudo apt install qt5-default
sudo apt install qtdeclarative5-dev
sudo apt install qtmultimedia5-dev libqt5multimedia5-plugins
sudo apt install libqt5svg5-dev
sudo apt install libqt5webview5-dev
```
Создаем папку для проекта, переходим в нее и извлекаем исходники
```shell
mkdir anilibria
cd anilibria/
git clone https://github.com/anilibria/anilibria-winmaclinux.git
cd anilibria-winmaclinux/src/
```
Выполняем сборку
```shell
qmake
sudo make install
```

