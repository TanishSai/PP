//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtPositioning 5.5
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4
import QtWebView 1.1
import QtQuick.Controls 1.4

Item {
    ToolBar {
        id: screentitle
        RowLayout {
            anchors.fill: parent
            ToolButton {
                iconSource: "qrc:/images/X-exit-icon.png"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState + " back pressed")
                    proxyPics.currentState = "My Request"
                }
            }
            Text {
                id: photoaddress
                text: qsTr("Photo Reject")
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
    TextField {
        id: whyreject
        opacity: .69
        anchors.top: screentitle.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: topmargin
        width: proxyPics.uiWidth * .8
        height: proxyPics.uiHeight / 11
        font.family: "Centuma"
        text: proxyPics.userEmail
        Layout.fillWidth: true
        placeholderText: "Explain why you are rejecting Photo, what can Photographer do to fix issue?"
        style: TextFieldStyle {
            textColor: "grey"
            background: Rectangle {
                implicitWidth: proxyPics.uiWidth *.7
                implicitHeight: proxyPics.uiHeight / 12
                radius: 2
                border.color: "grey"
                border.width: 1
            }
        }
        Keys.onReturnPressed: {

        }

        onTextChanged: {

        }
    }
}
