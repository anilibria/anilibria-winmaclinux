# Сборка для macOS

#### Если найдете ошибки или будет что добавить в эту инструкцию обязательно заводите ишью или делайте PR в этот репозиторий

## Предварительная подготовка
* Установить git (возможно через [home brew](https://brew.sh/) с помощью команды `brew install git`)
* Установить xcode (через AppStore) последней версии и обновлять когда будут выходить новые версии
* Установить [Qt Creator](https://www.qt.io/download-qt-installer). Требуется регистрация аккаунта и VPN если Вы из России, далее необходимо выбрать Kit 6.2.3 64bit ARM

## Подготовка проекта
* Создайте себе папку для работы например `mkdir ~/anilibria`
* Перейдите в нее `cd ~/anilibria`
* Извлеките репозиторий `git clone https://github.com/anilibria/anilibria-winmaclinux.git`

## Сборка проекта
* Запустите `Qt Creator` и выберите `Файл`-> `Открыть файл или проект` далее выберите файл путь (из примера ~/anilibria/anilibria-winmaclinux/src/CMakeLists.txt)
* Выберите проект `6.2.3 64bit` когда Вам предложат
* Далее перейдите в режим `Release` слева внизу будет кнопка Anilibria и экранчик внизу, нажмите ее и нажмите на Выпуск/Release
* Нажмите на кнопку с молотком она самая нижняя под кнопкой из предыдущего пункта

## Перед деплоем проекта
* Откройте приложение `Связка ключей`
* Выберите `Система` и далее выберите `Связка ключей` -> `Ассистент сертификации` -> `Создать сертификат`.
* В диалоге выберите имя (латинскими буквами без пробелов и запомните его) и `Тип идентификации` = `Самоподп. корневой сертификат`
* Галкните на `Заменить настройки по умолчанию` и нажмите `Продолжить`
* Выберите `Срок действия в днях` по больше если не хотите пересоздавать часто сертификат.
* Далее необходимо заполнить ряд полей по типу Организация, отдел и т.п. я верю в Вашу фантазию
* `Размер ключа` и `Алгоритм` оставьте по умолчанию а если быть точнее то 2048 бит и RSA
* На следующем экране оставьте только `Подпись` и далее `Подпись кода`
* Укажите связку ключей `Система` и нажмите на `Создать` и далее нажмите `Готово`
* Выберите сертификат в списке и нажмите на стролочку вправо чтобы раскрыть пункт
* Раскроется пункт `Закрытый ключ` нажмите ПКМ на нем и далее `Свойства`
* Выберите вкладку `Доступ` и жмакните на `Разрешить всем программам получать доступ к этому объекту`
* Далее преезагрузите компьютер

## libmpv
Если надо добавить плеер mpv необходимо вначале установить brew.
Далее выполнить команду:
```shell
brew install mpv
```
Необходимо узнать зависимости файла:
```shell
dyld_info -dependents libmpv.2.dylib
```
Далее необходимо скопировать файлы:
```shell
cp /usr/local/opt/mpv/lib/libmpv.2.dylib
cp /usr/local/opt/jpeg-turbo/lib/libjpeg.8.dylib
cp /usr/local/opt/libass/lib/libass.9.dylib
cp /usr/local/opt/ffmpeg/lib/libavcodec.61.dylib
cp /usr/local/opt/ffmpeg/lib/libavfilter.10.dylib
cp /usr/local/opt/ffmpeg/lib/libavformat.61.dylib
cp /usr/local/opt/ffmpeg/lib/libavutil.59.dylib
cp /usr/local/opt/ffmpeg/lib/libswresample.5.dylib
cp /usr/local/opt/ffmpeg/lib/libswscale.8.dylib
cp /usr/local/opt/libplacebo/lib/libplacebo.338.dylib
cp /usr/local/opt/mujs/lib/libmujs.dylib
cp /usr/local/opt/little-cms2/lib/libcms2.2.dylib
cp /usr/local/opt/libarchive/lib/libarchive.13.dylib
cp /usr/local/opt/ffmpeg/lib/libavdevice.61.dylib
cp /usr/local/opt/libbluray/lib/libbluray.2.dylib
cp /usr/local/opt/luajit/lib/libluajit-5.1.2.dylib
cp /usr/local/opt/rubberband/lib/librubberband.2.dylib
cp /usr/local/opt/uchardet/lib/libuchardet.0.dylib
cp /usr/local/opt/vapoursynth/lib/libvapoursynth-script.0.dylib
cp /usr/local/opt/zimg/lib/libzimg.2.dylib
```

## Деплой проекта
* Вам необходимо собрать проект в release моде из раздела `Сборка проекта`
* Далее перейдите в папку сборки она будет в нашем примере она будет находиться в папке ~/anilibria/anilibria-winmaclinux/ и называться примерно build_X-Release (вместо X обычно название kit и разрядность приложения)
* Внутри папки мы увидим папку AniLibria.app если она есть выполните следующую команду в этой папке `sudo macdeployqt AniLibria.app -appstore-compliant -dmg -qmldir=../src -codesign=signname`. Вместо `signname` вставьте имя сертификата который Вы создали в разделе `Перед деплоем проекта` которое было латиницей и без пробелов.
* Если все нормально то мы получим на выходе в этой папке файл AniLibria.dmg, обязательно проверьте что он работает путем монтирования и запуска приложения AniLibria
* Файл AniLibria.dmg его уже можно считать полностью сборанной и готовой к распространению программой

## Какие шаги нужны при повторной сборке с деплоем
* Затянуть последние изменения из git (необходимо быть в папке с проектом) `git pull`
* Выполните пункты из раздела `Сборка проекта`
* Выполните пункты из раздела `Деплой проекта`
  
 
