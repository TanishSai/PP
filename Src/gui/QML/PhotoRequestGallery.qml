/***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.XmlListModel 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

Item {
    id: photoAssignment
    anchors.fill: parent
    function isAssignmentOdCurretnUser(currentAssignmentName) {
        var name = currentAssignmentName + "-"
        var subString = proxyPics.userId + "-"
        return true
        //        return name.indexOf("-") == 0 ? false : (name.indexOf(subString) == -1 ? false : true)
    }

    Component.onCompleted: {
        console.log("UI State: " + proxyPics.currentState + " Photo Request Galary loaded")
        focus = true
        assignmntNameListXmllModel.source = "http://96.90.223.196/assignment/feed.xml"
    }
    XmlListModel {
        id: assignmntNameListXmllModel
        source: ""
        query: "/rss/sanjose/item"
        XmlRole {
            name: "name"
            query: "name/string()"
        }
        XmlRole {
            name: "address"
            query: "address/string()"
        }
        XmlRole {
            name: "id"
            query: "id/string()"
        }
        onCountChanged: {
            console.log("count:" + count)
        }
    }

    ListView {
        id: listview
        width: parent.width
        height: parent.height
        anchors.top: parent.top
        model: assignmntNameListXmllModel
        clip: true

        delegate: Image {
            width: parent.width
            source: "qrc:/images/bg.jpg"
            visible: name === "" ? false : isAssignmentOdCurretnUser(id)
            height: visible ? listview.height / 5 : 0

            Text {
                id: nameText
                font.family: "Centuma"
                font.bold: true
                font.pointSize: 22
                color: "orange"
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                anchors.left: parent.left
                anchors.leftMargin: 10
                elide: Text.ElideMiddle
                text: name
            }

            Text {
                id: addressText
                visible: false
                font.family: "Centuma"
                font.bold: true
                font.pointSize: 22
                color: "orange"
                wrapMode: Text.WordWrap
                textFormat: Text.RichText
                anchors.left: parent.right
                anchors.leftMargin: 10
                anchors.top: nameText.bottom
                anchors.topMargin: 5
                elide: Text.ElideMiddle
                text: address
            }
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    proxyPics.currentAssignmentName = name
                    proxyPics.currentSelectedAssignmentName = proxyPics.currentAssignmentName
                    proxyPics.currentAssignmentId = id
                    proxyPics.currentState = "Requested Photos"
                }
            }
        }
    }
    ToolBar {
        anchors.bottom: parent.bottom
        RowLayout {

            OvalButton {
                anchors.left: parent.left
                width: proxyPics.uiWidth / 2.5
                height: proxyPics.uiHeight / 11
                label: "BACK"
                onClicked: {
                    proxyPics.currentState = "Photo Request"
                }
            }

            OvalButton {
                anchors.right: parent.right
                height: proxyPics.uiHeight / 11
                width: proxyPics.uiWidth / 2.5
                label: "ASSIGNMENT"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState + " ASSIGNMENT")
                    proxyPics.currentState = "Requested Photos"
                }
            }
        }
    }
    Keys.onReleased: {
        console.log(event.key)
        if (event.key === Qt.Key_Back) {
            console.log("UI State:" + proxyPics.currentState
                        + "Hardware Back button captured Photo Request Gallery - wunderbar !")
            // fake soft back key press when hardware back key pressed
            proxyPics.currentState = "Photo Request"
            event.accepted = true
        }
    }
}
