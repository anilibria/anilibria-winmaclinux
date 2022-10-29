import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.0
import "../Controls"

ApplicationWindow {
    id: previewWindow
    minimumWidth: 500
    minimumHeight: 300
    width: 500
    height: 300
    flags: Qt.Dialog | Qt.WindowTitleHint | Qt.WindowCloseButtonHint
    modality: Qt.ApplicationModal
    title: "Предпросмотр темы"
    Material.accent: Material.Red
    Material.theme: applicationThemeViewModel.fieldList.basedOnTheme === 'Темная' ? Material.Dark : Material.Light


}
