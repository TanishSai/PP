// QML file: CreateAccount.qml
// Provides main login area for user to type username and password
// emits signal loginButtonCLicked passing the text data to C++ for authenication
// Author Louis Meadows
// (c) Copyright Positively Front Street 2015, 2016, 2017
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import QtQml 2.2
import QtQuick.Extras 1.4

//This qml is responsible for getting user input for login details
//It also has functionality to create new account.
Item {
    visible: true
    width: proxyPics.uiWidth
    height: proxyPics.uiHeight
    id: login
    signal clicked

    property int topmargin: proxyPics.uiHeight / 22

    OpacityAnimator {
        target: login
        from: .1
        to: 1
        duration: 1500
        running: true
    }

    Component.onCompleted: {
        console.log("UI State:" + proxyPics.currentState + " CREATE-ACCOUNT State")
        enableSignUpControls(true)
        clearTextFields()
        createaccountbutton.color = "transparent"
        login.state = "CREATE-ACCOUNT" //TODO - Do we really need to set this, Not sure why this has been called
        topmargin = 0
        email.anchors.top = phonenumber.bottom
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
        visible: true
        anchors.top: logo.bottom
        anchors.left: nameleft.right
        anchors.topMargin: topmargin
        width: proxyPics.uiWidth * .4
        height: proxyPics.uiHeight / 11
        Layout.fillWidth: true
        font.family: "Centuma"
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
        visible: true
        anchors.top: logo.bottom
        anchors.left: firstname.right
        anchors.topMargin: topmargin
        width: proxyPics.uiWidth * .4
        height: proxyPics.uiHeight / 11
        Layout.fillWidth: true
        font.family: "Centuma"
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
        anchors.top: phonenumber.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: topmargin
        width: proxyPics.uiWidth * .8
        height: proxyPics.uiHeight / 11
        font.family: "Centuma"
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
        }
    }

    Rectangle {
        id: passleft
        anchors.left: parent.left
        anchors.top: email.bottom
        anchors.topMargin: topmargin
        width: proxyPics.uiWidth * .1
        height: proxyPics.uiHeight / 11
        color: "transparent"
    }

    TextField {
        id: passcode
        anchors.top: email.bottom
        anchors.left: passleft.right
        anchors.topMargin: topmargin
        opacity: .69
        width: proxyPics.uiWidth * .8
        height: proxyPics.uiHeight / 11
        font.family: "Centuma"
        placeholderText: "Password"
        echoMode: TextInput.Password
        style: TextFieldStyle {
            textColor: "grey"
            background: Rectangle {
                radius: 2
                implicitWidth: passcode.width
                implicitHeight: passcode.height
                border.color: "grey"
                border.width: 1
            }
        }

        Keys.onReturnPressed: {
            proxyPics.userPassword = passcode.text // cause logon to be sent
            console.log("UI State: " + proxyPics.currentState + " email:" + email.text)
            proxyPics.userId = "user_" + email.text //TODO - Do we need to do it here or
            //can we move it down after if statements/validation
            if (email.text === "" || passcode.text === "") {
                missing_user_detail.visible = true
                return
            }
            if (phonenumber.text !== "")
                proxyPics.userId = "user_" + email.text
            console.log("----------2----------test----------" + proxyPics.userId)

            if (login.state === "CREATE-ACCOUNT") {
                console.log("UI State: " + proxyPics.currentState + " creating new user.."
                            + proxyPics.userId + " Email: " + email.text + " Phone Number: "
                            + phonenumber.text + " Passcode: " + passcode.text)
            }
        }

        onTextChanged: {
            hasLoginDetails()
        }
    }

    Rectangle {
        id: backbutton
        anchors.left: parent.left
        anchors.top: passcode.bottom
        anchors.topMargin: proxyPics.uiHeight / 22
        width: proxyPics.uiWidth * .3
        height: proxyPics.uiHeight / 11
        radius: 90
        color: "black"
        border {
            width: 3
            color: "white"
        }

        Text {
            font.family: "Centuma"
            anchors.centerIn: parent
            text: "BACK"
            color: "white"
            font.pointSize: 24
        }
        MouseArea {
            id: backbuttonMouseArea
            anchors.fill: parent

            onClicked: {
                proxyPics.currentState = "Login"
                console.log("UI State: " + proxyPics.currentState
                            + "continue clicked" + phonenumber.text)
            }
            hoverEnabled: true
        }
        scale: backbuttonMouseArea.pressed ? 1.5 : 1.00
    }

    Rectangle {
        id: continuebutton
        anchors.top: passcode.bottom
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
            text: "CONTINUE"
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
                proxyPics.userPassword = passcode.text
                proxyPics.firstName = firstname.text
                proxyPics.lastName = lastname.text

                if (phonenumber.text === "" || passcode.text === "")
                    missing_user_detail.visible = true
                else {
                    console.log("UI State: " + proxyPics.currentState
                                + "creating new user.." + proxyPics.userId
                                + email.text + phonenumber.text + passcode.text)
                }
                proxyPics.currentState = "Send Create User Packet"   // send user data to server
            }
            hoverEnabled: true
        }
        scale: continuebuttonMouseArea.pressed ? 1.5 : 1.00
    }

    Rectangle {
        anchors.bottom: parent.bottom
        anchors.left: parent.right
        anchors.margins: 10
        Picture {
            id: questionmark
            source: "qrc:/iso-icons/iso_grs_7000_4_0435.dat"
        }
        MouseArea {
            id: questionmarkMouseArea
            anchors.fill: parent
            hoverEnabled: true
            onClicked: {
                proxyPics.currentState = "how It Works"
            }
        }
        scale: questionmarkMouseArea.pressed ? 1.5 : 1.00
    }


    Keys.onReleased: {
        console.log(event.key)
        if (event.key === Qt.Key_Back) {
            console.log("UI State:" + proxyPics.currentState
                        + "Hardware Back button captured by Login- wunderbar !")
            passcode.focus = false
            email.visible = true
            proxyPics.currentState = "Splash"
            event.accepted = true
        }
    }

    //This function is used to enable signup controls based on state
    function enableSignUpControls(enableStatus) {
        continuebutton.visible = enableStatus
        createaccountbutton.visible = !enableStatus
        backbutton.visible = enableStatus
        firstname.visible = enableStatus
        lastname.visible = enableStatus
        phonenumber.visible = enableStatus
    }

    //This function is used to clear all text fields
    function clearTextFields() {
        email.text = ""
        firstname.text = ""
        lastname.text = ""
        phonenumber.text = ""
        passcode.text = ""
    }
}
