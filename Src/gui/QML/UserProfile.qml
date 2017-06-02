//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Rectangle {
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
                text: qsTr("User Info")
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
    Image {
        id: user_picture
        anchors.top: screentitle.bottom
        source: "qrc:/images/User-icon.png"
        fillMode: Image.PreserveAspectFit
        width: proxyPics.uiHeight / 15
        height: proxyPics.uiHeight / 15
    }
    Rectangle {
        id: user_data
        anchors.top: user_picture.bottom
        border.color: "grey"
        border.width: 3

        ColumnLayout {
            id: user_info
            anchors.top: user_picture.bottom
            Text {
                id: user_name
                width: parent.width
                height: proxyPics.uiHeight / 15
                verticalAlignment: Text.AlignHCenter
                font.family: "Centuma"
                text: proxyPics.firstName + " " + proxyPics.lastName
            }
            Text {
                id: user_city
                width: parent.width
                height: proxyPics.uiHeight / 15
                verticalAlignment: Text.AlignHCenter
                font.family: "Centuma"
                text: proxyPics.city
            }
            Text {
                id: member_since
                width: parent.width
                height: proxyPics.uiHeight / 15
                verticalAlignment: Text.AlignHCenter
                font.family: "Centuma"
                text: "Member since 2016"
            }
        }
    }

    Image {
        id: user_star
        anchors.top: user_data.bottom
        source: "qrc:/images/star.png"
        fillMode: Image.PreserveAspectFit
        width: proxyPics.uiHeight / 15
        height: proxyPics.uiHeight / 15
    }
    Text {
        id: user_rating
        anchors.top: user_data.bottom
        anchors.left: user_star.right
        font.family: "Centuma"
        text: "4.8(84)"
    }
    Rectangle {
        id: user_bio
        anchors.top: user_star.bottom
        width: parent.width
        height: proxyPics.uiHeight / 7
        border.color: "grey"
        border.width: 3
        Text {
            id: member_bio
            width: parent.width
            height: proxyPics.uiHeight / 7
            verticalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            font.family: "Centuma"
            text: "I'm a photographer and graphic designer based in Wasington, DC. I love ramen and cats!"
        }
    }
    Rectangle {
        id: user_stats
        anchors.top: user_bio.bottom
        Text {
            font.family: "Centuma"
        }
        Text {
            font.family: "Centuma"
        }
        Text {
            font.family: "Centuma"
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
