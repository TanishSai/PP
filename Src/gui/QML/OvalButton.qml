import QtQuick 2.5


// Copyright (c) Positively Front Street, Inc 2017
// Author Louis Meadows
// Created September 24th 2013 as Button.qml
// OvalButton.qml
// Was renamed to not conflict with Qt Quick 2 controls
// property allows c++ to connect with the modifer and change the value
Item {
    id: button
    width: buttonWidth
    height: buttonHeight
    property string text
    property color buttonColor: "#33EE33"
    property color onHoverColor: "black"
    property color borderColor: "gold"
    property string label
    property real labelSize: 14
    signal clicked

    smooth: true
    Rectangle {
        anchors.fill: parent
        radius: 60
        border {
            width: 3
            color: borderColor
        }

        Text {
            id: buttonLabel
            anchors.centerIn: parent
            text: label
            color: "white"
            font.family: "Centuma"
            font.pointSize: labelSize
        }
        MouseArea {
            id: buttonMouseArea
            anchors.fill: parent

            onClicked: {
                console.log("UI State: " + proxyPics.currentState + " user id:"
                            + proxyPics.userId + " " + buttonLabel.text + " clicked")
                button.clicked()
            }
            hoverEnabled: true
        }
        color: buttonMouseArea.pressed ? Qt.darker(buttonColor,
                                                   1.5) : buttonColor
        scale: buttonMouseArea.pressed ? 1.5 : 1.00
    }
}
