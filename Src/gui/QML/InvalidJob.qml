//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
//
import QtQuick 2.5
import QtQuick.Dialogs 1.1
Rectangle {
    color: "grey"
    property string caption: "please give correct details, password or user is incorrect"
    Component.onCompleted: {
        invalid_user.open()
    }

    MessageDialog {
        id: invalid_user
        icon: StandardIcon.Information
        title: "Invalid ProxyPics User !"
        text: caption
        onAccepted: {
             proxyPics.currentState = "Login"
            invalid_user.close()
        }
    }
}
