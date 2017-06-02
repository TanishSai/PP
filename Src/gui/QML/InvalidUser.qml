//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
//
import QtQuick 2.5
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

Rectangle {
    color: "grey"
    Component.onCompleted: {

    }
    ToolBar {
        id: screentitle
        RowLayout {
            anchors.fill: parent
            ToolButton {
                iconSource: "qrc:/images/X-exit-icon.png"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState + " back pressed")
                    proxyPics.currentState = "Login"
                }
            }
            Text {
                id: photoaddress
                text: qsTr("Invalid User")
                font.family: "Centuma"
                font.pointSize: 16
                font.bold: true
                anchors.centerIn: parent
            }
        }
    }
    Text {
        anchors.centerIn: parent
        font.family: "Centuma"
        text: "Invalid ProxyPics User ! <br>please give correct details, password or user is incorrect"
    }
    Timer {
        interval: 5000
        running: true
        onTriggered: {
            console.log("UI State: " + proxyPics.currentState + " Login Time Out")
            proxyPics.currentState = "Authentication Failure"
        }
    }
    focus: true

    Keys.onReleased: {
        console.log(event.key)
        if (event.key === Qt.Key_Back) {
            console.log("UI State:" + proxyPics.currentState
                        + "Hardware Back button captured Photo Assignment - wunderbar !")
            // fake soft back key press when hardware back key pressed
            proxyPics.currentState = "Login"
            event.accepted = true
        }
    }
}
