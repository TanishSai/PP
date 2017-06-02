//TODO - Please remove unwanted functions and variables.
//Also move the implementation to source file, this file is
//massive.

#ifndef PROXYPICS_H
#define PROXYPICS_H
#include <QDebug>
#include <QObject>
#include <QTimer>
#include <QSysInfo>
#include <QIcon>
#include <QtGlobal>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlEngine>
#include <QQmlContext>
#include <QQmlContext>
#include <QQuickView>
#include <QQuickItem>
#include <QScreen>
#include <QGeoCoordinate>
#include <QGeoPositionInfoSource>
#include <QNetworkAccessManager>
#include <QUdpSocket>
#include <QtNetwork>
#include <QImageWriter>
#include <QNetworkProxyFactory>
#include <QNetworkAccessManager>
#include <QQmlApplicationEngine>
#include <QXmlStreamWriter>
#include <QTextStream>
#include <QSslConfiguration>
#include <QNetworkProxyFactory>
#include <QHostInfo>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkInterface>
#include <QJsonDocument>
#include <QTcpServer>
#include <QQuickItem>
#include <QQuickView>
#include <QByteArray>
#include <QString>
#include <QTextStream>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
#include <QDomDocument>
#include <QDateTime>
#include <ctime>
#include <iostream>
#include <string>
#include "imageprovider.h"
#define DELIMITER "_@_"

#define SERVER_IP "96.90.223.196"
//#define SERVER_IP "192.168.42.1"
class ProxyPics : public QObject
{
    Q_PROPERTY(int uiWidth READ uiWidth WRITE setUiWidth NOTIFY uiWidthChanged)
    Q_PROPERTY(int uiHeight READ uiHeight WRITE setUiHeight NOTIFY uiHeightChanged)

    Q_PROPERTY(double deviceAltitude READ deviceAltitude WRITE setDeviceAltitude NOTIFY deviceAltitudeChanged)
    Q_PROPERTY(double deviceLatitude READ deviceLatitude WRITE setDeviceLatitude NOTIFY deviceLatitudeChanged)
    Q_PROPERTY(double deviceLongitude READ deviceLongitude WRITE setDeviceLongitude NOTIFY deviceLongitudeChanged)

    Q_PROPERTY(QString mapUrl READ mapUrl WRITE setMapUrl NOTIFY mapUrlChanged)
    Q_PROPERTY(QString mapMode READ mapMode WRITE setmapMode NOTIFY mapModeChanged)
    Q_PROPERTY(QString mapParameters READ mapParameters WRITE setmapParameters NOTIFY mapParametersChanged)
    Q_PROPERTY(QString mapCenter READ mapCenter WRITE setmapCenter NOTIFY mapCenterChanged)
    Q_PROPERTY(QString mapOrigin READ mapOrigin WRITE setmapOrigin NOTIFY mapOriginChanged)
    // origin defines the starting point from which to display directions.
    // The value can be either a place name, address or place ID. The string should be URL-escaped,
    // so an address such as "City Hall, New York, NY" should be converted to City+Hall,New+York,NY.
    // (The Google Maps Embed API supports both + and %20 when escaping spaces.)
    // Place IDs should be prefixed with place_id:. destination defines the end point of the directions.
    Q_PROPERTY(QString mapDestination READ mapDestination WRITE setMapDestination NOTIFY mapDestinationChanged)
    Q_PROPERTY(QString mapZoom READ mapZoom WRITE setmapZoom NOTIFY mapZoomChanged)
    Q_PROPERTY(QString mapType READ mapType WRITE setmapType NOTIFY mapTypeChanged)

