//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtWebView 1.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

Rectangle {
    id: home_screen
    property int assigment_count
    Component.onCompleted: {
        proxyPics.currentAssignmentAddress = "where am i"
        console.log("UI State:" + proxyPics.currentState + " user id:"
                    + proxyPics.userId + "Home Screen Loaded:" + proxyPics.value
                    + ":" + initialUrl + proxyPics.currentAssignmentAddress)
        proxyPics.mapDestination = proxyPics.currentAssignmentAddress
        proxyPics.mapUrl = initialUrl + proxyPics.currentAssignmentAddress
        console.log("UI State:" + proxyPics.currentState + " user id:" + proxyPics.userId
                    + " url:" + initialUrl + proxyPics.currentAssignmentAddress)
    }

    ToolBar {
        id: screentitle
        RowLayout {
            anchors.fill: parent
            ToolButton {
                iconSource: "qrc:/images/User-icon.png"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState)
                    proxyPics.currentState = "User Profile"
                }
            }
            Text {
                id: photoaddress
                text: qsTr("Home")
                font.pointSize: 16
                font.family: "Centuma"
                font.bold: true
                anchors.centerIn: parent
            }
            ToolButton {
                iconSource: "qrc:/images/Menu.png"
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState)
                    proxyPics.currentState = "Menu"
                }
            }
        }
    }

    // url: "https://maps.google.com"
    // When load completes google maps will have added the coordinates and map id both should be saved with assignment details
    // https://www.google.com/maps/place/Bradley+Ave,+San+Jose,+CA+95128/@37.317846,-121.9338256,17z/data=!4m2!3m1!1s0x808fcb3a9677d075:0xa60eb79004b5a5c7
    // We supply--------------------------------------------------|Google maps fills in ----------------------------------------------------------------->|
    // +95128/@37.317846,-121.9338256,17z/data=!4m2!3m1!1s0x808fcb3a9677d075:0xa60eb79004b5a5c7
    // zip---/Coordinates----------------/Google map id----------------------------------------|
    Rectangle {
        id: map
        objectName: "mapView"
        width: parent.width
        height: proxyPics.uiHeight * .75
        anchors.top: screentitle.bottom
        anchors.bottomMargin: proxyPics.uiHeight / 9
        border.color: "yellow"
        border.width: 25
        WebView {
            id: home_screen_map_view
            anchors.fill: parent
            url: proxyPics.mapUrl
            onLoadingChanged: {
                if (!loading)
                    console.log("UI State: " + proxyPics.currentState + " user id:"
                                + proxyPics.userId + " webview:" + url)
                else
                    console.log(loadRequest)
            }
        }
    }

    ToolBar {
        id: buttonbar
        anchors.bottom: parent.bottom
        RowLayout {
            anchors.fill: parent
            OvalButton {
                id: request_photos_button
                width: proxyPics.uiWidth / 2.3
                height: proxyPics.uiHeight / 22
                label: "REQUEST PHOTOS"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState
                                + " user id:" + proxyPics.userId)
                    proxyPics.currentState = "Photo Request"
                }
            }

            OvalButton {
                anchors.right: parent.right
                height: proxyPics.uiHeight / 22
                width: proxyPics.uiWidth / 2.3
                label: "FIND AN ASSIGNMENT"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState)
                    proxyPics.currentState = "Photo Assignment"
                }
            }
        }
    }
    Keys.onReleased: {
        console.log(event.key)
        if (event.key === Qt.Key_Back) {
            console.log("UI State: " + proxyPics.currentState
                        + " Hardware Back button captured Home- wunderbar !")
            // fake soft back key press when hardware back key pressed
            proxyPics.currentState = "Menu"
            event.accepted = true
        }
    }
}
