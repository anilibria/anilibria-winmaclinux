/*
    AniLibria - desktop client for the website anilibria.tv
    Copyright (C) 2020 Roman Vladimirov

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ALPHABETLISTMODEL_H
#define ALPHABETLISTMODEL_H

#include <QAbstractListModel>
#include <QSet>

class AlphabetListModel : public QAbstractListModel
{
    Q_OBJECT

private:
    QString m_alphabet;
    QSet<QString>* m_selectedCharacters;

    enum ItemRoles {
        AlphabetCharacterRole = Qt::UserRole + 1,
        CharacterSelectedRole
    };

public:
    explicit AlphabetListModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    QHash<int,QByteArray> roleNames() const override;

    Q_INVOKABLE void selectCharacter(const QString& character);
    Q_INVOKABLE void clearCharacters();
    Q_INVOKABLE QStringList getSelectedCharacters();
    Q_INVOKABLE QString getSelectedCharactersAsString();

private:
};

#endif // ALPHABETLISTMODEL_H