    Q_PROPERTY(QString assignmentXMLFeed READ assignmentXMLFeed WRITE setAssignmentXMLFeed NOTIFY assignmentXMLFeedChanged)
    Q_PROPERTY(QString assignmentValue READ assignmentValue WRITE setAssignmentValue NOTIFY assignmentValueChanged)
    Q_PROPERTY(QString assignmentPubDate READ assignmentPubDate WRITE setAssignmentPubDate NOTIFY assignmentPubDateChanged)
    Q_PROPERTY(QString assignmentId READ assignmentId WRITE setAssignmentId NOTIFY assignmentIdChanged)
    Q_PROPERTY(QString assignmentCreatorUserid READ assignmentCreatorUserid WRITE setAssignmentCreatorUserid NOTIFY assignmentCreatorUseridChanged)

    Q_PROPERTY(QString assignmentTicket READ assignmentTicket WRITE setAssignmentTicket NOTIFY assignmentTicketChanged)
    Q_PROPERTY(QString assignmentNotes READ assignmentNotes WRITE setAssignmentNotes NOTIFY assignmentNotesChanged)

    Q_PROPERTY(QString currentAssignmentId READ currentAssignmentId WRITE setCurrentAssignmentId NOTIFY currentAssignmentIdChanged)
    Q_PROPERTY(QString currentAssignmentName READ currentAssignmentName WRITE setCurrentAssignmentName NOTIFY currentAssignmentNameChanged)
    Q_PROPERTY(QString currentAssignmentAddress READ currentAssignmentAddress WRITE setCurrentAssignmentAddress NOTIFY currentAssignmentAddressChanged)
    Q_PROPERTY(bool newUserCreated READ newUserCreated WRITE setNewUserCreated NOTIFY newUserCreatedChanged)
    Q_PROPERTY(bool newJobCreated READ newJobCreated WRITE setNewJobCreated NOTIFY newJobCreatedChanged)
    Q_PROPERTY(QString currentState READ currentState WRITE setCurrentState NOTIFY currentStateChanged)
    Q_PROPERTY(QString streetAddress1 READ streetAddress1 WRITE setStreetAddress1 NOTIFY streetAddress1Changed)
    Q_PROPERTY(QString streetAddress2 READ streetAddress2 WRITE setStreetAddress2 NOTIFY streetAddress2Changed)
    Q_PROPERTY(QString city READ city WRITE setCity NOTIFY cityChanged)
    Q_PROPERTY(QString userId READ UserId WRITE setUserId NOTIFY UserIdChanged)

    // User's infomation accessable from QML using

    // proxyPics.userAssignmentCount =

