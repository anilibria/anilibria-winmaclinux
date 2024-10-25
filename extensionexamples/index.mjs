import { application, logger, httpClient, globalValues, releases } from './utilites.mjs'

// if need to make some logic dependence from application version
if (application.checkIfAppVersionIs('2.2.17')) logger.message('Application version is ' + application.getAppVersion());

// way to define items in menu for this item
export function menuItems() {
  return [
    {
      title: 'Проверить GET запрос',
      handler: function () {
        httpClient.get('https://jsonplaceholder.typicode.com/users/1',
          {
            callback: (res) => {
              logger.message(`Request result: ${res}`);
            }
          }
        )
      }
    },
    {
      title: 'Проверить POST запрос',
      handler: function () {
        httpClient.post('https://jsonplaceholder.typicode.com/posts',
          '{"test": "value"}',
          {
            callback: (res) => {
              logger.message(`Post Request result: ${res}`);
            }
          }
        )
      }
    },
    {
      title: 'Проверить сохранение в значения',
      handler: function () {
        globalValues.saveValue(`myvalue`, `LALALALLALA`);
        
        const savedValue = globalValues.readValue(`myvalue`);
        if (savedValue === `LALALALLALA`) {
          logger.message(`Values completely saved!!!!`);
        } else {
          logger.message(`Values saved failed!!!!`);
        }
      }
    },
    {
      title: 'Получить релиз',
      handler: function () {
        const release = releases.getRelease(9777)
        logger.message(release.code);
        logger.message(release.season);
        logger.message(release.status);
        logger.message(release.year);
      }
    },
    {
      title: 'Оставить сообщение для пользователя',
      handler: function () {
        logger.sendInformation('Привет пользователь :)');
      }
    }
  ]
}

// global triggers
export function releaseOpenedInVideoPlayer(releaseId, title, seria) {
  logger.message(`releaseOpenedInVideoPlayer(${releaseId}, ${title}, ${seria})`);
}

// way to define module name and author
export const moduleName = 'Module Example';
export const moduleAuthor = 'Ikari';