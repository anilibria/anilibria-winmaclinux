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

#include "alphabetlistmodel.h"

AlphabetListModel::AlphabetListModel(QObject *parent)
    : QAbstractListModel(parent),
      m_alphabet("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯAIR1279"),
      m_selectedCharacters(new QSet<QString>())
{
}

int AlphabetListModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid()) return 0;

    return m_alphabet.size();
}

QVariant AlphabetListModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();

    switch (role) {
        case AlphabetCharacterRole: {
            return QVariant(m_alphabet.at(index.row()));
        }
        case CharacterSelectedRole: {
            return QVariant(m_selectedCharacters->contains(m_alphabet.at(index.row())));
        }
    }

    return QVariant();
}

QHash<int, QByteArray> AlphabetListModel::roleNames() const
{
    return {
        {
            AlphabetCharacterRole,
            "alphabetCharacter"
        },
        {
            CharacterSelectedRole,
            "characterSelected"
        }
    };
}

void AlphabetListModel::selectCharacter(const QString &character)
{
    if (m_selectedCharacters->contains(character)) {
        m_selectedCharacters->remove(character);
    } else {
        m_selectedCharacters->insert(character);
    }

    auto index = m_alphabet.indexOf(character);
    auto topLeft = createIndex(index,0);
    emit dataChanged(topLeft, topLeft);}

void AlphabetListModel::clearCharacters()
{
    QSet<int> indexes;
    foreach (auto selectedCharacter, *m_selectedCharacters) {
        auto index = m_alphabet.indexOf(selectedCharacter);
        indexes.insert(index);
    }

    m_selectedCharacters->clear();

    foreach (auto index, indexes) {
        auto topLeft = createIndex(index,0);
        emit dataChanged(topLeft, topLeft);
    }
}

QStringList AlphabetListModel::getSelectedCharacters()
{
    return QStringList(m_selectedCharacters->values());
}
