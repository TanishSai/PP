//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc
// Origniated: January 15th 2016,
// Updated: July 2016
// Application: Assignment Details
import QtQuick 2.5
import QtPositioning 5.5
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.XmlListModel 2.0
import QtWebView 1.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

Item {
    id: assignmentdetails
    visible: true
    signal timeElapsed
    property string timeString
    anchors.fill: parent
    Component.onCompleted: {
        console.log("Assignment Details Component Complete")
        console.log(proxyPics.assignmentValue + " " + proxyPics.assignmentTicket)
        console.log(proxyPics.currentAssignmentAddress + " " + proxyPics.currentAssignmentId
                    + " " + proxyPics.currentAssignmentName)
        proxyPics.setMapDestination = proxyPics.currentAssignmentAddress
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
                    proxyPics.currentState = "Photo Assignment"
                }
            }
            Text {
                id: photoaddress
                text: qsTr("Assignment Details")
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
    Rectangle {
        id: assignmentvalue
        anchors.topMargin: 10
        anchors.top: screentitle.bottom
        height: proxyPics.uiWidth / 2
        width: proxyPics.uiWidth / 5
        Text {
            id: assignment_value
            color: "green"
            font.family: "Centuma"
            font.bold: true
            font.pointSize: 44
            text: proxyPics.assignmentValue
        }

        Text {
            color: "lightgrey"
            anchors.top: assignment_value.bottom
            text: "per photo"
            font.family: "Centuma"
        }
    }

    Rectangle {
        id: addressText
        height: proxyPics.uiWidth / 4
        width: proxyPics.uiWidth
        anchors.leftMargin: 10
        anchors.topMargin: 10
        anchors.top: screentitle.bottom
        anchors.left: assignmentvalue.right
        Text {

            width: proxyPics.uiWidth / 2
            color: "green"
            font.family: "Centuma"
            font.bold: true
            font.pointSize: 22
            wrapMode: Text.WordWrap
            textFormat: Text.RichText
            elide: Text.ElideMiddle
            text: proxyPics.currentAssignmentAddress
        }
    }
    Rectangle {
        id: type_verified_calander
        width: proxyPics.uiWidth
        height: proxyPics.uiWidth / 22
        anchors.top: addressText.bottom
        RowLayout {
            Image {
                Layout.preferredWidth: proxyPics.uiWidth / 23
                Layout.preferredHeight: proxyPics.uiWidth / 23
                fillMode: Image.PreserveAspectFit
                width: proxyPics.uiHeight / 15
                height: proxyPics.uiHeight / 15
                source: "qrc:/images/House Icon.png"
            }
            Text {
                font.family: "Centuma"
                font.pointSize: 10
                width: parent.width
                wrapMode: Text.WordWrap
                text: "Residential"
                clip: true
            }

            Image {
                Layout.preferredWidth: proxyPics.uiWidth / 23
                Layout.preferredHeight: proxyPics.uiWidth / 23
                fillMode: Image.PreserveAspectFit
                width: proxyPics.uiHeight / 15
                height: proxyPics.uiHeight / 15
                source: "qrc:/images/VERIFIED.png"
            }
            Text {
                font.family: "Centuma"
                font.pointSize: 10
                width: parent.width
                wrapMode: Text.WordWrap
                text: "Verified Payment"
                clip: true
            }

            Image {
                fillMode: Image.PreserveAspectFit
                width: proxyPics.uiHeight / 15
                height: proxyPics.uiHeight / 15
                Layout.preferredWidth: proxyPics.uiWidth / 19
                Layout.preferredHeight: proxyPics.uiWidth / 19
                source: "qrc:/images/CALENDAR.png"
            }
            Text {
                font.family: "Centuma"
                font.pointSize: 10
                width: parent.width
                wrapMode: Text.WordWrap
                text: "Posted " + proxyPics.getDifferenceWithCurrentTime(
                          proxyPics.assignmentPubDate) + " ago"
                clip: true
            }
        }
    }
    Item {
        id: map
        objectName: "mapView"
        anchors.top: type_verified_calander.bottom
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight * .6

        anchors.margins: 30
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

    OvalButton {
        id: accept_Button
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: proxyPics.uiHeight / 44
        anchors.bottom: parent.bottom
        width: proxyPics.uiWidth / 1.5
        height: proxyPics.uiHeight / 22
        label: "ACCEPT ASSIGNMENT"
        onClicked: {
            console.log("UI State: " + proxyPics.currentState + " " + accept_Button.label)
            accept_Button.label = "UPLOAD PHOTOS"
            proxyPics.currentState = "Accept Assignment"
            map.height = proxyPics.uiHeight * .8
        }
    }
    focus: true
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
