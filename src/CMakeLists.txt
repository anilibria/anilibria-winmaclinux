cmake_minimum_required(VERSION 3.16)

project(AniLiberty VERSION 2.2.29 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 20)

find_package(Qt6 REQUIRED COMPONENTS Quick Network Concurrent WebSockets)
find_package(Qt6 COMPONENTS Svg REQUIRED)
find_package(Qt6 COMPONENTS QuickControls2 REQUIRED)

qt_standard_project_setup(REQUIRES 6.5)

set(QT_QML_GENERATE_QMLLS_INI ON)

set(appsources
    globalhelpers.h globalhelpers.cpp
    globalconstants.h Classes/globalconstant.cpp

    Classes/customstyle.h Classes/customstyle.cpp
    Classes/Models/apitorrentmodel.h Classes/Models/apitorrentmodel.cpp
    Classes/Models/changesmodel.h Classes/Models/changesmodel.cpp
    Classes/Models/downloadedtorrentmodel.h Classes/Models/downloadedtorrentmodel.cpp
    Classes/Models/downloaditemmodel.h Classes/Models/downloaditemmodel.cpp
    Classes/Models/externalapplicationmodel.h Classes/Models/externalapplicationmodel.cpp
    Classes/Models/externalplaylistvideo.h Classes/Models/externalplaylistvideo.cpp
    Classes/Models/fullreleasemodel.h Classes/Models/fullreleasemodel.cpp
    Classes/Models/historymodel.h Classes/Models/historymodel.cpp
    Classes/Models/mainmenuitemmodel.h Classes/Models/mainmenuitemmodel.cpp
    Classes/Models/notificationmodel.h Classes/Models/notificationmodel.cpp
    Classes/Models/onlinevideomodel.h Classes/Models/onlinevideomodel.cpp
    Classes/Models/releaseonlinevideomodel.h Classes/Models/releaseonlinevideomodel.cpp
    Classes/Models/releaseseriesmodel.h Classes/Models/releaseseriesmodel.cpp
    Classes/Models/seenmarkmodel.h Classes/Models/seenmarkmodel.cpp
    Classes/Models/seenmodel.h Classes/Models/seenmodel.cpp
    Classes/Models/themeitemmodel.h Classes/Models/themeitemmodel.cpp
    Classes/Models/usersettingsmodel.h Classes/Models/usersettingsmodel.cpp
    Classes/Models/youtubevideomodel.h Classes/Models/youtubevideomodel.cpp

    Classes/ListModels/allmyanilibrialistmodel.h Classes/ListModels/allmyanilibrialistmodel.cpp
    Classes/ListModels/cinemahalllistmodel.h Classes/ListModels/cinemahalllistmodel.cpp
    Classes/ListModels/commoncomboboxlistmodel.h Classes/ListModels/commoncomboboxlistmodel.cpp
    Classes/ListModels/commonmenulistmodel.h Classes/ListModels/commonmenulistmodel.cpp
    Classes/ListModels/dowloadedtorrentslistmodel.h Classes/ListModels/dowloadedtorrentslistmodel.cpp
    Classes/ListModels/externalthemeslistmodel.h Classes/ListModels/externalthemeslistmodel.cpp
    Classes/ListModels/localthemeslistmodel.h Classes/ListModels/localthemeslistmodel.cpp
    Classes/ListModels/mainmenulistmodel.h Classes/ListModels/mainmenulistmodel.cpp
    Classes/ListModels/myanilibrialistmodel.h Classes/ListModels/myanilibrialistmodel.cpp
    Classes/ListModels/myanilibriasearchlistmodel.h Classes/ListModels/myanilibriasearchlistmodel.cpp
    Classes/ListModels/popupnotificationvideolist.h Classes/ListModels/popupnotificationvideolist.cpp
    Classes/ListModels/onlineplayervideolist.h Classes/ListModels/onlineplayervideolist.cpp
    Classes/ListModels/releasecardmenulistmodel.h Classes/ListModels/releasecardmenulistmodel.cpp
    Classes/ListModels/releaseonlineserieslistmodel.h Classes/ListModels/releaseonlineserieslistmodel.cpp
    Classes/ListModels/releaseseriescardlistmodel.h Classes/ListModels/releaseseriescardlistmodel.cpp
    Classes/ListModels/releaseserieslistmodel.h Classes/ListModels/releaseserieslistmodel.cpp
    Classes/ListModels/releasesimplelistmodel.h Classes/ListModels/releasesimplelistmodel.cpp
    Classes/ListModels/releaseslistmodel.h Classes/ListModels/releaseslistmodel.cpp
    Classes/ListModels/releasetorrentcommonlist.h Classes/ListModels/releasetorrentcommonlist.cpp
    Classes/ListModels/releasetorrentslist.h Classes/ListModels/releasetorrentslist.cpp
    Classes/ListModels/themefieldlistmodel.h Classes/ListModels/themefieldlistmodel.cpp
    Classes/ListModels/alphabetlistmodel.h Classes/ListModels/alphabetlistmodel.cpp

    Classes/RemotePlayer/externalplayerbase.h Classes/RemotePlayer/externalplayerbase.cpp
    Classes/RemotePlayer/remoteplayer.h Classes/RemotePlayer/remoteplayer.cpp
    Classes/RemotePlayer/remoteplayertransport.h Classes/RemotePlayer/remoteplayertransport.cpp
    Classes/RemotePlayer/websocketexternalplayer.h Classes/RemotePlayer/websocketexternalplayer.cpp

    Classes/Services/analyticsservice.h Classes/Services/analyticsservice.cpp
    Classes/Services/anilibriaapiservice.h Classes/Services/anilibriaapiservice.cpp
    Classes/Services/apiserviceconfigurator.h Classes/Services/apiserviceconfigurator.cpp
    Classes/Services/applicationsettings.h Classes/Services/applicationsettings.cpp
    Classes/Services/applicationversionchecker.h Classes/Services/applicationversionchecker.cpp
    Classes/Services/downloadmanager.h Classes/Services/downloadmanager.cpp
    Classes/Services/imageloader.h Classes/Services/imageloader.cpp
    Classes/Services/localstorageservice.h Classes/Services/localstorageservice.cpp
    Classes/Services/offlineimagecacheservice.h Classes/Services/offlineimagecacheservice.cpp
    Classes/Services/osextras.h Classes/Services/osextras.cpp
    Classes/Services/proxyconfigurator.h Classes/Services/proxyconfigurator.cpp
    Classes/Services/releaselinkedseries.h Classes/Services/releaselinkedseries.cpp
    Classes/Services/synchronizationservice.h Classes/Services/synchronizationservice.cpp
    Classes/Services/synchronizev2service.h Classes/Services/synchronizev2service.cpp
    Classes/Services/thememanagerservice.h Classes/Services/thememanagerservice.cpp
    Classes/Services/versionchecker.h Classes/Services/versionchecker.cpp

    Classes/ViewModels/torrentnotifierviewmodel.h Classes/ViewModels/torrentnotifierviewmodel.cpp
    Classes/ViewModels/applicationsviewmodel.h Classes/ViewModels/applicationsviewmodel.cpp
    Classes/ViewModels/applicationthemeviewmodel.h Classes/ViewModels/applicationthemeviewmodel.cpp
    Classes/ViewModels/authorizationviewmodel.h Classes/ViewModels/authorizationviewmodel.cpp
    Classes/ViewModels/extensioninnerobject.h Classes/ViewModels/extensioninnerobject.cpp
    Classes/ViewModels/extensionsviewmodel.h Classes/ViewModels/extensionsviewmodel.cpp
    Classes/ViewModels/externalplayerviewmodel.h Classes/ViewModels/externalplayerviewmodel.cpp
    Classes/ViewModels/filterdictionariesviewmodel.h Classes/ViewModels/filterdictionariesviewmodel.cpp
    Classes/ViewModels/globaleventtrackerviewmodel.h Classes/ViewModels/globaleventtrackerviewmodel.cpp
    Classes/ViewModels/imagebackgroundviewmodel.h Classes/ViewModels/imagebackgroundviewmodel.cpp
    Classes/ViewModels/mainviewmodel.h Classes/ViewModels/mainviewmodel.cpp
    Classes/ViewModels/myanilibriaviewmodel.h Classes/ViewModels/myanilibriaviewmodel.cpp
    Classes/ViewModels/notificationviewmodel.h Classes/ViewModels/notificationviewmodel.cpp
    Classes/ViewModels/onlineplayerviewmodel.h Classes/ViewModels/onlineplayerviewmodel.cpp
    Classes/ViewModels/onlineplayerwindowviewmodel.h Classes/ViewModels/onlineplayerwindowviewmodel.cpp
    Classes/ViewModels/releasecustomgroupsviewmodel.h Classes/ViewModels/releasecustomgroupsviewmodel.cpp
    Classes/ViewModels/releasesviewmodel.h Classes/ViewModels/releasesviewmodel.cpp
    Classes/ViewModels/synchronizationhub.h Classes/ViewModels/synchronizationhub.cpp
    Classes/ViewModels/useractivityviewmodel.h Classes/ViewModels/useractivityviewmodel.cpp
    Classes/ViewModels/userconfigurationviewmodel.h Classes/ViewModels/userconfigurationviewmodel.cpp
    Classes/ViewModels/youtubeviewmodel.h Classes/ViewModels/youtubeviewmodel.cpp
)
if (WIN32)
    # windows specified
    list(APPEND appsources PlayerMpv/mpvobject.h PlayerMpv/mpvobject.cpp)

    add_compile_definitions(NO_NEED_STANDART_PLAYER)
    add_compile_definitions(USE_MPV_PLAYER)
