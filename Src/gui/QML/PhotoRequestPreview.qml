//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtMultimedia 5.5
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

Item {
    focus: true
    signal closed
    signal nextClicked
    signal previousClicked
    signal busyIndicatorRunning
    id: photoPreview
    anchors.fill: parent
    visible: proxyPics.currentSelectedAssignmentName !== ""
    onVisibleChanged: {
        if (visible) {
            proxyPics.imageCount = imageManager.getImageCount(
                        proxyPics.currentAssignmentId)
            proxyPics.currentState = "Photo Gallery"
        } else
            proxyPics.imageCount = 0
    }
    opacity: 1
    Component.onCompleted: {
        var id = imageManager.getImageName(currentIndex, proxyPics.userId)
        proxyPics.previewSource = "image://imageprovider/" + id
        pixxmlModel.query = "/rss/users/" + proxyPics.userId + "/image"
        pixxmlModel.source = "http://96.90.223.196/user/userDetail.xml"
    }

    Connections {
        target: proxyPics
        onNewPhoto: {
            console.log("UI State: " + proxyPics.currentState + " New Photograph" + photograph)
            preview.source = photograph
        }

        onImageNameChanged: {
            console.log("UI State: " + proxyPics.currentState + " user id:"
                        + proxyPics.userId + " Image Name Changed: " + proxyPics.imageName)
        }
        onImageDownloaded: {
            anyImageAvailable = true
            console.log("UI State: " + proxyPics.currentState + " user id:"
                        + proxyPics.userId + " On Image Downloaded...")
            proxyPics.imageName = pixxmlModel.get(selectedIndex).name
            proxyPics.previewSource = "image://imageprovider/" + proxyPics.imageName
            proxyPics.imagetitle = pixxmlModel.get(
                        selectedIndex).assignmentname
            proxyPics.imageCount = imageManager.getImageCount(
                        proxyPics.currentAssignmentId)
        }
    }
    property int currentIndex: -1

    onCurrentIndexChanged: {
        console.log("UI State: " + proxyPics.currentState + " --- " + pixxmlModel.get(
                        currentIndex).name)
        proxyPics.imageName = imageManager.getImageName(
                    currentIndex, proxyPics.currentSelectedAssignmentName)
        proxyPics.previewSource = "image://imageprovider/" + proxyPics.imageName
        proxyPics.imagetitle = proxyPics.currentSelectedAssignmentName

        var counter = 0
        for (var i = 0; i < pixxmlModel.count; ++i) {
            if (pixxmlModel.get(
                        i).assignmentname === proxyPics.currentSelectedAssignmentName) {
                if (counter === currentIndex) {
                    root.showButton = pixxmlModel.get(
                                i).status === "Purchased" ? false : true
                    break
                }
                counter++
            }
        }
    }
    property string currentSelectedAssignmentName: ""
    onCurrentSelectedAssignmentNameChanged: {
        console.log("UI State:" + proxyPics.currentState
                    + "assignment name has changed" + proxyPics.currentSelectedAssignmentName)
        proxyPics.imageName = imageManager.getImageName(
                    0, proxyPics.currentSelectedAssignmentName)
        proxyPics.previewSource = "image://imageprovider/" + id
        proxyPics.imagetitle = proxyPics.currentSelectedAssignmentName
    }
    property int imageCount: proxyPics.imageCount
    onImageCountChanged: {
        console.log("UI State:" + proxyPics.currentState + "Image count Changed:" + imageCount)
        if (proxyPics.imageCount > 0) {
            currentIndex = 0
        }
        proxyPics.imagetitle = proxyPics.currentSelectedAssignmentName
        if (proxyPics.imageCount <= 0)
            root.showButton = false
        else
            root.showButton = true
    }

    property bool anyImageAvailable: false

    onAnyImageAvailableChanged: {
        console.log("UI State:" + proxyPics.currentState
                    + "Any Image Availble Changed:" + anyImageAvailable)
    }

    property bool showTitle: false
    onShowTitleChanged: {
        console.log("UI State:" + proxyPics.currentState + "Show Title Changed:" + showTitle)
    }
    ToolBar {
        id: screentitle
        RowLayout {
            anchors.fill: parent
            ToolButton {
                iconSource: "qrc:/images/REQUESTS ICON-main menu.png"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState)
                    proxyPics.currentState = "My Request"
                }
            }
            Text {
                id: photoaddress
                text: qsTr("Photo Request Preview")
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
    Rectangle {
        anchors.fill: parent
        color: "grey"
        opacity: 0.3
    }

    Image {
        id: preview
        anchors.fill: parent
        fillMode: Image.PreserveAspectFit
        smooth: true
        onProgressChanged: {
            console.log("UI State:" + proxyPics.currentState + "progress:" + progress)
        }
    }

    Rectangle {
        id: titleRect
        width: parent.width
        height: parent.height / 11
        opacity: 1
        anchors.top: parent.top
        color: "lightgreen"

        Text {
            id: title
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter
            text: proxyPics.currentSelectedAssignmentName
            font.family: "Centuma"
            font.bold: true
        }

        onVisibleChanged: {
            title.text = proxyPics.currentSelectedAssignmentName
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            console.log("UI State:" + proxyPics.currentState + "-test--"
                        + imageManager.getImageCount(
                            proxyPics.currentAssignmentId))
            parent.closed()
        }
    }
    // Next Button
    OvalButton {
        id: nextButton
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter
        visible: imageCount !== 0 && currentIndex !== imageCount - 1
        width: parent.height / 11
        height: parent.height / 11
        label: "next"
        onClicked: {
            currentIndex = currentIndex + 1
            nextClicked()
        }
    }

    // previour Button
    OvalButton {
        id: backbtn
        objectName: "back"
        visible: imageCount !== 0 && currentIndex !== 0
        width: parent.height / 11
        height: parent.height / 11
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        label: "back"
        onClicked: {
            currentIndex = currentIndex - 1
            previousClicked()
        }
    }

    Keys.onReleased: {
        if (event.key === Qt.Key_Back) {
            console.log("UI State:" + proxyPics.currentState + "Back button captured - wunderbar !")
            event.accepted = true
            parent.closed()
        }
    }
}
