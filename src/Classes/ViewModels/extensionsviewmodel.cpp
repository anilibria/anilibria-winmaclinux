#include <QJSEngine>
#include <QDebug>
#include <QUuid>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include "extensionsviewmodel.h"
#include "../../globalconstants.h"
#include "../../globalhelpers.h"

ExtensionsViewModel::ExtensionsViewModel(QObject *parent)
    : QObject{parent}
{
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &ExtensionsViewModel::requestFinished);

    m_valuesPath = getCachePath("extensionvalues.cache");
    m_extensionsPath = getCachePath("extensions.cache");
    readExtensions();
    readValues();

#ifdef QT_DEBUG
    m_extensions.append("C:/work/Repositories/Anilibria.Qt/testScript");
#endif

    adjustEngine();
    importExtensions();
}

void ExtensionsViewModel::releaseOpenedInVideoPlayer(int releaseId, const QString &title, int seria)
{
    foreach (auto module, m_importedModules) {
        if (!module.hasProperty("releaseOpenedInVideoPlayer")) continue;

        QJSValue handleFunction = module.property("releaseOpenedInVideoPlayer");
        if (handleFunction.isUndefined() || handleFunction.isNull()) continue;

        QJSValueList args;

        QJSValue releaseIdValue(releaseId);
        args.append(releaseIdValue);

        QJSValue titleValue(title);
        args.append(titleValue);

        QJSValue seriaValue(seria);
        args.append(seriaValue);

        handleFunction.call(args);
    }
}

void ExtensionsViewModel::makeHttpGet(const QString &url, const QList<QString> headers, const QJSValue &callback)
{
    if (!callback.isCallable()) {
        qDebug() << "[extension]: Callback for execute URL " << url << " not a function!";
        return;
    }

    auto uuid = QUuid::createUuid();
    auto identifier = uuid.toString();
    m_pendingCallbacks.insert(identifier, callback);

    QNetworkRequest request(url);
    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    auto reply = m_networkManager->get(request);
    reply->setProperty("pendingIndentifier", identifier);
}

void ExtensionsViewModel::log(const QString &message)
{
    qDebug() << "[extension]: " << message;
}

void ExtensionsViewModel::saveValue(const QString &key, const QString &value)
{
    m_values.insert(key, value);

    //immediatly save values to file
    saveValues();
}

QString ExtensionsViewModel::readValue(const QString &key)
{
    if (!m_values.contains(key)) return "";

    return m_values.value(key);
}

void ExtensionsViewModel::deleteValue(const QString &key)
{
    if (!m_values.contains(key)) return;

    m_values.remove(key);
}

void ExtensionsViewModel::addExtension(const QString &path)
{
    m_extensions.append(path);

    importExtension(path);

    remapDisplayExtensions();
}

void ExtensionsViewModel::importExtensions()
{
    foreach (auto extension, m_extensions) {
        importExtension(extension);
    }

    remapDisplayExtensions();
}

void ExtensionsViewModel::importExtension(const QString &path)
{
    auto pathToIndex = path + "/index.mjs";
    qDebug() << "[extension]: Try to load extension in path: " << pathToIndex;

    QJSValue module = m_engine->importModule(pathToIndex);
    if (module.isError()) {
        qDebug() << "[extension]: Error caught at line " << module.property("lineNumber").toInt() << ": " << module.toString();
        return;
    }

    QJSValue moduleNameString = module.property("moduleName");
    QString moduleName = "";
    if (moduleNameString.isString()) moduleName = moduleNameString.toString();

    QJSValue moduleAuthorString = module.property("moduleAuthor");
    QString moduleAuthor = "";
    if (moduleAuthorString.isString()) moduleAuthor = moduleAuthorString.toString();

    m_importedModules.insert(path, module);
    auto metadata = std::make_tuple(moduleName, moduleAuthor);
    m_importedModulesMetadata.insert(path, metadata);
}

void ExtensionsViewModel::adjustEngine()
{
    m_engine->installExtensions(QJSEngine::ConsoleExtension);

    m_engine->globalObject().setProperty("appVersion", ApplicationVersion);
    QJSValue extensionsObject = m_engine->newQObject(this);
    m_engine->globalObject().setProperty("appExtension", extensionsObject);
}

void ExtensionsViewModel::readValues()
{
    createIfNotExistsFile(m_valuesPath, "{}");

    auto json = getJsonContentFromFile(m_valuesPath);
    auto content = QJsonDocument::fromJson(json.toUtf8());
    auto values = content.object();

    foreach (auto key, values.keys()) {
        auto keyValue = values.value(key);
        if (keyValue.isString()) m_values.insert(key, keyValue.toString());
    }
}

void ExtensionsViewModel::saveValues()
{
    QJsonObject object;
    auto keys = m_values.keys();
    foreach (auto key, keys) {
        object.insert(key, m_values.value(key));
    }

    saveJsonObjectToFile(m_valuesPath, object);
}

void ExtensionsViewModel::readExtensions()
{
    createIfNotExistsFile(m_extensionsPath, "[]");

    auto json = getJsonContentFromFile(m_valuesPath);
    auto content = QJsonDocument::fromJson(json.toUtf8());
    auto values = content.array();

    foreach(auto extension, values) {
        m_extensions.append(extension.toString());
    }
}

void ExtensionsViewModel::saveExtensions()
{
    QJsonArray array;
    foreach (auto extension, m_extensions) {
        array.append(extension);
    }

    saveJsonArrayToFile(m_extensionsPath, array);
}

void ExtensionsViewModel::remapDisplayExtensions()
{
    m_displayedExtensions.clear();

    auto keys = m_importedModulesMetadata.keys();
    foreach (auto key, keys) {
        auto item = m_importedModulesMetadata[key];
        QVariantMap map;
        map["indentifier"] = key;
        map["title"] = std::get<0>(item);
        map["author"] = std::get<1>(item);

        m_displayedExtensions.append(map);
    }

    emit displayedExtensionsChanged();
}

void ExtensionsViewModel::requestFinished(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "[extension]: Error while try to make HTTP request: " << reply->errorString();
        return;
    }

    auto identifier = reply->property("pendingIndentifier").toString();
    auto callback = m_pendingCallbacks.value(identifier);

    auto body = reply->readAll();
    auto bodyString = QString(body);

    QJSValueList args;

    QJSValue resultValue(bodyString);
    args.append(resultValue);
    callback.call(args);
}
