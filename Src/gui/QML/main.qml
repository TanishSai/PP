// (c) copyright Positively Front Street, Inc 2016, 2017
// Author: Louis Meadows
// Designer: Katelyn Mattson
// Concept: Luke
import QtQuick 2.5
import Qt.labs.settings 1.0
import QtQuick.Controls 1.2
import QtQuick.LocalStorage 2.0
import QtQuick.Layouts 1.1
import QtQuick.Window 2.2
import QtQml 2.2

Window {
    id: root
    visible: true
    width: Screen.width
    height: Screen.height
    title: "ProxyPics!"

    property string autoLoginState: proxyPics.currentState
    onAutoLoginStateChanged:
    {
        settings.state = autoLoginState;
    }

    Splash {
        id: splash
        anchors.fill: parent
    }

    Loader {
        id: screenLoader
        anchors.fill: parent
        onLoaded: {
            console.log("UI State:" + proxyPics.currentState + " user id:" + proxyPics.userId
                        + " user password " + proxyPics.userPassword + " Loaded: " + source)
        }
    }

    Rectangle {
        id: uistate
        visible: false
        objectName: "uistate"
        color: "transparent"
        state: proxyPics.currentState

        states: [
            State {
                name: "Splash"
                PropertyChanges {
                    target: splash
                    visible: true
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Login"
                }
            },
            State {
                name: "Network Timeout"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/NetworkTimeOut.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Login"
                }
            },
            State {
                name: "NO IMAGE"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/NoImage.qml"
                }
                //Looks like this is not used but it is related to My Request
                PropertyChanges {
                    target: root
                    autoLoginState: "My Requests"
                }
            },
            State {
                name: "Not Authenticated"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/NotAuthenticated.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Login"
                }
            },
            State {
                name: "CREATE-ACCOUNT"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/CreateAccount.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Login"
                }
            },
            State {
                name: "Send Create User Packet"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/SendingCreateUser.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Login"
                }
            },
            State {
                name: "REQUEST PASSWORD"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/RequestPassword.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Login"
                }
            },

            State {
                name: "REGISTRATION SUCCESS"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/Registration_Success.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Login"
                }
            },
            State {
                name: "Already Registered"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/AlreadyRegistered.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Login"
                }
            },

            State {
                name: "Login"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/Login.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Login"
                }
            },
            State {
                name: "Authenticating"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/Authenticating.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Login"
                }
            },
            State {
                name: "Authentication Failure"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/NotAuthenticated.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Login"
                }
            },
            State {
                name: "FORGOT PASSWORD"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/ForgotPassword.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Login"
                }
            },

            State {
                name: "Invalid Job"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/InvalidUser.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Home"
                }
            },

            State {
                name: "Home"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/Home.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Home"
                }
            },
            State {
                name: "Menu"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/Menu.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Menu"
                }
            },
            State {
                name: "How it Works"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/HowItWorks.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Menu"
                }
            },
            State {
                name: "Photo Request"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/PhotoRequestScreen.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Home"
                }
            },
            State {
                name: "Invalid Photo Request"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/InvalidPhotoRequest.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Home"
                }
            },
            State {
                name: "Requested Photos"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/RequestedPhotos.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Home"
                }
            },
            State {
                name: "Terms"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/Terms.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Menu"
                }
            },

            State {
                name: "Settings"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/Settings.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Settings"
                }
            },
            State {
                name: "Social Networks"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/SocialNetworks.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Settings"
                }
            },
            State {
                name: "Notifications"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/Notifications.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Settings"
                }
            },
            State {
                name: "Location Settings"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/LocationSettings.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Settings"
                }
            },
            State {
                name: "Photo Gallery"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/PhotoRequestGallery.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "My Requests"
                }
            },
            State {
                name: "New Photo Request"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/NewPhotoRequest.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Home"
                }
            },
            State {
                name: "Photo Purchase"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/PayPal.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "My Requests"
                }
            },
            State {
                name: "Credit Card"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/SquareUp.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "My Requests"
                }
            },

            State {
                name: "Photo Reject"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/PhotoReject.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "My Requests"
                }
            },
            State {
                name: "Photo Request Preview"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/PhotoRequestPreview.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "My Requests"
                }
            },
            State {
                name: "Request Sent"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/RequestSent.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "My Requests"
                }
            },
            State {
                name: "My Requests"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/MyRequests.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "My Requests"
                }
            },
            State {
                name: "Photo Assignment"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/PhotoAssignmentScreen.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Home"
                }
            },
            State {
                name: "Assignment Details"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/AssignmentDetails.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Home"
                }
            },
            State {
                name: "Accept Assignment"

                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/AcceptAssignment.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Home"
                }
            },
            State {
                name: "My Assignments"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/MyAssignments.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "My Assignments"
                }
            },

            State {
                name: "Current Assignment"

                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/CurrentAssignment.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "My Assignments"
                }
            },
            State {
                name: "User Profile"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/UserProfile.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "Settings"
                }
            },
            State {
                name: "Photo Capture"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/PhotoCapture.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "My Assignments"
                }
            },
            State {
                name: "Photo Preview"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/PhotoPreview.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "My Assignments"
                }
            },

            State {
                name: "Photos Uploading"
                PropertyChanges {
                    target: screenLoader
                    source: "qrc:/Src/gui/QML/PhotosUploading.qml"
                }
                PropertyChanges {
                    target: root
                    autoLoginState: "My Assignments"
                }
            }
        ]
        onStateChanged: {
            updateApplicationSettings()
        }
    }


    //    The Settings type provides persistent platform-independent application settings.
    //    Note: This type is made available by importing the Qt.labs.settings module.
    //    Types in the Qt.labs module are not guaranteed to remain compatible in future versions.
    //    Users normally expect an application to remember its settings across sessions.
    //    The Settings type enables you to save and restore such application settings with the minimum of effort.
    //    Individual setting values are specified by declaring properties within a Settings element.
    //    All basic type properties are supported.
    //    The recommended approach is to use property aliases in order to get automatic property updates both ways.
    Settings {
        id: settings
        property string state
        property string userid
        property string userpassword
        property string useremail
        property string firstname
        property string lastname
        property string mapurl
        property string phonenumber
        property string altitude
        property string latitude
        property string longitude
        property string assignmentxmlfeed
        property string assignmentValue
        property string assignmentname
        property string assignmentaddress
        property string streetaddtess1
        property string streetaddress2
        property string city
        property string assignmentnotes
        property string assignmentTicket
        property string userassignmentcount
        property string postimagepath
        property string postimagename
        property string postimageticket
        property string imagecount

        //AssignmentDetails
       property string currentAssignmentId
       property string currentAssignmentName
       property string currentAssignmentAddress
       property string mapCenter
       property string assignmentPubDate
       property string mapDestination
    }

    Component.onCompleted: {
        proxyPics.uiWidth = Screen.width
        proxyPics.uiHeight = Screen.height

        //Make sure user id and password is set before the current state
        //is changed, becuase some screen's required to access the user name
        // to fetch data.
        proxyPics.userId = settings.userid
        proxyPics.userPassword = settings.userpassword

        proxyPics.imageCount = settings.imagecount
        proxyPics.postImageTicket = settings.postimageticket
        proxyPics.postImageName = settings.postimagename
        proxyPics.postImagePath = settings.postimagepath
        proxyPics.assignmentXMLFeed = settings.assignmentxmlfeed
        proxyPics.mapurl = settings.mapurl
        proxyPics.phoneNumber = settings.phonenumber
        proxyPics.userEmail = settings.useremail
        proxyPics.firstName = settings.firstname
        proxyPics.lastName = settings.lastname
        proxyPics.deviceAltitude = settings.altitude
        proxyPics.deviceLatitude = settings.latitude
        proxyPics.deviceLongitude = settings.longitude
        proxyPics.mapurl = settings.mapurl
        console.log("UI State: " + proxyPics.currentState
                    + " State machine now loaded Screen width: "
                    + proxyPics.uiWidth + " Screen Height: " + proxyPics.uiHeight
                    + " Pixel Density: " + Screen.pixelDensity + " Map Url: "
                    + proxyPics.mapurl + " Phone Number: " + proxyPics.phoneNumber
                    + " User Id: " + proxyPics.userId + " User Password: "
                    + proxyPics.userPassword + " Email: " + proxyPics.userEmail)


        //AssignmentDetails
        proxyPics.currentAssignmentId = settings.currentAssignmentId
        proxyPics.currentAssignmentName = settings.currentAssignmentName
        proxyPics.assignmentValue = settings.assignmentValue
        proxyPics.currentAssignmentAddress=settings.currentAssignmentAddress
        proxyPics.mapCenter = settings.mapCenter
        proxyPics.assignmentTicket = settings.assignmentTicket
        proxyPics.assignmentPubDate = settings.assignmentPubDate
        proxyPics.mapDestination = settings.mapDestination

        //set currentState to end of the all values set
        console.log("Componet on completed ", settings.state)
        autoLoginState = settings.state
        proxyPics.currentState = settings.state //restore last state of proxyPics if there was one

        if (settings.state === "")
            proxyPics.currentState = "Splash"
    }
    Component.onDestruction: {
        console.log(" user id: " + proxyPics.userId)
        console.log(" userpassword: " + proxyPics.userPassword)
        console.log(" phone number:" + proxyPics.phoneNumber)
        console.log(" user email: " + proxyPics.userEmail)
        console.log(" map url: " + proxyPics.mapurl)
        settings.state = proxyPics.currentState
        settings.userid = proxyPics.userId
        settings.userpassword = proxyPics.userPassword
        settings.useremail = proxyPics.userEmail
        settings.mapurl = proxyPics.mapurl
    }


    function updateApplicationSettings()
    {
        //Settings state is update with autoLoginState and this decided based on
        //the current state
        settings.state = autoLoginState;//proxyPics.currentState
        settings.userid = proxyPics.userId
        settings.userpassword = proxyPics.userPassword
        settings.useremail = proxyPics.userEmail
        settings.firstname = proxyPics.firstName
        settings.lastname = proxyPics.lastName
        settings.altitude = proxyPics.deviceAltitude
        settings.latitude = proxyPics.deviceLatitude
        settings.longitude = proxyPics.deviceLongitude
        settings.assignmentxmlfeed = proxyPics.assignmentXMLFeed
        settings.postimagepath = proxyPics.postImagePath
        settings.postimagename = proxyPics.postImageName
        settings.postimageticket = proxyPics.postImageTicket
        settings.imagecount = proxyPics.imageCount

        //AssignmentDetails
        settings.currentAssignmentId = proxyPics.currentAssignmentId
        settings.currentAssignmentName = proxyPics.currentAssignmentName
        settings.assignmentValue = proxyPics.assignmentValue
        settings.currentAssignmentAddress=proxyPics.currentAssignmentAddress
        settings.mapCenter = proxyPics.mapCenter
        settings.assignmentTicket = proxyPics.assignmentTicket
        settings.assignmentPubDate = proxyPics.assignmentPubDate
        settings.mapDestination = proxyPics.mapDestination

    }
} // End of Application Window
