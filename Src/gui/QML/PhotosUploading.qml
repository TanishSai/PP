//
// copyright (c) Positively Front Street, Inc 2016, 2017
// author: Louis Meadows
// created July 16th 2016
//

import QtQuick 2.5
import QtQuick.Dialogs 1.1
Rectangle {
    color: "grey"
    Component.onCompleted: {
        photouploadmessage.open()
    }

    MessageDialog {
        id: photouploadmessage
        icon: StandardIcon.Warning
        title: "Assignment Photo now being uploaded..."
        text: "Your photo is being uploaded and will be sent to the photo requestor"
        standardButtons: StandardButton.Ok
        onAccepted: {
            console.log("accepted")
            proxyPics.currentState = "Photo Capture"
            photouploadmessage.close()
        }

    }
}



