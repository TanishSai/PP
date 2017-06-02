//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
//
import QtQuick 2.5
import QtQuick.Dialogs 1.1
Rectangle {
    color: "grey"
    Component.onCompleted: {
        invalid_photo_request.open()
    }

    MessageDialog {
        id: invalid_photo_request
        icon: StandardIcon.Information
        title: "Invalid ProxyPics Photo Request !"
        text: "please give correct details of assignment"
        onAccepted: {
             proxyPics.currentState = "New Photo Request"
            invalid_photo_request.close()
        }
    }
}