    Q_PROPERTY(QString firstName READ firstName WRITE setFirstName NOTIFY firstNameChanged)
    Q_PROPERTY(QString lastName READ lastName WRITE setLastName NOTIFY lastNameChanged)
    Q_PROPERTY(QString phoneNumber READ phoneNumber WRITE setPhoneNumber NOTIFY phoneNumberChanged)
    // proxyPics.phoneNumber =
    Q_PROPERTY(QString userPassword READ userPassword WRITE setUserPassword NOTIFY userPasswordChanged)
    Q_PROPERTY(QString userEmail READ userEmail WRITE setUserEmail NOTIFY userEmailChanged)
    Q_PROPERTY(int userAssignmentCount READ userAssignmentCount WRITE setUserAssignmentCount NOTIFY userAssignmentCountChanged)
    Q_PROPERTY(QString imageName READ imageName WRITE setImageName NOTIFY imageNameChanged)
    Q_PROPERTY(QString postImagePath READ postImagePath WRITE setPostImagePath NOTIFY postImagePathChanged)
    Q_PROPERTY(QString postImageName READ postImageName WRITE setPostImageName NOTIFY postImageNameChanged)
    Q_PROPERTY(QString postImageTicket READ postImageTicket WRITE setPostImageTicket NOTIFY postImageTicketChanged)
    Q_PROPERTY(QString receiveImageId READ receiveImageId WRITE setReceiveImageId NOTIFY receiveImageIdChanged)
    Q_PROPERTY(QString receiveImageWithFileName READ receiveImageWithFileName WRITE setReceiveImageWithFileName NOTIFY receiveImageWithFileNameChanged)
    Q_PROPERTY(QString previewSource READ previewSource WRITE setPreviewSource NOTIFY previewSourceChanged)
    Q_PROPERTY(QString imagetitle READ imagetitle WRITE setImagetitle NOTIFY imagetitleChanged)
    Q_PROPERTY(QString imageCount READ imageCount WRITE setimageCount NOTIFY imageCountChanged)
    Q_PROPERTY(QString savedpath READ savedpath WRITE setSavedpath NOTIFY savedpathChanged)
    Q_PROPERTY(bool previewAvailable READ previewAvailable WRITE setPreviewAvailable NOTIFY previewAvailableChanged)
    Q_PROPERTY(QString currentSelectedAssignmentName READ currentSelectedAssignmentName WRITE setCurrentSelectedAssignmentName NOTIFY currentSelectedAssignmentNameChanged)
    Q_OBJECT

public:
    explicit ProxyPics(QObject *parent = 0);
    // invoked from QML
    Q_INVOKABLE static QUrl fromUserInput(const QString& userInput);
    void removeImageDetailFromUser();
    void purchaseImage();
    void requestNewTicketNumber(); // invoked from NewPhotoRequest.qml
    QString constructImageName(const QString &assignmentId, const QString &currentUser);
    void sendCreateNewUser();
    void addAssignmentToUserDetail();
    void addImageDetailForUser();
    void createUserAssignmentDetails();
    void updateMyAssignmentForPixUpload();
    void updateMyRequestPixPurchased();
    int uiWidth() const;
    int uiHeight() const;
    bool userAssignmentCount();
    bool isWaterMarkedRequired;
    QString imageFileName;
    QObject * proxyPicsUI;
    QString mapMode() const;
    QString mapParameters() const;
    QString mapCenter() const;
    QString mapOrigin() const;
    QString mapDestination() const;
    QString mapZoom() const;
    QString mapType() const;
    QString value() const;
    QGeoCoordinate geographical_position;
    //TODO - Seems this function is unsed, if it is required keep this, if not remove.
    QString macAddress();
    QString ipAddress();
    bool CheckPoint(QByteArray checkdata);
    QString currentAssignmentId() const;
    QString currentAssignmentName() const;
    QString currentAssignmentAddress() const;
    QString assignmentTicket() const;
    QString assignmentNotes() const;
    QString currentState() const;
    QString userPassword() const;
    QString userEmail() const;
    QString assignmentCreatorUserid() const;
    QString phoneNumber() const;
    QString imageName() const;
    bool newUserCreated() const;
    bool newJobCreated() const;
    QString streetAddress1() const;
    QString streetAddress2() const;
    QString city() const;
    QString UserId() const;
    QString mapUrl() const;
    double deviceAltitude() const;
    double deviceLatitude() const;
    double deviceLongitude() const;
    QString assignmentValue() const;
    QString assignmentPubDate() const;
    bool validateUser(const QString &userId, const QString &password);
    Q_INVOKABLE void postRequest(const QString &assignmentname, const QString &ticket, const QString &id, const QString &address, const int value, const QString &note);
    //TODO - This function is same as removeImageDetailFromUser - Move the implementation to
    // it and remove this function.
    bool removeImageDetail(const QString &userid, QString imageName, const QString &assignmentName, const QString &id);
    QString getTicketNumberFromId(const QString &id);
    ////TODO - Looks like this function is not used, please remove if so
    void addImageDetail(const QString &userid, const QString &imageName, const QString &assignmentName, const QString &id);
    bool writeXMLFile(QIODevice *device);
    QString assignmentXMLFeed() const;
    QString firstName() const;
    QString lastName() const;
    QString postImagePath() const;
    QString postImageName() const;
    QString postImageTicket() const;
    QString receiveImageId() const;
    QString receiveImageWithFileName() const;
    const double Pi = 3.14159265358979323846264338327950288419717;

