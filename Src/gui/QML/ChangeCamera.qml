//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtMultimedia 5.5

// TODO store store current deviceId in proxyPics.deviceId
Item {
    ListView {
        anchors.fill: parent

        model: QtMultimedia.availableCameras
        delegate: Text {
            font.family: "Centuma"
            text: modelData.deviceId

            MouseArea {
                anchors.fill: parent
                onClicked: camera.deviceId = modelData.deviceId
            }
        }
    }
}
