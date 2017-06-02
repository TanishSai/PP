//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtQuick.Controls 1.2

Rectangle {
    color: "grey"
    Component.onCompleted: {
        howItWorks.open()
    }

    id: noImageRect
    anchors.centerIn: parent
    Text {
        id: imageStatusText
        anchors.centerIn: parent
        font.family: "Centuma"
        text: "NO IMAGES POSTED"
    }

    BusyIndicator {
        id: busyIndi
        anchors.top: imageStatusText.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        running: true //imageCount > 0 && !anyImageAvailable

        onRunningChanged: {
            if (running)
                busyIndicatorRunning()
        }
    }
}
