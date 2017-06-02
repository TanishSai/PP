//***************************************************************************************************/
// Author: Louis Meadows
// Copyright: Positively Front Street, Inc 2017
import QtQuick 2.5
import QtPositioning 5.5
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import QtWebView 1.1
import QtQuick.Controls 1.4

Item {
    ToolBar {
        id: screentitle
        RowLayout {
            anchors.fill: parent
            ToolButton {
                iconSource: "qrc:/images/X-exit-icon.png"
                onClicked: {
                    console.log("UI State: " + proxyPics.currentState + " back pressed")
                    proxyPics.currentState = "Menu"
                }
            }
            Text {
                id: squareup
                text: qsTr("SquareUp")
                font.family: "Centuma"
                font.pointSize: 16
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
    Item {
        id: squareupwebview
        width: proxyPics.uiWidth
        height: proxyPics.uiHeight * .8
        anchors.top: screentitle.bottom
        WebView {
            id: webView
            anchors.fill: parent
            url: "https://www.proxyPics.com/squareup.html"

            onLoadingChanged: {
                if (!loading)
                    console.log("UI State: " + proxyPics.currentState + " user id:"
                                + proxyPics.userId + " paypal webview:" + url)
                else
                    console.log(loadRequest)
            }
        }
    }
}