endif ()

if (APPLE)
    # mac specified
    list(APPEND appsources PlayerMpv/mpvobject.h PlayerMpv/mpvobject.cpp)

    add_compile_definitions(NO_NEED_STANDART_PLAYER)
    add_compile_definitions(USE_MPV_PLAYER)
endif ()

if (NOT (APPLE OR WIN32))
    # linux specified
    list(APPEND appsources PlayerMpv/mpvobject.h PlayerMpv/mpvobject.cpp)

    add_compile_definitions(NO_NEED_STANDART_PLAYER)
    add_compile_definitions(USE_MPV_PLAYER)
endif ()

list(APPEND appsources main.cpp)

set(app_icon_resource_windows "${CMAKE_CURRENT_SOURCE_DIR}/fullsizesicon.rc")

set(MACOSX_BUNDLE_ICON_FILE anilibria.icns)
set(macos_ICON ${CMAKE_CURRENT_SOURCE_DIR}/anilibria.icns)
set_source_files_properties(${macos_ICON} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")

qt_add_executable(appAniLiberty
    ${appsources}
    ${app_icon_resource_windows}
    ${macos_ICON}
)

set_target_properties(${PROJECT-NAME} PROPERTIES OUTPUT_NAME "aniliberty")

qt_policy(SET QTP0004 NEW)

qt_add_qml_module(appAniLiberty
    URI aniliberty
    VERSION 1.0
    QML_FILES
        Main.qml

        Views/About.qml
        Views/Applications.qml
        Views/Authorization.qml
        Views/Cinemahall.qml
        Views/Extensions.qml
        Views/ExtensionsCard.qml
        Views/ExternalPlayerWindow.qml
        Views/FilterPanelIconButton.qml
        Views/LeftPanelIconButton.qml
        Views/Maintenance.qml
        Views/MyAnilibria.qml
        Views/OnlinePlayer.qml
        Views/ReleaseAlphabeticalCharacters.qml
        Views/ReleaseCard.qml
        Views/ReleaseCustomGroups.qml
        Views/ReleaseItem.qml
        Views/ReleasePosterPreview.qml
        Views/Releases.qml
        Views/ReleaseSeries.qml
        Views/ReleaseSeriesCard.qml
        Views/ThemeManager.qml
        Views/ThemeManagerPreview.qml
        Views/TorrentStream.qml
        Views/TorrentStreamCard.qml
        Views/Youtube.qml

        Views/Videoplayer/EmbeddedVideoPlayer.qml
        Views/Videoplayer/ExternalPlayer.qml
        Views/Videoplayer/MpvPlayer.qml

        Views/MyAnilibria/SimpleListSection.qml
        Views/MyAnilibria/StatisticsSection.qml

        Controls/AccentText.qml
        Controls/AppPanelButton.qml
        Controls/BackgroundImagePopup.qml
        Controls/CommonComboBox.qml
        Controls/CommonMenu.qml
        Controls/CommonMenuItem.qml
        Controls/CommonMenuModel.qml
        Controls/CommonSwitch.qml
        Controls/CommonTextField.qml
        Controls/CorneredImage.qml
        Controls/DefaultPopup.qml
        Controls/DictionaryComboBox.qml
        Controls/EmptyBoxArea.qml
        Controls/ExpandableHeader.qml
        Controls/IconButton.qml
        Controls/InfoAboutPlayer.qml
        Controls/InfoForNewcomers.qml
        Controls/LinkedText.qml
        Controls/MessageModal.qml
        Controls/NavigationScrollButton.qml
        Controls/PlainText.qml
        Controls/ReleaseOnlineVideosList.qml
        Controls/ReleaseSeriesList.qml
        Controls/ReleasesList.qml
        Controls/RoundedActionButton.qml
        Controls/RoundedTextBox.qml
        Controls/ToggleButton.qml
        Controls/TooltipedImage.qml
        Controls/forqt6/SystemColorDialog.qml
        Controls/forqt6/SystemOpenFileDialog.qml
        Controls/forqt6/SystemOpenFolderDialog.qml
        Controls/forqt6/SystemSaveFileDialog.qml

        CustomStyleQt6/BoxShadow.qml
        CustomStyleQt6/ComboBox.qml
        CustomStyleQt6/CursorDelegate.qml
        CustomStyleQt6/Drawer.qml
        CustomStyleQt6/ElevationEffect.qml
        CustomStyleQt6/Label.qml
        CustomStyleQt6/Menu.qml
        CustomStyleQt6/MenuItem.qml
        CustomStyleQt6/Popup.qml
        CustomStyleQt6/RectangularGlow.qml
        CustomStyleQt6/Switch.qml
        CustomStyleQt6/SwitchIndicator.qml
        CustomStyleQt6/TextField.qml
        CustomStyleQt6/ToolTip.qml

    RESOURCES
        Assets/Backgrounds/authbackground.jpg
        Assets/Backgrounds/bronze.png
        Assets/Backgrounds/gold.png
        Assets/Backgrounds/silver.png

        Assets/Icons/allreleases.svg
        Assets/Icons/alphabet.svg
        Assets/Icons/anilibrialogodark.svg
        Assets/Icons/anilibrialogodefault.svg
        Assets/Icons/applications.svg
        Assets/Icons/arrowup.svg
        Assets/Icons/back.svg
        Assets/Icons/broken.svg
        Assets/Icons/calendar.svg
        Assets/Icons/calendarcolor.svg
        Assets/Icons/catalogmenu.svg
        Assets/Icons/christmasball.svg
        Assets/Icons/cinemahallmenu.svg
        Assets/Icons/cinemaplay.svg
        Assets/Icons/close.svg
        Assets/Icons/closeeye.svg
        Assets/Icons/closewindow.svg
        Assets/Icons/closewindowwhite.svg
        Assets/Icons/coloredbell.svg
        Assets/Icons/coloredclosewindow.svg
        Assets/Icons/coloreddice.svg
        Assets/Icons/coloreddrawer.svg
        Assets/Icons/colorededit.svg
        Assets/Icons/coloredeye.svg
        Assets/Icons/coloredleftmenu.svg
        Assets/Icons/coloredminimize.svg
        Assets/Icons/coloredrefresh.svg
        Assets/Icons/coloredrightmenu.svg
        Assets/Icons/coloredsettings.svg
        Assets/Icons/coloredsortdown.svg
        Assets/Icons/coloredsortup.svg
        Assets/Icons/coloredstar.svg
        Assets/Icons/connect.svg
        Assets/Icons/contextmenu.svg
        Assets/Icons/copy.svg
        Assets/Icons/darkarrowup.svg
        Assets/Icons/darksortascending.svg
        Assets/Icons/darksortdescending.svg
        Assets/Icons/delete.svg
        Assets/Icons/donate.jpg
        Assets/Icons/donate.svg
        Assets/Icons/download.svg
        Assets/Icons/downloading.svg
        Assets/Icons/downloadtheme.svg
        Assets/Icons/emptybox.svg
        Assets/Icons/emptyfavorite.svg
        Assets/Icons/extensions.svg
        Assets/Icons/external.svg
        Assets/Icons/eye.svg
        Assets/Icons/fast-forward.svg
        Assets/Icons/favorite.svg
        Assets/Icons/fullscreensize.svg
        Assets/Icons/gofromfullscreen.svg
        Assets/Icons/gotofullscreen.svg
        Assets/Icons/grayedleftmenu.svg
        Assets/Icons/grayedrightmenu.svg
        Assets/Icons/hidebuttonmenu.svg
        Assets/Icons/history.svg
        Assets/Icons/house.svg
        Assets/Icons/idledownload.svg
        Assets/Icons/information.svg
        Assets/Icons/leftarrow.svg
        Assets/Icons/lefthalf.svg
        Assets/Icons/leftscroll.svg
        Assets/Icons/listcustom.svg
        Assets/Icons/logout.svg
        Assets/Icons/maintenance.svg
        Assets/Icons/menu.svg
        Assets/Icons/minimize.svg
        Assets/Icons/minus.svg
        Assets/Icons/mute.svg
        Assets/Icons/next.svg
        Assets/Icons/numberbookmark.svg
        Assets/Icons/online.svg
        Assets/Icons/pause.svg
        Assets/Icons/play-button.svg
        Assets/Icons/plus.svg
        Assets/Icons/popcorn.svg
        Assets/Icons/previous.svg
        Assets/Icons/rating.svg
        Assets/Icons/ratingcolor.svg
        Assets/Icons/refresh.svg
        Assets/Icons/releasepage.svg
        Assets/Icons/rewind.svg
        Assets/Icons/rewind10.svg
        Assets/Icons/rightarrow.svg
        Assets/Icons/righthalf.svg
        Assets/Icons/rightscroll.svg
        Assets/Icons/search.svg
        Assets/Icons/seenmarkpanel.svg
        Assets/Icons/series.svg
        Assets/Icons/seriesmenu.svg
        Assets/Icons/skip.svg
        Assets/Icons/snowflake.svg
        Assets/Icons/snowbackground.png
        Assets/Icons/sort.svg
        Assets/Icons/sortascending.svg
        Assets/Icons/sortdescending.svg
        Assets/Icons/speaker.svg
        Assets/Icons/spinner.gif
        Assets/Icons/star.svg
        Assets/Icons/startdownload.svg
        Assets/Icons/step-backward.svg
        Assets/Icons/step-forward.svg
        Assets/Icons/stopdownload.svg
        Assets/Icons/themes.svg
        Assets/Icons/toolbaroptions.svg
        Assets/Icons/trash.svg
        Assets/Icons/updated.svg
        Assets/Icons/user.svg
        Assets/Icons/utorrent.svg
        Assets/Icons/videoplayer.svg
        Assets/Icons/videoplayermenu.svg
        Assets/Icons/videos.svg
        Assets/Icons/vk.svg
        Assets/Icons/windowsize.svg
        Assets/Icons/youtube.svg

        Assets/Icons/authorization/discord.svg
        Assets/Icons/authorization/vk.svg
        Assets/Icons/authorization/patreon.svg
        Assets/Icons/authorization/google.svg

        Assets/Icons/notification/darknotification.svg
        Assets/Icons/notification/notification.svg
        Assets/Icons/notification/notificationselected.svg

        Assets/Icons/releases/customgroup.svg
        Assets/Icons/releases/disablegrouping.svg
        Assets/Icons/releases/enablegrouping.svg
        Assets/Icons/releases/magnet.svg

        Assets/Icons/videoplayer/darkexternalplayer.svg
        Assets/Icons/videoplayer/darkfullscreen.svg
        Assets/Icons/videoplayer/darkinfo.svg
        Assets/Icons/videoplayer/darkleftjump.svg
        Assets/Icons/videoplayer/darkmenu.svg
        Assets/Icons/videoplayer/darkmuteoff.svg
        Assets/Icons/videoplayer/darkmuteon.svg
        Assets/Icons/videoplayer/darknetwork.svg
        Assets/Icons/videoplayer/darknextseria.svg
        Assets/Icons/videoplayer/darkpause.svg
        Assets/Icons/videoplayer/darkplay.svg
        Assets/Icons/videoplayer/darkpreviousseria.svg
        Assets/Icons/videoplayer/darkreload.svg
        Assets/Icons/videoplayer/darkresize.svg
        Assets/Icons/videoplayer/darkrightjump.svg
        Assets/Icons/videoplayer/darkseenmark.svg
        Assets/Icons/videoplayer/darksettings.svg
        Assets/Icons/videoplayer/darksubtitles.svg
        Assets/Icons/videoplayer/darktopmost.svg
        Assets/Icons/videoplayer/externalplayer.svg
        Assets/Icons/videoplayer/fullscreen.svg
        Assets/Icons/videoplayer/info.svg
        Assets/Icons/videoplayer/leftjump.svg
        Assets/Icons/videoplayer/menu.svg
        Assets/Icons/videoplayer/muteoff.svg
        Assets/Icons/videoplayer/muteon.svg
        Assets/Icons/videoplayer/network.svg
        Assets/Icons/videoplayer/nextseria.svg
        Assets/Icons/videoplayer/pause.svg
        Assets/Icons/videoplayer/play.svg
        Assets/Icons/videoplayer/previousseria.svg
        Assets/Icons/videoplayer/reload.svg
        Assets/Icons/videoplayer/resize.svg
        Assets/Icons/videoplayer/rightjump.svg
        Assets/Icons/videoplayer/seenmark.svg
        Assets/Icons/videoplayer/settings.svg
        Assets/Icons/videoplayer/seenmarkselected.svg
        Assets/Icons/videoplayer/subtitles.svg
        Assets/Icons/videoplayer/topmost.svg
)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
set_target_properties(appAniLiberty PROPERTIES
    # MACOSX_BUNDLE_GUI_IDENTIFIER com.example.appAniLiberty
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

target_link_libraries(appAniLiberty
    PRIVATE Qt6::Quick
	PRIVATE Qt6::Network
	PRIVATE Qt6::Concurrent
	PRIVATE Qt6::WebSockets
        PRIVATE Qt6::Svg
        PRIVATE Qt6::QuickControls2
)

if (WIN32)
    target_link_libraries(appAniLiberty PRIVATE kernel32)

    # libmpv for windows
    target_include_directories(appAniLiberty PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/windows-mpv)
    target_link_libraries(appAniLiberty PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/windows-mpv/libmpv.dll.lib")
endif ()
if (APPLE)
    set(ENV{PKG_CONFIG_PATH} "/usr/local/bin/pkg-config") # change path to PkgConfig (it can be founded in Qt directory which is not correct)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(LIBMPV REQUIRED mpv)
    target_include_directories(appAniLiberty PUBLIC ${LIBMPV_INCLUDE_DIRS})
    target_link_libraries(appAniLiberty PUBLIC ${LIBMPV_LIBRARIES})
    target_compile_options(appAniLiberty PUBLIC ${LIBMPV_CFLAGS_OTHER})

    #find_package(mpv REQUIRED)
    #target_link_libraries(YourTargetName PRIVATE mpv::mpv)

    #find_path(MPV_INCLUDE_DIR NAMES mpv/client.h PATHS /usr/local/include /opt/homebrew/include)
    #find_library(MPV_LIBRARY NAMES mpv PATHS /usr/local/lib /opt/homebrew/lib)

    #if(MPV_INCLUDE_DIR AND MPV_LIBRARY)
    #    include_directories(${MPV_INCLUDE_DIR})
    #    target_link_libraries(YourTargetName PRIVATE ${MPV_LIBRARY})
    #else()
    #    message(FATAL_ERROR "mpv library or headers not found!")
    #endif()
endif ()
if (NOT (APPLE OR WIN32))
    # libmpv for macos
    find_package(PkgConfig)
    pkg_check_modules(LIBMPV REQUIRED mpv)

    target_include_directories(appAniLiberty PUBLIC ${LIBMPV_INCLUDE_DIRS})
    target_link_libraries(appAniLiberty PUBLIC ${LIBMPV_LIBRARIES})
    target_compile_options(appAniLiberty PUBLIC ${LIBMPV_CFLAGS_OTHER})
endif ()


include(GNUInstallDirs)
install(TARGETS appAniLiberty
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
