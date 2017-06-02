
//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtQml 2.2

Rectangle {
     color: "red"

    Text {
        anchors.centerIn: parent
        font.family: "Centuma"
        text: "Authenticating"
    }

    Timer {
        interval: 5000
        running: true
        onTriggered: {
            console.log("UI State: " + proxyPics.currentState + " Login Time Out")
            proxyPics.currentState = "Network Timeout"
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