    Q_INVOKABLE void resetOnLogout();

signals:
    void imageCountChanged(QString imageCount);
    void assignmentCount(int count);
    void imageDownloaded();
    void userCreatedResult(bool flag);
    void validateUserInfo(bool flag);
    void userAssignmentCountChanged();
    void assignmentTicketNumber(QString ticketNumber);

    /*!
      * A signal emitted when the login is performed.
      * \param username the username entered in the dialog
      * \param password the password entered in the dialog
      * \param index the number of the username selected in the combobox
      */
    void acceptLogin( QString& username, QString& password);
    void userLogoff();
    void notLogined();
    void successfullyLogined();
    void loggedIn();
    void notLoggedIn(QString errorMessage);
    void loggedOut();
    void rawResponse(QByteArray raw,QString error);
    void jsonResponse(QJsonDocument doc);

    void error(QAbstractSocket::SocketError socketError);

    void proxyAuthenticationRequired(const QNetworkProxy & proxy, QAuthenticator * authenticator);
    void stateChanged(QAbstractSocket::SocketState socketState);
    void newPhoto(QString photo);

    void uiWidthChanged(int uiWidth);

    void uiHeightChanged(int uiHeight);

    void mapModeChanged(QString mapMode);

    void mapParametersChanged(QString mapParameters);

    void mapCenterChanged(QString mapCenter);

    void mapOriginChanged(QString mapOrigin);

    void mapDestinationChanged(QString mapDestination);

    void mapZoomChanged(QString mapZoom);

    void mapTypeChanged(QString mapType);

    void currentAssignmentIdChanged(QString currentAssignmentId);

    void currentAssignmentNameChanged(QString currentAssignmentName);

    void currentAssignmentAddressChanged(QString currentAssignmentAddress);

    void assignmentTicketChanged(QString assignmentTicket);

    void assignmentNotesChanged(QString assignmentNotes);

    void currentStateChanged(QString currentState);

    void userPasswordChanged(QString userPassword);

    void userEmailChanged(QString userEmail);

    void assignmentCreatorUseridChanged(QString assignmentCreatorUserid);

    void phoneNumberChanged(QString phoneNumber);

    void imageNameChanged(QString imageName);

    void newUserCreatedChanged(bool newUserCreated);
    void newJobCreatedChanged(bool newJobCreated);

    void streetAddress1Changed(QString streetAddress1);

    void streetAddress2Changed(QString streetAddress2);

    void cityChanged(QString city);

    void UserIdChanged(QString UserId);

    void mapUrlChanged(QString mapUrl);

    void deviceAltitudeChanged(double deviceAltitude);

    void deviceLatitudeChanged(double deviceLatitude);

    void deviceLongitudeChanged(double deviceLongitude);

    void assignmentValueChanged(QString assignmentValue);

    void assignmentPubDateChanged(QString assignmentPubDate);

    void assignmentXMLFeedChanged(QString assignmentXMLFeed);

    void firstNameChanged(QString firstName);

    void lastNameChanged(QString lastName);

    void postImagePathChanged(QString postImagePath);

    void postImageNameChanged(QString postImageName);

    void postImageTicketChanged(QString postImageTicket);

    void receiveImageIdChanged(QString receiveImageId);

    void receiveImageWithFileNameChanged(QString receiveImageWithFileName);

    void previewSourceChanged(QString previewSource);

    void imagetitleChanged(QString imagetitle);

    void savedpathChanged(QString savedpath);

    void previewAvailableChanged(bool previewAvailable);

    void currentSelectedAssignmentNameChanged(QString currentSelectedAssignmentName);

