//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.Controls.Styles 1.4


Rectangle {
    color: "#29af34"
    border.color: "black"
    ToolBar {
        id: screentitle
        RowLayout {
            anchors.fill: parent
            ToolButton {
                iconSource: "qrc:/images/X-exit-icon.png"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState + " back pressed")
                    proxyPics.currentState = "Settings"
                }
            }
            Text {
                id: photoaddress
                text: qsTr("Social Networks")
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


}
