//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtQuick.XmlListModel 2.0
import SortFilterProxyModel 0.2

Rectangle {
    property int categoryMode: 0 //active // 1- Accepted // 2 - Accomplished // 3 - paid
    color: "#0c3845"

    MyRequestsHeader {
        id: header
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight / 7

        onClosedClicked: {
            proxyPics.currentState = "Menu"
        }

        onPopupMenuClicked: {
            proxyPics.currentState = "Menu"
        }

        onActiveClicked: {
            categoryMode = 0
            filterProxyModel.filterValue = ""
        }
        onAcceptedClciked: {
            categoryMode = 1
            filterProxyModel.filterValue = "accepted"
        }
        onAccomplishedClicked: {
            categoryMode = 2
            filterProxyModel.filterValue = "accomplished"
        }
        onPaidClicked: {
            categoryMode = 3
            filterProxyModel.filterValue = "completed"
        }
    }

    XmlListModel {
        id: xmlModel
        source: "http://96.90.223.196/user/" + proxyPics.userId + "/myRequest.xml"
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
        filterValue: ""
    }

    ListView {
        id: listview
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight - (header.height)
        anchors.top: header.bottom
        model: filterProxyModel
        clip: true

        delegate: PhotoAssignmentScreenItem {
            width: parent.width
            height: visible ? proxyPics.uiHeight / 6.5 : 0

            pASIassignmentId: id
            assignmentNameString: name
            valueString: "$" + value
            addressString: address
            pASIticket: ticket
            pASIpubDate: pubDate

            Component.onCompleted: {
                if (categoryMode === 2)
                    proxyPics.receiveImageNameList(id)
            }

            onItemClicked: {
                if (categoryMode === 0)
                {
                    // edit request
                } else if (categoryMode === 1) {
                    // show the user accespteed the assignment
                } else if ( categoryMode === 2 ) {
                    // browse uploaded pic
                    proxyPics.currentState = "Requested Photos"
                } else {

                }

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
            proxyPics.currentState = "Menu"
            event.accepted = true
        }
    }
}