    void assignmentIdChanged(QString assignmentId);

public slots:
    QString getDifferenceWithCurrentTime(const QString &dateTime);
    void saveLoggedInUserDetails();
    void clearSavedUserLoginDetails();
    void connected();
    void disconnected();
    void pixSrverDisconnected();
    void hostFound();
    void requestFinished(QNetworkReply *reply);
    void photoCaptured(QString photograph);
    void picRequest(QString picAdrress);
    void updateStateMachine(QString currentState);
    void positionUpdated(const QGeoPositionInfo &info);
    void readSocket();
    bool updateAccomplishedCount(const QString &imageName, const QString &ticket);
    bool updatePurchasedDetailToFeed(QString &imageName, const QString &ticket);
    void saveImage();
    void disconnectAllPixSharing();
    void pictureReadyRead();
    void bytesWritten(qint64 bytesWritten);
    void picServerBytesWritten(qint64 bytesWritten);
    void readPendingDatagrams();
    //TODO - Looks like this function is unsed, if it not used please remove it.
    bool saveSingleImageData();
    void readUserDetailSocket();

    // ProxyPics Setters
    void setUiHeight(const int uiHeight);
    void setUiWidth(const int uiWidth);
    void setUserAssignmentCount(const int count);
    void setassignmentXMLFeed(const QString assignmentXMLFeed);
    void setAssignmentXMLFeed(const QString assignmentXMLFeed);
    void setfirstName(const QString firstName);
    void setlastName(const QString lastName);
    void setFirstName(const QString firstName);
    void setLastName(const QString lastName);
    void setmapMode(const QString mapMode);
    void setmapParameters(const QString mapParameters);
    void setmapCenter(const QString mapCenter);
    void setmapOrigin(const QString mapOrigin);
    void setmapZoom(const QString mapZoom);
    void setmapType(const QString mapType);
    void setCurrentAssignmentName(const QString currentAssignmentName);
    void setCurrentAssignmentAddress(const QString currentAssignmentAddress);
    void setAssignmentTicket(const QString assignmentTicket);
    void setAssignmentNotes(QString assignmentNotes);    
    void setUserPassword(const QString userPassword);
    void setCurrentState(const QString currentState);
    void setUserEmail(const QString userEmail);
    void setassignmentCreatorUserid(const QString assignmentCreatorUserid);
    void setPhoneNumber(const QString phoneNumber);
    void setCurrentAssignmentId(const QString currentAssignmentId);
    void setAssignmentCreatorUserid(const QString assignmentCreatorUserid);
    void setImageName(const QString imageName);
    void setNewUserCreated(bool newUserCreated);
    void setNewJobCreated(bool newJobCreated);
    void setstreetAddress1(const QString streetAddress1);
    void setStreetAddress2(const QString streetAddress2);
    void setCity(const QString city);
    void setUserId(const QString UserId);
    void setStreetAddress1(const QString streetAddress1);
    void setMapUrl(const QString mapUrl);
    void setDeviceAltitude(const double deviceAltitude);
    void setDeviceLatitude(const double deviceLatitude);
    void setDeviceLongitude(const double deviceLongitude);
    void setAssignmentValue(const QString assignmentValue);
    void setAssignmentPubDate(const QString assignmentPubDate);
    void setPostImagePath(const QString postImagePath);
    void setPostImageName(QString postImageName, bool fullPath = false);
    void setPostImageTicket(const QString postImageTicket);
    void setReceiveImageId(QString receiveImageId);
    void setReceiveImageWithFileName(QString receiveImageWithFileName);
    //TODO move generated code out of header file
    void setpreviewSource(QString previewSource)
    {
        if (m_previewSource == previewSource)
            return;

        m_previewSource = previewSource;
        emit previewSourceChanged(previewSource);
    }

    void setimagetitle(QString imagetitle)
    {
        if (m_imagetitle == imagetitle)
            return;

        m_imagetitle = imagetitle;
        emit imagetitleChanged(imagetitle);
    }

    void setpreviewAvailable(bool previewAvailable)
    {
        if (m_previewAvailable == previewAvailable)
            return;

        m_previewAvailable = previewAvailable;
        emit previewAvailableChanged(previewAvailable);
    }

