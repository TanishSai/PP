//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtPositioning 5.5
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtWebView 1.1
import QtQuick.Controls 1.4

Item {
    id: current_assignment
    visible: true
    signal timeElapsed
    property string timeString
    anchors.fill: parent
    Component.onCompleted: {
        console.log("UI State: " + proxyPics.currentState
                    + " Current Assignment Component Complete")
        if (proxyPics.value > 9)
            proxyPics.value = 9
        console.log(proxyPics.value)
        console.log(proxyPics.currentAssignmentAddress)
        proxyPics.setMapDestination(proxyPics.currentAssignmentAddress)
        proxyPics.mapUrl = initialUrl + proxyPics.currentAssignmentAddress
    }

    ToolBar {
        id: screentitle
        RowLayout {
            anchors.fill: parent
            ToolButton {
                iconSource: "qrc:/images/X-exit-icon.png"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState + " back pressed")
                    proxyPics.currentState = "My Assignments"
                }
            }
            Text {
                id: photoaddress
                text: qsTr(proxyPics.currentAssignmentName);//qsTr("Assignments near you")
                font.family: "Centuma"
                font.pointSize: 16
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
    Item {
        id: map
        objectName: "mapView"
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight * .8
        anchors.top: screentitle.bottom
        WebView {
            id: webView
            anchors.fill: parent
            url: proxyPics.mapUrl

            // When load completes google maps will have added the coordinates and map id both should be saved with assignment details
            // https://www.google.com/maps/place/Bradley+Ave,+San+Jose,+CA+95128/@37.317846,-121.9338256,17z/data=!4m2!3m1!1s0x808fcb3a9677d075:0xa60eb79004b5a5c7
            // We supply--------------------------------------------------|Google maps fills in ----------------------------------------------------------------->|
            // +95128/@37.317846,-121.9338256,17z/data=!4m2!3m1!1s0x808fcb3a9677d075:0xa60eb79004b5a5c7
            // zip---/Coordinates----------------/Google map id----------------------------------------|
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
        anchors.bottom: parent.bottom
        RowLayout {
            id: layout_assignment_button
            spacing: 6
            Rectangle {
                id: camera_Button
                width: proxyPics.uiWidth / 7
                height: proxyPics.uiWidth / 7
                property string text
                property color buttonColor: "#33EE33"
                property color onHoverColor: "black"
                property color borderColor: "gold"
                radius: 90
                border {
                    width: 3
                    color: borderColor
                }
                Image {
                    id: camera_Buttonimage
                    anchors.centerIn: parent
                    width: proxyPics.uiWidth / 9
                    height: proxyPics.uiWidth / 9
                    fillMode: Image.Stretch
                    source: "qrc:/images/take-photo-icon.png"
                }

                MouseArea {
                    id: camera_ButtonMouseArea
                    anchors.fill: parent

                    onClicked: {
                        console.log("UI State: " + proxyPics.currentState + " "
                                    + camera_Button.label)
                        proxyPics.currentState = "Photo Capture"
                    }
                }
                color: camera_ButtonMouseArea.pressed ? Qt.darker(
                                                            buttonColor,
                                                            1.5) : buttonColor
                scale: camera_ButtonMouseArea.pressed ? 1.5 : 1.00
            }

            Rectangle {
                id: upload_Button

                Layout.preferredWidth: proxyPics.uiWidth / 4
                Layout.preferredHeight: proxyPics.UiHeight / 22
                radius: 90
                border {
                    width: 3
                    color: "gold"
                }
                property color buttonColor: "#33EE33"
                property color onHoverColor: "black"
                property color borderColor: "gold"
                Text {
                    anchors.centerIn: parent
                    text: "UPLOAD"
                    color: "white"
                    font.pointSize: 14
                    font.family: "Centuma"
                    font.bold: true
                }
//                MouseArea {
//                    id: upload_ButtonMouseArea
//                    anchors.fill: parent
//                    onClicked: {
//                        console.log("UI State: " + proxyPics.currentState + " "
//                                    + upload_Button.label)

//                        // send picture to server
//                        proxyPics.currentState = "Photos Uploading"
//                        console.log("UI State: " + proxyPics.currentState + " "
//                                    + proxyPics.assignmentCreatorUserid + " " + proxyPics.savedpath)
//                        proxyPics.savedpath = ""
//                    }
//                }
//                color: upload_ButtonMouseArea.pressed ? Qt.darker(
//                                                            buttonColor,
//                                                            1.5) : buttonColor
//                scale: upload_ButtonMouseArea.pressed ? 1.5 : 1.00
            }
        }
    }
    focus: true
    Timer {
        id: uploadtimer
        interval: 3500
        running: uistate === "Photos Uploading"
        onTriggered: {
            running = false
            proxyPics.currentState = "Photo Assignment"
            console.log("upload timer triggered")
        }
    }

    Keys.onReleased: {
        console.log(event.key)
        if (event.key === Qt.Key_Back) {
            console.log("UI State:" + proxyPics.currentState
                        + "Hardware Back button captured Photo Assignment - wunderbar !")
            // fake soft back key press when hardware back key pressed
            proxyPics.currentState = "Home"
            event.accepted = true
        }
    }
}
