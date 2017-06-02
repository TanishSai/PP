import QtQuick 2.2
Item {
    property int rootItemWidth:750
    property int rootItemHeight:250

    property string pASIassignmentId: ""
    property alias assignmentNameString: assignmentName.text
    property alias valueString: value.text
    property alias addressString: address.text
    property string pASIticket: ""
    property string pASIpubDate: ""
    property string pASIlatitude: ""
    property string pASIlongitude: ""
    property alias postedString: posted.text
    property alias mileString: mile.text

    signal itemClicked()

    Image {
        id: layer_0
        source: "qrc:/images/assignmentItemImages/layer_0.png"
        x: ((parent.width/rootItemWidth)*0)
        y: ((parent.height/rootItemHeight)*0)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: background_copy
        source: "qrc:/images/assignmentItemImages/background_copy.png"
        x: ((parent.width/rootItemWidth)*0)
        y: ((parent.height/rootItemHeight)*0)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: layer_7_copy
        source: "qrc:/images/assignmentItemImages/layer_7_copy.png"
        x: ((parent.width/rootItemWidth)*0)
        y: ((parent.height/rootItemHeight)*0)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: layer_20
        source: "qrc:/images/assignmentItemImages/layer_20.png"
        x: ((parent.width/rootItemWidth)*35)
        y: ((parent.height/rootItemHeight)*33)
        opacity: 0
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: layer_19
        source: "qrc:/images/assignmentItemImages/layer_19.png"
        x: ((parent.width/rootItemWidth)*645)
        y: ((parent.height/rootItemHeight)*221)
        opacity: 0
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: layer_18
        source: "qrc:/images/assignmentItemImages/layer_18.png"
        x: ((parent.width/rootItemWidth)*435)
        y: ((parent.height/rootItemHeight)*221)
        opacity: 0
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: layer_17
        source: "qrc:/images/assignmentItemImages/layer_17.png"
        x: ((parent.width/rootItemWidth)*195)
        y: ((parent.height/rootItemHeight)*123)
        opacity: 0
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: layer_17_copy
        source: "qrc:/images/assignmentItemImages/layer_17_copy.png"
        x: ((parent.width/rootItemWidth)*35)
        y: ((parent.height/rootItemHeight)*135)
        opacity: 0
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Image {
        id: layer_16
        source: "qrc:/images/assignmentItemImages/layer_16.png"
        x: ((parent.width/rootItemWidth)*195)
        y: ((parent.height/rootItemHeight)*33)
        opacity: 0
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Text {
        id: value
        text: qsTr("$4")
//        fontSizeMode: Text.VerticalFit
        color: "#76f8d5"
        smooth: true
        x: layer_20.x
        y: layer_20.y
        verticalAlignment: Text.AlignTop
        width: ((parent.width/rootItemWidth)*layer_20.sourceSize.width)
        height: ((parent.height/rootItemHeight)*layer_20.sourceSize.height)
        opacity: 1
        font.pixelSize: (0.8269*height)
        font.family: "BritannicBold"
        font.bold: true
    }

    Image {
        id: layer_11
        source: "qrc:/images/assignmentItemImages/layer_11.png"
        x: ((parent.width/rootItemWidth)*615)
        y: ((parent.height/rootItemHeight)*203)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Text {
        id: address
        text: (value.implicitHeight+" width :"+value.width+" height: "+value.height+" pixel "+value.font.pixelSize)
        smooth: true
        x: layer_17.x
        y: layer_17.y
//        maximumLineCount: 2
        wrapMode: Text.WrapAnywhere
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
        opacity: 1
        width: ((parent.width/rootItemWidth)*layer_17.sourceSize.width)
        height: ((parent.height/rootItemHeight)*layer_17.sourceSize.height)
        font.pixelSize: (0.8269*height)
        font.family: "Calibri"
        color: "#40d0ff"
    }
    Image {
        id: layer_14
        source: "qrc:/images/assignmentItemImages/layer_14.png"
        x: ((parent.width/rootItemWidth)*30)
        y: ((parent.height/rootItemHeight)*196)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Text {
        id: posted
        text: getTimeElapsed(pASIpubDate)//qsTr("Posted 10 min ago")
        color: "#40d0ff"
        smooth: true
        x: layer_18.x
        y: layer_18.y
        opacity: 1
        width: ((parent.width/rootItemWidth)*layer_18.sourceSize.width)
        height: ((parent.height/rootItemHeight)*layer_18.sourceSize.height)
        font.pixelSize: (0.8269*height)< 10 ? 10 : (0.8269*height)
        font.family: "Calibri"
        verticalAlignment: Text.AlignVCenter
    }
    Text {
        id: mile
        text: qsTr("1.0 Mile")
        color: "#40d0ff"
        smooth: true
        x: layer_19.x
        y: layer_19.y
        opacity: 1
        elide: Text.ElideMiddle
        width: ((parent.width/rootItemWidth)*layer_19.sourceSize.width)
        height: ((parent.height/rootItemHeight)*layer_19.sourceSize.height)
        font.pixelSize: (0.8269*height)< 10 ? 10 : (0.8269*height)
        font.family: "Calibri"
        verticalAlignment: Text.AlignVCenter
    }
    Image {
        id: layer_12
        source: "qrc:/images/assignmentItemImages/layer_12.png"
        x: ((parent.width/rootItemWidth)*400)
        y: ((parent.height/rootItemHeight)*201)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }
    Text {
        id: assignmentName
        text: qsTr("44 front street test test santa cruz, ca, 95060")
        color: "#76f8d5"
        smooth: true
//        maximumLineCount: 2
        wrapMode: Text.WrapAnywhere
        elide: Text.ElideRight
        horizontalAlignment: Text.AlignLeft
        x: layer_16.x
        y: layer_16.y
        opacity: 1
        width: ((parent.width/rootItemWidth)*layer_16.sourceSize.width)
        height: ((parent.height/rootItemHeight)*layer_16.sourceSize.height)
        font.pixelSize: (0.8269*height)
        font.family: "Calibri"
    }
    Text {
        id: per_photo
        text: qsTr("Per Photo")
        color: "#65f3f9"
        smooth: true
        x: layer_17_copy.x
        y: layer_17_copy.y
        opacity: 1
        width: ((parent.width/rootItemWidth)*layer_17_copy.sourceSize.width)
        height: ((parent.height/rootItemHeight)*layer_17_copy.sourceSize.height)
        font.pixelSize: (0.8269*height)
        font.family: "Calibri"
    }
    Image {
        id: layer_21
        source: "qrc:/images/assignmentItemImages/layer_21.png"
        x: ((parent.width/rootItemWidth)*185)
        y: ((parent.height/rootItemHeight)*207)
        opacity: 1
        width: ((parent.width/rootItemWidth)*sourceSize.width)
        height: ((parent.height/rootItemHeight)*sourceSize.height)
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            proxyPics.currentAssignmentId = pASIassignmentId
            proxyPics.currentAssignmentName = assignmentNameString
            proxyPics.currentSelectedAssignmentName = assignmentNameString
            proxyPics.assignmentValue = valueString
            proxyPics.currentAssignmentAddress = addressString
            proxyPics.mapCenter = proxyPics.currentAssignmentAddress
            proxyPics.assignmentTicket = pASIticket
            proxyPics.assignmentPubDate = pASIpubDate
            proxyPics.mapDestination = proxyPics.currentAssignmentAddress

            itemClicked()
        }
    }

    function getTimeElapsed(d)
    {
        console.log("d():: "+d);

        var ret ="Posted ";
        var dReportedDate = new Date(d);
        var dCurrentDate = new Date();
        if(isNaN(dReportedDate.getTime()))
        {
            return ret;
        }

        var diff = Math.ceil(Math.abs(dCurrentDate.getTime() - dReportedDate.getTime())/1000);
        if(diff<60) { //secs
            ret += "few seconds ago"
        }
        else if( (diff/60) < 60){//min
            ret += Math.ceil(diff/60)+"  minute(s) ago"
        }
        else if((diff/(60*60)) <24) {//hrs
            ret += Math.ceil(diff/(60*60))+" hour(s) ago"
        }
        else {//days
            ret += Math.ceil(diff/(60*60*24))+" day(s) ago"
        }

        return ret;
    }

}
