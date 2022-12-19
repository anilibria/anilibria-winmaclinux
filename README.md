# AniLibria.Qt

Если Вы найдете ошибки или будут идеи по улучшению приложения то оформляйте ишью в этом репозитории или пишите [на этом сайте](http://anilibriadesktop.reformal.ru/), заранее спасибо. Следите за новостями о выпуске новых версий, входящих в них изменений, а также прочих новостей в [телеграм канале](https://t.me/desktopclientanilibria).  

[![Project CI](https://github.com/anilibria/anilibria-winmaclinux/actions/workflows/c-cpp.yml/badge.svg)](https://github.com/anilibria/anilibria-winmaclinux/actions/workflows/c-cpp.yml)

## Системные требования:

**Windows** - Windows 7+, с графической картой поддерживающей OpenGL 2.1+  
**macOS** - macOS 10.13+ (устройства с M1 пока не поддерживаются)  
**Linux** - Есть deb пакет, rpm пакет, aur пакет, сборка из исходников для остальных (информация ниже)

## Откуда качать приложение?

Последнюю стабильную версию качаем [отсюда](https://github.com/anilibria/anilibria-winmaclinux/releases/latest).  
Все версии [здесь](https://github.com/anilibria/anilibria-winmaclinux/releases).

## Как установить и обновлять?

### Windows

Для установки просто качаем релиз и распаковываем, внутри запуcкаем файл Anilibria.exe.  
Для обновления выполняем ровно тоже действие, старую версию можно будет удалить или сохранить это остается на Ваше усмотрение.  

### macOS

Для установки качаем релиз и монтируем диск, далее запускаем приложение AniLibria.  
Если у Вас отображается сообщение **Программу AniLibria не удалось открыть так как ее автор является неустановленным разработчиком** то воспользуйтесь следующей [инструкцией](https://support.apple.com/ru-ru/guide/mac-help/mh40616/mac).  
Для обновления выполняем ровно тоже действие, старую версию можно будет удалить или сохранить это остается на Ваше усмотрение.

### Linux

Подробные инструкции для пакетов и сборки из исходников ниже.

## Пакеты для пакетных менеджеров Linux

### Arch linux
Установите пакет из aur [anilibria-winmaclinux-git](https://aur.archlinux.org/packages/anilibria-winmaclinux-git) своим любимым aur helper'ом. Например `yay`:

```console
$ yay -S anilibria-winmaclinux-git
```
### Rosa Linux

```console
$ sudo dnf install anilibria-winmaclinux
```

### AltLinux
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

## Сборка из исходников:

### Готовые инструкции для сборки под дистрибутивы:

[Ubuntu/Linux Mint 20](https://github.com/anilibria/anilibria-winmaclinux/blob/master/linuxmint20.md)  
[Fedora](https://github.com/anilibria/anilibria-winmaclinux/blob/master/fedora.md)  
[macOS](https://github.com/anilibria/anilibria-winmaclinux/blob/master/macosbuild.md)

### Для сборки необходимо:
- Минимальная поддерживаемая версия Qt 5.13.1
- Поддержка C++11 (минимальные версии компиляторов MSVC15, GCC 4.8 или CLANG 3.3)
- Модули Qt - multimedia, graphicaleffects, svg, websocket
- Для сборки проекта используется qmake
- Для Windows сборки требуется зависимость libVLC версии 3.0.8

### Linux

OpenSSL 1.1.1 и старше  
GStreamer 1.0

```bash
cd src
qmake
sudo make install
```
Для более быстрой сборки рекомендуется использование флага `-jX`, где X - количество парраллельно работающих процессов компиляции. Как правило, указывается количество ядер/потоков, поддерживаемых Вашим процессором.
