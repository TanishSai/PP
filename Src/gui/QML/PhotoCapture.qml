//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtMultimedia 5.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

Item {
    anchors.fill: parent
    property real currentZoom: 1
    property real maximumZoom: 1
    property string focusLabel: "focus"
    property bool cameraActive
    onCameraActiveChanged: {
        console.log("UI State:" + proxyPics.currentState + "Camera Active Changed:" + cameraActive)
        console.log(QtMultimedia.defaultCamera.deviceId)
        console.log(QtMultimedia.defaultCamera.displayName)
        console.log(QtMultimedia.defaultCamera.position)
        console.log(QtMultimedia.defaultCamera.orientation)
    }
    property bool directCameraView: false

    onDirectCameraViewChanged: {
        console.log("UI State:" + proxyPics.currentState
                    + "Direct Camera View Changed:" + directCameraView)
    }
    Component.onCompleted: {
//        camera.focus = Camera.FocusPointFaceDetection
        camera.captureMode = Camera.CaptureStillImage
        camera.start()
        cameraActive = camera.cameraStatus == Camera.ActiveStatus
                && camera.focus.isFocusModeSupported(Camera.FocusAuto)
        console.log("UI State: " + proxyPics.currentState + " Camera Active...")
        console.log(QtMultimedia.defaultCamera.deviceId)
        console.log(QtMultimedia.defaultCamera.displayName)
        console.log(QtMultimedia.defaultCamera.position)
        console.log(QtMultimedia.defaultCamera.orientation)
        console.log(camera.cameraState)
        console.log(camera.cameraStatus)
        console.log(camera.maximumDigitalZoom)
    }

    id: cameraUI
    state: "PhotoCapture"
    states: [
        State {
            name: "Inactive"
        },

        State {
            name: "PhotoCapture"
            StateChangeScript {
                script: {
                    camera.captureMode = Camera.CaptureStillImage
                    camera.start()
                }
            }
            PropertyChanges {
                target: viewfinder
                source: camera
            }
        },
        State {
            name: "PhotoPreview"
        }
    ]
    ToolBar {
        id: screentitle
        RowLayout {
            anchors.fill: parent
            ToolButton {
                iconSource: "qrc:/images/X-exit-icon.png"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState + " back pressed")
                    proxyPics.currentState = "Photo Request"
                }
            }
            Text {
                id: photoaddress
                text: QtMultimedia.defaultCamera.displayName
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

    Camera {
        id: camera
        captureMode: Camera.CaptureStillImage
        property bool captureAndUpload: false

        onLockStatusChanged: {
            if (camera.lockStatus == Camera.Unlocked)
                focusLabel = "Focus"
            else if (camera.lockStatus == Camera.Searching)
                focusLabel = "Focusing"
            else
                focusLabel = "Unlock"
            cameraActive = camera.cameraStatus == Camera.ActiveStatus
                    && camera.focus.isFocusModeSupported(Camera.FocusAuto)
        }

        imageCapture {
            onImageCaptured: {
//                proxyPics.currentState = "Photo Preview"
//                proxyPics.previewAvailable = true
//                cameraUI.state = "PhotoPreview"
            }
            onImageSaved: {
                proxyPics.savedpath = proxyPics.saveCapturePixLocally(path)
                console.log("UI State:" + proxyPics.currentState
                            + " image save " + proxyPics.savedpath
                            + proxyPics.assignmentCreatorUserid + " " + proxyPics.currentAssignmentId)

                if (camera.captureAndUpload)
                {
                    // send picture to server
                    console.log("UI State: " + proxyPics.currentState
                                + " ** " + proxyPics.savedpath
                                + " ** " + proxyPics.postImageName)

                    // this will upload the image to server
                    proxyPics.setPostImageName(proxyPics.savedpath, true)
                    proxyPics.currentState = "Photos Uploading"
                    proxyPics.postImageName = ""
                    camera.captureAndUpload = false
                }
            }
        }
    }
    VideoOutput {
        id: viewfinder
        anchors.fill: parent
        source: camera
        orientation: 270
    }

    FocusScope {
        property Camera camera

        property int buttonsPanelheight: buttonPaneShadow.height

        signal previewSelected
        signal videoModeSelected
        id: captureControls
        onPreviewSelected: {
            console.log("UI State:" + proxyPics.currentState + " -----------------" + proxyPics.imageCount)
            cameraUI.state = "PhotoPreview"
        }
        anchors.fill: parent
        ToolBar {
            id: buttonRow
            anchors.bottom: parent.bottom
            RowLayout {
                anchors.fill: parent
                BackButton {
                    id: backButton
                    onClicked: {
                        console.log("UI State:" + proxyPics.currentState + " leaving camera")
                        proxyPics.currentState = "Current Assignment"
                    }
                }
                PhotoCaptureButton{
                      onClicked: {
                        console.log("UI State: "+proxyPics.currentState)
                        proxyPics.currentState = "Photo Capture"
                        console.log("UI State: " + proxyPics.currentState + " Photo taken")
                        camera.imageCapture.capture()
                    }
                }
                UploadButton {
                    onClicked: {
                        camera.captureAndUpload = true
                        // capture and have a local copy
                        camera.imageCapture.capture()
                    }
                }
                PhotoGalleryButton {
                    onClicked: {
                        proxyPics.currentState = "Photo Preview"
                        captureControls.previewSelected()
                    }
                }
                FocusButton {
                    property Camera camera: captureControls.camera
                    onClicked: {
                        if (camera.lockStatus == Camera.Unlocked)
                            camera.searchAndLock()
                        else
                            camera.unlock()
                    }
                }
            }
        }
    }

    ZoomControl {
        width: 300
        height: parent.height / 3
        onZoomTo: {
            currentZoom = camera.digitalZoom
            maximumZoom = Math.min(4.0, camera.maximumDigitalZoom)
            camera.setDigitalZoom(value)
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