    void setcurrentSelectedAssignmentName(QString currentSelectedAssignmentName)
    {
        if (m_currentSelectedAssignmentName == currentSelectedAssignmentName)
            return;

        m_currentSelectedAssignmentName = currentSelectedAssignmentName;
        emit currentSelectedAssignmentNameChanged(currentSelectedAssignmentName);
    }

    void setassignmentId(QString assignmentId)
    {
        if (m_assignmentId == assignmentId)
            return;

        m_assignmentId = assignmentId;
        emit assignmentIdChanged(assignmentId);
    }

    void setimageCount(QString imageCount)
    {
        if (m_imageCount == imageCount)
            return;

        m_imageCount = imageCount;
        emit imageCountChanged(imageCount);
    }

    void setAssignmentId(QString assignmentId)
    {
        if (m_assignmentId == assignmentId)
            return;

        m_assignmentId = assignmentId;
        emit assignmentIdChanged(assignmentId);
    }

    void setPreviewSource(QString previewSource)
    {
        if (m_previewSource == previewSource)
            return;

        m_previewSource = previewSource;
        emit previewSourceChanged(previewSource);
    }

    void setImagetitle(QString imagetitle)
    {
        if (m_imagetitle == imagetitle)
            return;

        m_imagetitle = imagetitle;
        emit imagetitleChanged(imagetitle);
    }

    void setSavedpath(QString savedpath)
    {
        if (m_savedpath == savedpath)
            return;

        m_savedpath = savedpath;
        emit savedpathChanged(savedpath);
    }

    void setPreviewAvailable(bool  previewAvailable)
    {
        if (m_previewAvailable == previewAvailable)
            return;

        m_previewAvailable = previewAvailable;
        emit previewAvailableChanged(previewAvailable);
    }

    void setCurrentSelectedAssignmentName(QString currentSelectedAssignmentName)
    {
        if (m_currentSelectedAssignmentName == currentSelectedAssignmentName)
            return;

        m_currentSelectedAssignmentName = currentSelectedAssignmentName;
        emit currentSelectedAssignmentNameChanged(currentSelectedAssignmentName);
    }

    void m_UserDetailSocket_connected();
    void m_socket_connected();
    void tcpSockect_connected();
    void udpSocketError(QAbstractSocket::SocketError socketError);
    void tcpSocketError(QAbstractSocket::SocketError socketError);
    void m_SocketError(QAbstractSocket::SocketError socketError);
    void m_PixShareSocketError(QAbstractSocket::SocketError socketError);
    void m_UserDetailSocketError(QAbstractSocket::SocketError socketError);
    void udpBytesWritten(qint64 bytesWritten);
    void tcpBytesWritten(qint64 bytesWritten);
    void userDetailBytesWritten(qint64 bytesWritten);
    double getDistanceFromCurrentPos(double lat1d, double lon1d);
    QString saveCapturePixLocally(QString path);
    bool isPixUploadedToServer(const QString& image);
    void receiveImageNameList(QString id);

public:
    QString previewSource() const
    {
        return m_previewSource;
    }

    QString imagetitle() const
    {
        return m_imagetitle;
    }

    QString savedpath() const
    {
        return m_savedpath;
    }


    bool previewAvailable() const
    {
        return m_previewAvailable;
    }

    QString currentSelectedAssignmentName() const
    {
        return m_currentSelectedAssignmentName;
    }

    QString assignmentId() const
    {
        return m_assignmentId;
    }

    QString imageCount() const
    {
        return m_imageCount;
    }

private:
    // This function converts decimal degrees to radians
    double deg2rad(double deg);

    double distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d);
    void writeDataToPixServer(const char* data);

