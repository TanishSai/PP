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
    id: photoAssignmentHeader
    objectName: "photoAssignmentHeader"

    signal message(string msg)
    signal mapit
    signal ditanceClicked
    signal priceHTL
    signal priceLTH
    signal popupMenuClicked
    signal closedClicked

    property int rootItemWidth:750
    property int rootItemHeight:150

    Image {
        id: layer_18
        source: "qrc:/images/photAssignmentHeader/layer_18.png"
        x: ((parent.width/rootItemWidth)*0)
        y: ((parent.height/rootItemHeight)*0)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: text_field
        source: "qrc:/images/photAssignmentHeader/text_field.png"
        x: ((parent.width/rootItemWidth)*320)
        y: ((parent.height/rootItemHeight)*67)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: layer_25
        source: "qrc:/images/photAssignmentHeader/layer_25.png"
        x: ((parent.width/rootItemWidth)*491)
        y: ((parent.height/rootItemHeight)*108)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
        MouseArea {
            anchors.fill: parent
            onClicked: {
                priceLTH()
                light.x = layer_25.x + (layer_25.width/2) - (light.width/2)
                console.log("3 "+light.x)
            }
        }
    }
    Image {
        id: layer_24
        source: "qrc:/images/photAssignmentHeader/layer_24.png"
        x: ((parent.width/rootItemWidth)*298)
        y: ((parent.height/rootItemHeight)*108)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)

        MouseArea {
            anchors.fill: parent
            onClicked: {
                priceHTL()
                light.x = layer_24.x + (layer_24.width/2) - (light.width/2)
                console.log("2 "+light.x)
            }
        }
    }
    Image {
        id: layer_23
        source: "qrc:/images/photAssignmentHeader/layer_23.png"
        x: ((parent.width/rootItemWidth)*126)
        y: ((parent.height/rootItemHeight)*108)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)

        MouseArea {
            anchors.fill: parent
            onClicked: {
                ditanceClicked()
                light.x = layer_23.x + (layer_23.width/2) - (light.width/2)
                console.log("1 "+light.x)
            }
        }
    }
    Image {
        id: layer_22
        source: "qrc:/images/photAssignmentHeader/layer_22.png"
        x: ((parent.width/rootItemWidth)*216)
        y: ((parent.height/rootItemHeight)*59)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: layer_21
        source: "qrc:/images/photAssignmentHeader/layer_21.png"
        x: ((parent.width/rootItemWidth)*469)
        y: ((parent.height/rootItemHeight)*59)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: layer_20
        source: "qrc:/images/photAssignmentHeader/layer_20.png"
        x: ((parent.width/rootItemWidth)*0)
        y: ((parent.height/rootItemHeight)*0)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)

        MouseArea {
            anchors.fill: parent
            onClicked: closedClicked()
        }
    }
    Image {
        id: layer_19
        source: "qrc:/images/photAssignmentHeader/layer_19.png"
        x: ((parent.width/rootItemWidth)*699)
        y: ((parent.height/rootItemHeight)*0)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)

        MouseArea {
            anchors.fill: parent
            onClicked: popupMenuClicked()
        }
    }

    Image {
        id: light
        source: "qrc:/images/photAssignmentHeader/light.png"
        x: ((parent.width/rootItemWidth)*104)
        y: ((parent.height/rootItemHeight)*130)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)

        NumberAnimation on x {
            duration: 1000
        }
    }
}
