//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
//
import QtQuick 2.5
import QtQuick.Dialogs 1.1
Rectangle {
    color: "grey"
    Component.onCompleted: {
        alreadyregistered.open()
    }

    MessageDialog {
        id: alreadyregistered
        icon: StandardIcon.Information
        title: "already registered"
        text: "user detail is already registered, attemting to login."
        onAccepted: {
            proxyPics.currentState = "Login"
            alreadyregistered.close()
        }
    }
}
