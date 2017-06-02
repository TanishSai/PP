//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017

import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.XmlListModel 2.0
import SortFilterProxyModel 0.2

Rectangle {
    color: "#0c3845"
    Component.onCompleted: {

        console.log("UI State: " + proxyPics.currentState
                    + " State machine now loaded Screen width: "
                    + proxyPics.uiWidth + " Screen Height: " + proxyPics.uiHeight
                    //                    + " Pixel Density: "
                    //                    + Screen.pixelDensity
                    + " Map Url: " + proxyPics.mapurl + " Phone Number: " + proxyPics.phoneNumber
                    + " User Id: " + proxyPics.userId + " User Password: "
                    + proxyPics.userPassword + " Email: " + proxyPics.userEmail)
    }

    MyAssignmentsHeader {
        id: header
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight / 7

        onClosedClicked: {
            proxyPics.currentState = "Home"
        }

        onPopupMenuClicked: {
            proxyPics.currentState = "Menu"
        }

        onActiveClicked: filterProxyModel.filterValue = "accepted"
        onPendingClicked: filterProxyModel.filterValue = "pending"
        onCompletedClicked: filterProxyModel.filterValue = "completed"
        onExpiredClicked: filterProxyModel.filterValue = "expired"
    }

    // to refresh the elapse time every minute
    Timer {
        running: parent.visible
        repeat: true
        interval: 60*1000

        onTriggered: {
            xmlModel.source = ""
            xmlModel.source = "http://96.90.223.196/user/" + proxyPics.userId + "/myAssignments.xml"
        }
    }

    XmlListModel {
        id: xmlModel
        source: "http://96.90.223.196/user/" + proxyPics.userId + "/myAssignments.xml"
        query: "/sanjose/item"
        onProgressChanged: {
            if (xmlModel.progress === 1.0) {
                console.log("UI State: " + proxyPics.currentState + " assignments downloaded")
            } else {
                console.log("UI State: " + proxyPics.currentState
                            + " rss feed progress changed:" + xmlModel.progress)
            }
        }
        XmlRole {
            name: "value"
            query: "value/string()"
        }
        XmlRole {
            name: "notes"
            query: "notes/string()"
        }
        XmlRole {
            name: "address"
            query: "address/string()"
        }
        XmlRole {
            name: "pubDate"
            query: "pubDate/string()"
        }
        XmlRole {
            name: "name"
            query: "name/string()"
        }
        XmlRole {
            name: "id"
            query: "id/string()"
        }
        XmlRole {
            name: "ticket"
            query: "ticket/string()"
        }
        XmlRole {
            name: "accompolishedby"
            query: "accomplishedBy/string()"
        }
        XmlRole {
            name: "acceptedDate"
            query: "acceptedDate/string()"
        }
        XmlRole {
            name: "state"
            query: "state/string()"
        }

        onCountChanged: {
            console.log("UI State:" + proxyPics.currentState + "count:" + count)
        }
    }

    SortFilterProxyModel {
        id: filterProxyModel
        sourceModel: xmlModel
        filterCaseSensitivity: Qt.CaseInsensitive
        filterRoleName: "state"
        filterValue: "accepted"
    }

    ListView {
        id: listview
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight - (proxyPics.uiHeight / 4.5)
        anchors.top: header.bottom
        model: filterProxyModel
        clip: true

        delegate: PhotoAssignmentScreenItem {
            width: parent.width
            height: visible ? proxyPics.uiHeight / 6.5 : 0

            pASIassignmentId: id
            assignmentNameString: name
            valueString: value
            addressString: address
            pASIticket: ticket
            pASIpubDate: pubDate

            onItemClicked: {
                proxyPics.currentState = "Current Assignment"
                console.log(proxyPics.currentAssignmentName+"****"+proxyPics.currentAssignmentAddress)
            }
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
