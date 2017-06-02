// copyrigt (c) Positively Front Street, Inc 2017
// author: Louis Meadows
// created July 15th 2016
// 03SEP16 Louis Meadows, working on issue #1143
import QtQuick 2.5
import QtQuick.Dialogs 1.1

Rectangle {
    color: "grey"

    id: request_sent
    signal currentAssignment
    property string message: ""
    property string acceptedStateString: ""
    property string yesStateString: ""
    property string rejrctedStateString: ""
    property string msgTitle: ""
    signal accepted
    signal popUpYes
    signal popUpRejected
    onMessageChanged: {
        if (message !== "")
            console.log("UI State:" + proxyPics.currentState + "message:" + message)
    }

    Component.onCompleted: {
        requestSent.open()
    }

    Connections {
        target: proxyPics
        onNewJobCreatedChanged : {
            if (proxyPics.newJobCreated)
            {
                timer.stop()
                requestSent.close()
                acceptassignment.open()
            }
        }
    }

    Timer {
        id: timer
        interval: 5000
        running: true
        onTriggered: {
            console.log("UI State: " + proxyPics.currentState + " Login Time Out")
            proxyPics.currentState = "Network Timeout"
        }
    }

    MessageDialog {
        id: acceptassignment
        icon: StandardIcon.Warning
        title: msgTitle
        text: "Your Photo Request has been submited."
              + "Now just sitback, and wait for the Photos to roll in."
              + "Assignment " + " is successfully created."
        standardButtons: rejrctedStateString === "" ? StandardButton.Ok : StandardButton.Cancel
                                                      | StandardButton.Ok
        onAccepted: {
            console.log("UI State:" + proxyPics.currentState + " accepted")
            proxyPics.currentState = "Home"
            acceptassignment.close()
            proxyPics.newJobCreated = false
        }

        onRejected: {
            console.log("cancel " + rejrctedStateString)
            proxyPics.currentState = rejrctedStateString
            acceptassignment.close()
            proxyPics.newJobCreated = false
        }
    }

    MessageDialog {
        id: requestSent
        icon: StandardIcon.Warning
        title: msgTitle
        text: "Your Photo Request is under process ..."
    }
}
