# Как создавать расширения для приложения AniLibria.Qt

## Что из себя представляет расширение
Расширение это набор из одного или более mjs файлов. Файлы содержат код на языке JavaScript.
Для расширения необходимо создать отдельную папку разместить в ней как минимум один файл `index.mjs`.
Файл `index.mjs` является точкой входа в расширение.

## Глобальное API
Глобальное API также упаковано в более удобный объектный вариант в файле `utilities.mjs`.
Я буду указывать как глобальный вариант так и объектный из `utilities.mjs`.

### appVersion/application.getAppVersion()
Это глобальная константа содержащая версию приложения в виде строки.

### application.checkIfAppVersionIs(version)
Метод для проверки что версия равна той что передана в параметре version

### appExtension.log(message)/application.message(message)
Написать в консоль сообщение из message с префиксом `[extension]`.

### application.saveValue(name, value)/globalValues.saveValue(name, value)
Сохранить строковое значение из value в пременную с именем name.

### application.readValue(name)/globalValues.readValue(name)
Получить значение из переменной с именем name.

### application.deleteValue(name)/globalValues.deleteValue(name)
Удалить переменную с именем name.

### appExtension.getRelease(id)/releases.getRelease(id)
Получить релиз по идентификатору из id.
В результате будет объект со следующими полями:
* title
* code
* season
* status
* year
* description
* releaseType
* id
* countOnlineVideos
* countTorrents
* announce
* originalName
* rating
* timestamp
* type
* series
* genres
* voicers

### appExtension.getFavorites()/releases.getFavorites()
Получить массив с идентификаторами релизов добавленных в избранное

### appExtension.getSeens()/releases.getSeens()
Получить массив с идентификаторами просмотренных полностью релизов

### appExtension.getSeens()/releases.getReleasesCount()
Получить общее количество релизов

### appExtension.makeHttpGet(url, headers, callback)/httpClient.get(url, options)
Сделать HTTP GET запрос.
* url - полный URL адрес
* headers - Массив со строками для указания заголовков запросов
* callback - Функция которая будет вызвана для передачи результата

### appExtension.makeHttpDelete(url, headers, callback)/httpClient.delete(url, options)
Сделать HTTP GET запрос.
* url - полный URL адрес
* headers - Массив со строками для указания заголовков запросов
* callback - Функция которая будет вызвана для передачи результата

### appExtension.makeHttpPost(url, headers, callback)/httpClient.post(url, options)
Сделать HTTP POST запрос.
* url - полный URL адрес
* body - тело запроса
* headers - Массив со строками для указания заголовков запросов
* callback - Функция которая будет вызвана для передачи результата

### appExtension.makeHttpPut(url, headers, callback)/httpClient.put(url, options)
Сделать HTTP POST запрос.
* url - полный URL адрес
* body - тело запроса
* headers - Массив со строками для указания заголовков запросов
* callback - Функция которая будет вызвана для передачи результата

## Тригеры
Тригеры это события происходящие в приложении которые Вы можете обрабатывать.

### releaseOpenedInVideoPlayer
События происходит каждый раз когда меняется видео в видеоплеере (через серию, карточку из списка и т.п.).
Параметры:
* releaseId - идентификатор релиза
* title - русское название релиза
* seria - номер серии по порядку (серии начинаются с 0)


