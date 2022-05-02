#ifndef MYANILIBRIALISTMODEL_H
#define MYANILIBRIALISTMODEL_H

#include <QObject>
#include <QAbstractListModel>
#include <QMap>
#include <QList>
#include <QSet>
#include "../ViewModels/userconfigurationviewmodel.h"

class MyAnilibriaListModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(UserConfigurationViewModel* userConfiguration READ userConfiguration WRITE setUserConfiguration NOTIFY userConfigurationChanged)
private:
    QList<QString> m_sections { QList<QString>() };
    QSet<QString> m_fullSections { QSet<QString>() };
    QMap<QString, QString> m_sectionTitles { QMap<QString, QString>() };
    UserConfigurationViewModel* m_userConfiguration { nullptr };
    enum MyAnilibriaSectionRole {
        SectionIdRole = Qt::UserRole + 1,
        SectionTitleRole,
        ComponentIdRole,
        HeaderVisibleRole
    };

public:
    explicit MyAnilibriaListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const override;

    UserConfigurationViewModel* userConfiguration() const noexcept { return m_userConfiguration; }
    void setUserConfiguration(const UserConfigurationViewModel* userConfiguration) noexcept;

    void fillSections(const QSet<QString>& sections) noexcept;
    QStringList getSections() const noexcept;

    void setSectionHideMark(const QString& section, const bool notVisible) noexcept;

    Q_INVOKABLE void toggleSectionHideMark(const QString& section);
    Q_INVOKABLE void setNotVisibleAllMarks(const bool visible);

private:
    bool getHideSection(const QString& section) const noexcept;

signals:
    void userConfigurationChanged();
    void needSaveSections();

};

#endif // MYANILIBRIALISTMODEL_H
