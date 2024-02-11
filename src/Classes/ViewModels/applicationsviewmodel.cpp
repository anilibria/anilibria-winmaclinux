#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QFile>
#include "applicationsviewmodel.h"
#include "../../globalhelpers.h"

ApplicationsViewModel::ApplicationsViewModel(QObject *parent)
    : QObject{parent}
{
    createApplications();

    readCache();
    refresh();

    connect(m_networkManager, &QNetworkAccessManager::finished, this, &ApplicationsViewModel::versionDownloaded);
    connect(m_versionChecker, &ApplicationVersionChecker::newVersionAvailable, this, &ApplicationsViewModel::newVersionAvailable);
}

void ApplicationsViewModel::refresh()
{
    m_items.clear();

    foreach(auto application, m_applications) {
        QVariantMap map;
        map["applicationName"] = application->name();
        map["isInstalled"] = application->isInstalled();
        map["isHaveNewVersion"] = application->isHaveNewVersion();
        map["includedInDistributive"] = application->isIncludedInsideDistributive();
        map["installedPath"] = application->installedPath();
        map["description"] = application->description();
        m_items.append(map);
    }

    emit itemsChanged();
}

void ApplicationsViewModel::installByIndex(const QString& name, const QString &path)
{
    auto iterator = std::find_if(
        m_applications.begin(),
        m_applications.end(),
        [name](ExternalApplicationModel* model) {
            return model->name() == name;
        }
    );
    if (iterator == m_applications.end()) return;

    auto application = *iterator;

    if (!application->isHaveNewVersion()) return;

    m_currentApplication = application;
    m_currentApplication->setInstalledPath(path);

    auto isArm = QSysInfo::currentCpuArchitecture().toLower().startsWith("arm");
#ifdef Q_OS_WIN
    QString osPath = "windows";
#endif

#ifdef Q_OS_MACOS
    QString osPath = "macos";
#endif

#ifdef Q_OS_LINUX
    QString osPath = "linux";
#endif

    osPath = osPath + (isArm ? "arm64" : "64");
    auto version = application->newVersion();
    auto downloadUrl = QString("https://github.com/") + application->repositoryPath() + "/releases/download/" + version + "/" + osPath + ".zip";
    auto url = QUrl(downloadUrl);
    QNetworkRequest request(url);
    m_networkManager->get(request);
    m_loading = true;
    emit loadingChanged();
}

void ApplicationsViewModel::checkNewVersions()
{
    foreach (auto application, m_applications) {
        if (application->isIncludedInsideDistributive()) continue;
        if (!application->isInstalled()) continue;

        m_versionChecker->checkNewVersionAvailable(application->name(), application->repositoryPath(), application->installedVersion());
    }
}

void ApplicationsViewModel::createApplications()
{
    auto torrentStream = new ExternalApplicationModel();
    torrentStream->setName("TorrentStream");
    torrentStream->setDescription("Программа позволяет стримить торренты и являеться аналогом просмотра онлайн, также позволяет Вам скачивать торренты, следить за прогрессом скачивания а впоследствии смотреть уже скачанные видео через плеер приложения со всеми фичами которые там есть");
    torrentStream->setRepositoryPath("trueromanus/TorrentStream");
#ifdef Q_OS_WIN
    torrentStream->setExecutableName("TorrentStream.exe");
    torrentStream->setIsIncludedInsideDistributive(true);
    torrentStream->setIsInstalled(true);
    torrentStream->setIsHaveNewVersion(false);
#else
    torrentStream->setExecutableName("TorrentStream");
    torrentStream->setIsIncludedInsideDistributive(false);
    torrentStream->setIsInstalled(false);
    torrentStream->setIsHaveNewVersion(false);
#endif

    m_applications.append(torrentStream);

    auto installer = new ExternalApplicationModel();
    installer->setName("Installer/AutoUpdater/Launcher");
    installer->setDescription("Программа позволяет устанавливать и обновлять версии приложения. Также может быть установлена как точка запуска для приложения которая при каждом старте будет проверять наличие новых версий и обновлять при их наличии.");
    installer->setRepositoryPath("trueromanus/AnilibriaQtInstaller");
    installer->setIsIncludedInsideDistributive(false);
    installer->setIsInstalled(false);
    installer->setIsHaveNewVersion(false);
#ifdef Q_OS_WIN
    torrentStream->setExecutableName("AnilibriaQtInstaller.exe");
#else
    torrentStream->setExecutableName("AnilibriaQtInstaller");
#endif
    m_applications.append(installer);
}

void ApplicationsViewModel::readCache()
{
    createIfNotExistsCacheFile(m_cacheFileName, "[]");
    auto cachePath = getJsonContentFromFile(m_cacheFileName);

    QJsonDocument jsonDocument = QJsonDocument::fromJson(cachePath.toUtf8());
    auto installedItems = jsonDocument.array();

    foreach (auto installedItem, installedItems) {
        auto itemObject = installedItem.toObject();
        if (!itemObject.contains("index")) continue;
        if (!itemObject.contains("path")) continue;
        if (!itemObject.contains("version")) continue;

        auto index = itemObject["index"].toInt();
        auto path = itemObject["path"].toString();
        auto version = itemObject["version"].toString();

        if (index >= m_applications.size()) continue;
        auto application = m_applications.value(index);
        if (application->isIncludedInsideDistributive()) continue;

        application->setInstalledPath(path);
        application->setInstalledVersion(version);
        application->setIsInstalled(true);
    }

}

void ApplicationsViewModel::writeCache()
{
    QJsonArray array;
    int index = -1;
    foreach (auto application, m_applications) {
        index++;
        if (application->isIncludedInsideDistributive()) continue;
        if (!application->isInstalled()) continue;

        QJsonObject applicationItem;
        applicationItem["index"] = index;
        applicationItem["path"] = application->installedPath();
        applicationItem["version"] = application->installedVersion();

        array.append(applicationItem);
    }

    saveJsonArrayToFile(getCachePath(m_cacheFileName), array);
}

void ApplicationsViewModel::versionDownloaded(QNetworkReply *reply)
{
    m_loading = false;
    auto fullPath = m_currentApplication->installedPath() + "/" + m_currentApplication->executableName();
    if(QFile::exists(fullPath)) QFile::remove(fullPath);

    QFile file(fullPath, this);
    file.write(reply->readAll());
    file.close();

    m_currentApplication->setIsInstalled(true);

    writeCache();
    emit loadingChanged();
}

void ApplicationsViewModel::newVersionAvailable(QString version, QString url, QString appIdentifier)
{
    foreach (auto application, m_applications) {
        if (application->isIncludedInsideDistributive()) continue;
        if (!application->isInstalled()) continue;
        if (application->name() != appIdentifier) continue;

        application->setIsHaveNewVersion(true);
        application->setNewVersion(version);
    }

    emit itemsChanged();
}
