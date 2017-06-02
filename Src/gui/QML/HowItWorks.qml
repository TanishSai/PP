//
// (c) copyright Positively Front Street, Inc 2016
// Author: Louis Meadows
// Designer: Katelyn Mattson
import QtQuick 2.5
import QtQuick.Dialogs 1.2
Rectangle {
    color: "grey"
    Component.onCompleted: {
        howItWorks.open()
    }

    MessageDialog {
        id: howItWorks
        icon: StandardIcon.Information
        title: "How ProxyPics Works for you !"
        text: "Jane needs photos of a property She is planning to purchase or sell,"
              + "but Jane needs the photos before she gets back to town...Jane puts a request on ProxyPics"
              + "<->Jack has a phone and a few extra minutes."
              + "He pulls up ProxyPics, and sees that Jane needs a photo in his area."
              + "Jack accepts the assignment and takes the photos and they are sent to Jane for approval" + " On approval the photo is stored on Jane's phone and payment is sent to Jack!"
        onAccepted: {
            proxyPics.currentState = "Menu"
            howItWorks.close()
        }
    }

}
