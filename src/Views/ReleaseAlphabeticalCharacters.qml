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

import QtQuick 2.12
import QtQuick.Layouts 1.3
import Anilibria.ListModels 1.0
import "../Controls"

Item {
    id: root
    anchors.fill: parent

    signal clearFilters()

    onClearFilters: {
        mainGrid.model.clearCharacters();
        releasesViewModel.items.alphabetsFilter = "";
    }

    MouseArea {
        anchors.fill: parent
        onPressed: {
            page.showAlpabeticalCharaters = false;
        }
    }

    Rectangle {
        color: "black"
        anchors.fill: parent
        opacity: .5
    }

    GridView {
        id: mainGrid
        width: 800
        height: 300
        cellWidth: 80
        cellHeight: 80
        anchors.centerIn: parent
        model: AlphabetListModel {
        }
        delegate: itemDelegate
    }

    Component {
        id: itemDelegate

        Rectangle {
            width: 78
            height: 78

            ToggleButton {
                anchors.fill: parent
                text: alphabetCharacter
                isChecked: characterSelected
                onButtonClicked: {
                    mainGrid.model.selectCharacter(alphabetCharacter);

                    releasesViewModel.items.alphabetsFilter = mainGrid.model.getSelectedCharactersAsString();

                    releasesViewModel.items.refresh();
                }
                onButtonAlreadyClicked: {
                    mainGrid.model.selectCharacter(alphabetCharacter);

                    releasesViewModel.items.alphabetsFilter = mainGrid.model.getSelectedCharactersAsString();
                    releasesViewModel.items.refresh();
                }
            }
        }
    }
}
