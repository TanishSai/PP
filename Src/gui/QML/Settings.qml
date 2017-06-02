//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4

Rectangle {
    Component.onCompleted: {
        if (proxyPics.firstName !== "")
            first_name_text.text = proxyPics.firstName
        else
            first_name_text.text = "first name"

        if (proxyPics.lastName !== "")
            last_name_text.text = proxyPics.lastName
        else
            last_name_text.text = "last name"
    }

    ToolBar {
        id: screentitle
        RowLayout {
            anchors.fill: parent
            ToolButton {
                iconSource: "qrc:/images/X-exit-icon.png"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState + " back pressed")
                    proxyPics.currentState = "Menu"
                }
            }
            Text {
                id: photoaddress
                text: qsTr("Settings")
                font.family: "Centuma"
                font.pointSize: 16
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

    Rectangle {
        id: user_name
        width: parent.width
        height: parent.height / 11
        anchors.top: screentitle.bottom

        Rectangle {
            anchors.left: user_name.right
            width: proxyPics.uiWidth
            RowLayout {

                TextField {
                    id: first_name_text
                    focus: true
                    Layout.fillWidth: true
                    font.family: "Centuma"

                    placeholderText: "first name"
                    style: TextFieldStyle {
                        textColor: "black"
                        background: Rectangle {
                            radius: 2
                            border.color: "grey"
                            border.width: 1
                            implicitWidth: proxyPics.uiWidth
                            implicitHeight: proxyPics.uiHeight / 22
                        }
                    }
                    Keys.onReturnPressed: {
                        console.log("Return pressed: " + text)
                        focus = false
                        proxyPics.firstName = first_name_text.text
                    }
                }
                TextField {
                    id: last_name_text
                    focus: true
                    Layout.fillWidth: true
                    font.family: "Centuma"

                    placeholderText: "last name"
                    style: TextFieldStyle {
                        textColor: "black"
                        background: Rectangle {
                            radius: 2
                            border.color: "grey"
                            border.width: 1
                            implicitWidth: proxyPics.uiWidth
                            implicitHeight: proxyPics.uiHeight / 22
                        }
                    }
                    Keys.onReturnPressed: {
                        console.log("Return pressed: " + text)
                        focus = false
                        proxyPics.lastName = last_name_text.text
                    }
                }
            }
        }
        Image {
            anchors.right: parent.right
            source: "qrc:/images/icon-go.png"
            fillMode: Image.PreserveAspectFit
            width: proxyPics.uiHeight / 20
            height: proxyPics.uiHeight / 20
        }
        MouseArea {
            id: user_name_ButtonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
            }
        }
        color: user_name_ButtonMouseArea.pressed ? Qt.darker("white",
                                                             1.5) : "white"
        scale: user_name_ButtonMouseArea.pressed ? 1.0 : 1.00
    }

    Rectangle {
        id: user_email
        width: parent.width
        height: parent.height / 11
        anchors.top: user_name.bottom

        TextField {
            id: user_email_text
            focus: true
            Layout.fillWidth: true
            font.family: "Centuma"

            placeholderText: "email address"
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 2
                    border.color: "grey"
                    border.width: 1
                    implicitWidth: proxyPics.uiWidth
                    implicitHeight: proxyPics.uiHeight / 22
                }
            }
            Keys.onReturnPressed: {
                console.log("Return pressed: " + text)
                focus = false
                proxyPics.lastName = last_name_text.text
            }
        }
        Image {
            anchors.right: parent.right
            source: "qrc:/images/icon-go.png"
            fillMode: Image.PreserveAspectFit
            width: proxyPics.uiHeight / 20
            height: proxyPics.uiHeight / 20
        }
    }

    Rectangle {
        id: user_phone_number
        width: parent.width
        height: parent.height / 11
        anchors.top: user_email.bottom

        TextField {
            id: phone_number_text
            focus: true
            Layout.fillWidth: true
            font.family: "Centuma"

            placeholderText: "phone number"
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 2
                    border.color: "grey"
                    border.width: 1
                    implicitWidth: proxyPics.uiWidth
                    implicitHeight: proxyPics.uiHeight / 22
                }
            }
            Keys.onReturnPressed: {
                console.log("Return pressed: " + text)
                focus = false
                proxyPics.lastName = last_name_text.text
            }
        }
        Image {
            anchors.right: parent.right
            source: "qrc:/images/icon-go.png"
            fillMode: Image.PreserveAspectFit
            width: proxyPics.uiHeight / 20
            height: proxyPics.uiHeight / 20
        }
        MouseArea {
            id: user_phone_number_ButtonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
            }
        }
        color: user_phone_number_ButtonMouseArea.pressed ? Qt.darker(
                                                               "white",
                                                               1.5) : "white"
        scale: user_phone_number_ButtonMouseArea.pressed ? 1.0 : 1.00
    }
    Rectangle {
        id: social_networks
        width: parent.width
        height: parent.height / 11
        anchors.top: user_phone_number.bottom
        Text {
            width: parent.width
            height: proxyPics.uiHeight / 11
            verticalAlignment: Text.AlignHCenter
            font.family: "Centuma"
            text: "Social Networks"
        }
        TextField {
            id: social_networks_text
            focus: true
            Layout.fillWidth: true
            font.family: "Centuma"
            placeholderText: "Social Networks"
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 2
                    border.color: "grey"
                    border.width: 1
                    implicitWidth: proxyPics.uiWidth
                    implicitHeight: proxyPics.uiHeight / 22
                }
            }
            Keys.onReturnPressed: {
                console.log("Return pressed: " + text)
                focus = false
                proxyPics.lastName = last_name_text.text
            }
        }
        Image {
            anchors.right: parent.right
            source: "qrc:/images/icon-go.png"
            fillMode: Image.PreserveAspectFit
            width: proxyPics.uiHeight / 20
            height: proxyPics.uiHeight / 20
        }
        MouseArea {
            id: social_networks_ButtonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
                proxyPics.currentState = "Social Networks"
            }
        }
        color: social_networks_ButtonMouseArea.pressed ? Qt.darker(
                                                             "white",
                                                             1.5) : "white"
        scale: social_networks_ButtonMouseArea.pressed ? 1.0 : 1.00
    }
    Rectangle {
        id: notifications
        width: parent.width
        height: parent.height / 11
        anchors.top: social_networks.bottom
        Text {
            width: parent.width
            height: proxyPics.uiHeight / 11
            verticalAlignment: Text.AlignHCenter
            font.family: "Centuma"
            text: "Notifications"
        }
        TextField {
            id: notifications_text
            focus: true
            Layout.fillWidth: true
            font.family: "Centuma"
            placeholderText: "notifications"
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 2
                    border.color: "grey"
                    border.width: 1
                    implicitWidth: proxyPics.uiWidth
                    implicitHeight: proxyPics.uiHeight / 22
                }
            }
            Keys.onReturnPressed: {
                console.log("Return pressed: " + text)
                focus = false
                proxyPics.lastName = last_name_text.text
            }
        }
        Image {
            anchors.right: parent.right
            source: "qrc:/images/icon-go.png"
            fillMode: Image.PreserveAspectFit
            width: proxyPics.uiHeight / 20
            height: proxyPics.uiHeight / 20
        }
        MouseArea {
            id: notifications_ButtonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
                proxyPics.currentState = "Notifications"
            }
        }
        color: notifications_ButtonMouseArea.pressed ? Qt.darker("white",
                                                                 1.5) : "white"
        scale: notifications_ButtonMouseArea.pressed ? 1.0 : 1.00
    }
    Rectangle {
        id: location_settings
        width: parent.width
        height: parent.height / 11
        anchors.top: notifications.bottom
        Text {
            width: parent.width
            height: proxyPics.uiHeight / 11
            verticalAlignment: Text.AlignHCenter
            font.family: "Centuma"
            text: "Location Settings"
        }
        TextField {
            id: location_settings_text
            focus: true
            Layout.fillWidth: true
            font.family: "Centuma"
            placeholderText: "Location Settings"
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 2
                    border.color: "grey"
                    border.width: 1
                    implicitWidth: proxyPics.uiWidth
                    implicitHeight: proxyPics.uiHeight / 22
                }
            }
            Keys.onReturnPressed: {
                console.log("Return pressed: " + text)
                focus = false
                proxyPics.lastName = last_name_text.text
            }
        }
        Image {
            anchors.right: parent.right
            source: "qrc:/images/icon-go.png"
            fillMode: Image.PreserveAspectFit
            width: proxyPics.uiHeight / 20
            height: proxyPics.uiHeight / 20
        }
        MouseArea {
            id: location_settings_ButtonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
                proxyPics.currentState = "Location Settings"
            }
        }
        color: location_settings_ButtonMouseArea.pressed ? Qt.darker(
                                                               "white",
                                                               1.5) : "white"
        scale: location_settings_ButtonMouseArea.pressed ? 1.0 : 1.00
    }
    Rectangle {
        id: change_password
        width: parent.width
        height: parent.height / 11
        anchors.top: location_settings.bottom
        Text {
            width: parent.width
            height: proxyPics.uiHeight / 11
            verticalAlignment: Text.AlignHCenter
            font.family: "Centuma"
            text: "Change your Password"
        }
        TextField {
            id: change_password_text
            focus: true
            Layout.fillWidth: true
            font.family: "Centuma"

            placeholderText: "new password"
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 2
                    border.color: "grey"
                    border.width: 1
                    implicitWidth: proxyPics.uiWidth
                    implicitHeight: proxyPics.uiHeight / 22
                }
            }
            Keys.onReturnPressed: {
                console.log("Return pressed: " + text)
                focus = false
                proxyPics.lastName = last_name_text.text
            }
        }
        Image {
            anchors.right: parent.right
            source: "qrc:/images/icon-go.png"
            fillMode: Image.PreserveAspectFit
            width: proxyPics.uiHeight / 20
            height: proxyPics.uiHeight / 20
        }
        MouseArea {
            id: change_password_ButtonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
            }
        }
        color: change_password_ButtonMouseArea.pressed ? Qt.darker(
                                                             "white",
                                                             1.5) : "white"
        scale: change_password_ButtonMouseArea.pressed ? 1.0 : 1.00
    }
    Rectangle {
        id: payment_settings
        width: parent.width
        height: parent.height / 11
        anchors.top: change_password.bottom
        RowLayout {
            CheckBox {
                width: parent.width / 2
                height: proxyPics.uiHeight / 11
                text: "Paypal"
            }
            CheckBox {

                width: parent.width / 2
                height: proxyPics.uiHeight / 11
                text: "Credit Card (SquareUp)"
            }
        }
        Image {
            anchors.right: parent.right
            source: "qrc:/images/icon-go.png"
            fillMode: Image.PreserveAspectFit
            width: proxyPics.uiHeight / 20
            height: proxyPics.uiHeight / 20
        }
    }

    focus: true

    Keys.onReleased: {
        console.log(event.key)
        if (event.key === Qt.Key_Back) {
            console.log("UI State:" + proxyPics.currentState
                        + "Hardware Back button captured Photo Assignment - wunderbar !")
            // fake soft back key press when hardware back key pressed
            proxyPics.currentState = "Home"
            event.accepted = true
        }
    }
}
