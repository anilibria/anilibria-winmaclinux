import QtQuick 2.15
import QtQuick.Layouts 1.15
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
