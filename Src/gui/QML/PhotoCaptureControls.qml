//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
//
import QtQuick 2.5
import QtMultimedia 5.6
import QtQuick.Window 2.2
FocusScope {
    property Camera camera

    property int buttonsPanelWidth: buttonPaneShadow.width

    signal previewSelected
    signal videoModeSelected
    id : captureControls

    Rectangle {
        id: buttonPaneShadow
        width: bottomColumn.width + 16
        height: parent.height
        anchors.top: parent.top
        anchors.right: parent.right
        color: Qt.rgba(0.08, 0.08, 0.08, 1)
        radius: 30
        Column {
            anchors {
                right: parent.right
                top: parent.top
                margins: 8
            }

            id: buttonsColumn
            spacing: 8

            FocusButton {
                camera: captureControls.camera
                visible: camera.cameraStatus == Camera.ActiveStatus && camera.focus.isFocusModeSupported(Camera.FocusAuto)
            }

            CameraButton {
                text: "Capture"
                visible: camera.imageCapture.ready
                onClicked: camera.imageCapture.capture()
            }

            CameraPropertyButton {
                id : wbModesButton
                value: CameraImageProcessing.WhiteBalanceAuto
                model: ListModel {
                    ListElement {
                        icon: "../Images/camera_auto_mode.png"
                        value: CameraImageProcessing.WhiteBalanceAuto
                        text: "Auto"
                    }
                    ListElement {
                        icon: "../Images/camera_white_balance_sunny.png"
                        value: CameraImageProcessing.WhiteBalanceSunlight
                        text: "Sunlight"
                    }
                    ListElement {
                        icon: "../Images/camera_white_balance_cloudy.png"
                        value: CameraImageProcessing.WhiteBalanceCloudy
                        text: "Cloudy"
                    }
                    ListElement {
                        icon: "../Images/camera_white_balance_incandescent.png"
                        value: CameraImageProcessing.WhiteBalanceTungsten
                        text: "Tungsten"
                    }
                    ListElement {
                        icon: "../Images/camera_white_balance_flourescent.png"
                        value: CameraImageProcessing.WhiteBalanceFluorescent
                        text: "Fluorescent"
                    }
                }
                onValueChanged: captureControls.camera.imageProcessing.whiteBalanceMode = wbModesButton.value
            }

            CameraButton {
                text: "View"
                onClicked: captureControls.previewSelected()
                visible: proxyPics.previewAvailable
            }
        }

        Column {
            anchors {
                bottom: parent.bottom
                right: parent.right
                margins: 8
            }

            id: bottomColumn
            spacing: 8

            CameraListButton {
                model: QtMultimedia.availableCameras
                onValueChanged: captureControls.camera.deviceId = value
            }


            CameraButton {
                id: quitButton
                text: "Quit"
                onClicked: Qt.quit()
            }
        }


    }


    ZoomControl {
        x : 0
        y : 0
        width : 100
        height: parent.height

        currentZoom: camera.digitalZoom
        maximumZoom: Math.min(4.0, camera.maximumDigitalZoom)
        onZoomTo: camera.setDigitalZoom(value)
    }
}
