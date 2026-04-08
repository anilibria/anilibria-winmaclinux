import QtQuick 2.15
import "../../Controls"

Item {
    width: scrollview.width
    height: simpleList.hasItems ? (needHideSection ? 38 : 368) : 0

    ExpandableHeader {
        id: expandableHeader
        anchors.top: parent.top
        anchors.topMargin: 8
        width: parent.width
        height: simpleList.hasItems ? 30 : 0
        visible: simpleList.hasItems
        isFirst: firstSection
        isLast: lastSection
        headerTitle: title + " (" + simpleList.countItems + ")"
        headerHided: needHideSection
        onHeaderPressed: {
            myAnilibriaViewModel.myList.toggleSectionHideMark(currentIndex)
        }
        onMoveSection: {
            myAnilibriaViewModel.restoreScroll = scrollview.contentY;
            myAnilibriaViewModel.moveSection(direction, currentIndex);
        }
    }

    ReleasesList {
        id: simpleList
        width: parent.width
        visible: simpleList.hasItems || !needHideSection
        height: !simpleList.hasItems || needHideSection ? 0 : 330
        anchors.top: expandableHeader.bottom
        filterMode: sectionId
    }
}
