//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtMultimedia 5.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4


Item {
    property alias source: preview.source
    property alias status: statusText.text
    property int currentIndex: 0
    Connections {
        target: proxyPics
        onCurrentAssignmentIdChanged: {
            console.log("UI State:" + proxyPics.currentState
                        + "assignment name has changed" + proxyPics.currentSelectedAssignmentName)
            proxyPics.imageName = imageManager.getImageName(
                        0, proxyPics.currentSelectedAssignmentName)
//            proxyPics.previewSource = "image://imageprovider/" + proxyPics.imageName
            preview.source = "image://imageprovider/" + proxyPics.imageName
            proxyPics.imagetitle = proxyPics.currentSelectedAssignmentName
        }
        onCurrentSelectedAssignmentNameChanged: {
            console.log("UI State:" + proxyPics.currentState
                        + "assignment name has changed" + proxyPics.currentSelectedAssignmentName)
            var id = imageManager.getImageName(0, proxyPics.currentSelectedAssignmentName)
            proxyPics.imageName = id
//            proxyPics.previewSource = "image://imageprovider/" + id
            preview.source = "image://imageprovider/" + id
            proxyPics.imagetitle = proxyPics.currentSelectedAssignmentName
        }

        onImageCountChanged: {
            console.log("UI State:" + proxyPics.currentState + "Image count Changed:" + proxyPics.imageCount)
            if (proxyPics.imageCount > 0) {
                currentIndex = 0
                refrehImage()
            }
            proxyPics.imagetitle = proxyPics.currentSelectedAssignmentName
        }
        onNewPhoto: {
            console.log("UI State: " + proxyPics.currentState
                        + " New Photograph taken " + photograph)
            preview.source = photograph
        }

        onImageNameChanged: {
            console.log("UI State: " + proxyPics.currentState
                        + " Image Name Changed: " + proxyPics.imageName)
        }
    }

    function refrehImage()
    {
        proxyPics.imageName = imageManager.getImageName(
                    currentIndex, proxyPics.currentSelectedAssignmentName)
        console.log("UI State: " + proxyPics.currentState + " --- " + proxyPics.imageName)
        //proxyPics.previewSource = "image://imageprovider/" + proxyPics.imageName
        preview.source = "image://imageprovider/" + proxyPics.imageName
        proxyPics.imagetitle = proxyPics.currentSelectedAssignmentName
    }

    onCurrentIndexChanged: {
        refrehImage()
    }

    onVisibleChanged: {
        if (visible)
        {
            refrehImage()
        }
    }

    signal closed

    Component.onCompleted: {
        focus = true
        proxyPics.imageCount = imageManager.getImageCount(proxyPics.currentAssignmentId)
        console.log("UI State: " + proxyPics.currentState + " *** " + proxyPics.currentAssignmentId)
    }
    ToolBar {
        id: screentitle
        RowLayout {
            anchors.fill: parent
            ToolButton {
                iconSource: "qrc:/images/camera_2x.png"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState)
                    proxyPics.currentState = "Photo Capture"
                }
            }
            Text {
                id: photoaddress
                text: qsTr("Photo Preview")
                font.pointSize: 16
                font.family: "Centuma"
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
        id: preview
        width: parent.width
        height: parent.height
        fillMode: Image.PreserveAspectFit
        smooth: true
    }

    Rectangle {
        id: statusRect
        anchors.bottom: parent.bottom
        height: 30
        width: parent.width
        color: "lightblue"

        Text {
            id: statusText
            anchors.verticalCenter: parent.verticalCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Centuma"
            text: "Pending"
        }
    }

    // Next Button
    OvalButton {
        id: nextButton
        objectName: "nextButton"

        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        visible: proxyPics.imageCount !== 0 && currentIndex <= (proxyPics.imageCount - 2)
        width: parent.height / 11
        height: parent.height / 11
        label: "next"
        onClicked: {
            currentIndex = currentIndex + 1
        }
    }

    // previour Button
    OvalButton {
        id: backbtn
        objectName: "back"
        visible: proxyPics.imageCount !== 0 && currentIndex !== 0
        width: parent.height / 11
        height: parent.height / 11
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        label: "back"
        onClicked: {
            currentIndex = currentIndex - 1
            //previousClicked()
        }
    }

    OvalButton {
        anchors.left: parent.left
        anchors.bottomMargin: 10
        anchors.bottom: parent.bottom
        width: proxyPics.uiWidth / 2.5
        height: proxyPics.uiHeight / 11
        label: "Delete"
        onClicked: {
            console.log("UI State: " + proxyPics.currentState + " Delete")
        }
    }

    UploadButton {
        id: upload_Button
        anchors.right: parent.right
        anchors.bottomMargin: 10
        anchors.bottom: parent.bottom
        height: proxyPics.uiHeight / 11
        width: proxyPics.uiWidth / 2.5

        visible: proxyPics.isPixUploadedToServer(proxyPics.imageName)

        onClicked: {
            // send picture to server
            console.log("UI State: " + proxyPics.currentState
                        + " " + proxyPics.imageName
                        + " " + proxyPics.postImageName)

            // this will upload the image to server
            proxyPics.postImageName = proxyPics.imageName
            proxyPics.currentState = "Photos Uploading"
            proxyPics.postImageName = ""
        }

        scale: upload_ButtonMouseArea.pressed ? 1.5 : 1.00
    }

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
