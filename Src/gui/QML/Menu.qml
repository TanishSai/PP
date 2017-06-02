//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.2
import QtQuick.Dialogs 1.2

Rectangle {
    width: proxyPics.uiWidth
    height: proxyPics.uiHeight
    Rectangle {
        id: logo
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight / 8
        color: "green"
        Image {
            width: proxyPics.uiHeight / 11
            height: proxyPics.uiHeight / 11
            anchors.centerIn: parent
            source: "qrc:/images/ProxyPics-logo-01.png"
            fillMode: Image.PreserveAspectFit
        }
    }
    Rectangle {
        id: home
        anchors.top: logo.bottom
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight / 11
        border.width: 3
        border.color: "grey"

        RowLayout {

            anchors.fill: parent
            spacing: 6
            Image {

                Layout.preferredWidth: proxyPics.uiHeight / 16
                Layout.preferredHeight: proxyPics.uiHeight / 16
                fillMode: Image.PreserveAspectFit
                width: proxyPics.uiHeight / 16
                height: proxyPics.uiHeight / 16

                id: home_image
                source: "qrc:/images/HOME ICON-for main menu.png"
            }
            Text {
                anchors.left: home_image.right
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.family: "Centuma"
                font.pointSize: 32
                text: "Home"
        }

    }
        MouseArea {
            id: home_ButtonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
                proxyPics.currentState = "Home"
            }
        }
        color: home_ButtonMouseArea.pressed ? Qt.darker("white", 1.5) : "white"
        scale: home_ButtonMouseArea.pressed ? 1.0 : 1.00
    }
    Rectangle {
        id: my_assignments
        anchors.top: home.bottom
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight / 11
        border.width: 3
        border.color: "grey"

        RowLayout {

            anchors.fill: parent
            spacing: 6
            Image {
                Layout.preferredWidth: (proxyPics.uiHeight / 16)
                Layout.preferredHeight: proxyPics.uiHeight / 16
                fillMode: Image.PreserveAspectFit
                width: (proxyPics.uiHeight / 16)
                height: proxyPics.uiHeight / 16
                id: my_assignments_image

                source: "qrc:/images/ASSIGNMENTS ICON-main menu.png"
            }
            Text {

                anchors.left: my_assignments_image.right
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "My Assignments"
                font.family: "Centuma"
                font.pointSize: 32
            }
        }
        MouseArea {
            id: my_assignments_ButtonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
                proxyPics.currentState = "My Assignments"
            }
        }
        color: my_assignments_ButtonMouseArea.pressed ? Qt.darker("white",
                                                                  1.5) : "white"
        scale: my_assignments_ButtonMouseArea.pressed ? 1.0 : 1.00
    }
    Rectangle {
        id: my_requests
        anchors.top: my_assignments.bottom
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight / 11
        border.width: 3
        border.color: "grey"
        RowLayout {

            anchors.fill: parent
            spacing: 6
            Image {

                Layout.preferredWidth: proxyPics.uiHeight / 16
                Layout.preferredHeight: proxyPics.uiHeight / 16
                fillMode: Image.PreserveAspectFit
                width: proxyPics.uiHeight / 16
                height: proxyPics.uiHeight / 16
                id: my_requests_image

                source: "qrc:/images/REQUESTS ICON-main menu.png"
            }
            Text {
                anchors.left: my_requests_image.right

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "My Requests"
                font.family: "Centuma"
                font.pointSize: 32
            }
        }
        MouseArea {
            id: my_requests_ButtonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
                proxyPics.currentState = "My Requests"
            }
        }
        color: my_requests_ButtonMouseArea.pressed ? Qt.darker("white",
                                                               1.5) : "white"
        scale: my_requests_ButtonMouseArea.pressed ? 1.0 : 1.00
    }

    Rectangle {
        id: help_button
        anchors.top: my_requests.bottom
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight / 11
        border.width: 3
        border.color: "grey"

        RowLayout {

            anchors.fill: parent
            spacing: 6
            Image {

                Layout.preferredWidth: proxyPics.uiHeight / 16
                Layout.preferredHeight: proxyPics.uiHeight / 16
                fillMode: Image.PreserveAspectFit
                width: proxyPics.uiHeight / 16
                height: proxyPics.uiHeight / 16
                id: help_button_image

                source: "qrc:/images/HELP ICON-main menu.png"
            }
            Text {
                anchors.left: help_button_image.right

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "How it works"
                font.family: "Centuma"
                font.pointSize: 32
            }
        }
        MouseArea {
            id: help_ButtonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
                proxyPics.currentState = "How it Works"
            }
        }
        color: help_ButtonMouseArea.pressed ? Qt.darker("white", 1.5) : "white"
        scale: help_ButtonMouseArea.pressed ? 1.0 : 1.00
    }

    Rectangle {
        id: terms_button
        anchors.top: help_button.bottom
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight / 11
        border.width: 3
        border.color: "grey"
        RowLayout {

            anchors.fill: parent
            spacing: 6
            Image {

                Layout.preferredWidth: (proxyPics.uiHeight / 16) + 6
                Layout.preferredHeight: proxyPics.uiHeight / 16
                fillMode: Image.PreserveAspectFit
                width: (proxyPics.uiHeight / 16) + 6
                height: proxyPics.uiHeight / 16
                id: terms_button_image

                source: "qrc:/images/TERMS ICON-main menu.png"
            }
            Text {
                anchors.left: terms_button_image.right

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "Terms"
                font.family: "Centuma"
                font.pointSize: 32
            }
        }
        MouseArea {
            id: terms_ButtonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
                proxyPics.currentState = "Terms"
            }
        }
        color: terms_ButtonMouseArea.pressed ? Qt.darker("white", 1.5) : "white"
        scale: terms_ButtonMouseArea.pressed ? 1.0 : 1.00
    }
    Rectangle {
        id: camera_button
        anchors.top: terms_button.bottom
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight / 11
        border.width: 3
        border.color: "grey"

        RowLayout {

            anchors.fill: parent
            spacing: 6
            Image {

                Layout.preferredWidth: (proxyPics.uiHeight / 16) + 6
                Layout.preferredHeight: proxyPics.uiHeight / 16
                fillMode: Image.PreserveAspectFit
                width: (proxyPics.uiHeight / 16) + 6
                height: proxyPics.uiHeight / 16

                id: camera_button_image

                source: "qrc:/images/square.png"
            }
            Text {
                anchors.left: camera_button_image.right

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "Credit Card Payment"
                font.family: "Centuma"
                font.pointSize: 32
            }
        }
        MouseArea {
            id: camera_ButtonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
                proxyPics.currentState = "Credit Card"
            }
        }
        color: camera_ButtonMouseArea.pressed ? Qt.darker("white",
                                                          1.5) : "white"
        scale: camera_ButtonMouseArea.pressed ? 1.0 : 1.00
    }
    Rectangle {
        id: profile_button
        anchors.top: camera_button.bottom
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight / 11
        border.width: 3
        border.color: "grey"
        RowLayout {
            anchors.fill: parent
            spacing: 6
            Image {
                Layout.preferredWidth: (proxyPics.uiHeight / 16) + 2
                Layout.preferredHeight: proxyPics.uiHeight / 16
                fillMode: Image.PreserveAspectFit
                width: (proxyPics.uiHeight / 16) + 2
                height: proxyPics.uiHeight / 16
                id: profile_button_image
                source: "qrc:/images/PayPal.PNG"
            }
            Text {
                anchors.left: profile_button_image.right

                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "Make PayPal Payment"
                font.family: "Centuma"
                font.pointSize: 32
            }
        }
        MouseArea {
            id: profile_ButtonMouseArea
            anchors.fill: parent
            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
                proxyPics.currentState = "Photo Purchase"
            }
        }
        color: profile_ButtonMouseArea.pressed ? Qt.darker("white",
                                                           1.5) : "white"
        scale: profile_ButtonMouseArea.pressed ? 1.0 : 1.00
    }
    Rectangle {
        id: settings_button
        anchors.top: profile_button.bottom
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight / 11
        border.width: 3
        border.color: "grey"
        RowLayout {
            anchors.fill: parent
            spacing: 6
            Image {
                Layout.preferredWidth: (proxyPics.uiHeight / 16) + 3
                Layout.preferredHeight: proxyPics.uiHeight / 16
                fillMode: Image.PreserveAspectFit
                width: (proxyPics.uiHeight / 16) + 3
                height: proxyPics.uiHeight / 16
                id: settings_button_image
                source: "qrc:/images/settings.png"
            }
            Text {
                anchors.left: settings_button_image.right
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "Settings"
                font.family: "Centuma"
                font.pointSize: 32
            }
        }

        MouseArea {
            id: settings_ButtonMouseArea
            anchors.fill: parent
            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
                proxyPics.currentState = "Settings"
            }
        }
        color: settings_ButtonMouseArea.pressed ? Qt.darker("white",
                                                            1.5) : "white"
        scale: settings_ButtonMouseArea.pressed ? 1.0 : 1.00
    }

    Rectangle {
        id: logout_button
        anchors.top: settings_button.bottom
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight / 11
        border.width: 3
        border.color: "grey"
        RowLayout {
            anchors.fill: parent
            spacing: 6
            Image {
                id: logout_button_image
                Layout.preferredWidth: (proxyPics.uiHeight / 16) + 3
                Layout.preferredHeight: proxyPics.uiHeight / 16
                fillMode: Image.PreserveAspectFit
                width: (proxyPics.uiHeight / 16) + 3
                height: proxyPics.uiHeight / 16
                source: "qrc:/images/logout-icon.png"
            }
            Text {
                anchors.left: logout_button_image.right
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "Logout"
                font.family: "Centuma"
                font.pointSize: 32
            }
        }

        MouseArea {
            id: logout_ButtonMouseArea
            anchors.fill: parent
            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " ")
                proxyPics.currentState = "Login"
                proxyPics.userId = ""
                proxyPics.userPassword = ""
                proxyPics.state = "";
                proxyPics.resetOnLogout();
            }
        }
        color: logout_ButtonMouseArea.pressed ? Qt.darker("white",
                                                          1.5) : "white"
        scale: logout_ButtonMouseArea.pressed ? 1.0 : 1.00
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
