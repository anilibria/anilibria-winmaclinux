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

    m_innerObject->setup(m_values);

    readExtensions();
    readValues();

    adjustEngine();
    importExtensions();

    connect(m_innerObject, &ExtensionInnerObject::makeHttpGetHandler, this, &ExtensionsViewModel::makeHttpGet);
    connect(m_innerObject, &ExtensionInnerObject::makeHttpPostHandler, this, &ExtensionsViewModel::makeHttpPost);
    connect(m_innerObject, &ExtensionInnerObject::makeHttpPutHandler, this, &ExtensionsViewModel::makeHttpPut);
    connect(m_innerObject, &ExtensionInnerObject::makeHttpDeleteHandler, this, &ExtensionsViewModel::makeHttpDelete);
    connect(m_innerObject, &ExtensionInnerObject::sendInformationNotificationHandler, this, &ExtensionsViewModel::sendInformationNotification);
}

void ExtensionsViewModel::setReleases(const ReleasesViewModel* releases) noexcept
{
    if (m_releases == releases) return;

    m_releases = const_cast<ReleasesViewModel *>(releases);
    m_innerObject->setupReleases(m_releases);
    emit releasesChanged();
}

void ExtensionsViewModel::releaseOpenedInVideoPlayer(int releaseId, const QString &title, int seria)
{
    if (m_importedModules.isEmpty()) return;

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

        auto result = handleFunction.call(args);
        if (result.isError()) {
            qDebug() << "[extension]: Error caught at line " << result.property("lineNumber").toInt() << ": " << result.toString();
            return;
        }
    }
}

QString ExtensionsViewModel::releaseOpenedInReleaseCard(int releaseId)
{
    if (m_importedModules.isEmpty()) return "";

    QString releaseCardContent = "";

    foreach (auto module, m_importedModules) {
        if (!module.hasProperty("releaseOpenedInReleaseCard")) continue;

        QJSValue handleFunction = module.property("releaseOpenedInReleaseCard");
        if (handleFunction.isUndefined() || handleFunction.isNull()) continue;

        QJSValueList args;

        QJSValue releaseIdValue(releaseId);
        args.append(releaseIdValue);

        auto result = handleFunction.call(args);
        if (result.isError()) {
            qDebug() << "[extension]: Error caught at line " << result.property("lineNumber").toInt() << ": " << result.toString();
            return "";
        }

        if (!result.isString()) return "";

        releaseCardContent += result.toString();
    }

    return releaseCardContent;
}

void ExtensionsViewModel::saveValue(const QString &key, const QString &value)
{
    m_values->insert(key, value);
}

QString ExtensionsViewModel::readValue(const QString &key)
{
    if (!m_values->contains(key)) return "";

    return m_values->value(key);
}

void ExtensionsViewModel::deleteValue(const QString &key)
{
    if (!m_values->contains(key)) return;

    m_values->remove(key);
}

void ExtensionsViewModel::addExtension(const QString &path)
{
    QString innerPath = path;
    m_extensions.append(removeFileProtocol(innerPath));

    importExtension(innerPath);

    remapDisplayExtensions();

    saveExtensions();
}

void ExtensionsViewModel::runMenuCommand(const QString &identifier, int index)
{
    if (!m_importedModulesMenuIndexes.contains(identifier)) return;

    auto indexes = m_importedModulesMenuIndexes.value(identifier);
    auto startIndex = std::get<0>(indexes);
    auto itemIndex = startIndex + index;
    if (itemIndex >= m_importedModulesMenuCallbacks.size()) return;

    auto callback = m_importedModulesMenuCallbacks.value(itemIndex);
    callback.call();
}

void ExtensionsViewModel::saveValues()
{
    QJsonObject object;
    auto keys = m_values->keys();
    foreach (auto key, keys) {
        object.insert(key, m_values->value(key));
    }

    saveJsonObjectToFile(m_valuesFileName, object);
}

void ExtensionsViewModel::deleteExtension(const QString &path)
{
    if (!m_extensions.contains(path)) return;

    m_extensions.removeOne(path);
    saveExtensions();

    remapDisplayExtensions();
}

void ExtensionsViewModel::openCard()
{
    refreshGlobalVariables();

    m_isOpenedCard = true;
    emit isOpenedCardChanged();
}

void ExtensionsViewModel::closeCard()
{
    m_isOpenedCard = false;
    emit isOpenedCardChanged();
}

void ExtensionsViewModel::refreshVariables()
{
    refreshGlobalVariables();
}

bool ExtensionsViewModel::hasValue(const QString &key) noexcept
{
    return m_values->contains(key);
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

    if (module.hasProperty("menuItems")) {
        QJSValue menuItemsFunction = module.property("menuItems");
        auto items = menuItemsFunction.call();
        if (items.isArray()) {
            int countItems = 0;
            int startIndex = m_importedModulesMenuCallbacks.size();

            auto length = items.property("length").toInt();
            for (int i = 0; i < length; i++) {
                auto item = items.property(i);
                if (!item.hasProperty("title") || !item.hasProperty("handler")) continue;

                auto title = item.property("title").toString();
                auto handler = item.property("handler");
                countItems++;
                m_importedModulesMenuCallbacks.append(handler);
                m_importedModulesMenuTitles.append(title);
            }

            if (countItems > 0) {
                auto indexes = std::make_tuple(startIndex, countItems);
                m_importedModulesMenuIndexes.insert(path, indexes);
            }
        }
    }
}

void ExtensionsViewModel::adjustEngine()
{
    m_engine->installExtensions(QJSEngine::ConsoleExtension);

    m_engine->globalObject().setProperty("appVersion", ApplicationVersion);
    QJSValue extensionsObject = m_engine->newQObject(m_innerObject);
    m_engine->globalObject().setProperty("appExtension", extensionsObject);
}

