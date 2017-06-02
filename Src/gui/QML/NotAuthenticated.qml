
// copyrigt (c) Positively Front Street, Inc
// author: Louis Meadows
// created July 15th 2016
// 03SEP16 Louis Meadows, working on issue #1143
import QtQuick 2.5
import QtQuick.Dialogs 1.1
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0

Rectangle {
    color: "grey"
    Component.onCompleted: {
        acceptassignment.open()
    }

    ToolBar {
        id: screentitle
        RowLayout {
            anchors.fill: parent
            ToolButton {
                iconSource: "qrc:/images/User-icon.png"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState)
                    proxyPics.currentState = "User Profile"
                }
            }
            Text {
                id: photoaddress
                text: qsTr("Not Authenticated")
                font.pointSize: 16
                font.family: "Centuma"
                font.bold: true
                anchors.centerIn: parent
            }
            ToolButton {
                iconSource: "qrc:/images/Menu.png"
                Layout.alignment: Qt.AlignRight
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState)
                    proxyPics.currentState = "Menu"
                }
            }
        }
    }

    id: popup
    signal currentAssignment
    property string message: "Invalid User Credentials"
    property string acceptedStateString: "Login"
    property string yesStateString: ""
    property string rejrctedStateString: ""
    property string msgTitle: ""
    signal popUpAccepted
    signal popUpYes
    signal popUpRejected
    onMessageChanged: {
        if (message !== "")
            console.log("UI State:" + proxyPics.currentState + "message:" + message)
    }

    MessageDialog {
        id: acceptassignment
        icon: StandardIcon.Warning
        title: msgTitle
        text: message
        standardButtons: rejrctedStateString === "" ? StandardButton.Ok : StandardButton.Cancel
                                                      | StandardButton.Ok
        onAccepted: {
            console.log("UI State:" + proxyPics.currentState + "accepted")
            proxyPics.currentState = acceptedStateString
            acceptassignment.close()
        }

        onRejected: {
            console.log("cancel " + rejrctedStateString)
            proxyPics.currentState = rejrctedStateString
            acceptassignment.close()
        }
    }
}
