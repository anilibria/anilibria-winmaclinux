# Вопросы/ответы а также дополнительная информация по использованию плеера mpv в приложении

## Как использовать свой mpv.conf?
### 1. Через папку кеша
Необходимо открыть папку кеша:
* Linux - `cd ~/<user>/.local/share/EmptyFlow/AnilibriaDesktopClient`
* Windows - `cd %LOCALAPPDATA%\EmptyFlow\AnilibriaDesktopClient`
* macOS - `cd /Users/<user>/Library/Application Support/EmptyFlow/AnilibriaDesktopClient`
  
Далее в этой папке создаем папку `mpv`.  
Создаем в ней файл `mpv.conf` и добавляем интересующие Вас настройки для плеера mpv.

## Где не подключен?
Плеер mpv не подключен для flatpak.  
В данный момент пытаемся решить данную проблему.  

## Приложение падает при старте на Windows (ошибка 0xc0000142 или похожая)
Проблема в том что mpv плеер у Вас не работает. Для решения необходимо заменить файлик libmpv-2.dll.  
Скачать данный файлик можно ниже, после скачивание необходимо распаковать архив и скопировать указанный выше файл в папку с приложением заменив тот что там есть.
  
Необходимо сделать следующее:
- Вначале попробовать скачать этот файлик https://sourceforge.net/projects/mpv-player-windows/files/libmpv/mpv-dev-x86_64-v3-20240505-git-c4b6d0d.7z/download
- Если не помогло то этот https://sourceforge.net/projects/mpv-player-windows/files/libmpv/mpv-dev-x86_64-20240505-git-c4b6d0d.7z/download
- Если и это не помогло то тогда открывайте https://sourceforge.net/projects/mpv-player-windows/files/libmpv/ и качайте последовательно все архивы содержащие в названии x86_64. Какой-то из них должен сработать

## Интересные дополнения
* [Anime4k](https://github.com/bloc97/Anime4K)


