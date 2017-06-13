QT += webengine# (C) Copyright Positively Front Street, Inc 2016
#
# Author thurberdog (Louis Meadows) @nokwebdev
#
TEMPLATE = app
TARGET = ProxyPics
VERSION = 0.3.1

# Required Qt mobules
QT += qml
QT += xml
QT += core
QT += quick
QT += network
QT += sensors
QT += multimedia
QT += positioning
QT += webengine

#QTPLUGIN += qtsensors_ios
# We want to compile the QML both for performance
# and that's the only way the Apple Store will accept
# ProxyPics or any other mobile application
#CONFIG += qtquickcompiler
CONFIG += c++1y
# ProxyPics C++ Source Code, where  the Busines Logic
# Should be placed
HEADERS += \
    Src/core/proxypics.h \
    Src/core/main.h \
    Src/core/imageprovider.h \
    Src/core/myassignments.h
SOURCES += \
    Src/core/main.cpp \
    Src/core/proxypics.cpp \
    Src/core/imageprovider.cpp \
    Src/core/myassignments.cpp
DISTFILES += \
    android/AndroidManifest.xml \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradlew \
    android/res/values/libs.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew.bat \
    ios/Info.plist \ 
    ios/action_2x.png \
    ios/add_2x.png \
    ios/Appicon-100x100.png \
    ios/Appicon-114x114.png \
    ios/Appicon-29x29.png \
    ios/Appicon-40x40.png \
    ios/Appicon-50x50.png \
    ios/Appicon-58x58.png \
    ios/Appicon-80x80.png \
    ios/Appicon-87x87.png \
    ios/camera_2x.png \
    ios/compose_2x.png \
    ios/Default-568h@2x.png \
    ios/Default-667h@2x.png \
    ios/Default-736h@3x.png \
    ios/Default.png \
    ios/Default@2x.png \
    ios/LaunchImage-2208x1242.png \
    ios/LaunchImage-640x960.png \
    ios/LaunchImage@3x.png \
    ios/refresh_2x.png \
    ios/trash_2x.png \
    html/squareup.html


RESOURCES += images.qrc \
             qml.qrc \
    iso-icons.qrc \
    html.qrc \
    fonts.qrc
ios {
    QMAKE_INFO_PLIST = ios/Info.plist
    ios_icon.files = $$files($$PWD/ios/AppIcon*.png)
    QMAKE_BUNDLE_DATA += ios_icon
    ios_artwork.files = $$files($$PWD/ios/iTunesArtwork*.png)
    QMAKE_BUNDLE_DATA += ios_artwork
    app_launch_images.files = $$files($$PWD/ios/LaunchImage*.png)
    QMAKE_BUNDLE_DATA += app_launch_images
    app_launch_screen.files = $$files($$PWD/ios/MyLaunchScreen.xib)
    QMAKE_BUNDLE_DATA += app_launch_screen

    QMAKE_IOS_DEPLOYMENT_TARGET = 9.3
    # Note for devices: 1=iPhone, 2=iPad, 1,2=Universal.
    QMAKE_IOS_TARGETED_DEVICE_FAMILY = 1
}

include(SortFilterProxyModel.pri)

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =
include(deployment.pri)
ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
ISO_ICONS = \
    iso_grs_7000_4_0435
