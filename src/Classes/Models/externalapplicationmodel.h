#ifndef EXTERNALAPPLICATIONMODEL_H
#define EXTERNALAPPLICATIONMODEL_H

#include <QString>

class ExternalApplicationModel
{
private:
    bool m_isInstalled { false };
    bool m_isIncludedInsideDistributive { false };
    bool m_isHaveNewVersion { false };
    QString m_installedVersion { "" };
    QString m_name { "" };
    QString m_installedPath { "" };
    QString m_description { "" };
    QString m_executableName { "" };
    QString m_repositoryPath { "" };
    QString m_newVersion { "" };

public:
    ExternalApplicationModel();

    bool isInstalled() const noexcept { return m_isInstalled; }
    void setIsInstalled(bool isInstalled) { m_isInstalled = isInstalled; }

    bool isIncludedInsideDistributive() const noexcept { return m_isIncludedInsideDistributive; }
    void setIsIncludedInsideDistributive(bool isIncludedInsideDistributive) { m_isIncludedInsideDistributive = isIncludedInsideDistributive; }

    bool isHaveNewVersion() const noexcept { return m_isHaveNewVersion; }
    void setIsHaveNewVersion(bool isHaveNewVersion) { m_isHaveNewVersion = isHaveNewVersion; }

    QString name() const noexcept { return m_name; }
    void setName(const QString& name) { m_name = name; }

    QString installedVersion() const noexcept { return m_installedVersion; }
    void setInstalledVersion(const QString& installedVersion) { m_installedVersion = installedVersion; }

    QString installedPath() const noexcept { return m_installedPath; }
    void setInstalledPath(const QString& installedPath) { m_installedPath = installedPath; }

    QString description() const noexcept { return m_description; }
    void setDescription(const QString& description) { m_description = description; }

    QString executableName() const noexcept { return m_executableName; }
    void setExecutableName(const QString& executableName) { m_executableName = executableName; }

    QString repositoryPath() const noexcept { return m_repositoryPath; }
    void setRepositoryPath(const QString& repositoryPath) { m_repositoryPath = repositoryPath; }

    QString newVersion() const noexcept { return m_newVersion; }
    void setNewVersion(const QString& newVersion) { m_newVersion = newVersion; }

};

#endif // EXTERNALAPPLICATIONMODEL_H
