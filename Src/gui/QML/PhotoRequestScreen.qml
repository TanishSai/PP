/***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
// File: PhotoRequestScreen.qml
import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.0
import QtPositioning 5.5
import QtQuick.XmlListModel 2.0

Item {
    id: photoRequstScreen
    width: proxyPics.uiWidth
    height: proxyPics.uiHeight

    property bool anyImageAvailable: false

    onAnyImageAvailableChanged: {
        console.log("UI State: " + proxyPics.currentState + " user id:" + proxyPics.userId
                    + " Any Image Availble Changed: " + anyImageAvailable)
    }
    property bool showButton: false
    onShowButtonChanged: {
        console.log("UI State:" + proxyPics.currentState + " user id:"
                    + proxyPics.userId + " Show Button Changed:" + showButton)
    }

    property int currentIndex: -1

    onCurrentIndexChanged: {
        console.log("UI State: " + proxyPics.currentState + " user id:"
                    + proxyPics.userId + " --- " + pixxmlModel.get(
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
                    showButton = pixxmlModel.get(
                                i).status === "Purchased" ? false : true
                    break
                }
                counter++
            }
        }
    }

    property int selectedIndex: 0
    XmlListModel {
        id: pixxmlModel
        XmlRole {
            name: "name"
            query: "name/string()"
        }
        XmlRole {
            name: "status"
            query: "status/string()"
        }
        XmlRole {
            name: "assignmentname"
            query: "assignmentname/string()"
        }
        XmlRole {
            name: "assignmentid"
            query: "assignmentid/string()"
        }
        onCountChanged: {
            console.log(" user id: " + proxyPics.userId + "count: " + count
                        + "count: " + pixxmlModel.count)
            if (count === 0)
                return
            var counter = 0
            for (var i = 0; i < pixxmlModel.count; ++i) {
                proxyPics.receiveImageWithFileName = pixxmlModel.get(i).name
                console.log("UI State:" + proxyPics.currentState + " user id:"
                            + proxyPics.userId + " model --" + pixxmlModel.get(
                                i).name + "__" + proxyPics.currentSelectedAssignmentName)
                if (pixxmlModel.get(
                            i).assignmentname === proxyPics.currentSelectedAssignmentName)
                    counter++
                console.log("count: " + count)
                for (var i = 0; i < pixxmlModel.count; ++i) {
                    proxyPics.receiveImageWithFileName = pixxmlModel.get(i).name
                }
                proxyPics.imageCount = imageManager.getImageCountForCurrentUserCurrentAssignemnt(
                            proxyPics.userId, proxyPics.currentAssignmentId)
                proxyPics.imageCount = counter
            }
        }
    }

    function isAssignmentOdCurretnUser(currentAssignmentName) {
        console.log("UI State:" + proxyPics.currentState + currentAssignmentName.indexOf(
                        proxyPics.userId) + " * " + currentAssignmentName + "**" + proxyPics.userId)
        return currentAssignmentName.indexOf(
                    proxyPics.userId) === -1 ? false : true
    }

    Component.onCompleted: {
        console.log("UI State: " + proxyPics.currentState + " user id:"
                    + proxyPics.userId + " Photo Request Screen loaded")
        pixxmlModel.query = "/rss/image_" + proxyPics.assignmentTicket + "/image"
        pixxmlModel.source = "http://96.90.223.196/assignment/feed.xml"

    }

    transformOrigin: Item.Center

    Connections {
        target: proxyPics
        onAssignmentTicketNumber: {
            proxyPics.assignmentTicket = ticketNumber
            console.log("UI State:" + proxyPics.currentState + " ticket number: "
                        + ticketNumber + " " + proxyPics.assignmentTicket)
        }
        onImageDownloaded: {
            anyImageAvailable = true
            console.log("UI State: " + proxyPics.currentState + " user id:"
                        + proxyPics.userId + " On Image Downloaded..." + proxyPics.imageName)
            proxyPics.imageName = pixxmlModel.get(selectedIndex).assignmentname
            proxyPics.previewSource = "image://imageprovider/" + proxyPics.imageName
            proxyPics.imagetitle = proxyPics.currentSelectedAssignmentName
            proxyPics.imageCount = imageManager.getImageCount(
                        proxyPics.currentAssignmentId)
        }
        onUserAssignmentCountChanged: {
            console.log("UI State: " + proxyPics.currentState + " user id:" + proxyPics.userId
                        + "userAssignmentCountChanged " + proxyPics.userAssignmentCount)
        }

        onImageNameChanged: {
            console.log("UI State: " + proxyPics.currentState + " user id:"
                        + proxyPics.userId + " Image Name Changed: " + proxyPics.imageName)
        }
        onImageCountChanged: {
            console.log("UI State:" + proxyPics.currentState + "Image count Changed:" + imageCount)
            if (imageCount > 0) {
                currentIndex = 0
            }
            proxyPics.imagetitle = proxyPics.currentSelectedAssignmentName
        }

        onCurrentSelectedAssignmentNameChanged: {
            console.log("UI State:" + proxyPics.currentState + "assignment name has changed"
                        + proxyPics.currentSelectedAssignmentName)
            var id = imageManager.getImageName(
                        0, proxyPics.currentSelectedAssignmentName)
            proxyPics.imageName = id
            proxyPics.previewSource = "image://imageprovider/" + id
            proxyPics.imagetitle = proxyPics.currentSelectedAssignmentName
        }
        onCurrentAssignmentIdChanged: {
            console.log("UI State:" + proxyPics.currentState + "assignment id has changed"
                        + proxyPics.currentSelectedAssignmentName)
            proxyPics.imageName = imageManager.getImageName(
                        0, proxyPics.currentSelectedAssignmentName)

            proxyPics.previewSource = "image://imageprovider/" + proxyPics.imageName
            proxyPics.imagetitle = proxyPics.currentSelectedAssignmentName
        }
    }

    XmlListModel {
        id: assignmentListModel
        source: ""
        query: "/rss/sanjose/item"

        XmlRole {
            name: "name"
            query: "name/string()"
        }
        onCountChanged: {
            console.log("count:" + count)
        }
    }
    Item {
        id: assignment_list
        anchors.top: parent.top
        width: parent.width
        height: parent.height * .8

        ListView {
            id: listview
            width: parent.width
            height: parent.height
            anchors.top: parent.top
            model: assignmentListModel

            clip: true

            delegate: Image {
                id: imageId
                width: listview.width
                height: listview.height / 5
                source: "qrc:/images/bg.jpg"
                Text {
                    id: assignment_Name
                    font.family: "Centuma"
                    font.bold: true
                    font.pointSize: 12
                    color: "orange"
                    wrapMode: Text.WordWrap
                    textFormat: Text.RichText
                    anchors.left: parent.left
                    anchors.leftMargin: 10
                    anchors.top: parent.top
                    anchors.topMargin: 5
                    elide: Text.ElideMiddle
                    text: name
                    onTextChanged: {
                        imageId.visible = isAssignmentOdCurretnUser(
                                    assignment_Name.text)
                    }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        proxyPics.currentSelectedAssignmentName = name
                        selectedIndex = index
                        console.log("UI State: " + proxyPics.currentState
                                    + " current selected assignment: "
                                    + proxyPics.currentSelectedAssignmentName)
                    }
                }
            }
        }
    }

     ToolBar {
        anchors.bottom: parent.bottom
        RowLayout {
            BackButton {
                 onClicked: {
                    console.log("UI State: " + proxyPics.currentState + " BACK ")
                    proxyPics.currentState = "Home"
                }
            }

            OvalButton {
                anchors.right: parent.right
                height: proxyPics.uiHeight / 22
                width: proxyPics.uiWidth / 2.3
                label: "NEW PHOTO REQUEST"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState + " NEW PHOTO REQUEST ")
                    proxyPics.currentState = "New Photo Request"
                }
            }
        }
    }
    Keys.onReleased: {
        console.log(event.key)
        if (event.key === Qt.Key_Back) {
            console.log("UI State: " + proxyPics.currentState
                        + " Hardware Back button captured Photo Request - wunderbar !")
            event.accepted = true
            proxyPics.currentState = "Home"
        }
    }
}
