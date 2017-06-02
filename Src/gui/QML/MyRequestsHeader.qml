import QtQuick 2.2
Item {
    property int rootItemWidth:750
    property int rootItemHeight:175

    signal activeClicked
    signal acceptedClciked
    signal accomplishedClicked
    signal paidClicked
    signal popupMenuClicked
    signal closedClicked

    Image {
        id: layer_0
        source: "qrc:/images/myRequestHeader/layer_0.png"
        x: ((parent.width/rootItemWidth)*0)
        y: ((parent.height/rootItemHeight)*0)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: popup
        source: "qrc:/images/myRequestHeader/popup.png"
        x: ((parent.width/rootItemWidth)*674)
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
        id: x
        source: "qrc:/images/myRequestHeader/x.png"
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
        id: accepted
        source: "qrc:/images/myRequestHeader/accepted.png"
        x: ((parent.width/rootItemWidth)*207)
        y: ((parent.height/rootItemHeight)*112)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
        MouseArea {
            anchors.fill: parent
            onClicked: {
                acceptedClciked()
                light.x = accepted.x + (accepted.width/2) - (light.width/2)
            }
        }
    }
    Image {
        id: light
        source: "qrc:/images/myRequestHeader/light.png"
        x: ((parent.width/rootItemWidth)*46)
        y: ((parent.height/rootItemHeight)*135)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)

        NumberAnimation on x {
            duration: 1000
        }
    }
    Image {
        id: accomplished
        source: "qrc:/images/myRequestHeader/accomplished.png"
        x: ((parent.width/rootItemWidth)*383)
        y: ((parent.height/rootItemHeight)*112)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
        MouseArea {
            anchors.fill: parent
            onClicked: {
                accomplishedClicked()
                light.x = accomplished.x + (accomplished.width/2) - (light.width/2)
            }
        }
    }
    Image {
        id: paid
        source: "qrc:/images/myRequestHeader/paid.png"
        x: ((parent.width/rootItemWidth)*619)
        y: ((parent.height/rootItemHeight)*112)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
        MouseArea {
            anchors.fill: parent
            onClicked: {
                paidClicked()
                light.x = paid.x + (paid.width/2) - (light.width/2)
            }
        }
    }
    Image {
        id: active
        source: "qrc:/images/myRequestHeader/active.png"
        x: ((parent.width/rootItemWidth)*76)
        y: ((parent.height/rootItemHeight)*111)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
        MouseArea {
            anchors.fill: parent
            onClicked: {
                activeClicked()
                light.x = active.x + (active.width/2) - (light.width/2)
            }
        }
    }
}