void ExtensionsViewModel::readValues()
{
    createIfNotExistsCacheFile(m_valuesFileName, "{}");

    auto json = getJsonContentFromFile(m_valuesFileName);
    auto content = QJsonDocument::fromJson(json.toUtf8());
    auto values = content.object();

    foreach (auto key, values.keys()) {
        auto keyValue = values.value(key);
        if (keyValue.isString()) m_values->insert(key, keyValue.toString());
    }

    refreshGlobalVariables();
}

void ExtensionsViewModel::readExtensions()
{
    createIfNotExistsCacheFile(m_extensionsFileName, "[]");

    auto json = getJsonContentFromFile(m_extensionsFileName);
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

    saveJsonArrayToFile(m_extensionsFileName, array);
}

void ExtensionsViewModel::remapDisplayExtensions()
{
    m_displayedExtensions.clear();

    auto keys = m_extensions;
    foreach (auto key, keys) {
        auto item = m_importedModulesMetadata[key];
        QVariantMap map;
        map["indentifier"] = key;
        map["extensionTitle"] = std::get<0>(item);
        map["author"] = std::get<1>(item);

        if (m_importedModulesMenuIndexes.contains(key)) {
            auto indexes = m_importedModulesMenuIndexes.value(key);
            auto startIndex = std::get<0>(indexes);
            auto countItems = std::get<1>(indexes);
            map["menuItems"] = QVariant(m_importedModulesMenuTitles.mid(startIndex, countItems));
        }

        m_displayedExtensions.append(map);
    }

    emit displayedExtensionsChanged();
}

void ExtensionsViewModel::refreshGlobalVariables()
{
    m_globalVariables.clear();

    auto keys = m_values->keys();
    foreach (auto key, keys) {
        auto value = m_values->value(key);

        QVariantMap map;
        map["indentifier"] = key;
        map["value"] = value;

        m_globalVariables.append(map);
    }

    emit globalVariablesChanged();
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

void ExtensionsViewModel::makeHttpGet(const QString &url, const QList<QString> headers, const QJSValue &callback)
{
    if (!callback.isCallable()) {
        qDebug() << "[extension]:makeHttpGet: Callback for execute URL " << url << " not a function!";
        return;
    }

    auto uuid = QUuid::createUuid();
    auto identifier = uuid.toString();
    m_pendingCallbacks.insert(identifier, callback);

    QNetworkRequest request(url);

    foreach (auto header, headers) {
        auto parts = header.split(" ");
        if (parts.size() == 2) {
            auto name = parts.value(0);
            auto value = parts.value(1);
            request.setRawHeader(name.toUtf8(), value.toUtf8());
        } else {
            request.setRawHeader(header.toUtf8(), "");
        }
    }

    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    auto reply = m_networkManager->get(request);
    reply->setProperty("pendingIndentifier", identifier);

}

void ExtensionsViewModel::makeHttpDelete(const QString &url, const QList<QString> headers, const QJSValue &callback)
{
    if (!callback.isCallable()) {
        qDebug() << "[extension]:makeHttpDelete: Callback for execute URL " << url << " not a function!";
        return;
    }

    auto uuid = QUuid::createUuid();
    auto identifier = uuid.toString();
    m_pendingCallbacks.insert(identifier, callback);

    QNetworkRequest request(url);

    foreach (auto header, headers) {
        auto parts = header.split(" ");
        if (parts.size() == 2) {
            auto name = parts.value(0);
            auto value = parts.value(1);
            request.setRawHeader(name.toUtf8(), value.toUtf8());
        } else {
            request.setRawHeader(header.toUtf8(), "");
        }
    }

    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    auto reply = m_networkManager->deleteResource(request);
    reply->setProperty("pendingIndentifier", identifier);
}

void ExtensionsViewModel::makeHttpPost(const QString &url, const QList<QString> headers, const QString &body, const QJSValue &callback)
{
    if (!callback.isCallable()) {
        qDebug() << "[extension]:makeHttpGet: Callback for execute URL " << url << " not a function!";
        return;
    }

    auto uuid = QUuid::createUuid();
    auto identifier = uuid.toString();
    m_pendingCallbacks.insert(identifier, callback);

    QNetworkRequest request(url);

    foreach (auto header, headers) {
        auto parts = header.split(" ");
        if (parts.size() == 2) {
            auto name = parts.value(0);
            auto value = parts.value(1);
            request.setRawHeader(name.toUtf8(), value.toUtf8());
        } else {
            request.setRawHeader(header.toUtf8(), "");
        }
    }

    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    auto reply = m_networkManager->post(request, body.toUtf8());
    reply->setProperty("pendingIndentifier", identifier);
}

void ExtensionsViewModel::makeHttpPut(const QString &url, const QList<QString> headers, const QString &body, const QJSValue &callback)
{
    if (!callback.isCallable()) {
        qDebug() << "[extension]:makeHttpPut: Callback for execute URL " << url << " not a function!";
        return;
    }

    auto uuid = QUuid::createUuid();
    auto identifier = uuid.toString();
    m_pendingCallbacks.insert(identifier, callback);

    QNetworkRequest request(url);

    foreach (auto header, headers) {
        auto parts = header.split(" ");
        if (parts.size() == 2) {
            auto name = parts.value(0);
            auto value = parts.value(1);
            request.setRawHeader(name.toUtf8(), value.toUtf8());
        } else {
            request.setRawHeader(header.toUtf8(), "");
        }
    }

    request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    auto reply = m_networkManager->put(request, body.toUtf8());
    reply->setProperty("pendingIndentifier", identifier);
}

void ExtensionsViewModel::sendInformationNotification(const QString &message)
{
    emit sendInformationNotificationReceived(message);
}
