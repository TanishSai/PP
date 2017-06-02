//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtWebView 1.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

Rectangle {
    id: locationsettings_screen
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
                iconSource: "qrc:/images/X-exit-icon.png"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState + " back pressed")
                    proxyPics.currentState = "Settings"
                }
            }
            Text {
                id: locationsettingstext
                text: qsTr("ProxyPics Location Settings")
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
}
