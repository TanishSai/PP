/***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
// Origniated: January 15th 2016
// Application: PhotoAssignmentScreen
import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.XmlListModel 2.0
//import QtWebView 1.1
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.1
import SortFilterProxyModel 0.2

Item {
    id: photoAssignment
    objectName: "photoAssignment"

    signal message(string msg)
    signal mapit

    Component.onCompleted: {
        console.log("UI State:" + proxyPics.currentState
                    + "Photo Assignment Screen loaded " + proxyPics.city)
        proxyPics.assignmentXMLFeed = "http://96.90.223.196/assignment/feed.xml"
        //This is required, if last state before Application close
        xmlModel.source = proxyPics.assignmentXMLFeed
    }

    PhotoAssignmentScreenHeader {
        id: header
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight / 7

        onMessage: {
            message(msg)
        }
        onMapit: {
            mapit()
        }

        onDitanceClicked: {
            filterProxyModel.sortRoleName = "distance"
            filterProxyModel.ascendingSortOrder = true
        }

        onPriceHTL: {
            filterProxyModel.sortRoleName = "value"
            filterProxyModel.ascendingSortOrder = false
        }

        onPriceLTH: {
            filterProxyModel.sortRoleName = "value"
            filterProxyModel.ascendingSortOrder = true
        }
        onClosedClicked: {
            proxyPics.currentState = "Home"
        }

        onPopupMenuClicked: {
            proxyPics.currentState = "Menu"
        }
    }

    XmlListModel {
        id: xmlModel
        source: proxyPics.assignmentXMLFeed
        // TODO - the below code need to enabled while feed.xml has assignment on city based tag
        // query: "/rss/" + proxyPics.city + "/item"
        // now we are having only sanjose
        query: "/rss/sanjose/item"
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
            query: "value/number()"
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
            name: "assignmentPubDate"
            query: "pubDate/string()"
        }
        XmlRole {
            name: "accompolishedby"
            query: "accomplishedBy/string()"
        }
        XmlRole {
            name: "accepted"
            query: "accepted/string()"
        }
        XmlRole {
            name: "latitude"
            query: "latitude/string()"
        }
        XmlRole {
            name: "longitude"
            query: "longitude/string()"
        }
        XmlRole {
            name: "distance"
            query: "x/number()"
        }

        onCountChanged: {
            console.log("UI State:" + proxyPics.currentState + "count:" + count)
        }
    }

    SortFilterProxyModel {
        id: filterProxyModel
        sourceModel: xmlModel
        sortCaseSensitivity: Qt.CaseInsensitive
        sortRoleName: "value"
        ascendingSortOrder:false
    }

    ListView {
        id: listview
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight - (proxyPics.uiHeight / 6)
        anchors.top: header.bottom
        model: filterProxyModel
        clip: true

        delegate: PhotoAssignmentScreenItem {
            width: parent.width
            height: visible ? proxyPics.uiHeight / 6.5 : 0
            visible: accepted === "true" ? false : true

            pASIassignmentId: id
            assignmentNameString: name
            valueString: "$" + value
            addressString: address
            pASIticket: ticket
            pASIpubDate: pubDate
            pASIlatitude: latitude
            pASIlongitude: longitude
            mileString: proxyPics.getDistanceFromCurrentPos(latitude, longitude) + "km"
            postedString: "Posted "+proxyPics.getDifferenceWithCurrentTime(assignmentPubDate)+ " ago"

            Component.onCompleted: {
                distance = proxyPics.getDistanceFromCurrentPos(pASIlatitude, pASIlongitude)
            }

            onItemClicked: {
                proxyPics.currentState = "Assignment Details"
                console.log(proxyPics.currentAssignmentAddress)
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
