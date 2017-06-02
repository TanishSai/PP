//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
//
import QtQuick 2.5
import QtQuick.Dialogs 1.1
Rectangle {
    color: "grey"
    Component.onCompleted: {
        newusercreated.open()
    }

    MessageDialog {
        id: newusercreated
        icon: StandardIcon.Information
        title: "New user created"
        text: "You're now logged on, enjoy ProxyPics !"
        onAccepted: {
             proxyPics.currentState = "REGISTRATION SUCCESS"
            newusercreated.close()
        }
}
}
