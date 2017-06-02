//
// copyrigt (c) Positively Front Street, Inc
// author: Louis Meadows
// created July 15th 2016
//

import QtQuick 2.5
import QtQuick.Dialogs 1.1

    MessageDialog {
        id: acceptassignment
        icon: StandardIcon.Warning
        title: "Accept Assignment ?"
        text: "You have 30 mins, to complete this assignment before it goes back on the market."
        standardButtons: StandardButton.Ok
        onAccepted: {
            console.log("accepted")
            uistate.state = "Current Assignment"
        }

    }



