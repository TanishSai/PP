//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
//
import QtQuick 2.5
import QtQuick.Dialogs 1.2
Item {

       MessageDialog {
           icon: StandardIcon.Information
           visible: ( proxyPics.currentState === "Request Success")
           title: "Success !"
           text: "Your Photo Request has been submited.<br>"
                 + "Now just sitback, and wait for the Pics to roll in."
           onAccepted: {
               console.log("UI State:"+proxyPics.currentState+"holy cow batman they accepted!")
               proxyPics.currentState = "Home"
           }
       }
}