private:
    QByteArray newData;
    QString dataString;
    QString                 localHostName;

    QSslConfiguration       sslConfiguration;
    QNetworkAccessManager * netManager;
    QNetworkRequest         request;
    QTcpSocket* m_UserDetailSocket = NULL;
    QTcpSocket* m_PixShareSocket = NULL;
    // Return current software version.
    static QString SoftwareVersion();
    QScreen * uiScreen; // need to know a little bit about the screen
    QSize uiSize;       // so we can lay the gui out correctly
    qreal uiPixelRatio;
    QRect uiGeomety;
    bool isImageDownLoaded(const QString &imageName);
    QString picDirPath;
    QDir imageDir;
    QString imagePath;
    QFileInfo info;
    QImage image;
    QString m_idImageRequestedFor;
    QStringList m_imageNameList;
    int ImageSaveCounter;
    QString m_imageRequestedFor;
    QByteArray m_collectedData;
    QByteArray packet;
    QString m_msg;
    QString m_state;   // current state of the user interface
    QString m_photo;   // current photograph captured by camera
    QQmlApplicationEngine * engine;
    bool                    loggedOn;
    quint16 sessionport;
    QString                      * hostIP;
    QHostAddress                   hostAddress;
    QByteArray                     wsse;
    QTcpServer                   * tcpServer;
    QTcpSocket                   * m_socket = NULL;
    QTcpSocket                   * tcpSocket = NULL;
    QNetworkAccessManager        * accessManager;
    QString                        userName;
    QString                        userPass;
    QByteArray USER;
    QByteArray PASSWORD;
    QByteArray RAWHEADER;
    QByteArray PASSWORDDIGEST;
    QByteArray NONCE;
    QByteArray CREATED;
    QString state;
    QTimer *timer;
    QQmlContext *cntx;
    int m_uiWidth;
    int m_uiHeight;
    QString pictureAddress;

    QGeoPositionInfoSource *source;
    QGeoCoordinate geoCoordinate;

    QByteArray checkdata;
    const QString googleApiKey;
    QString m_mapMode;
    QString m_mapParameters;
    QString m_mapCenter;
    QString m_mapOrigin;
    QString m_mapDestination;
    QString m_mapZoom;
    QString m_mapType;
    QString m_value;
    QByteArray data;
    quint16 checkpointport;
    bool bindSuccess;
    QUdpSocket                * udpSocket;
    QHostAddress proxyPicsHost;
    QString m_whereiam;
    double m_longitude;
    double m_latitude;
    //QString m_userid;

    QString m_currentAssignmentId;

    QString m_currentAssignmentName;
    QString m_currentAssignmentAddress;
    int m_userAssignmentCount;
    QString m_assignmentTicket;
    QString m_assignmentNotes;
    QString m_currentState;
    QString m_userPassword;
    QString m_userEmail;
    QString m_assignmentCreatorUserid;
    QString m_phoneNumber;
    QString m_imageName;
    bool m_newUserCreated;
    bool m_newJobCreated;
    QString m_streetAddress1;

    void setMapDestination(QString mapDestination);

    QString m_streetAddress2;
    QString m_city;
    QString m_UserId;
    QString m_mapUrl;
    double m_deviceAltitude;
    double m_deviceLatitude;
    double m_deviceLongitude;
    qint64 bytes_Written;
    QString checkString;
    QString m_assignmentValue;
    QString m_assignmentPubDate;
    QString m_assignmentXMLFeed;
    QString m_firstName;
    QString m_lastName;
    QString m_postImagePath;
    QString m_postImageName;
    QString m_postImageTicket;
    QString m_receiveImageId;
    QString m_receiveImageWithFileName;
    void create_connect_m_socket();
    void create_connect_m_UserDetailSocket();
    void create_connect_tcpSocket();
    void create_connect_udpSocket();
    void create_connect_m_PixShareSocket();
    QString m_previewSource;
    QString m_imagetitle;

    QString m_savedpath;

    bool m_previewAvailable;
    QString m_currentSelectedAssignmentName;
    QString m_assignmentId;

    QString m_imageCount;
    QString proxyPicDirPath;
    QString filePath;
    int m_imageByteSize;
    bool imageDataDownloadStarted = false;
};

#endif // PROXYPICS_H
