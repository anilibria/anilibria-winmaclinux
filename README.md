# AniLiberty.Qt

Если Вы найдете ошибки или будут идеи по улучшению приложения то оформляйте ишью в этом репозитории или пишите в [телеграм группе](https://t.me/+Le_oNL4Tw745YWUy), заранее спасибо. Следите за новостями о выпуске новых версий, входящих в них изменений, а также прочих новостей в [телеграм канале](https://t.me/desktopclientanilibria). 

[![Project CI](https://github.com/anilibria/anilibria-winmaclinux/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/anilibria/anilibria-winmaclinux/actions/workflows/c-cpp.yml)
[![Flatpak x86_64 Build](https://github.com/anilibria/anilibria-winmaclinux/actions/workflows/flatpak.yml/badge.svg?branch=deploy)](https://github.com/anilibria/anilibria-winmaclinux/actions/workflows/flatpak.yml)

## Системные требования:

**Windows** - Windows 7+, с графической картой поддерживающей OpenGL 2.1+  
**macOS** - macOS 13+ (устройства с M1 поддерживаются через Rosetta2)  
для версий младше 13 необходимо [скачать архив](https://github.com/anilibria/anilibria-winmaclinux/releases/download/2.2.28/libmpvmacos.zip) и после установки версии распаковать файлы из архива в папку `<путь где установлено приложение>/AniLibria.app/Contents/Frameworks/`. Если и это не помогло можете скачать приложение IINA и из нее папку Contents/Frameworks в аналогичную в приложении AniLibria.  
**Linux** - Есть deb пакет, flatpak пакет, aur пакет, сборка из исходников для остальных (информация ниже)

## Откуда качать приложение?

Последнюю стабильную версию качаем [отсюда](https://github.com/anilibria/anilibria-winmaclinux/releases/latest).  
Все версии [здесь](https://github.com/anilibria/anilibria-winmaclinux/releases).

## Как установить и обновлять?

### Windows

Для установки просто качаем релиз и распаковываем, внутри запуcкаем файл Anilibria.exe.  
Для обновления выполняем ровно тоже действие, старую версию можно будет удалить или сохранить это остается на Ваше усмотрение.  
На `Windows 7/8` необходимо заменить библиотеку `libmpv-2.dll` на другую которую можно [скачать тут](https://sourceforge.net/projects/mpv-player-windows/files/libmpv/mpv-dev-x86_64-20230917-git-181eddc.7z/download).  
Если на `Windows 7/8` Вы скачали версию Qt6 Вам надо заменить файлы в папке на [эти](https://github.com/crystalidea/qt6windows7) иначе ничего работать не будет.

### macOS

Для установки качаем релиз и монтируем диск, далее запускаем приложение AniLibria.  
  
После скачивания необходимо перейти в папку куда Вы скачали файл dmg в терминале и выполнить команду
```shell
xattr -d com.apple.quarantine AniLibria.dmg
```
Для обновления выполняем ровно тоже действие, старую версию можно будет удалить или сохранить это остается на Ваше усмотрение.

## Связанные проекты
[TorrentStream](https://github.com/trueromanus/TorrentStream) - приложение веб сервер позволяющий скачивать торренты, стримить их а также проксировать видео для плеера  
[AnilibriaQtInstaller](https://github.com/trueromanus/AnilibriaQtInstaller) - веб инсталлер/апдейтер для приложения  
[LocalCacheChecker](https://github.com/trueromanus/LocalCacheChecker) - приложение для синхронизации релизов/франшиз/расписания

### Linux (пакеты и сборка из исходников)

Подробные инструкции для пакетов и сборки из исходников ниже.

## Пакеты для пакетных менеджеров Linux

[![Packaging status](https://repology.org/badge/vertical-allrepos/anilibria-winmaclinux.svg)](https://repology.org/project/anilibria-winmaclinux/versions)

### Arch linux
Установите пакет из aur [anilibria-winmaclinux-git](https://aur.archlinux.org/packages/anilibria-winmaclinux-git) своим любимым aur helper'ом. Например `yay`:

```console
$ yay -S anilibria-winmaclinux-git
```
### Rosa Linux

```console
$ sudo dnf install anilibria-winmaclinux
```

### ALT Linux
Вы можете установить anilibria-winmaclinux через любой графический менеджер пакетов или через консоль:

```console
$ su - -c 'apt-get install anilibria-winmaclinux'
```

### Debian
Скачайте deb пакет со страницы релизов. Допустим скачали в корень домашней директории.  
Далее выполните в консоли:  
```console
$ sudo dpkg -i ~/anilibria_0.0.0_amd64.deb
$ sudo apt-get install -f
```

### NixOS
Добавить пакет `anilibria-winmaclinux` в список пакетов:

* Общесистемно:

    ```nix
    environment.systemPackages = with pkgs; [ anilibria-winmaclinux ];
    ```

* Для определенного пользователя (в примере имя пользователя - bob)

    ```nix
    users.users.bob.packages = with pkgs; [ anilibria-winmaclinux ];
    ```

### [Nix](nix.md)

### Flatpak
Скачайте flatpack пакет со страницы релиза и выполните следующую команду:
```console
$ flatpak install --user anilibria.flatpak
```
## Сборка из исходников:

### Готовые инструкции для сборки под дистрибутивы:

[Ubuntu/Linux Mint 23+](https://github.com/anilibria/anilibria-winmaclinux/blob/master/linuxmint20.md)  
[openSUSE](https://github.com/anilibria/anilibria-winmaclinux/blob/master/openSUSE.md)  
[Fedora](https://github.com/anilibria/anilibria-winmaclinux/blob/master/fedora.md)  
[macOS](https://github.com/anilibria/anilibria-winmaclinux/blob/master/macosbuild.md)  
[PostmarketOs](https://github.com/anilibria/anilibria-winmaclinux/blob/master/postmarketos.md)  
[Void Linux](https://github.com/anilibria/anilibria-winmaclinux/blob/master/voidlinux.md)  
[Steam Deck](https://github.com/JerzyEx/anilibria_at_steamdeck)

### Для сборки необходимо:
Поддерживаются две версии Qt6 и Qt5. Рекомендовано собирать на Qt6 потому что Qt5 скоро перестанет поддерживаться
- Для Qt6 минимальная поддерживаемая версия 6.4. Для Qt5 минимальная поддерживаемая версия Qt 5.15.2.
- Для Qt6 минимальная версия C++20. Для Qt5 минимальная версия C++14.
- Модули Qt - multimedia, graphicaleffects, svg, websocket, particleeffects
- Для сборки проекта Qt6 используется cmake, для сборки проекта Qt5 используется qmake.
- Для сборки требуется зависимость libmpv. Для Qt5 также нужно libVLC версии 3.0.20.

### Linux

OpenSSL 1.1.1 и старше  
GStreamer 1.10  
libmpv-2

Для Qt6
```bash
cmake -S src -B build
cmake --build build -t install
```

Для Qt5
```bash
cd src
qmake
make
sudo make install
```
Для более быстрой сборки рекомендуется использование флага `-jX`, где X - количество парраллельно работающих процессов компиляции. Как правило, указывается количество ядер/потоков, поддерживаемых Вашим процессором.

В зависимости от дистрибутива, требуется выполнение `qmake-qt5` вместо `qmake`.

### Контрибуторы проекта
Большое спасибо Всем контрибуторам и волонтерам проекта!  
Вы можете [посмотреть вклад каждого из них.](https://github.com/anilibria/anilibria-winmaclinux/graphs/contributors)  
Если Вы хотите стать одним из контрибуторов или волонтеров то любая помощь приветствуется!
  
![list of conributors](https://contrib.rocks/image?repo=anilibria/anilibria-winmaclinux)  
*Contribution list made with [contrib.rocks](https://contrib.rocks).*
