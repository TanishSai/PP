// QML file: Login.qml
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
    property int rootItemWidth:750
    property int rootItemHeight:1334

    OpacityAnimator {
        target: login
        from: .1
        to: 1
        duration: 1500
        running: true
    }

    Component.onCompleted: {
        console.log("UI State:" + proxyPics.currentState + " user id:"
                    + proxyPics.userId + " Login QML completed")
        console.log("UI State:" + proxyPics.currentState + " SIGN-IN State")
        enableSignUpControls(false)
        clearTextFields()
        hasLoginDetails()
    }

    Image {
        id: layer_0
        source: "qrc:/images/loginPage/layer_0.png"
        x: ((parent.width/rootItemWidth)*0)
        y: ((parent.height/rootItemHeight)*0)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }

    Image {
        id: go_button
        source: signinbuttonMouseArea.pressed ? "qrc:/images/loginPage/go_button_open.png" : "qrc:/images/loginPage/go_button.png"
        x: signinbuttonMouseArea.pressed ? ((parent.width/rootItemWidth)*272) : ((parent.width/rootItemWidth)*312)
        y: signinbuttonMouseArea.pressed ? ((parent.height/rootItemHeight)*697) : ((parent.height/rootItemHeight)*739)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)

        MouseArea {
            id: signinbuttonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " user id:"
                            + proxyPics.userId + " SIGN IN clicked" + proxyPics.userPassword)

                proxyPics.userId = "user_" + email.text
                proxyPics.userPassword = passcode.text
                proxyPics.currentState = "Authenticating"
                return
            }
            hoverEnabled: true
        }
    }

    Image {
        id: logo
        source: "qrc:/images/loginPage/logo.png"
        x: ((parent.width/rootItemWidth)*263)
        y: ((parent.height/rootItemHeight)*67)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: text_field_pw
        source: "qrc:/images/loginPage/text_field_pw.png"
        x: ((parent.width/rootItemWidth)*100)
        y: ((parent.height/rootItemHeight)*610)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)

        TextField {
            id: passcode
            opacity: .69
            anchors.fill: parent
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            font.family: "Centuma"
            text: proxyPics.userPassword
            placeholderText: "Password"
            echoMode: TextInput.Password
            style: TextFieldStyle {
                textColor: "white"
                background: Rectangle {
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                    color: "transparent"
                }
            }

            Keys.onReturnPressed: {

                console.log("UI State: " + proxyPics.currentState + " email:" + email.text)
                proxyPics.userId = "user_" + email.text //TODO - Do we need to do it here or
                //can we move it down after if statements/validation
                if (email.text === "" || passcode.text === "") {
                    missing_user_detail.visible = true
                    return
                }
                proxyPics.userId = "user_" + email.text

                proxyPics.userPassword = passcode.text
                proxyPics.currentState = "Authenticating"
            }

            onTextChanged: {
                hasLoginDetails()
            }
        }

    }

    Image {
        id: text_field_emial
        source: "qrc:/images/loginPage/text_field_emial.png"
        x: ((parent.width/rootItemWidth)*100)
        y: ((parent.height/rootItemHeight)*526)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)

        TextField {
            id: email
            opacity: .69
            anchors.fill: parent
            anchors.leftMargin: 5
            anchors.rightMargin: 5
            font.family: "Centuma"
            text: proxyPics.userEmail
            placeholderText: "Email"
            style: TextFieldStyle {
                textColor: "white"
                background: Rectangle {
                    implicitWidth: parent.width
                    implicitHeight: parent.height
                    color: "transparent"
                }
            }

            Keys.onReturnPressed: {
                console.log("UI State: " + proxyPics.currentState + " Email:" + proxyPics.userEmail)
                proxyPics.userId = "user_" + proxyPics.userEmail
                passcode.forceActiveFocus()
            }

            onTextChanged: {
                hasLoginDetails()
            }
        }
    }

    Image {
        id: create_account
        source: createAccountMA.pressed ? "qrc:/images/loginPage/create_account_glow.png" : "qrc:/images/loginPage/create_account.png"
        x: createAccountMA.pressed ? ((parent.width/rootItemWidth)*84) : ((parent.width/rootItemWidth)*192)
        y: createAccountMA.pressed ? ((parent.height/rootItemHeight)*849) : ((parent.height/rootItemHeight)*978)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)

        MouseArea {
            id: createAccountMA
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " create account clicked")
                proxyPics.currentState = "CREATE-ACCOUNT"
            }
            hoverEnabled: true
        }

    }
    Image {
        id: forgot_password
        source: "qrc:/images/loginPage/forgot_password.png"
        x: ((parent.width/rootItemWidth)*224)
        y: ((parent.height/rootItemHeight)*1142)
        opacity: 0.6
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)

        MouseArea {
            id:forgotpasswordbuttonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " create account clicked")
                login.state = "FORGOT PASSWORD"
                proxyPics.currentState = "FORGOT PASSWORD"
            }
            hoverEnabled: true
        }
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

    //Checks for the username and passcode, enables/ disables based on user input's
    function hasLoginDetails() {
        if (email.text !== "") {
            go_button.enabled = true
            go_button.opacity = 1
        } else {
            go_button.enabled = false
            go_button.opacity = 0.3
        }
    }

    //This function is used to enable signup controls based on state
    function enableSignUpControls(enableStatus) {
        go_button.visible = !enableStatus
    }

    //This function is used to clear all text fields
    function clearTextFields() {
        email.text = ""
        passcode.text = ""
    }
}
