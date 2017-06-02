import QtQuick 2.2
Item {
    property int rootItemWidth:750
    property int rootItemHeight:175

    signal activeClicked
    signal pendingClicked
    signal completedClicked
    signal expiredClicked
    signal popupMenuClicked
    signal closedClicked

    Image {
        id: layer_0
        source: "qrc:/images/myAssignmentHeader/layer_0.png"
        x: ((parent.width/rootItemWidth)*0)
        y: ((parent.height/rootItemHeight)*0)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: x
        source: "qrc:/images/myAssignmentHeader/x.png"
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
        id: popup
        source: "qrc:/images/myAssignmentHeader/popup.png"
        x: ((parent.width/rootItemWidth)*675)
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
        id: pending
        source: "qrc:/images/myAssignmentHeader/pending.png"
        x: ((parent.width/rootItemWidth)*213)
        y: ((parent.height/rootItemHeight)*112)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)

        MouseArea {
            anchors.fill: parent
            onClicked: {
                pendingClicked()
                light.x = parent.x + (parent.width/2) - (light.width/2)
            }
        }
    }
    Image {
        id: light
        source: "qrc:/images/myAssignmentHeader/light.png"
        x: ((parent.width/rootItemWidth)*46)
        y: ((parent.height/rootItemHeight)*135)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: completed
        source: "qrc:/images/myAssignmentHeader/completed.png"
        x: ((parent.width/rootItemWidth)*381)
        y: ((parent.height/rootItemHeight)*112)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
        MouseArea {
            anchors.fill: parent
            onClicked: {
                completedClicked()
                light.x = parent.x + (parent.width/2) - (light.width/2)
            }
        }
    }
    Image {
        id: expired
        source: "qrc:/images/myAssignmentHeader/expired.png"
        x: ((parent.width/rootItemWidth)*586)
        y: ((parent.height/rootItemHeight)*112)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
        MouseArea {
            anchors.fill: parent
            onClicked: {
                expiredClicked()
                light.x = parent.x + (parent.width/2) - (light.width/2)
            }
        }
    }
    Image {
        id: active
        source: "qrc:/images/myAssignmentHeader/active.png"
        x: ((parent.width/rootItemWidth)*76)
        y: ((parent.height/rootItemHeight)*111)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
        MouseArea {
            anchors.fill: parent
            onClicked: {
                activeClicked()
                light.x = parent.x + (parent.width/2) - (light.width/2)
            }
        }
    }
}
