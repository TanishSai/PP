//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import QtQuick.Extras 1.4
import QtQuick.LocalStorage 2.0

Rectangle {
    color: "white"

    Component.onCompleted: {
        console.log("UI State:" + proxyPics.currentState + " user id:"
                    + proxyPics.userId + " New Photo Req QML completed")
        clearTextFields()
    }

    ToolBar {
        id: screentitle
        RowLayout {
            anchors.fill: parent
            ToolButton {
                iconSource: "qrc:/images/X-exit-icon.png"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState + " back pressed")
                    proxyPics.currentState = "Home"
                }
            }
            Text {
                id: photoaddress
                text: qsTr("Post a Job")
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

    Text {
        id: assignment_name_label
        anchors.top: screentitle.bottom
        anchors.topMargin: 10
        text: "Job Title"
        color: "green"
        font.family: "Centuma"
        font.pointSize: 22
    }

    Rectangle {
        id: assignment_name
        anchors.top: assignment_name_label.bottom
        radius: 30
        height: proxyPics.uiHeight / 22
        TextField {
            id: assignment_name_text
            focus: true
            Layout.fillWidth: true
            font.family: "Centuma"

            placeholderText: "This is your headline..."
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
            }
        }
    }

    Text {
        id: category_text
        anchors.top: assignment_name.bottom
        anchors.leftMargin: 0
        anchors.topMargin: 10
        text: "Category"
        color: "green"
        font.family: "Centuma"
        font.pointSize: 22
    }
    //TODO add category QPROPERTY
    ComboBox {
        id: category
        editable: false
        anchors.top: category_text.bottom
        model: ListModel {
            id: model
            ListElement {

                text: "Residential"
                color: "Yellow"
            }
            ListElement {

                text: "Commerical"
                color: "Green"
            }
            ListElement {

                text: "Landmark"
                color: "Brown"
            }
        }
        onAccepted: {
            if (find(currentText) === -1) {
                model.append({
                                 text: editText
                             })
                currentIndex = find(editText)
            }
        }
    }


    Text {
        id: address_text
        anchors.top: category.bottom
        anchors.topMargin: 10
        font.family: "Centuma"
        text: "Address"
        color: "green"
        font.pointSize: 22
    }

    Rectangle {
        id: photo_address
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: address_text.bottom
        radius: 30
        width: parent.width
        height: (proxyPics.uiHeight / 22) * 4
        TextField {
            id: street_address_1
            anchors.horizontalCenter: parent.horizontalCenter
            Layout.fillWidth: true
            font.family: "Centuma"
            text: proxyPics.streetAddress1

            placeholderText: "Subject Address Line 1"
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    implicitWidth: proxyPics.uiWidth
                    implicitHeight: proxyPics.uiHeight / 22
                    border.color: "grey"
                    border.width: 1
                }
            }

            Keys.onReturnPressed: {
                focus = false
                proxyPics.currentAssignmentAddress = text
                console.log(text)
            }
        }

        TextField {
            id: street_address_2
            anchors.top: street_address_1.bottom
            Layout.fillWidth: true
            font.family: "Centuma"
            placeholderText: "Subject Address Line 2"
            text: proxyPics.streetAddress2
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    implicitWidth: proxyPics.uiWidth
                    implicitHeight: proxyPics.uiHeight / 22
                    border.color: "grey"
                    border.width: 1
                }
            }

            Keys.onReturnPressed: {
                focus = false
                proxyPics.currentAssignmentAddress = text
                console.log(text)
                city_textfield.focus = true
            }
        }
        TextField {
            id: city_textfield
            Layout.fillWidth: true
            anchors.top: street_address_2.bottom
            font.family: "Centuma"
            text: proxyPics.city
            placeholderText: "City"
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    implicitWidth: proxyPics.uiWidth
                    implicitHeight: proxyPics.uiHeight / 22
                    border.color: "#333"
                    border.width: 1
                }
            }
            Keys.onReturnPressed: {
                focus = false
            }
        }

        TextField {
            id: state_textfield
            Layout.fillWidth: true
            anchors.top: city_textfield.bottom
            font.family: "Centuma"
            text: proxyPics.state
            placeholderText: "State"
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    implicitWidth: proxyPics.uiWidth
                    implicitHeight: proxyPics.uiHeight / 22
                    border.color: "#333"
                    border.width: 1
                }
            }
            Keys.onReturnPressed: {
                focus = false
            }
        }
    }

    Text {
        id: photo_notes
        anchors.top: photo_address.bottom
        anchors.topMargin: 10
        font.family: "Centuma"
        text: "Job Details"
        color: "green"
        font.pointSize: 22
    }

    Rectangle {
        anchors.top: photo_notes.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        width: proxyPics.uiWidth
        height: (proxyPics.uiHeight / 22) * 4
        TextField {
            id: noteRect
            Layout.fillWidth: true
            font.family: "Centuma"
            text: proxyPics.assignmentNotes
            placeholderText: "Tell them a bit about the job..."
            style: TextFieldStyle {
                textColor: "black"
                background: Rectangle {
                    radius: 2
                    implicitWidth: proxyPics.uiWidth
                    implicitHeight: proxyPics.uiHeight / 22
                    border.color: "#333"
                    border.width: 1
                }
            }

            Keys.onReturnPressed: {
                proxyPics.userAssignmentCount++
                proxyPics.currentAssignmentName = assignment_name_text.text
                proxyPics.streetAddress1 = street_address_1.text
                proxyPics.streetAddress2 = street_address_2.text
                proxyPics.city = city_textfield.text
                proxyPics.state = state_textfield.text
                proxyPics.notes = noteRect.text
                proxyPics.value = value_text.text
                proxyPics.currentAssignmentAddress = proxyPics.streetAddress1+" "
                            + proxyPics.streetAddress2 + " "
                            + proxyPics.city + " "
                            + proxyPics.state

                proxyPics.mapDestination = proxyPics.currentAssignmentAddress
                console.log("UI State:" + proxyPics.currentState + " -- ticket: "
                            + proxyPics.assignmentTicket
                            + proxyPics.assignmentValue
                            + proxyPics.currentAssignmentName
                            + proxyPics.currentAssignmentAddress
                            + "Notes:" + proxyPics.assignmentNotes)

                if (proxyPics.currentAssignmentName !== "") {
                    proxyPics.currentAssignmentId = proxyPics.currentAssignmentName + "-assignment-"
                            + proxyPics.assignmentTicket + "-" + proxyPics.userId
                            
                    // TODO - assigning value is moved to next page (paypal) as it under construction default $5 is assigned
                    proxyPics.assignmentValue = 5
                    if (proxyPics.assignmentValue !== "") {
                        if (proxyPics.streetAddress1 !== "") {
                            if (proxyPics.city !== "") {
                                if (proxyPics.state !== "") {
                                    if (proxyPics.userId === "" || proxyPics.userPassword === "")
                                        proxyPics.currentState = "Invalid Job"
                                    else
                                        proxyPics.currentState = "Request Sent"
                                } else {
                                    proxyPics.currentState = "Invalid Photo Request"
                                }
                            } else {
                                proxyPics.currentState = "Invalid Photo Request"
                            }
                        } else {
                            proxyPics.currentState = "Invalid Photo Request"
                        }
                    } else {
                        proxyPics.currentState = "Invalid Photo Request"
                    }
                }
            }
        }
    }
    OvalButton {
        id: create_Button
        width: proxyPics.uiWidth / 2.5
        height: proxyPics.uiHeight / 11
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 10
        anchors.bottom: parent.bottom
        label: "SUBMIT JOB"
        onClicked: {
            proxyPics.currentAssignmentName = assignment_name_text.text
            proxyPics.streetAddress1 = street_address_1.text
            proxyPics.streetAddress2 = street_address_2.text
            proxyPics.city = city_textfield.text
            proxyPics.state = state_textfield.text
            proxyPics.notes = noteRect.text
            proxyPics.currentAssignmentAddress = proxyPics.streetAddress1+" "
                        + proxyPics.streetAddress2 + " "
                        + proxyPics.city + " "
                        + proxyPics.state
            console.log("UI State:" + proxyPics.currentState + " -- ticket: "
                        + proxyPics.assignmentTicket + proxyPics.assignmentValue
                        + proxyPics.currentAssignmentName + proxyPics.currentAssignmentAddress
                        + "Notes:" + proxyPics.assignmentNotes)

            if (proxyPics.currentAssignmentName !== "") {
                proxyPics.currentAssignmentId = proxyPics.currentAssignmentName
                        + "-assignment-" + proxyPics.assignmentTicket + "-" + proxyPics.userId
                proxyPics.userAssignmentCount++
                proxyPics.setMapDestination = proxyPics.currentAssignmentAddress
                // TODO - assigning value is moved to next page (paypal) as it under construction default $5 is assigned
                proxyPics.assignmentValue = 5
                if (proxyPics.assignmentValue !== "") {
                    if (proxyPics.streetAddress1 !== "") {
                        if (proxyPics.city !== "") {
                            if (proxyPics.state !== "") {
                                if (proxyPics.userId === "" || proxyPics.userPassword === "")
                                    proxyPics.currentState = "Invalid Job"
                                else
                                    proxyPics.currentState = "Request Sent"
                            } else {
                                proxyPics.currentState = "Invalid Photo Request"
                            }
                        } else {
                            proxyPics.currentState = "Invalid Photo Request"
                        }
                    } else {
                        proxyPics.currentState = "Invalid Photo Request"
                    }
                } else {
                    proxyPics.currentState = "Invalid Photo Request"
                }
            } else {
                proxyPics.currentState = "Invalid Photo Request"
            }


        }
    }

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

    //This function is used to clear all text fields
    function clearTextFields() {
        street_address_1.text = ""
        street_address_2.text = ""
        city_textfield.text = ""
        state_textfield.text = ""
    }
}
