//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import QtQml 2.2
import QtQuick.Extras 1.4

Item {

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

    Text {
        id: success
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: logo.bottom
        color: "white"
        text: "SUCCESS!"
        font.family: "Centuma"
        font.pointSize: 32
    }

    Text {
        id: your_a_member
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: success.bottom
        color: "white"
        text: "You are now a member of ProxyPics"
        font.family: "Centuma"
        font.pointSize: 14
    }

    Rectangle {
        id: line
        height: 15
        width: proxyPics.uiWidth * .8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: your_a_member.bottom
        color: "white"
    }

    Text {
        id: link
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: line.bottom
        color: "white"
        text: "Link to PayPal"
        font.family: "Centuma"
        font.pointSize: 14
    }

    Text {
        id: to_get_paid
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: link.bottom
        color: "white"
        text: "to get paid for photos!"
        font.family: "Centuma"
        font.pointSize: 14
    }
    TextField {
        id: email_for_paypal
        opacity: .69
        anchors.top: to_get_paid.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: topmargin
        width: proxyPics.uiWidth * .8
        height: proxyPics.uiHeight / 11
        //TODO add Q_PROPERTY to retain paypay email
        //       text: proxyPics.userEmail
        font.family: "Centuma"
        Layout.fillWidth: true
        placeholderText: "Email for Paypal Payments"
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
            console.log("UI State: " + proxyPics.currentState + " Email:" + email_for_paypal.text)
            proxyPics.userId = "user_" + email.text
            passcode.forceActiveFocus()
        }

        onTextChanged: {

        }
    }
    TextField {
        id: verify_paypal
        opacity: .69
        anchors.top: email_for_paypal.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: topmargin
        width: proxyPics.uiWidth * .8
        height: proxyPics.uiHeight / 11
        //TODO add Q_PROPERTY to retain paypay email
        //       text: proxyPics.userEmail
        Layout.fillWidth: true
        font.family: "Centuma"
        placeholderText: "Verify Paypal Email"
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
            console.log("UI State: " + proxyPics.currentState + " Email:" + email_for_paypal.text)
            proxyPics.userId = "user_" + email.text
            passcode.forceActiveFocus()
        }

        onTextChanged: {

        }
    }
    CheckBox {
        id: agree_to_terms
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: verify_paypal.bottom
        checked: false

        text: qsTr("Agree to terms")
    }

    Rectangle {
        id: link_to_paypal
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: agree_to_terms.bottom
        radius: 60
        width: proxyPics.uiWidth * .8
        height: proxyPics.uiHeight / 11
        Image {
            width: link_to_paypal.width
            height: link_to_paypal.height
            source: "qrc:/images/PayPal.PNG"
        }
        MouseArea {
            id: link_to_paypalMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState)
                proxyPics.currentState = "Home"
            }
        }
        color: link_to_paypalMouseArea.pressed ? Qt.darker("orange",
                                                           1.5) : "orange"
        scale: link_to_paypalMouseArea.pressed ? 1.5 : 1.00
    }

    Keys.onReleased: {
        console.log(event.key)
        if (event.key === Qt.Key_Back) {
            console.log("UI State: " + proxyPics.currentState
                        + " Hardware Back button captured Home- wunderbar !")
            // fake soft back key press when hardware back key pressed
            proxyPics.currentState = "Login"
            event.accepted = true
        }
    }
}
