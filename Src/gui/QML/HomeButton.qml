//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
//
import QtQuick 2.5

Rectangle {
    id: home_Button
    anchors.bottomMargin: 1
    anchors.bottom: parent.bottom
    width: proxyPics.uiWidth/7
    height: proxyPics.uiWidth/7
    property string text
    property color buttonColor: "#33EE33"
    property color onHoverColor: "black"
    property color borderColor: "gold"
    signal clicked

    smooth: true
    radius: 90
    border {
        width: 3
        color: borderColor
    }
    Image {
        id: home_Buttonimage
        anchors.centerIn: parent
        width: proxyPics.uiWidth/9
        height: proxyPics.uiWidth/9
        fillMode: Image.Stretch
        source: "qrc:/images/House Icon.png"

    }


    MouseArea {
        id: home_ButtonMouseArea
        anchors.fill: parent

        onClicked:{
              home_Button.clicked()
        }

    }
    color: home_ButtonMouseArea.pressed ? Qt.darker(buttonColor, 1.5) : buttonColor
    scale: home_ButtonMouseArea.pressed ? 1.5 : 1.00
}
