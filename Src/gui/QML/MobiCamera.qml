import QtQuick 2.5
import QtMultimedia 5.5
import QtQuick.Window 2.2

import QtQml 2.2

Item {
    id: camera_UI
    objectName: "RootElement"
    width: Screen.width
    height: Screen.height
    onVisibleChanged: {

            if (visible) {
                state = "PhotoCapture"
                cameraUI.state = "PhotoCapture"
                console.log("Camera Active...")
            } else {
                state = "Inactive"
                cameraUI.state = "Inactive"
                console.log("Camera Inactive...")
            }

    }

    FocusScope {
        property Camera camera

        property int buttonsPanelheight: buttonPaneShadow.height

        signal previewSelected
        signal videoModeSelected
        id: captureControls
        visible: cameraUI.state === "PhotoCapture"
        onPreviewSelected: {
            console.log("-----------------")
            cameraUI.state = "PhotoPreview"
        }
        anchors.fill: parent

        Rectangle {
            id: buttonPaneShadow
            anchors.bottom: parent.bottom
            height: bannerHeight
            width: parent.width
            color: "transparent"

            Row {
                id: buttonRow
                anchors.bottom: parent.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.margins: 8
                anchors.bottomMargin: 2
                OvalButton {
                    width: visible ? btnWidth : 0
                    height: bannerHeight
                    label: "Capture"
                    visible: camera.imageCapture.ready
                    onClicked: {
                        console.log("_______________________________________________")
                        camera.imageCapture.capture()
                    }
                }
                OvalButton {
                    width: visible ? btnWidth : 0
                    height: bannerHeight
                    label: "View"
                    onClicked: captureControls.previewSelected()
                    visible: previewAvailable
                }
                OvalButton {
                    property Camera camera: captureControls.camera
                    width: visible ? btnWidth : 0
                    height: bannerHeight
                    label: focusLabel
                    visible: cameraActive
                    onClicked: {
                        if (camera.lockStatus == Camera.Unlocked)
                            camera.searchAndLock()
                        else
                            camera.unlock()
                    }
                }

                OvalButton {
                    id: quitButton
                    width: visible ? btnWidth : 0
                    height: bannerHeight
                    label: "Back"
                    onClicked: {
                        console.log("leaving camera")
                        uistate.state = "Current Assignment"
                    }
                }
            }
        }

        ZoomControl {
            x: 0
            y: 0
            width: 100
            height: parent.height

            onZoomTo: {
                currentZoom = camera.digitalZoom
                maximumZoom = Math.min(4.0, camera.maximumDigitalZoom)
                camera.setDigitalZoom(value)
            }
        }
    }

}
