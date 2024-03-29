# Как пользоваться функционалом "Свой фильтр"

## Что это такое?
Это специальный фильтр, его можно найти в Тематических фильтрах. Суть его работы следующая, берется файлик со скриптом написанным Вами и прогоняется по всем релизам.
Вы можете в скрипте написать какую угодно логику фильтрации, все ограничено только Вашей фантазией.

## Как пользоваться?
Вначале необходимо создать файл скрипта с расширением ajs (как его написать ниже). Далее откройте настройки каталога релизов (кнопка с шестеренкой) и **Файл со своим фильтром**. Можете выбрать файл нажав на 
кнопку **Выбрать** или вписав путь руками. После чего выбираете раздел **Свой фильтр** и смотрите на результат. Если Вы допустили ошибки в скрипте, Вы получите соответствующее сообщение в
центре уведомлений.

## Как писать скрипты?
Для написания скриптов используется язык JavaScript в редакции ES7 (но есть некоторые фичи которые могут не поддерживаться).  
Текст сприпта выполняется как есть (т.е. выполняется функция eval из JS), Вам в скрипте необходимо вернуть **true** или **false**
в зависимости от того нужно ли показывать релиз в списке(true) или нет (false). Для работы доступен глобально объявленный объект **release**.
Простые сценарии могут быть написаны в одну строку.  
(В примере ниже мы фильтруем релизы по русскому названию **title**, останутся только те релизы в которых в названии присутствуют символы **ар**)

```js
release.title.indexOf('ар') > -1
```

Более сложные сценарии могут быть разбиты на функции и содержать больше кода, например

```js
function check(item) {
	return item.title.indexOf('ар') > -1;
}

check(release)
```

### Поля доступные в объекте release
**title** поле содержит название релиза на русском  
**code** поле содержит название код релиза  
**season** поле содержит сезон (зима лето и т.п.)  
**status** поле содержит статус выпуска (в работе, завершен и т.п.)  
**year** поле содержит год выпуска (в виде строки)  
**description** поле содержит описание (строка может содержать HTML разметку)  
**releaseType** поле содержит тип релиза  
**id** поле содержит идентификатор релиза  
**countOnlineVideos** поле содержит количество серий онлайн в виде числа  
**countTorrents** поле содержит количество торрентов в виде числа  
**announce** поле содержит анонсированное сообщение  
**originalName** поле содержит оригинальное имя релиза (на японском или другом языке)  
**rating** поле содержит рейтинг в виде числа  
**timestamp** поле содержит дату обновления в виде числа в формате unix timestamp  
**type** поле содержит дату тип релиза  
**series** поле содержит количество серий  
**genres** поле содержит жанры в виде массив со строками  
**voicers** поле содержит войсеров в виде массив со строками
