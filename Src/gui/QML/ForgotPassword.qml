//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtPositioning 5.5
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4

Item {
    ToolBar {
        id: screentitle
        RowLayout {
            anchors.fill: parent
            ToolButton {
                iconSource: "qrc:/images/X-exit-icon.png"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState + " back pressed")
                    proxyPics.currentState = "Login"
                    //TODO - Not sure why we are quiting application, check with Lou
                    //Qt.quit()
                }
            }
            Text {
                id: photoaddress
                text: qsTr("Forgot Password")
                font.family: "Centuma"
                font.pointSize: 16
                font.bold: true
                anchors.centerIn: parent
            }

        }
    }
    Image {
        id: logo
        opacity: .69
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight / 11
        anchors.top: parent.top
        anchors.topMargin: proxyPics.uiHeight / 11
        fillMode: Image.PreserveAspectFit
        source: "qrc:/images/ProxyPics-logo-01.png"
    }

    Rectangle {
        id: nameleft
        focus: true
        anchors.left: parent.left
        anchors.top: logo.bottom
        anchors.topMargin: topmargin
        width: proxyPics.uiWidth * .1
        height: proxyPics.uiHeight / 11
        color: "transparent"
    }

    TextField {
        id: firstname
        opacity: .69
        visible: false
        anchors.top: logo.bottom
        anchors.left: nameleft.right
        anchors.topMargin: topmargin
        width: proxyPics.uiWidth * .4
        height: proxyPics.uiHeight / 11
        Layout.fillWidth: true
        font.family: "Centuma"
        text: proxyPics.firstName
        placeholderText: "First Name"
        style: TextFieldStyle {
            textColor: "grey"
            background: Rectangle {
                radius: 2
                implicitWidth: firstname.width
                implicitHeight: firstname.height
                border.color: "grey"
                border.width: 1
            }
        }
        Keys.onEnterPressed: {
            console.log("Enter:" + firstname.text)
            lastname.forceActiveFocus()
        }
        Keys.onReturnPressed: {
            console.log("Return:" + firstname.text)
            lastname.forceActiveFocus()
        }
    }

    TextField {
        id: lastname
        opacity: .69
        visible: false
        anchors.top: logo.bottom
        anchors.left: firstname.right
        anchors.topMargin: topmargin
        width: proxyPics.uiWidth * .4
        height: proxyPics.uiHeight / 11
        Layout.fillWidth: true
        font.family: "Centuma"
        text: proxyPics.lastName
        placeholderText: "Last Name"
        style: TextFieldStyle {
            textColor: "grey"
            background: Rectangle {
                radius: 2
                implicitWidth: lastname.width
                implicitHeight: lastname.height
                border.color: "grey"
                border.width: 1
            }
        }
        Keys.onReturnPressed: {
            proxyPics.userId = "user_" + phonenumber.text
            console.log("UI State:" + proxyPics.currentState + "Return:" + phonenumber.text)
            phonenumber.forceActiveFocus()
        }
    }

    TextField {
        id: phonenumber
        opacity: .69
        anchors.top: firstname.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: topmargin
        width: proxyPics.uiWidth * .8
        height: proxyPics.uiHeight / 11
        Layout.fillWidth: true
        font.family: "Centuma"
        text: proxyPics.phoneNumber
        placeholderText: "Phone Number"
        style: TextFieldStyle {
            textColor: "grey"
            background: Rectangle {
                radius: 2
                implicitWidth: phonenumber.width
                implicitHeight: phonenumber.height
                border.color: "grey"
                border.width: 1
            }
        }

        Keys.onReturnPressed: {
            proxyPics.userId = "user_" + phonenumber.text
            console.log("UI State:" + proxyPics.currentState + "Return:" + phonenumber.text)
            email.forceActiveFocus()
        }
    }

    TextField {
        id: email
        opacity: .69
        anchors.top: logo.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: topmargin
        width: proxyPics.uiWidth * .8
        height: proxyPics.uiHeight / 11
        font.family: "Centuma"
        text: proxyPics.userEmail
        Layout.fillWidth: true
        placeholderText: "Email"
        style: TextFieldStyle {
            textColor: "grey"
            background: Rectangle {
                implicitWidth: email.width
                implicitHeight: email.height
                radius: 2
                border.color: "grey"
                border.width: 1
            }
        }
        Keys.onReturnPressed: {
            console.log("UI State: " + proxyPics.currentState + " Email:" + email.text)
            proxyPics.userId = "user_" + email.text
            passcode.forceActiveFocus()
        }

        onTextChanged: {
            hasLoginDetails()
        }
    }

    Rectangle {
        id: continuebutton
        anchors.top: createaccountbutton.bottom
        anchors.left: backbutton.right
        anchors.topMargin: proxyPics.uiHeight / 22
        width: proxyPics.uiWidth * .7
        height: proxyPics.uiHeight / 11
        visible: false
        smooth: true
        radius: 90
        color: "black"
        border {
            width: 3
            color: "white"
        }

        Text {
            font.family: "Centuma"
            anchors.centerIn: parent
            text: "REQUEST PASSWORD"
            color: "white"
            font.pointSize: 24
        }
        MouseArea {
            id: continuebuttonMouseArea
            anchors.fill: parent

            onClicked: {

                console.log("UI State: " + proxyPics.currentState
                            + "continue clicked" + phonenumber.text)
                 // We must now write the user data back to c++ from javascript
                proxyPics.userId = "user_" + phonenumber.text
                proxyPics.userEmail = email.text
                proxyPics.phoneNumber = phonenumber.text
                proxyPics.firstName = firstname.text
                proxyPics.lastName = lastname.text

                if (phonenumber.text === "")
                    missing_user_detail.visible = true
                else {
                    console.log("UI State: " + proxyPics.currentState
                                + "creating new user.." + proxyPics.userId
                                + email.text + phonenumber.text + passcode.text)
                }
                proxyPics.currentState = "REQUEST PASSWORD"  // send user data to server
            }
            hoverEnabled: true
        }
        scale: continuebuttonMouseArea.pressed ? 1.5 : 1.00
    }

}
