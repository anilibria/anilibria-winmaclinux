#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QProcess>
#include <QNetworkRequest>
#include <QCoreApplication>
#include <QFile>
#include <QGuiApplication>
#include <QClipboard>
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
    connect(m_versionChecker, &ApplicationVersionChecker::noVersionAvailable, this, &ApplicationsViewModel::noVersionAvailable);

#ifdef Q_OS_MACOS
    m_lastInstallPathPrefix = "xattr -d com.apple.quarantine ";
#endif

#ifdef Q_OS_LINUX
    m_lastInstallPathPrefix = "sudo chmod +x ";
#endif
}

void ApplicationsViewModel::setInstallPath(const QString &installPath) noexcept
{
    if (m_installPath == installPath) return;

    m_installPath = installPath;
    emit installPathChanged();
}

void ApplicationsViewModel::setInstallIndex(const QString &installIndex) noexcept
{
    if (m_installIndex == installIndex) return;

    m_installIndex = installIndex;
    emit installIndexChanged();
}

bool ApplicationsViewModel::isInstalledInstaller() const noexcept
{
    return m_applications.value(1)->isInstalled();
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

void ApplicationsViewModel::installByIndex()
{
    if (m_installIndex.isEmpty()) return;
    if (m_loading) return;

    auto name = m_installIndex;
    auto iterator = std::find_if(
        m_applications.begin(),
        m_applications.end(),
        [name](ExternalApplicationModel* model) {
            return model->name() == name;
        }
    );
    if (iterator == m_applications.end()) return;

    auto application = *iterator;
    if (!application->isInstalled() && (m_installPath == nullptr || m_installPath.isEmpty())) return;
    if (application->isInstalled() && !application->isHaveNewVersion()) return;

    m_currentApplication = application;
    if (!application->isInstalled()) m_currentApplication->setInstalledPath(m_installPath);

    auto isArm = QSysInfo::currentCpuArchitecture().toLower().startsWith("arm");
    QString extension = "";
#ifdef Q_OS_WIN
    QString osPath = "windows";
    extension = ".exe";
#endif

#ifdef Q_OS_MACOS
    QString osPath = "macos";
#endif

#ifdef Q_OS_LINUX
    QString osPath = "linux";
#endif

#ifdef Q_OS_FREEBSD
    QString osPath = "linux";
#endif

    osPath = osPath + (isArm ? "arm64" : "64");
    auto version = application->newVersion();
    qDebug() << "Install version: " << version;
    auto downloadUrl = QString("https://github.com/") + application->repositoryPath() + "/releases/download/" + version + "/" + osPath + extension;
    qDebug() << "Install URL: " << downloadUrl;
    auto url = QUrl(downloadUrl);
    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    m_networkManager->get(request);
    m_loading = true;
    emit loadingChanged();
}

void ApplicationsViewModel::checkNewVersions()
{
    m_loading = true;
    emit loadingChanged();
    foreach (auto application, m_applications) {
        if (application->isIncludedInsideDistributive()) continue;

        m_versionChecker->checkNewVersionAvailable(application->name(), application->repositoryPath(), application->installedVersion());
    }
}

void ApplicationsViewModel::clearInstallData()
{
    m_installPath = "";
    m_installIndex  = -1;

    emit installPathChanged();
    emit installIndexChanged();
}

void ApplicationsViewModel::deleteByIndex(const QString& index)
{
    if (index.isEmpty()) return;

    auto name = index;
    auto iterator = std::find_if(
        m_applications.begin(),
        m_applications.end(),
        [name](ExternalApplicationModel* model) {
            return model->name() == name;
        }
        );
    if (iterator == m_applications.end()) return;

    auto item = *iterator;

    auto path = item->installedPath() + "/" + item->executableName();
    QFile file (path);
    if (!file.remove(path)) return;

    item->setInstalledPath("");
    item->setIsInstalled(false);
    item->setInstalledVersion("");
    item->setIsHaveNewVersion(false);

    writeCache();
    refresh();
    checkNewVersions();
}

void ApplicationsViewModel::runInstaller()
{
    auto application = m_applications[1];
    auto pathToInstaller = application->installedPath() + "/" + application->executableName();
    QStringList arguments;
    QProcess::startDetached(pathToInstaller, arguments);
    QCoreApplication::quit();
}

void ApplicationsViewModel::copyLastInstalledPath()
{
    QClipboard *clipboard = QGuiApplication::clipboard();
    clipboard->setText(m_lastInstallPathPrefix + m_lastInstallPath);

    m_lastInstallPath = "";
    emit lastInstallPathChanged();
}

void ApplicationsViewModel::createApplications()
{
    auto torrentStream = new ExternalApplicationModel();
    torrentStream->setName("TorrentStream");
    torrentStream->setDescription("Программа позволяет стримить торренты и является аналогом просмотра онлайн, также позволяет Вам скачивать торренты, следить за прогрессом скачивания а впоследствии смотреть уже скачанные видео через плеер приложения со всеми фичами которые там есть");
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
    installer->setExecutableName("AnilibriaQtInstaller.exe");
#else
    installer->setExecutableName("AnilibriaQtInstaller");
#endif
    m_applications.append(installer);

    auto cacheChecker = new ExternalApplicationModel();
    cacheChecker->setName("LocalCacheChecker");
    cacheChecker->setDescription("Программа позволяет выполнять синхронизацию релизов, франшиз и расписания и формировать файлы кеша которые впоследствии может потребить приложение AniLibria.Qt.");
    cacheChecker->setRepositoryPath("trueromanus/LocalCacheChecker");
    cacheChecker->setIsIncludedInsideDistributive(false);
    cacheChecker->setIsInstalled(false);
    cacheChecker->setIsHaveNewVersion(false);
#ifdef Q_OS_WIN
    cacheChecker->setExecutableName("LocalCacheChecker.exe");
#else
    cacheChecker->setExecutableName("LocalCacheChecker");
#endif
    m_applications.append(cacheChecker);
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

    saveJsonArrayToFile(m_cacheFileName, array);
}

void ApplicationsViewModel::versionDownloaded(QNetworkReply *reply)
{
    m_loading = false;

    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << reply->errorString();
        emit loadingChanged();
        return;
    }

    auto content = reply->readAll();
    if (content.size() == 0) {
        qDebug() << "Response content is empty";
        emit loadingChanged();
        return;
    }

    auto fullPath = m_currentApplication->installedPath() + "/" + m_currentApplication->executableName();
    if(QFile::exists(fullPath)) QFile::remove(fullPath);

    qDebug() << "Full path for installed application: " << fullPath;

    QFile file(fullPath);
    if (!file.open(QFile::WriteOnly)) {
        emit loadingChanged();
        return;
    }

    file.write(content);
    file.close();

    qDebug() << "File sucessfully saved";

    m_currentApplication->setIsInstalled(true);
    m_currentApplication->setIsHaveNewVersion(false);
    m_currentApplication->setInstalledVersion(m_currentApplication->newVersion());

    if (m_currentApplication->name() == "TorrentStream") {
        emit changeTorrentStreamPath(fullPath);
    }

#ifdef Q_OS_LINUX
    m_lastInstallPath = fullPath;
    emit lastInstallPathChanged();
#endif

    writeCache();
    emit loadingChanged();
    refresh();
}

void ApplicationsViewModel::newVersionAvailable(QString version, QString url, QString appIdentifier)
{
    foreach (auto application, m_applications) {
        if (application->isIncludedInsideDistributive()) continue;
        if (application->name() != appIdentifier) continue;

        application->setNewVersion(version);
        if (application->isInstalled()) application->setIsHaveNewVersion(true);
    }

    m_loading = false;
    emit loadingChanged();
    refresh();
}

void ApplicationsViewModel::noVersionAvailable()
{
    m_loading = false;
    emit loadingChanged();
}
