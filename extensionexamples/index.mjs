import { application, logger, httpClient, globalValues } from './utilites.mjs'

// if need to make some logic dependence from application version
if (application.checkIfAppVersionIs('2.2.17')) logger.message('Application version is ' + application.getAppVersion());

// way to define items in menu for this item
export function menuItems() {
  return [
    {
      title: 'Проверить GET запрос',
      handler: function () {
        logger.message(`Get started`);
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
    }
  ]
}

// global triggers
export function releaseOpenedInVideoPlayer(releaseId, title, seria) {
  logger.message(`releaseOpenedInVideoPlayer(${releaseId}, ${title}, ${seria})`);
}

// way to define module name and author
export const moduleName = 'Test Module Name';
export const moduleAuthor = 'Megaman';