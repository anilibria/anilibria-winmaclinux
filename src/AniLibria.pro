QT += quick multimedia network webview concurrent svg
CONFIG += c++11

windows {
    QT += av
}

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# If you need not check version remove or comment this line
DEFINES += USE_VERSION_CHECK

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Classes/Models/changesmodel.cpp \
    Classes/Models/externalplaylistvideo.cpp \
    Classes/Models/fullreleasemodel.cpp \
    Classes/Models/historymodel.cpp \
    Classes/Models/onlinevideomodel.cpp \
    Classes/Models/releasemodel.cpp \
    Classes/Models/releasetorrentmodel.cpp \
    Classes/Models/seenmarkmodel.cpp \
    Classes/Models/seenmodel.cpp \
    Classes/Models/usersettingsmodel.cpp \
    Classes/Models/youtubevideomodel.cpp \
    Classes/Services/analyticsservice.cpp \
    Classes/Services/anilibriaapiservice.cpp \
    Classes/Services/applicationsettings.cpp \
    Classes/Services/downloadmanager.cpp \
    Classes/Services/imageloader.cpp \
    Classes/Services/localstorageservice.cpp \
    Classes/Services/offlineimagecacheservice.cpp \
    Classes/Services/synchronizationservice.cpp \
    Classes/Services/versionchecker.cpp \
    main.cpp

RESOURCES += qml.qrc

RC_ICONS = fullsizesicon.ico

ICON = anilibria.icns

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Classes/Models/changesmodel.h \
    Classes/Models/externalplaylistvideo.h \
    Classes/Models/fullreleasemodel.h \
    Classes/Models/historymodel.h \
    Classes/Models/onlinevideomodel.h \
    Classes/Models/releasemodel.h \
    Classes/Models/releasetorrentmodel.h \
    Classes/Models/seenmarkmodel.h \
    Classes/Models/seenmodel.h \
    Classes/Models/usersettingsmodel.h \
    Classes/Models/youtubevideomodel.h \
    Classes/Services/analyticsservice.h \
    Classes/Services/anilibriaapiservice.h \
    Classes/Services/applicationsettings.h \
    Classes/Services/downloadmanager.h \
    Classes/Services/imageloader.h \
    Classes/Services/localstorageservice.h \
    Classes/Services/offlineimagecacheservice.h \
    Classes/Services/synchronizationservice.h \
    Classes/Services/versionchecker.h \
    globalconstants.h


