//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
//
import QtQuick 2.5
import QtQuick.Controls 1.2

Rectangle {
    color: "black"
    Item {
        anchors.fill: parent

        Image {
            id: startup
            anchors.fill: parent
            fillMode: Image.PreserveAspectFit
            width: proxyPics.uiWidth
            height: proxyPics.uiHeight
            source: "qrc:/images/1-START-UP.png"
        }

        OpacityAnimator {
            target: startup
            from: 1
            to: 0
            duration: 750
            running: true
            onStopped: {
                console.log(proxyPics.currentState)
                if (proxyPics.currentState === "Splash")
                    proxyPics.currentState = "Login"
            }
        }
    }
    Image {
        id: backgroundimage
        anchors.fill: parent
        source: "qrc:/images/Proxy-Login-BKGD.png"
        opacity: 0
        OpacityAnimator {
            target: backgroundimage
            from: 0
            to: .49
            duration: 750
            running: true
        }
    }
}
