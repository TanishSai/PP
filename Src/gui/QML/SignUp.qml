// (c) 2016 copyright Positively Front Street, Inc
// Author: Louis Meadows
// Designer: Katelyn Mattson
// Concept: Luke
// This is the sign up screen for ProxyPics
//
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Window 2.2

Item {
    id: signup
    width: Screen.width * .9
    height: Screen.height * .75
    anchors.centerIn: parent
    signal message(string msg)
    signal signuprequest(string username, string email, string number)
    property bool keyboardvisible: false

    Rectangle {
        id: emailrect
        y: (Screen.height / 10)
        width: parent.width
        height: email.implicitHeight
        Text {
            id: emailLabel
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: "email"
            style: Text.Sunken
            font.pointSize: 32
        }
        Rectangle {
            border.color: "green"
            border.width: 10
            color: "skyblue"
            anchors.left: emailLabel.right
            anchors.leftMargin: 20
            anchors.right: parent.right
            height: parent.height
            TextInput {
                id: email
                text: ""
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter
                height: parent.height
                width: parent.width - 25
                font.pointSize: 32
            }
        }
    }
    Rectangle {
        y: (Screen.height / 10) * 2
        width: parent.width
        height: phonenumber.implicitHeight
        anchors.left: parent.left
        Text {
            id: mobileLabel
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            text: "mobile#"
            style: Text.Sunken
            font.pointSize: 32
        }
        Rectangle {
            border.color: "green"
            border.width: 10
            color: "skyblue"
            anchors.left: mobileLabel.right
            anchors.leftMargin: 20
            anchors.right: parent.right
            height: parent.height
            TextInput {
                id: phonenumber
                text: ""
                anchors.right: parent.right
                horizontalAlignment: Text.AlignHCenter
                height: parent.height
                width: parent.width - 25
                font.pointSize: 32
            }

        }
    }
    Rectangle {
        y: (Screen.height / 10) * 3
        width: agree.implicitWidth
        height: agree.implicitHeight
        anchors.centerIn: parent
        CheckBox {
            id: agree
            anchors.centerIn: parent
            text: "Agree to terms"
        }
    }

    Rectangle {
        id: submit
        y: (Screen.height / 10) * 4
        height: Screen.height / 10
        width: parent.width
        border.color: "lime"
        Text {
            anchors.centerIn: parent
            text: "Submit"
        }
        radius: 60
        MouseArea {
            id: submitMouseArea
            anchors.fill: parent
            onClicked: {
                email.focus = false
                console.log("submit clicked")
                signup.message("submit clicked!")
                signuprequest("ProxyPics 0.0.7", email.text, phonenumber.text)
            }
        }
        color: submitMouseArea.pressed ? Qt.darker("green", 1.5) : "green"
        scale: submitMouseArea.pressed ? 1.1 : 1.00
    }
}
