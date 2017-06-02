//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
//
import QtQuick 2.5
import QtQuick.Dialogs 1.1
Rectangle {
    color: "grey"
    Component.onCompleted: {
        missinguserdetails.open()
    }

    MessageDialog {
        id: missinguserdetails
        icon: StandardIcon.Information
        title: "Missing user detail"
        text: "Please give correct detail"
        onAccepted: {
            proxyPics.currentState = "Login"
            missinguserdetails.close()
        }
    }
}
