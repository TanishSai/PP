//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtQuick.XmlListModel 2.0

Item {
    id: root
    property int currentIndex: 0

    Component.onCompleted: {
        focus = true
        proxyPics.imageCount = imageManager.getImageCount(proxyPics.currentAssignmentId)
        console.log("UI State: " + proxyPics.currentState + " *** " + proxyPics.currentAssignmentId)
    }

    Connections {
        target: proxyPics
        onImageCountChanged: {
            console.log("UI State:" + proxyPics.currentState + "Image count Changed:" + proxyPics.imageCount)
            if (proxyPics.imageCount > 0) {
                currentIndex = 0
                refrehImage()
            }
            proxyPics.imagetitle = proxyPics.currentSelectedAssignmentName
        }
    }

    function refrehImage()
    {
        proxyPics.imageName = imageManager.getImageNameForCurrentUserCurrentAssignemnt(
                    currentIndex, proxyPics.userId , proxyPics.currentSelectedAssignmentName)
        console.log("UI State: " + proxyPics.currentState + " --- " + proxyPics.imageName)
        //proxyPics.previewSource = "image://imageprovider/" + proxyPics.imageName
        preview.source = "image://imageprovider/" + proxyPics.imageName
        proxyPics.imagetitle = proxyPics.currentSelectedAssignmentName
    }

    onCurrentIndexChanged: {
        refrehImage()
    }

    onVisibleChanged: {
        console.log("visible:" + visible)
        if (visible) {
            focus = true
            refrehImage()
        }
        if (proxyPics.userId !== "") {
            console.log("current user id:" + proxyPics.userId)
        }
    }

    Rectangle {
        id: baseRect
        visible: proxyPics.currentSelectedAssignmentName === ""
        color: "lightgray"
        anchors.fill: parent
        Text {
            anchors.centerIn: parent
            font.family: "Centuma"
            text: qsTr("Error in selected assignment")
        }
    }

    Image {
        id: preview
        width: parent.width
        height: parent.height
        fillMode: Image.PreserveAspectFit
        visible: !baseRect.visible
        smooth: true
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
        label: "Reject"
        onClicked: {

            proxyPics.currentState = "Photo Reject"
        }
    }

    OvalButton {
        id: photoAssignmentButton
        objectName: "photoAssignmentButton"
        anchors.right: parent.right
        anchors.bottomMargin: 10
        anchors.bottom: parent.bottom
        height: proxyPics.uiHeight / 11
        width: proxyPics.uiWidth / 2.5
        label: "Purchase"
        onClicked: {
            console.log("UI State: " + proxyPics.currentState + " " + photoAssignmentButton.label)
            proxyPics.currentState = "Photo Purchase"
        }
    }

    Keys.onReleased: {
        console.log(event.key)
        if (event.key === Qt.Key_Back) {
            console.log("UI State:" + proxyPics.currentState
                        + "Hardware Back button Requested Photos- wunderbar !")
            // fake soft back key press when hardware back key pressed
            proxyPics.currentState = "Photo Request"
            event.accepted = true
        }
    }
}
