#include "proxypics.h"
#include <cmath>
#define earthRadiusKm 6371.0

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::ProxyPics
 * @param parent
 */

ProxyPics::ProxyPics(QObject *parent) : QObject(parent)
{
    data = QByteArray("ProxyPics");

    m_userAssignmentCount = -1;
    proxyPicsHost = QHostAddress(SERVER_IP);
    QNetworkProxyFactory::setUseSystemConfiguration(true);  // setup Network


    sslConfiguration.setProtocol(QSsl::SecureProtocols );
    hostAddress = QHostAddress(SERVER_IP);

    qDebug() <<__FUNCTION__<<__LINE__<< "hostIP" << hostAddress;


    QList<QHostAddress> ipAddressesList = QNetworkInterface::allAddresses();

    // use the first non-localhost IPv4 address
    QString tmp;
    for (int i = 0; i < ipAddressesList.size(); ++i) {
        qDebug()<<__FUNCTION__<<__LINE__ << ipAddressesList.at(i).toString();
        if (ipAddressesList.at(i) != QHostAddress::LocalHost &&
                ipAddressesList.at(i).toIPv4Address()) {
            tmp = ipAddressesList.at(i).toString();
            break;
        }
    }
    // if we did not find one, use IPv4 Any
    if (tmp.isEmpty())
        tmp = QHostAddress(QHostAddress::AnyIPv4).toString();
//UDP
    create_connect_udpSocket();  // Checkpoints, tracking data (user location)
//TCP/IP
    create_connect_m_socket();
    create_connect_tcpSocket();
    create_connect_m_PixShareSocket();
    create_connect_m_UserDetailSocket();
//HTTP
    netManager       = new QNetworkAccessManager();
    connect(netManager,
                SIGNAL(finished(QNetworkReply *)),
                SLOT(requestFinished(QNetworkReply *)));
    netManager->connectToHost("www.proxypics.com");
    qDebug()<<__FUNCTION__<<__LINE__<<"Network Manager Connected"<< netManager;
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::create_connect_m_socket
 */
void ProxyPics::create_connect_m_socket()
{
    m_socket = new QTcpSocket(this);

    connect(m_socket, SIGNAL(hostFound()), this, SLOT(hostFound()));
    connect(m_socket, SIGNAL(connected()), this, SLOT(m_socket_connected()));
    connect(m_socket, SIGNAL(disconnected()), this, SLOT(disconnected()));
    connect(m_socket, SIGNAL(readyRead()),
            this, SLOT(readSocket()));
    connect(m_socket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(m_SocketError(QAbstractSocket::SocketError)));
    connect(m_socket, SIGNAL(bytesWritten(qint64)),
            this, SLOT(bytesWritten(qint64)));
    m_socket->connectToHost(SERVER_IP, 8800);
    m_socket->waitForConnected(1000);

}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::create_connect_m_UserDetailSocket
 */
void ProxyPics::create_connect_m_UserDetailSocket()
{
    m_UserDetailSocket = new QTcpSocket(this);

    connect(m_UserDetailSocket, SIGNAL(hostFound()),
            this, SLOT(hostFound()));
    connect(m_UserDetailSocket, SIGNAL(connected()),
            this, SLOT(m_UserDetailSocket_connected()));
    connect(m_UserDetailSocket, SIGNAL(disconnected()),
            this, SLOT(disconnected()));
    connect(m_UserDetailSocket, SIGNAL(readyRead()),
            this, SLOT(readUserDetailSocket()));
    connect(m_UserDetailSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(m_UserDetailSocketError(QAbstractSocket::SocketError)));
    connect(m_UserDetailSocket, SIGNAL(bytesWritten(qint64)),
            this, SLOT(userDetailBytesWritten(qint64)));
    m_UserDetailSocket->bind(hostAddress,6600,QAbstractSocket::ShareAddress);
    m_UserDetailSocket->connectToHost(hostAddress,6600);
    m_UserDetailSocket->waitForConnected(1000);
    bool socketOpen = m_UserDetailSocket->open(QIODevice::ReadWrite);
    if (!socketOpen) {
        qDebug()<<__FUNCTION__<<__LINE__
               <<" Could not create socket"
              <<m_UserDetailSocket->error()
             <<m_UserDetailSocket->errorString();
        return;
    } else {
        qDebug()<<__FUNCTION__<<__LINE__<<"m_UserDetailSocket Created Successfully";
    }
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::create_connect_tcpSocket
 */
void ProxyPics::create_connect_tcpSocket()

{
    tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, SIGNAL(hostFound()),
            this, SLOT(hostFound()));
    connect(tcpSocket, SIGNAL(connected()),
            this, SLOT(tcpSockect_connected()));
    connect(tcpSocket, SIGNAL(disconnected()),
            this, SLOT(disconnected()));
    connect(tcpSocket, SIGNAL(readyRead()),
            this, SLOT(readUserDetailSocket()));
    connect(tcpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(tcpSocketError(QAbstractSocket::SocketError)));
    connect(tcpSocket, SIGNAL(bytesWritten(qint64)),
            this, SLOT(tcpBytesWritten(qint64)));
    tcpSocket->bind(hostAddress,8800,QAbstractSocket::ShareAddress);
    tcpSocket->connectToHost(hostAddress,8800);
    tcpSocket->waitForConnected(1000);

    bool socketOpen = tcpSocket->open(QIODevice::ReadWrite);

    if (!socketOpen) {
        qDebug()<<__FUNCTION__<<__LINE__<<" Could not create socket"<<tcpSocket->error()<<tcpSocket->errorString();
        return;
    } else {
        qDebug()<<__FUNCTION__<<__LINE__<<"Socket Created Successfully";

    }


}


/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::create_connect_udpSocket
 */
void ProxyPics::create_connect_udpSocket()

{
    checkpointport = 7700; //39805;
    udpSocket      = new QUdpSocket(this);

    connect(udpSocket, SIGNAL(error(QAbstractSocket::SocketError)),
            this, SLOT(udpSocketError(QAbstractSocket::SocketError)));
    connect(udpSocket, SIGNAL(bytesWritten(qint64)),
            this, SLOT(udpBytesWritten(qint64)));

    udpSocket->bind(checkpointport,QUdpSocket::ShareAddress);
    bytes_Written = udpSocket->writeDatagram(data.data(),data.size(),proxyPicsHost,checkpointport);

}
/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::create_connect_m_PixShareSocket
 */
void ProxyPics::create_connect_m_PixShareSocket()

{
    if (m_PixShareSocket == NULL)
    {
        m_PixShareSocket = new QTcpSocket(this);

        connect(m_PixShareSocket, SIGNAL(readyRead()), this, SLOT(pictureReadyRead()));

//        connect(m_PixShareSocket, SIGNAL(disconnected()), this, SLOT(pixSrverDisconnected()));
        connect(m_PixShareSocket, SIGNAL(error(QAbstractSocket::SocketError)),
                this, SLOT(m_PixShareSocketError(QAbstractSocket::SocketError)));
        connect(m_PixShareSocket, SIGNAL(bytesWritten(qint64)),
                this, SLOT(picServerBytesWritten(qint64)));
    }

    m_PixShareSocket->connectToHost(SERVER_IP, 9900);
    m_PixShareSocket->waitForConnected(1000);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::bytesWritten : This slot is executed when bytes
 * were written to socket.
 * @param bytesWritten : Number of bytes written
 */
void ProxyPics::udpBytesWritten(qint64 bytesWritten)
{
    qDebug()<<__FUNCTION__<<__LINE__<<bytesWritten<<" Bytes Written";
    bytes_Written = bytesWritten;
}
/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::bytesWritten : This slot is executed when bytes
 * were written to socket.
 * @param bytesWritten : Number of bytes written
 */
void ProxyPics::userDetailBytesWritten(qint64 bytesWritten)
{
    qDebug()<<__FUNCTION__<<__LINE__<<bytesWritten<<" Bytes Written";
    bytes_Written = bytesWritten;
}

double ProxyPics::getDistanceFromCurrentPos(double lat1d, double lon1d)
{
    qDebug() << "deviceLatitude " << deviceLatitude() << " deviceLongitude " << deviceLongitude();
    return distanceEarth(lat1d, lon1d, deviceLatitude(), deviceLongitude());
}

// this funtion only called while photographer take pic and saves in local spae
// Means these are the refference pic and not yet uploaded
QString ProxyPics::saveCapturePixLocally(QString path)
{
    QFile file(path);
    if (file.exists())
    {
        QString picDirPath = QDir::homePath();
        picDirPath.append("/PPImages");
        QDir imageDir(picDirPath);
        if(!imageDir.exists())
        {
            QDir dir = QDir::home();
            dir.mkdir(picDirPath);
        }

        QString newImagePath = QString("%1/not_uploaded%2.JPG").arg(picDirPath, constructImageName(currentAssignmentId(),UserId()));
        QFile::copy(path, newImagePath);
        file.remove();
        return newImagePath;
    }

    return QString();
}

bool ProxyPics::isPixUploadedToServer(const QString &image)
{
    return image.startsWith("not_uploaded");
}

void ProxyPics::receiveImageNameList(QString id)
{
    writeDataToPixServer((QString("imageNameList&&%1").arg(id.remove(" "))).toStdString().c_str());
}

double ProxyPics::deg2rad(double deg)
{
    return (deg * Pi / 180);
}

/**
 * Returns the distance between two points on the Earth.
 * Direct translation from http://en.wikipedia.org/wiki/Haversine_formula
 * @param lat1d Latitude of the first point in degrees
 * @param lon1d Longitude of the first point in degrees
 * @param lat2d Latitude of the second point in degrees
 * @param lon2d Longitude of the second point in degrees
 * @return The distance between the two points in kilometers
 */
double ProxyPics::distanceEarth(double lat1d, double lon1d, double lat2d, double lon2d)
{
    double lat1r, lon1r, lat2r, lon2r, u, v;
    lat1r = deg2rad(lat1d);
    lon1r = deg2rad(lon1d);
    lat2r = deg2rad(lat2d);
    lon2r = deg2rad(lon2d);
    u = sin((lat2r - lat1r)/2);
    v = sin((lon2r - lon1r)/2);
    return 2.0 * earthRadiusKm * asin(sqrt(u * u + cos(lat1r) * cos(lat2r) * v * v));
}

void ProxyPics::writeDataToPixServer(const char *data)
{
    if (m_PixShareSocket == NULL || m_PixShareSocket->state() == QAbstractSocket::UnconnectedState)
    {
        create_connect_m_PixShareSocket();
    }

    m_PixShareSocket->write(data);
}
/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::bytesWritten : This slot is executed when bytes
 * were written to socket.
 * @param bytesWritten : Number of bytes written
 */
void ProxyPics::tcpBytesWritten(qint64 bytesWritten)
{
    qDebug()<<__FUNCTION__<<__LINE__<<bytesWritten<<" Bytes Written";
    bytes_Written = bytesWritten;
}
/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::bytesWritten : This slot is executed when bytes
 * were written to socket.
 * @param bytesWritten : Number of bytes written
 */
void ProxyPics::bytesWritten(qint64 bytesWritten)
{
    qDebug()<<__FUNCTION__<<__LINE__<<bytesWritten<<" Bytes Written";
    bytes_Written = bytesWritten;
}

void ProxyPics::picServerBytesWritten(qint64 bytesWritten)
{
    qDebug()<<__FUNCTION__<<__LINE__<<bytesWritten<<" Bytes Written "<<m_imageByteSize;
    bytes_Written = bytesWritten;
    if (m_imageByteSize != 0 && m_imageByteSize == bytesWritten)
    {
        m_PixShareSocket->close();
        m_imageByteSize = 0;
    }
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::udpSocketError  This slot is executed when an error
 * has occured with socket
 * @param socketError Type of error
 */
void ProxyPics::udpSocketError(QAbstractSocket::SocketError socketError)
{
    if (udpSocket != NULL)
        qDebug()<<__FUNCTION__<<__LINE__<<"Socket"<<udpSocket->errorString();
    else
        qDebug()<<__FUNCTION__<<__LINE__<<"Warning udpSocket is Null";

    switch (socketError) {

    case QAbstractSocket::RemoteHostClosedError:
        qDebug()<<__FUNCTION__<<__LINE__<<"Remote Host Closed";
        break;

    case QAbstractSocket::HostNotFoundError:
        qDebug()<<__FUNCTION__<<__LINE__<<"The host was not found. Please check the "
                                        "host name and port settings.";
        break;

    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<__FUNCTION__<<__LINE__<<"The connection was refused by the peer. "
                                        "Make sure the server is running, "
                                        "and check that the host name and port "
                                        "settings are correct.";
        break;

    case QAbstractSocket::UnknownSocketError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketAccessError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketResourceError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketTimeoutError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::DatagramTooLargeError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::NetworkError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::AddressInUseError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketAddressNotAvailableError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::UnsupportedSocketOperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::UnfinishedSocketOperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::OperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyAuthenticationRequiredError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionClosedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionRefusedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionTimeoutError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyNotFoundError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyProtocolError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslHandshakeFailedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslInternalError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslInvalidUserDataError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::TemporaryError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    }


}


/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::udpSocketError  This slot is executed when an error
 * has occured with socket
 * @param socketError Type of error
 */
void ProxyPics::tcpSocketError(QAbstractSocket::SocketError socketError)
{
    if (tcpSocket != NULL)
        qDebug()<<__FUNCTION__<<__LINE__<<"Socket"<<tcpSocket->errorString();
    else
        qDebug()<<__FUNCTION__<<__LINE__<<"Warning tcpSocket is Null";

    switch (socketError) {

    case QAbstractSocket::RemoteHostClosedError:
        qDebug()<<__FUNCTION__<<__LINE__<<"Remote Host Closed";
        break;

    case QAbstractSocket::HostNotFoundError:
        qDebug()<<__FUNCTION__<<__LINE__<<"The host was not found. Please check the "
                                        "host name and port settings.";
        break;

    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<__FUNCTION__<<__LINE__<<"The connection was refused by the peer. "
                                        "Make sure ProxyPics tcp listner on server is running, "
                                        "and check that the host name and port "
                                        "settings are correct.";
        break;

    case QAbstractSocket::UnknownSocketError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketAccessError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketResourceError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketTimeoutError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::DatagramTooLargeError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::NetworkError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::AddressInUseError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketAddressNotAvailableError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::UnsupportedSocketOperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::UnfinishedSocketOperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::OperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyAuthenticationRequiredError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionClosedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionRefusedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionTimeoutError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyNotFoundError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyProtocolError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslHandshakeFailedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslInternalError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslInvalidUserDataError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::TemporaryError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    }


}


/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::m_PixShareSocketError  This slot is executed when an error
 * has occured with socket
 * @param socketError Type of error
 */
void ProxyPics::m_PixShareSocketError(QAbstractSocket::SocketError socketError)
{
    if (tcpSocket != NULL)
        qDebug()<<__FUNCTION__<<__LINE__<<"Socket"<<m_PixShareSocket->errorString();
    else
        qDebug()<<__FUNCTION__<<__LINE__<<"Warning m_PixShareSocket is Null";

    switch (socketError) {

    case QAbstractSocket::RemoteHostClosedError:
        qDebug()<<__FUNCTION__<<__LINE__<<"Remote Host Closed";
        break;

    case QAbstractSocket::HostNotFoundError:
        qDebug()<<__FUNCTION__<<__LINE__<<"The host was not found. Please check the "
                                        "host name and port settings.";
        break;

    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<__FUNCTION__<<__LINE__<<"The connection was refused by the peer. "
                                        "Make sure the ProxyPics Picture server is running, "
                                        "and check that the host name and port "
                                        "settings are correct.";
        break;

    case QAbstractSocket::UnknownSocketError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketAccessError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketResourceError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketTimeoutError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::DatagramTooLargeError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::NetworkError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::AddressInUseError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketAddressNotAvailableError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::UnsupportedSocketOperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::UnfinishedSocketOperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::OperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyAuthenticationRequiredError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionClosedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionRefusedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionTimeoutError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyNotFoundError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyProtocolError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslHandshakeFailedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslInternalError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslInvalidUserDataError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::TemporaryError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    }


}


/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::m_PixShareSocketError  This slot is executed when an error
 * has occured with socket
 * @param socketError Type of error
 */
void ProxyPics::m_UserDetailSocketError(QAbstractSocket::SocketError socketError)
{
    if (tcpSocket != NULL)
        qDebug()<<__FUNCTION__<<__LINE__<<"Socket"<<m_UserDetailSocket->errorString();
    else
        qDebug()<<__FUNCTION__<<__LINE__<<"Warning m_UserDetailSocket is Null";

    switch (socketError) {

    case QAbstractSocket::RemoteHostClosedError:
        qDebug()<<__FUNCTION__<<__LINE__<<"Remote Host Closed";
        break;

    case QAbstractSocket::HostNotFoundError:
        qDebug()<<__FUNCTION__<<__LINE__<<"The host was not found. Please check the "
                                        "host name and port settings.";
        break;

    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<__FUNCTION__<<__LINE__<<"The connection was refused by the peer. "
                                        "Make sure the userDetail listener on server is running, "
                                        "and check that the host name and port "
                                        "settings are correct.";
        break;

    case QAbstractSocket::UnknownSocketError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketAccessError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketResourceError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketTimeoutError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::DatagramTooLargeError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::NetworkError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::AddressInUseError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketAddressNotAvailableError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::UnsupportedSocketOperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::UnfinishedSocketOperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::OperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyAuthenticationRequiredError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionClosedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionRefusedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionTimeoutError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyNotFoundError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyProtocolError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslHandshakeFailedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslInternalError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslInvalidUserDataError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::TemporaryError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    }


}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::socketError  This slot is executed when an error
 * has occured with socket
 * @param socketError Type of error
 */
void ProxyPics::m_SocketError(QAbstractSocket::SocketError socketError)
{

    if (m_socket != NULL)
        qDebug()<<__FUNCTION__<<__LINE__<<"Socket"<<m_socket->errorString();
    else
        qDebug()<<__FUNCTION__<<__LINE__<<"Warning m_socket is Null";


    switch (socketError) {

    case QAbstractSocket::RemoteHostClosedError:
        qDebug()<<__FUNCTION__<<__LINE__<<"Remote Host Closed";
        break;

    case QAbstractSocket::HostNotFoundError:
        qDebug()<<__FUNCTION__<<__LINE__<<"The host was not found. Please check the "
                                        "host name and port settings.";
        break;

    case QAbstractSocket::ConnectionRefusedError:
        qDebug()<<__FUNCTION__<<__LINE__<<"The connection was refused by the peer. "
                                        "Make sure the server is running, "
                                        "and check that the host name and port "
                                        "settings are correct.";
        break;

    case QAbstractSocket::UnknownSocketError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketAccessError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketResourceError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketTimeoutError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::DatagramTooLargeError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::NetworkError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::AddressInUseError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SocketAddressNotAvailableError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::UnsupportedSocketOperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::UnfinishedSocketOperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::OperationError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyAuthenticationRequiredError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionClosedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionRefusedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyConnectionTimeoutError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyNotFoundError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::ProxyProtocolError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslHandshakeFailedError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslInternalError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::SslInvalidUserDataError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    case QAbstractSocket::TemporaryError:
        qDebug()<<__FUNCTION__<<__LINE__<<socketError;
        break;

    }


}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::saveImage : This function is used to save the image
 */
void ProxyPics::saveImage()
{
    int counter = ImageManager::getImageCount(m_idImageRequestedFor);

    qDebug()<<__FUNCTION__<<__LINE__<<" Save Image count: "<<counter;

    QImage image;
    image.loadFromData(m_collectedData, "JPG");
    qDebug() << __LINE__<<m_collectedData.size()<< __FUNCTION__<< counter << image.isNull();

    if (!image.isNull())
    {
        m_collectedData.clear();
        QString picDirPath = QDir::homePath();
        picDirPath.append("/PPImages");
        QDir imageDir(picDirPath);
        if(!imageDir.exists())
        {
            QDir dir = QDir::home();
            dir.mkdir(picDirPath);
        }

        QString imagePath = picDirPath;
        imagePath.append(QString("/name%1_%2.jpg").arg(counter++).arg(m_idImageRequestedFor));
        QImageWriter writer;
        writer.setFormat("JPG");
        writer.setFileName(imagePath);
        writer.write(image);
    }
}

/**
 * @brief ProxyPics::pictureReadyRead : This slot is executed when an data is available from
 * socket which shares pictures
 */
void ProxyPics::pictureReadyRead()
{
   QByteArray newData = m_PixShareSocket->readAll();
   dataString = QString(newData);
   bool image_completed =dataString.contains("imageCompleated");
   bool image_name_list =dataString.contains("imageNameList");
   bool image_file_name =dataString.contains("imageFileName");
   bool new_ticket =dataString.contains("newTicketNumber_");
   QByteArray imageDDS("imageDataDownloadStarted");
   QByteArray imageDDF("imageDataDownloadFinished");
   bool image_DataDownload_Started = newData.contains(imageDDS);
//   bool image_download_finished = newData.endsWith(imageDDF);

   qDebug()<< __FUNCTION__<<__LINE__<< "Receive Image Data:"<<newData.size();
    if (image_DataDownload_Started)
    {
        imageDataDownloadStarted = true;
        m_imageByteSize = dataString.section("#", -1).toInt();
        qDebug()<< __FUNCTION__<<__LINE__<< m_imageByteSize;
        return;
    }
    if (imageDataDownloadStarted)
    {
        m_collectedData.append(newData);
        if (m_collectedData.size() == m_imageByteSize)
        {
            m_imageByteSize = 0;
            imageDataDownloadStarted = false;
            qDebug()<< newData.size() << __FUNCTION__<<__LINE__<< "save Image Data:"<<m_collectedData.size();
            pixSrverDisconnected();
            return;
        }
        qDebug()<< __FUNCTION__<<__LINE__<< "save Image Data:"<<m_collectedData.size();
        return;
    }

    m_collectedData.append(newData);
    if (image_completed)
    {
        qDebug()<< __FUNCTION__<<__LINE__<<dataString;
        saveImage();
        m_collectedData.clear();
    } else if (image_name_list) {
        qDebug()<< __FUNCTION__<<__LINE__<<dataString;
        m_imageNameList = dataString.split("#");
        m_imageNameList.removeAll("imageNameList");

        // remove repeated items
        // remove non water marked images
        QStringList tempList;
        for (int i=0; i<m_imageNameList.count(); ++i)
        {
            QString imageName = m_imageNameList.at(i);
            if (!imageName.contains("_WM", Qt::CaseSensitive))
                continue;

            if (!tempList.contains(imageName))
                tempList.append(imageName);
        }

        m_imageNameList = tempList;

        if (!m_imageNameList.isEmpty())
        {
            imageFileName = m_imageNameList.at(0);
            isWaterMarkedRequired = false;
            setReceiveImageWithFileName(imageFileName);
        }
        m_collectedData.clear();
    } else if (image_file_name) {
        qDebug()<< __FUNCTION__<<__LINE__<<dataString;
        m_collectedData.clear();
    } else if (new_ticket) {
        m_assignmentTicket = dataString.remove("newTicketNumber_");
        m_collectedData.clear();
    } else {
        //m_collectedData.append(newData);
        qDebug()<< __FUNCTION__<<__LINE__<< m_collectedData.size();
//        image.loadFromData(m_collectedData, "JPG");
//        if (!image.isNull())
//        {
//            qDebug()<< __FUNCTION__<<__LINE__;
//            pixSrverDisconnected();
//        }
    }
}


/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::saveSingleImageData
 * @return
 */
bool ProxyPics::saveSingleImageData()
{
    image.loadFromData(m_collectedData, "JPG");

    if (!image.isNull())
    {
        qDebug() << __FUNCTION__ <<__LINE__<< m_imageRequestedFor;
        m_collectedData.clear();
        QString picDirPath = QDir::homePath();
        picDirPath.append("/PPImages");
        QDir imageDir(picDirPath);
        if(!imageDir.exists())
        {
            QDir dir = QDir::home();
            dir.mkdir(picDirPath);
        }

        QString imagePath = picDirPath;
        imagePath.append(QString("/%1.jpg").arg(m_imageRequestedFor));
        QImageWriter writer;
        writer.setFormat("JPG");
        writer.setFileName(imagePath);
        writer.write(image);
    }

    QImage image;
    image.loadFromData(m_collectedData, "JPG");

    if (!image.isNull())
    {
         m_collectedData.clear();
        QString picDirPath = QDir::homePath();
        picDirPath.append("/ProxyPics/Images");
        QDir imageDir(picDirPath);
        if(!imageDir.exists())
        {
            QDir dir = QDir::home();
            dir.mkdir(picDirPath);
        }

        QString imagePath = picDirPath;
        imagePath.append(QString("/%1.jpg").arg(m_imageRequestedFor));
        QImageWriter writer;
        writer.setFormat("JPG");
        writer.setFileName(imagePath);
        writer.write(image);
        ImageSaveCounter++;
    }

    return true;
}

/**
 * @brief ProxyPics::setReceiveImageId
 * @param receiveImageId
 */
void ProxyPics::setReceiveImageId(QString receiveImageId)
{
    if (m_receiveImageId == receiveImageId)
        return;

    m_receiveImageId = receiveImageId;
    emit receiveImageIdChanged(receiveImageId);
}

/**
 * @brief ProxyPics::setReceiveImageWithFileName
 * @param receiveImageWithFileName
 */
void ProxyPics::setReceiveImageWithFileName(QString receiveImageWithFileName)
{
    if (m_receiveImageWithFileName == receiveImageWithFileName)
        return;

    m_receiveImageWithFileName = receiveImageWithFileName;
    emit receiveImageWithFileNameChanged(receiveImageWithFileName);
    QString modifiedFileName = receiveImageWithFileName;

//     if (isWaterMarkedRequired)
//        modifiedFileName.append(QString("_WM.jpg"));

    qDebug() <<__LINE__<< __FUNCTION__<< modifiedFileName;
    if (isImageDownLoaded(modifiedFileName))
    {
        emit imageDownloaded();
    }

    m_imageRequestedFor = modifiedFileName;
    writeDataToPixServer((QString("imageFileName#%1").arg(modifiedFileName)).toStdString().c_str());
}

/**
 * @brief ProxyPics::updatePurchasedDetailToFeed
 * @param imageName
 * @param ticket
 * @return
 */

bool ProxyPics::updatePurchasedDetailToFeed(QString &imageName, const QString &ticket)
{
    qDebug()<<__FUNCTION__<<__LINE__<<"Update Purchased Detail To Feed - ticket:"<<ticket<<" Image Name:"<<imageName;

    QString data = QString("updatePurchasedDetailToFeed#%1#%2").arg(imageName).arg(ticket);
    m_collectedData.clear();
    writeDataToPixServer((data).toStdString().c_str());
    // download real image
    isWaterMarkedRequired = false;
    setReceiveImageWithFileName(imageName);
    return true;
}

/**
 * @brief ProxyPics::updateAccomplishedCount
 * @param imageName
 * @param ticket
 * @return
 */
bool ProxyPics::updateAccomplishedCount(const QString &imageName, const QString &ticket)
{
    qDebug()<<__FUNCTION__<<__LINE__<<"Update Accomplished Count - ticket:"<<ticket<<" Image Name:"<<imageName;
    QString data = QString("updateAccomplishedCount#%1#%2").arg(imageName).arg(ticket);
    m_collectedData.clear();
    writeDataToPixServer((data).toStdString().c_str());
    qDebug()<<__FUNCTION__<<__LINE__<<data;
    return true;
}

/**
 * @brief ProxyPics::requestNewTicketNumber
 */
void ProxyPics::requestNewTicketNumber()
{
    qDebug()<<__FUNCTION__<<__LINE__<<"Request New Ticket Number";
    QString data = QString("getTicketNumber");
    m_collectedData.clear();
    writeDataToPixServer((data).toStdString().c_str());
    m_PixShareSocket->waitForBytesWritten();
}


/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::readSocket This slot is executed when ever data
 * is available on socket.
 */
void ProxyPics::readSocket()
{
    packet = m_socket->readAll();
    qDebug() <<"---------------------------------"<< __FUNCTION__ <<__LINE__<<  packet;
    QString data = packet;
    if (data.contains("request_created"))
        setNewJobCreated(true);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::postRequest : This function is used to post request for new photos
 * @param assignmentName : Name of the assignment
 * @param ticket Ticket name of the photo request
 * @param address The address of the photo request
 * @param value The value of the photo request
 * @param note Note of the photo request
 */

void ProxyPics::postRequest(const QString &assignmentname,
                                const QString &ticket,
                                const QString &id,
                                const QString &address,
                                const int value,
                                const QString &note)
{
    qDebug() << __FUNCTION__ <<__LINE__<<"Post Request:"<<address<<" value:"<<value<<" note:"<<note;
    QString msg = assignmentname;
    msg.append(DELIMITER);
    msg.append(ticket);
    msg.append(DELIMITER);
    msg.append(id);
    msg.append(DELIMITER);
    msg.append(QString::number(value));
    if (assignmentValue()!=QString::number(value)) {
       setAssignmentValue(QString::number(value));
       qDebug() << __FUNCTION__ <<__LINE__<<"value changed"<<value;
    }
    if (currentAssignmentAddress()!=address) {
        setCurrentAssignmentAddress(address);
       qDebug() << __FUNCTION__ <<__LINE__<<"address changed"<<address;
    }
    msg.append(DELIMITER);
    msg.append(address);
    msg.append(DELIMITER);
    msg.append(note);
    m_socket->write(msg.toStdString().c_str());
    m_msg = msg;
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::readUserDetailSocket : This slot is executed when an data is avaialble on
 * user details socket
 */

void ProxyPics::readUserDetailSocket()
{
    packet = m_UserDetailSocket->readAll();
    qDebug() << __FUNCTION__ <<__LINE__<<"Packet:" << packet;
    QString dataString = QString(packet);

    if (dataString.contains("userCreated"))
    {
        setCurrentState("REGISTRATION SUCCESS");
//        emit userCreatedResult(true);
        return;
    }
    else if (dataString.contains("userCreationFails"))
    {
        //NO need to do this as qml timer expires, failure is anyhow handled there
//        emit userCreatedResult(false);
        return;
    }

    if (dataString.startsWith("valid"))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<"Valid User Detail";
        setCurrentState("Home");
        saveLoggedInUserDetails();
    }
    else if (dataString.contains("invalid"))
    {
        qDebug()<<__FUNCTION__<<__LINE__<<"Invalid user detail";
        setCurrentState("Authentication Failure");
    }
}


/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::sendCreateNewUser This function is used to request server to create a
 * new user
 * @param userid Userid of the new user
 * @param email Email of the new user
 * @param mobile Mobile number of the user
 * @param password Password of the new user
 */
void ProxyPics::sendCreateNewUser()
{
    qDebug() << __FUNCTION__ <<__LINE__;
    QString delimiter = QString("_#@#_");
    QString data = QString("new_user");
    data.append(delimiter);
    data.append(UserId());
    data.append(delimiter);
    data.append(firstName());
    data.append(delimiter);
    data.append(lastName());
    data.append(delimiter);
    data.append(userPassword());
    data.append(delimiter);
    data.append(phoneNumber());
    data.append(delimiter);
    data.append(userEmail());
    data.append(delimiter);
    QString a = QString::number(deviceAltitude());
    data.append(a);
    data.append(delimiter);
    QString longitude = QString::number(deviceLongitude());
    data.append(longitude);
    data.append(delimiter);
    QString latitude = QString::number(deviceLatitude());
    data.append(latitude);
    m_UserDetailSocket->write((data).toStdString().c_str());
    qDebug()<<__FUNCTION__<<__LINE__<<data;
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::photoCaptured This function is called when an
 * image is captured
 * @param photograph Preview of the photograph
 */

void ProxyPics::photoCaptured(QString photograph)
{
    qDebug()<<__FUNCTION__<<__LINE__<<"State:"<< m_state<<" Photograph: "<<photograph;
    m_photo = photograph;
    emit newPhoto(photograph);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::googleMapsEmbedApiRequest
 * @param mapmode
 * @return
 */

//QUrl ProxyPics::googleMapsEmbedApiRequest(QString mapmode)
//{
//    setmapMode(mapmode);
//    QUrl requestUrl;
//    QString request;
//    qDebug()<<__FUNCTION__<<__LINE__<<"Building Google Maps Request";

//    if (m_mapMode == "Address") {

//    }

//    // "www.google.com/maps/"      url: initialUrl + "@" + coord.latitude+","+coord.longitude
//    if (m_mapMode == "Coordinate"){
//       request = "@" + m_deviceLatitude +","+m_deviceLongitude;
//       qDebug()<<__FUNCTION__<<__LINE__<<"Coordinate"<<request;
//       requestUrl = QUrl(request);
//       return requestUrl;
//    }
////    Place mode
////    Place mode displays a map pin at a particular place or address,
////    such as a landmark, business, geographic feature, or town.
//    if (m_mapMode == "Place"){
//       request ="?q="+m_mapDestination;
//       qInfo()<<"Place"<<request;
//       requestUrl = fromUserInput(request);
//       return requestUrl;
//    }
////Directions mode
////    Directions mode displays the path between two or more specified points on the map,
////    as well as the distance and travel time.

//    if (m_mapMode == "Directions"){
//        qInfo()<<"Directions"<<request;
//      return requestUrl;
//    }
////Search mode
////    Search mode displays results for a search across the visible map region.
////    It's recommended that a location for the search be defined,
////    either by including a location in the search term (record+stores+in+Seattle)
////    or by including a center and zoom parameter to bound the search.
//    if (m_mapMode == "Search"){
//        qInfo()<<"Search"<<request;
//       return requestUrl;
//    }
//    qCritical()<<"Invalid URL";
//    return QUrl();  // Invalid url
//}

///**
// * @brief ProxyPics::setMapUrl
// * @param map_url_update
// */
//void ProxyPics::setMapUrl(QString map_url_update){
//       qDebug()<<__FUNCTION__<<__LINE__<<map_url_update;
//}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::positionUpdated This slot is exectued when an
 * change in Position
 * @version 0.3.6
 * @param info The instance of GeoPositionInfo
 *
 */

void ProxyPics::positionUpdated(const QGeoPositionInfo &info)
   {

    if (info.isValid()){
        if (geoCoordinate == info.coordinate())
            return;

        QString checkString =  "Position updated:";
        geoCoordinate = info.coordinate();  // get current coordinates
        QString coordinates = geoCoordinate.toString();

//   The QGeoCoordinate class defines a geographical position on the surface of the Earth.
//   A QGeoCoordinate is defined by latitude, longitude, and optionally, altitude.
//   Use type() to determine whether a coordinate is a 2D coordinate (has latitude and longitude only) or 3D coordinate (has latitude,
//   longitude and altitude). Use distanceTo() and azimuthTo() to calculate the distance and bearing between coordinates.
//   The coordinate values should be specified using the WGS84 datum.
//   For more information on geographical terms see this article on coordinates and another on geodetic systems including WGS84.
//   Azimuth in this context is equivalent to a compass bearing based on true north.
       //
        if (geoCoordinate.isValid()){
          qDebug() <<__FUNCTION__<<__LINE__<< "Geo Coordinate:"<<geoCoordinate;
           }else {
          qDebug() <<__FUNCTION__<<__LINE__<< "Geo Position Info not valid yet, waiting..";
        }

   }
}


/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::picRequest This function is used to request image for
 * the given address
 * @param picAdrress Address for the requested pic
 */
void ProxyPics::picRequest(QString picAddress)
{
    if(pictureAddress==picAddress)
        return;

    qDebug()<<__FUNCTION__<<__LINE__<<"ProxyPics photo request at:"<<picAddress;
    pictureAddress = picAddress;
    setMapDestination(picAddress);
    QByteArray checkaddress = pictureAddress.toLocal8Bit();
    CheckPoint(checkaddress);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setmapDestination Setter Method for map destination
 * @param mapDestination Map Destination
 */
void ProxyPics::setMapDestination(QString mapDestination)
{
    picRequest(mapDestination);
    qDebug()<<__FUNCTION__<<__LINE__<<"Setting map destination"<<mapDestination;

    m_mapDestination = mapDestination;
    emit mapDestinationChanged(mapDestination);
}


 /**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setUiHeight This function is used set ui height
 * @param uiHeight The height of the ui to be set
 */

 void ProxyPics::setUiHeight(const int uiHeight)
 {
     if (m_uiHeight == uiHeight)
         return;

     qDebug()<<__FUNCTION__<<__LINE__<<"UI Height:"<<uiHeight;
     m_uiHeight = uiHeight;
     emit uiHeightChanged(uiHeight);
 }


 /**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setUiWidth This function is used set ui width
 * @param uiWidth The width of ui to be set
 */

 void ProxyPics::setUiWidth(const int uiWidth)
 {
     if (m_uiWidth == uiWidth)
         return;
     qDebug()<<__FUNCTION__<<__LINE__<<"UI Width"<<uiWidth;
     m_uiWidth = uiWidth;
     emit uiWidthChanged(uiWidth);
 }



/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::updateStateMachine This function is used update state
 * @param currentState The new state
 */
void ProxyPics::updateStateMachine(QString currentState)
{
   qDebug()<<__FUNCTION__<<__LINE__<<"ProxyPics UI State:"<<currentState;
   state = currentState;
   QByteArray checkdata = currentState.toLocal8Bit();

   CheckPoint(checkdata);

   if (state=="Request Sent"){
       postRequest(currentAssignmentName(),
                   assignmentTicket(),
                   currentAssignmentId(),
                   currentAssignmentAddress(),
                   assignmentValue().toInt(),
                   assignmentNotes());

   } else if (state=="Accept Assignment") {
       createUserAssignmentDetails();
   } else if (state=="Photos Uploading") {
       setPostImageTicket(assignmentTicket());
       setPostImagePath(savedpath());
       addImageDetailForUser();
   } else if (state=="Send Create User Packet") {
       sendCreateNewUser();
   } else if (state=="Photo Request") {
       QString delimiter = QString("_#@#_");
       QString data = QString("get_assignment_count");
       data.append(delimiter);
       data.append(UserId());
       qDebug()<<__FUNCTION__<<__LINE__<<data;
       m_UserDetailSocket->write((data).toStdString().c_str());
   } else if (state=="Photo Assignment"){
       QString imageNameAssigninServer =
       constructImageName(currentAssignmentId(),UserId());
       qDebug()<<__FUNCTION__<<__LINE__<<imageNameAssigninServer;

       source = QGeoPositionInfoSource::createDefaultSource(this);
        if (source) {
            connect(source, SIGNAL(positionUpdated(QGeoPositionInfo)),
                    this, SLOT(positionUpdated(QGeoPositionInfo)));
            source->setUpdateInterval(-1);
            source->startUpdates();
        }
   } else if (state=="New Photo Request") {
       requestNewTicketNumber();
   } else if (state=="Photo Purchanse") {
       purchaseImage();
       setCurrentAssignmentName("");

   } else if (state=="Photo Reject") {
      removeImageDetailFromUser();
      setCurrentAssignmentName("");
   } else if (state =="Photo Purchase") {
       // todo it should move to "purchase successful"
       setReceiveImageWithFileName(imageName());
       updateMyRequestPixPurchased();
   }
}


/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::fromUserInput : constructs a url from user input
 * @param userInput : input string to convert in to url
 * @return : Constructed url, if empty return's blank page as url
 */

QUrl ProxyPics::fromUserInput(const QString& userInput)
{
    if (userInput.isEmpty())
        return QUrl::fromUserInput("about:blank");
    const QUrl result = QUrl::fromUserInput(userInput);
    return result.isValid() ? result : QUrl::fromUserInput("about:blank");
}

// Return system serial number.
/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::macAddress This function is used to findout mac address
 * of the system
 * @return Mac Address of the system
 */
QString ProxyPics::macAddress()
{
    // Use the ethernet MAC address of the network interface to get a unique number.
    // e.g. D4:BE:D9:66:27:DD
    QString mac="unknown";
    QList<QNetworkInterface> list = QNetworkInterface::allInterfaces();

    for (int nIter = 0; nIter < list.count(); nIter++) {
        qDebug()<<__FUNCTION__<<__LINE__<<" "<<list[nIter].hardwareAddress();
        QNetworkInterface::InterfaceFlags flags = list[nIter].flags();
        if ((flags & QNetworkInterface::IsUp) && !(flags & QNetworkInterface::IsLoopBack))
            mac = list[nIter].hardwareAddress();
    }

    return mac;
}

// Return IP address of local machine.
/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::IpAddress This function is used out local ipAddress of
 * the system
 * @return Ip Address of Local Machine
 */
QString ProxyPics::ipAddress()
{
    // A machine can have multiple addresses. Pick the first one that is IPV4 and not loopback.
    QList<QHostAddress> list = QNetworkInterface::allAddresses();
    for (int nIter = 0; nIter < list.count(); nIter++) {
        qDebug()<<__FUNCTION__<<__LINE__<<" "<<list[nIter].toString();
        if (!list[nIter].isLoopback())
            if (list[nIter].protocol() == QAbstractSocket::IPv4Protocol)
                return list[nIter].toString();
    }
    return macAddress();
}


/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::readPendingDatagrams This slot is executed when ever data is availble
 * on UDP socket to read datagarms
 */
void ProxyPics::readPendingDatagrams()
{
    while (udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        udpSocket->readDatagram(datagram.data(), datagram.size(),
                                &sender, &senderPort);
        qDebug()<<__FUNCTION__<<__LINE__<<" Data Gram Recieved: "<<datagram;
    }
}


/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::CheckPoint This function is used to update location and data
 * @param data : The check data
 * @return Returns the status
 */
bool ProxyPics::CheckPoint(QByteArray data)
{
    QByteArray userdata;
    QByteArray proxypicsdata;
    QString checkString = macAddress();
    QString tmp;
    QString delimiter = QString("_#@#_");
    proxypicsdata.append("proxypics_data");
    proxypicsdata.append(delimiter);
    proxypicsdata.append(currentState());
    proxypicsdata.append(delimiter);
    userdata.append("user_data");
    userdata.append(delimiter);
    userdata.append(UserId());
    userdata.append(delimiter);
    userdata.append(firstName());
    userdata.append(delimiter);
    userdata.append(lastName());
    userdata.append(delimiter);
    userdata.append(userPassword());
    userdata.append(delimiter);
    userdata.append(phoneNumber());
    userdata.append(delimiter);
    userdata.append(userEmail());
    userdata.append(delimiter);
    checkString.append(ipAddress());
    checkString.append("user_location");
    checkString.append(delimiter);
    checkString.append(" ,Altitude: ");
    checkString.append(delimiter);
    checkString.append(QString::number(deviceAltitude()));
    checkString.append(delimiter);
    checkString.append(" ,Longitude: ");
    checkString.append(QString::number(deviceLongitude()));
    checkString.append(delimiter);
    checkString.append(" Latitude: ");
    tmp = QString::number(m_deviceLatitude);
    checkString.append(tmp);
    checkString.append(QString::number(deviceLatitude()));
    QByteArray checkdata = checkString.toLocal8Bit();
    checkdata.append(proxypicsdata);
    checkdata.append(userdata);
    checkdata.append(data);
    bytes_Written = udpSocket->writeDatagram(checkdata.data(),checkdata.size(),proxyPicsHost,checkpointport);
    qDebug()<<__FUNCTION__<<__LINE__<<checkString;
    return true;
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::createUserAssignmentDetails This function is used to create an assignment for the
 * user on accepting assignment
 */
void ProxyPics::createUserAssignmentDetails()
{
    QString assignmentname = currentAssignmentName();
    QString ticket = assignmentTicket();
    QString id = currentAssignmentId();
    QString address = currentAssignmentAddress();
    QString value = assignmentValue();
    QString note = assignmentNotes();
    qDebug()<< assignmentname<<__FUNCTION__<<__LINE__<< ticket<<" Client Request:"<<address<<" value:"<<value<<" notes:"<<note;

    //crate the msg
    QString msg = "assignemnt_accepted";
    msg.append(DELIMITER);
    msg.append(UserId());
    msg.append(DELIMITER);
    msg.append(assignmentname);
    msg.append(DELIMITER);
    msg.append(ticket);
    msg.append(DELIMITER);
    msg.append(id);
    msg.append(DELIMITER);
    msg.append(value);
    msg.append(DELIMITER);
    msg.append(address);
    msg.append(DELIMITER);
    msg.append(note);
    msg.append(DELIMITER);
    msg.append(assignmentPubDate());
    m_socket->write(msg.toStdString().c_str());
    m_msg = msg;
}

void ProxyPics::updateMyAssignmentForPixUpload()
{
    QString assignmentname = currentAssignmentName();
    QString ticket = assignmentTicket();
    QString id = currentAssignmentId();
    QString address = currentAssignmentAddress();
    QString value = assignmentValue();
    QString note = assignmentNotes();
    qDebug()<< assignmentname<<__FUNCTION__<<__LINE__<< ticket<<" Client Request:"<<address<<" value:"<<value<<" notes:"<<note;

    //crate the msg
    QString msg = "assignment_pix_uploaded";
    msg.append(DELIMITER);
    msg.append(UserId());
    msg.append(DELIMITER);
    msg.append(assignmentname);
    msg.append(DELIMITER);
    msg.append(ticket);
    msg.append(DELIMITER);
    msg.append(id);
    msg.append(DELIMITER);
    msg.append(value);
    msg.append(DELIMITER);
    msg.append(address);
    msg.append(DELIMITER);
    msg.append(note);
    msg.append(DELIMITER);
    msg.append(assignmentPubDate());
    m_socket->write(msg.toStdString().c_str());
    m_msg = msg;
}

void ProxyPics::updateMyRequestPixPurchased()
{
    QString assignmentname = currentAssignmentName();
    QString ticket = assignmentTicket();
    QString id = currentAssignmentId();
    QString address = currentAssignmentAddress();
    QString value = assignmentValue();
    QString note = assignmentNotes();
    qDebug()<< assignmentname<<__FUNCTION__<<__LINE__<< ticket<<" Client Request:"<<address<<" value:"<<value<<" notes:"<<note;

    //crate the msg
    QString msg = "assignment_pix_purchased";
    msg.append(DELIMITER);
    msg.append(UserId());
    msg.append(DELIMITER);
    msg.append(assignmentname);
    msg.append(DELIMITER);
    msg.append(ticket);
    msg.append(DELIMITER);
    msg.append(id);
    msg.append(DELIMITER);
    msg.append(value);
    msg.append(DELIMITER);
    msg.append(address);
    msg.append(DELIMITER);
    msg.append(note);
    msg.append(DELIMITER);
    msg.append(assignmentPubDate());
    m_socket->write(msg.toStdString().c_str());
    m_msg = msg;
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::addImageDetailForUser This function is used to add the image to current assignment
 * @param userId  The userid of the login user
 * @param imageName Name of the image
 * @param assignmentName Name of the assignment
 * @param id Assignment id
 */
void ProxyPics::addImageDetailForUser()
{
    qDebug()<<__FUNCTION__<<__LINE__<< " Add Image Detail for User: ";

    QString delimiter = QString("_@_");
    QString data = QString("add_image_to_user");
    data.append(delimiter);

    data.append(UserId());
    data.append(delimiter);
    data.append(imageName());
    data.append(delimiter);
    data.append(currentAssignmentName());
    data.append(delimiter);
    data.append(currentAssignmentId());
    qDebug()<<__FUNCTION__<<__LINE__<<data;
    m_socket->write(data.toStdString().c_str());

    // todo - srinivas (this need to remove while all details are moved to myAssignments.xml and myRequest.xml)
    // m_UserDetailSocket->write((data).toStdString().c_str());
}


/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::purchaseImage This function is used to request the serve to make purchase of image
 * @param userid UserId of logged in user
 * @param imageName Name of the image
 * @param assignmentName Name of the assignment
 * @param id Assignment id
 */


void ProxyPics::purchaseImage()
{
    qDebug()<<__FUNCTION__<<__LINE__<<"Purchase Image - User ID:";

    m_UserDetailSocket->close();
    // remove info from server userdetails.xml
    QString image_name = imageName();
    image_name.remove(QString(".jpg"));
    qDebug() << __FUNCTION__ <<__LINE__ << "--";
    QString delimiter = QString("_#@#_");
    QString data = QString("purchase_image");
    data.append(delimiter);

    data.append(UserId());
    data.append(delimiter);
    data.append(image_name);
    data.append(delimiter);
    data.append(currentSelectedAssignmentName());
    data.append(delimiter);
    data.append(currentAssignmentId());
    qDebug()<<__FUNCTION__<<__LINE__<<data;

    m_UserDetailSocket->write((data).toStdString().c_str());

    QString ticket = getTicketNumberFromId(UserId());
    if (!ticket.isEmpty())
        updatePurchasedDetailToFeed(image_name, ticket);


}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::addAssignmentToUserDetail This function is used to add assignment
 * to the list of the given user
 * @param userid Userid of the logged in user
 * @param assignmentId Id of assignment
 */

void ProxyPics::addAssignmentToUserDetail()
{
    qDebug()<<__FUNCTION__<<__LINE__<<"Add Assignment to User Detail - ";

    QString delimiter = QString("_#@#_");
    QString data = QString("add_assignment");
    data.append(delimiter);
    data.append(UserId());
    data.append(delimiter);
    data.append(assignmentId());
    qDebug()<<__FUNCTION__<<__LINE__<<data;
    m_UserDetailSocket->write((data).toStdString().c_str());

}


/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::addImageDetail
 * @param userid
 * @param imageName
 * @param assignmentName
 * @param id
 * @return
 */
//TODO - Looks like this function is not used, please remove if so
void ProxyPics::addImageDetail(const QString &userid, const QString &imageName, const QString &assignmentName, const QString& id)
{
    qDebug()<<__FUNCTION__<<__LINE__<<"Add Image Detail - User ID:"<<userid<<" Image Name: "<<imageName<<" Assignment Name: "<<assignmentName<<" ID: "<<id;

}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::removeImageDetailFromUser Removes the image details from the user
 * @param userId UsedId of the logged in user
 * @param imageName Name of the image
 * @param assignmentName Name of the assignment
 * @param id Assignment id
 */

void ProxyPics::removeImageDetailFromUser()
{
    qDebug()<<__FUNCTION__<<__LINE__;
    removeImageDetail(UserId(), imageName(), currentSelectedAssignmentName(), currentAssignmentId());
}

/**
 * @brief ProxyPics::removeImageDetail
 * @param userid
 * @param imageName
 * @param assignmentName
 * @param id
 * @return
 */
bool ProxyPics::removeImageDetail(const QString &userid, QString imageName, const QString &assignmentName, const QString& id)
{
    // remove image from local system
    qDebug()<<__FUNCTION__<<__LINE__<<"Remove Image Detail - User ID:"<<userid<<" Image Name:"<<imageName<<" Assignment Name:"<<assignmentName<<" ID:"<<id;

    QString picDirPath = QDir::homePath();
    picDirPath.append("/PPImages");
    QDir imageDir(picDirPath);
    if(!imageDir.exists())
    {
        QDir dir = QDir::home();
        dir.mkdir(picDirPath);
    }

    QString imagePath = QString("%1/%2").arg(picDirPath).arg(imageName);
    if (QFileInfo::exists(imagePath) )
    {
        QFile::remove(imagePath);
    }

    // remove info from server userdetails.xml
    imageName.remove(QString(".jpg"));
    qDebug() << __FUNCTION__ << __LINE__<<userid << "--" << imageName << "--" <<assignmentName<<"--"<<id;
    QString delimiter = QString("_#@#_");
    QString data = QString("remove_image_from_user");
    data.append(delimiter);

    data.append(userid);
    data.append(delimiter);
    data.append(imageName);
    data.append(delimiter);
    data.append(assignmentName);
    data.append(delimiter);
    data.append(id);
    qDebug()<<__FUNCTION__<<__LINE__<<data;

    m_UserDetailSocket->write((data).toStdString().c_str());
    QString ticket = getTicketNumberFromId(id);
    if (!ticket.isEmpty())
        updateAccomplishedCount(imageName, ticket);

    return true;
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::getTicketNumberFromId This function is used to extract ticket from Id
 * @param id Assignment id
 */
QString ProxyPics::getTicketNumberFromId(const QString &id)
{
    qDebug()<<__FUNCTION__<<__LINE__<<"get ticket numner from id:"<<id;

    QStringList list = id.split("-");
    int index = list.indexOf("assignment");
    if (index != -1)
    {
        return list.at(index+1);
    }

    return QString();
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::disconnectAllPixSharing This function is used to close socket connection
 *  of pix sharing
 */
void ProxyPics::disconnectAllPixSharing()
{
    qDebug()<<__FUNCTION__<<__LINE__<<"Disconnect All Pix Sharing";
    saveImage();
    qDebug() << __FUNCTION__;
    disconnect(m_PixShareSocket, SIGNAL(readyRead()), 0, 0);
}

/**
 * @version 0.3.6
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @brief ClientRequest::writeXMLFile This function is used to write xml file
 * @param device The Input/Output device interface
 * @return status of file writting.
 */
bool ProxyPics::writeXMLFile(QIODevice *device)
{
    qDebug()<<__FUNCTION__<<__LINE__<<"write XML file";
    QXmlStreamWriter xml(device);
    xml.writeStartDocument();
    xml.writeDTD("<!DOCTYPE xml>");
    xml.writeStartElement("myRequest");
    xml.writeAttribute("version", "1.0");
    xml.writeEndDocument();
    return true;
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setPostImagePath
 * @param postImagePath The path of the image
 */
void ProxyPics::setPostImagePath(const QString postImagePath)
{
    qDebug()<<__FUNCTION__<<__LINE__<<"post image path:"<<postImagePath;
    if (m_postImagePath == postImagePath)
        return;

    m_postImagePath = postImagePath;
    emit postImagePathChanged(postImagePath);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setPostImageName This function is used to upload image to server for
 * an assignment
 * @param postImageName Name of the image.
 */
void ProxyPics::setPostImageName(QString postImageName, bool fullPath)
{
    qDebug() << __FUNCTION__ <<__LINE__
             <<"Post Image:"<<postImageName<<" ticket:"<<m_postImageTicket
            <<" path:"<<m_postImagePath;

    if (m_postImageName == postImageName)
        return;

    m_postImageName = postImageName;
    if (!fullPath)
        postImageName.remove(".JPG");

    emit postImageNameChanged(postImageName);

    if (m_postImageName.isEmpty())
        return;

    // check for given file and mark it as uploaded by removing not_uploaded tag
    QString picDirPath = QDir::homePath();
    picDirPath.append("/PPImages");
    QDir imageDir(picDirPath);
    if(!imageDir.exists())
    {
        QDir dir = QDir::home();
        dir.mkdir(picDirPath);
    }

    QString oldImagePath = fullPath ? postImageName : QString("%1/%2.JPG").arg(picDirPath, postImageName);
    postImageName.remove(".JPG");
    QString path = fullPath ? postImageName.remove("not_uploaded") : QString("%1/%2").arg(picDirPath, postImageName.remove("not_uploaded"));
    QFile file(oldImagePath);
    if (file.exists())
    {
        QFile::rename(oldImagePath, path);
    }

    QFile imageFile(path);
    if (!imageFile.exists())
        return;

    m_postImagePath = path;

    image.load(m_postImagePath, "JPG");
    if (image.isNull())
    {
        qDebug() << __FUNCTION__ << "invalid path or corrupted image";
        return;
    }

    QBuffer buffer;
    QImageWriter writer(&buffer, "JPG");

    //Send Image to Server with QImageWriter's 'write' method
    writer.write(image);

//    qDebug() << __FUNCTION__ << buffer.data();
    QByteArray data;
    QDataStream stream( &data, QIODevice::WriteOnly );
    stream.setVersion( QDataStream::Qt_5_5 );
    stream << (quint32)buffer.data().size();
    data.append( buffer.data() );

    m_imageByteSize = buffer.size();
    QString imagesendTag = QString("receiveIt#%1#%2#%3").arg(postImageName.section("/", -1)).arg(m_postImageTicket).arg(QString::number(buffer.size()));
    writeDataToPixServer(imagesendTag.toStdString().c_str());
    m_PixShareSocket->waitForBytesWritten();

    qDebug() << __FUNCTION__ << (quint32)buffer.size();
//    writeDataToPixServer(buffer.data());
    m_PixShareSocket->write(buffer.data());
    m_PixShareSocket->waitForBytesWritten();

    updateMyAssignmentForPixUpload();
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setPostImageTicket
 * @param postImageTicket
 */
void ProxyPics::setPostImageTicket(const QString postImageTicket)
{
    if (m_postImageTicket == postImageTicket)
        return;

    m_postImageTicket = postImageTicket;
    emit postImageTicketChanged(postImageTicket);
    qDebug() << __FUNCTION__<<__LINE__<< m_postImageTicket;

    // get the list from xml w.r.t image id
    QDomDocument doc("doc");
    QFile file("http://96.90.223.196/assignment/feed.xml");
    if (file.open(QIODevice::ReadOnly))
    {
        if (!doc.setContent(&file)) {
            qDebug() << __FUNCTION__<< __LINE__<< doc.toString();
        }
        file.close();
    }
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::isImageDownLoaded This function is used to check whether
 * image has been downloaded
 * @param imageName Name of the image
 * @return States whether downloaded or not
 */
bool ProxyPics::isImageDownLoaded(const QString &imageName)
{
    qDebug()<<__FUNCTION__<<__LINE__<<"is Image Downloaded - Image Name"<<imageName;

    QString picDirPath = QDir::homePath();
    picDirPath.append("/PPImages");
    QDir imageDir(picDirPath);
    if(!imageDir.exists())
    {
        return false;
    }

    QString imagePath = picDirPath;
    if (imageName.contains(".jpg"))
        imagePath.append(QString("/%1").arg(imageName));
    else
        imagePath.append(QString("/%1.jpg").arg(imageName));

    QFileInfo info(imagePath);

    return info.exists();
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::getImageName This function is used to construct image name for
 * the given user and assignmentId
 * @param assignmentId Id of the assignment
 * @param currentUser Name of the current user
 * @return Name of the constructed image
 */
QString ProxyPics::constructImageName(const QString &assignmentId, const QString &currentUser)
{
    qDebug()<<__FUNCTION__<<__LINE__<<" assignment ID: "<<assignmentId<<" current User: "<<currentUser;
    QString timestamp = QDateTime::currentDateTime().toString().remove(" ");
    timestamp.remove(":");
    QString id = assignmentId;
    id.remove(" ");
    return QString("%1-%2-%3").arg(id).arg(timestamp).arg(currentUser);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::saveLoggedInUserDetails This function save the details of logged in user information in txt file
 */
void ProxyPics::saveLoggedInUserDetails()
{
    qDebug()<<__FUNCTION__<<__LINE__<<m_UserId<<" "<<m_userPassword;
    QString proxyPicDirPath = QDir::homePath();
    proxyPicDirPath.append("/PP");
    QDir ppDir(proxyPicDirPath);
    if(!ppDir.exists())
    {
        QDir dir = QDir::home();
        dir.mkdir(proxyPicDirPath);
    }

    QString filePath = proxyPicDirPath.append("/%1").arg("login.pp");

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        QTextStream out(&file);
        out << m_UserId << DELIMITER << m_userEmail << DELIMITER << m_userPassword;
        file.close();
    }
}


void ProxyPics::clearSavedUserLoginDetails()
{
    qDebug()<<__FUNCTION__<<__LINE__<<proxyPicDirPath;

    proxyPicDirPath = QDir::homePath();
    proxyPicDirPath.append("/PP");
    QDir ppDir(proxyPicDirPath);
    if(!ppDir.exists())
    {
        return;
    }

    filePath = proxyPicDirPath.append("/%1").arg("login.pp");

    QFile file(filePath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Truncate | QIODevice::Text))
    {
        file.close();
    }
}


/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief setCurrentAssignmentId : This is setter method of Assignment ID
 * @param currentAssignmentId Id of current Assignment
 */

void ProxyPics::setCurrentAssignmentId(QString currentAssignmentId)
{
    qDebug()<<__FUNCTION__<<__LINE__<<m_UserId<<" "<<m_userPassword;
    if (m_currentAssignmentId == currentAssignmentId)
        return;

    m_currentAssignmentId = currentAssignmentId;
    emit currentAssignmentIdChanged(currentAssignmentId);
    QStringList split = currentAssignmentId.split("-");
    if (split.isEmpty())
        return;
    QString userid = split.at(split.count()-1);
    setAssignmentCreatorUserid(userid);

}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setNewUserCreated Setter method for user created
 * @param userCreated Creation state
 */
void ProxyPics::setNewUserCreated(bool userCreated)
{
    emit userCreatedResult(userCreated);
    qDebug()<<__FUNCTION__<<__LINE__<<m_UserId<<" "<<m_userPassword;
    if (m_newUserCreated == userCreated)
        return;
    qDebug()<<__FUNCTION__<<__LINE__<<userCreated;
    m_newUserCreated = userCreated;
    emit newUserCreatedChanged(userCreated);
    if (userCreated==true){
        updateStateMachine("REGISTRATION SUCCESS");
    } else {
        updateStateMachine("Already Registered");
    }
}

void ProxyPics::setNewJobCreated(bool newJobCreated)
{
    m_newJobCreated = newJobCreated;
    if (newJobCreated)
        emit newJobCreatedChanged(newJobCreated);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::requestFinished : This slot is executed when replay
 * has finished processing
 * @param reply Instance of network replay
 */
void ProxyPics::requestFinished(QNetworkReply *reply)
{

    QByteArray      resp;
    QJsonDocument   document;
    QJsonParseError jsonerror;
    QString         error;

    qDebug()<<__FUNCTION__<<__LINE__<<QObject::tr("HTTP-Request Finished: ")<<reply->error();
    qDebug()<<__FUNCTION__<<__LINE__<<QObject::tr(" Bytes Available: ")<<reply->bytesAvailable()<<endl;

    if (reply->bytesAvailable()) {
        resp = reply->readAll();
        qDebug()<<__FUNCTION__<<__LINE__<<QObject::tr("Bytes: ")<<resp<<endl;
        emit rawResponse(resp,error);

        document = QJsonDocument::fromJson(resp, &jsonerror);
        if (jsonerror.error == QJsonParseError::NoError) {
            emit jsonResponse(document);
        } else {
            qWarning()<<"Json Parse Error:"<<jsonerror.errorString();
        }
    }
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::connected : This slot is exectued when an socket connection
 * has established
 */
void ProxyPics::connected()
{

    qDebug() << __FUNCTION__ <<__LINE__<<"Connected";

}
/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::connected : This slot is exectued when an socket connection
 * has established
 */
void ProxyPics::tcpSockect_connected()
{

    qDebug() << __FUNCTION__ <<__LINE__<<"tcpSocket Connected"
             << "tcpSocket WRITEABLE?"<<tcpSocket->isWritable();

}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::connected : This slot is exectued when an socket connection
 * has established
 */
void ProxyPics::m_socket_connected()
{

    qDebug() << __FUNCTION__ <<__LINE__<<"m_socket Connected"
             << "m_socket WRITEABLE?"<<m_socket->isWritable();

}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::connected : This slot is exectued when an socket connection
 * has established
 */
void ProxyPics::m_UserDetailSocket_connected()
{

    qDebug() << __FUNCTION__ <<__LINE__<<"m_UserDetailSocket Connected"
             << "m_UserDetailSocket WRITEABLE?"<<m_UserDetailSocket->isWritable();
  }
/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::disconnected This slot is executed when an socket is
 * disconnected
 */
void ProxyPics::disconnected()
{
    qDebug() << __FUNCTION__ <<__LINE__<< "Disconnected";
}

void ProxyPics::pixSrverDisconnected()
{
    // this funtion indicated the pix server successfully transfered the pix data
    // disconnection indicated to have local copy of it
    qDebug() << __FUNCTION__ <<__LINE__<< "Pix Disconnected" << m_collectedData.size();
//    m_collectedData.clear();
//    m_collectedData.append("\xFF\xD8\xFF\xE0\x00\x10JFIF\x00\x01\x01\x01\x00`\x00`\x00\x00\xFF\xDB\x00""C\x00\b\x06\x06\x07\x06\x05\b\x07\x07\x07\t\t\b\n\f\x14\r\f\x0B\x0B\f\x19\x12\x13\x0F\x14\x1D\x1A\x1F\x1E\x1D\x1A\x1C\x1C $.' \",#\x1C\x1C(7),01444\x1F'9=82<.342\xFF\xDB\x00""C\x01\t\t\t\f\x0B\f\x18\r\r\x18""2!\x1C!22222222222222222222222222222222222222222222222222\xFF\xC0\x00\x11\b\x01\xE0\x02\x80\x03\x01\"\x00\x02\x11\x01\x03\x11\x01\xFF\xC4\x00\x1F\x00\x00\x01\x05\x01\x01\x01\x01\x01\x01\x00\x00\x00\x00\x00\x00\x00\x00\x01\x02\x03\x04\x05\x06\x07\b\t\n\x0B\xFF\xC4\x00\xB5\x10\x00\x02\x01\x03\x03\x02\x04\x03\x05\x05\x04\x04\x00\x00\x01}\x01\x02\x03\x00\x04\x11\x05\x12!1A\x06\x13Qa\x07\"q\x14""2\x81\x91\xA1\b#B\xB1\xC1\x15R\xD1\xF0$3br\x82\t\n\x16\x17\x18\x19\x1A%&'()*456789:CDEFGHIJSTUVWXYZcdefghijstuvwxyz\x83\x84\x85\x86\x87\x88\x89\x8A\x92\x93\x94\x95\x96\x97\x98\x99\x9A\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDA\xE1\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xF1\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xFA\xFF\xC4\x00\x1F\x01\x00\x03\x01\x01\x01\x01\x01\x01\x01\x01\x01\x00\x00\x00\x00\x00\x00\x01\x02\x03\x04\x05\x06\x07\b\t\n\x0B\xFF\xC4\x00\xB5\x11\x00\x02\x01\x02\x04\x04\x03\x04\x07\x05\x04\x04\x00\x01\x02w\x00\x01\x02\x03\x11\x04\x05!1\x06\x12""AQ\x07""aq\x13\"2\x81\b\x14""B\x91\xA1\xB1\xC1\t#3R\xF0\x15""br\xD1\n\x16$4\xE1%\xF1\x17\x18\x19\x1A&'()*56789:CDEFGHIJSTUVWXYZcdefghijstuvwxyz\x82\x83\x84\x85\x86\x87\x88\x89\x8A\x92\x93\x94\x95\x96\x97\x98\x99\x9A\xA2\xA3\xA4\xA5\xA6\xA7\xA8\xA9\xAA\xB2\xB3\xB4\xB5\xB6\xB7\xB8\xB9\xBA\xC2\xC3\xC4\xC5\xC6\xC7\xC8\xC9\xCA\xD2\xD3\xD4\xD5\xD6\xD7\xD8\xD9\xDA\xE2\xE3\xE4\xE5\xE6\xE7\xE8\xE9\xEA\xF2\xF3\xF4\xF5\xF6\xF7\xF8\xF9\xFA\xFF\xDA\x00\f\x03\x01\x00\x02\x11\x03\x11\x00?\x00\xDB\xF0\xD5\xB5\xB2\xD9\xC5)\x01\xD5{\x11\xCDO\xA8\xDD\xC3y,\x80\xA8\x8B\xCB\xFB\xB8\x15""B\xDEQ\xA6\xDC\xAC$\x80\x8D\xD2\xAAj\x9A\xCC,\xD2%\xBC*J\xFD\xF6""5\xAE\xC6\x92vd\x97^#H\xA0M\x88\xB2\xC8\x06>a\xD2\xB3\x92in\x8E\xE9\xE4 \xB1\xF9P\x1A\xCCI\x17\xCC""f\x0B\x8C\xF2\x05k\xDA""C\xE6\x10\xFC\x99O\xB5K\x16\xE0\xEC\xB6\xB1""0\x15\x9F\r\xF2\xB4\x85O\x1C\xD6\xBD\xFD\x9F\xEE\xC2\xB8 \x9A\xE7\xEE,^&,\xBD(L\x89v:\x0Bk\xA2\x9D\x0E""EkC:J\xBE\x86\xB8\xDBk\xA7\x8C\x85nEl[\xDD\x06\xC1V\xFC(qL\x9B\x9D\x12\x12\xA4U\xA4`\xF8\xECk&\xDA\xF0""6\x03\x9A\xBE\xA7\xA1SR\x9BLM]\x1Av\xD7\x0Fn\xE1\x91\x88#\xB8\xAE\xAFM\xD6R\xE0\b\xE7!_\xD7\xD6\xB8\xA8\xE4\xE3\x06\xAC\xA3\xED<\x1A\xD6\xEAJ\xCC\xCFX\xBB\xA3\xD1\x01\xC8\xC8\xA2\xB9m?[\x92\xDF\t7\xCF\x1F\xF2\xAE\x92\x0B\x98\xAE""c\x0F\x13\x86\x1F\xCA\xB3\x94\x1CMc5\"Z(\xA2\xA4\xB0\xA2\x8A(\x00\xA2\x8A(\x00\xA2\x8A(\x00\xA2\x8A(\x00\xA2\x8A(\x02)\xA3\x12!\xCDs\xBA\x8D\x97%\x80\xE6\xBAz\xA5w\b`h\"H\xE3|\xE7\x85\xB6\xB5L\xB7\x19\x1Dj}B\xCF""9 V#\xB3\xC0\xF8l\xE2\x84\xC8\xB1\xAD\xE7""f\x90\xC9Y\xC9p\b\xEBR\ts@X\xB4_\xDE\xA3g\xCD""Dd\xA6\x97\xA0\x07""3S\t\xA6\x17\xA6\x16\xA4""1\xECj\"i\x0BTl\xD4\x00\x16\xA6\x13""A8\xA6\x13\xCD\x03\x1AM4\x9ARi\x8Ch\x01\xA6\x98iI\xE6\x9A\xD4\x00\xD2y\xA6\x13JM4\xD2\x18\x1A""c\x1AR}\xE9\x86\x80\x10\x9Ai\xE9KM&\x80\x1Ai\xA6\x9CO\x14\xD3@\x11I\x1E\xEE\xD5\x97}f\xB3\xC6""A\x1C\xFA\xD6\xC1\xA8""dL\xE6\x90\xCE,i\x93""5\xC9L\x1D\xA3\xBDt\xBA""f\x9A\xB1(\xE3\x00u5:[\x86\x93\x91\xC7z\xB2[\x03j\xF0(\xD5\x80\xF7\x93\x0B\xB5:T$\xD1\xD6\x9B\x9CS\x01I\xA6\x93\x9A\t\xA6\x9A@-%\x19\xA4\xA6\x01I\xD6\x8AJ@\x1FZL\xF7\xA5\xEBM$\x0E(\x00\xCD!\xA2\x9BH\x05\xCD&i)3\xDA\x98\xD8Rw\xA3""8\xA6\x96\xA0""B\x93M'\x8AJBh\x00&\x9AM\x05\xA9\xB9\xA0""b\x96\xA6\xE7\x9C\xD2Rf\x80\x14\x9A""anh'\x8A\x8C\x9A,\x06\xED\xD6\xAFkqq\xE6\x98""e\">1\x9E\x05P{\xB8n]\xD2\xD2""2\xAA\xDC\xB6j\x03$qG\"\x11\x9C\xB7Z\xB3ok\x10\x94I\x16v\xB0\xE8+k\xA2\xEDr\xCD\x92\x07\n\x19rW\xA7\x15\xD8h\xBAk`\xDC\xC9\x80\x80g\x91U\xB4\x1D\x10\xDEH\x1D\xC0X\xD7\x93[\x1A\xFE\xA3o\xA5\xDA%\xB5\xB1\x0E\xC7\xEF\n\x12\xBB-\xDA*\xE7""7\xA9_\xA5\xD5\xEBm?*\x9C\n\xA8P8\xC5X6p\xEA \xCBj\xDB&\xEE\x87\xBDS&[g\xF2\xE6R\xA4z\xD0\xD1\xCFr\xAD\xCD\x8FVA\xCDU\x8D\x9E\x17\xEE+l\x10\xEB\x9A\x82kd\x90\x1FZ\x10\x82\x0B\x90p\t\xE6\xB5m\xAE\xCA\x10\t\xC8\xAEq\xD1\xA2<v\xAB\x16\xF7""Ep\x1B\xA5;\x01\xD8\xC5""2J\xB9\x06\xAC#\x95\xAE""f\x0B\x97\x04""27\x1D\xC5l\xDB\xDE,\x80\x06\xEBSf\x83sM_\x8E*h/\xA7\xB4\x90I\x0B\x91\x8E\xA2\xA8\xAB""c\x91\xD2\xA5R\x1A\xA9L\x87\x13\xB1\xD3""5\xE8/@\x8ER#\x9B\xDF\xA1\xAD\x8A\xF3\xA3\x16\xEF\x99\x0E\xD7\x15\xAF\xA5x\x8D\xED\x99m\xEF""2\xC9\xD0""7qI\xA4\xF6-K\xA3:\xEA+9u{y&H\xE3\xCB\x06""8\xDD\xD0V\x8DKV-4\xF6\n(\xA2\x90\xC2\x8A(\xA0\x02\x8A(\xA0\x02\x8A(\xA0\x02\x99\"\xE5i\xF4\x1E\x94\t\xEC""b\xDD\xC0\x0Ex\xAEv\xFA\xD0\x1C\xF1]}\xCAu\xACk\xA8s\x9E*\b8\xD9U\xE0""c\xE9OI\xC1\xEFZ7\x96\xD9\xCF\x15\x89""4m\x13\x12:SL\x0B\xFEo\x1Di|\xCE+9'\xCDJ%\xCDP\xACZ/M/Po\xA0\xBD!\x92\x16\xA6\x96\xA8\xCB""f\x9B\xBA\x80\x1E\xCD\x9Ai4\xD2\xD8\xA6\x16\xA0\x07""1\xA8\xC9\xA0\x9Ai4\x86\x04\xD3X\xD0Z\x98M0\x03\xD6\x9AM\x04\xD3I\xA4\x02\x13M&\x82i\r\x00!\xA6\x93""A9\xA4\xEB@\r4\x84\xD0M4\x9A\x00Ri\xA4\xD1\x9C\xD3O4\fN\xD4\xD3N\xA6\x93@\bz\xD3h'\xAD""74\x00\xA6\x90\x9AJ(\x00\xA4""4\x1A""C\xD7\xDE\x90\xC5""4\x9D\xA9\t\xA4\xCD\x02\f\xD2\x13\xCD&i)\f3IE&i\x88L\xD2\x13\xCEh4\xD3@\x01?\x9D!4\x13\x8Anh\x18\x13\xCD!4\xD2i\t\xA0\x00\x9A\t\xA4\xCD""4\x9A\x10\x03""6)\xA4\xF3\xEDHM4\x9A\x00Rj6'\xB5.i\x84\xF3L\x0B\x92\xC0\x04\x89\xD0\xEE\x1D+\xA5\xD0t\xB6\xBB\x9D\x0B.#S\xC9\xAC\xBD""6\xD1\xAF%H\xC2\xE4\xF4\xCDv\x89p\x9A""E\x90\x82""07\xE3\x93Z#d\xAC\xAEl\\\xEAV\x9Au\xA1\x8E""5\x18Q\xD4W\t{r\xD7\xB7\r#\x1E\xA7\x81L\xD4""5\xA1}!\x8DXaO8\xEFT\xD2R\x0F=)\xBD\f\xA4\xEE\xC9\x90\xBCN\x19\t\x04w\x15\xD1i\xD7\xBA~\xA6\xA2\xD7T\x1B\x18\xF0\xB3\x0E\xDFZ\xE7\x95\xC3t\xA7m\xEE)&A\xD0j\xBE\x13\xBE\xD2\xD7\xED\x16\xE3\xED\x16\xA7\x90\xE9\xCE\x05""b\x86\xCF\x06\xB7\xBC=\xE2\xEB\x9D\x1C\x88&\xCC\xF6g\x83\x1B\x1EW\xE9\xFE\x15\xD3\xDEx\x7FG\xF1""E\xA1\xBC\xD2\xA4Hg<\x90""8\xE7\xD0\x8E\xD4\xEE\x16\xECy\xC9\x8DXc\x15Z[R9Z\xD6\xBD\xD3/t\xBB\x83\r\xD4,\x84\x1C\x06\xC7\r\xF4""5\\\xE7\x1C\x8C\xD0#:9Z#ZV\xF7 \x8E\xB8""5Nx\x83r\x06\rD\x81\x95\xA9\xA1\x1D=\xAD\xE1$)9\xADH\x89nz\n\xC9\xD2lf\xB8M\xEB\x19""8\xEAkPn\x8C\xED \x82*$2\xE2\xE0\x8Al\xAB\x13\x80%`\xBE\x86\xAB=\xC8\x89\x0B\x1E\xD5\x97\x14\xF2_^\x80N\x17?\xA5\x11\x14\x8D\xC8/g\xD2\xE7\x07\xE5\x91""3\x91\xB8""f\xBB=7Z\xB6\xD4""c\x1BX$\x9D\xD0\x9A\xE4\xA6\x8A\xD8[\xAA*\x8C\x81\xCD""f2In|\xC8[\x8FJwR\x12\xBCOR\xA2\xB8\xFD\x1B\xC4\x84\xA7\x95.X\x8A\xEA-oa\xBBL\xC6\xDF""0\xEA\xA7\xA8\xA4\xD5\x8D\x14\x93,QE\x14\x8A\n(\xA2\x80\n(\xA2\x80\n(\xA2\x80 \x9Dr+2\xE1""3\x9A\xD6\x93\xA5P\x99z\xD4\xB3""3\x02\xE6.\x0F\x15\x8Bu\x00""9\xE2\xBAk\x88\xF3\x9A\xC8\xB9\x8F\xADH#\x98\x9E\x13\x19$TK/cZ\xD7""0\xE7""5\x93<89\x14\xD3(\x94K\xEFK\xBC\x9A\xA2$ \xE0\xD4\xAB'\xBD""1\x16w\x1AM\xD5\x0E\xFC\xD1\xBE\x98\x89\x89\xA6\x93Q\xEE\xA4-Hc\xF7SwSsI\x9A`8\x9Ai4\x84\xE3\xBD""4\x9A@)4\xC2y\xA4&\x90\x9A\x02\xE2\xE6\x9AO\x14\x85\xA9\xA4\xD0\x00M!4\x94\x84\xD2\x00&\x98iI\xE2\x9B\x9A\x06\x19\xA4\xCF""4\x94""f\x98\x83<S\x18\xD2\xE6\x98OZC\x10\x9AJ\r&i\x80g\x8AJ\x0FJBx\xA4\x17\f\xF3HM\x07\xD6\x9A\x7FZ\x00\\\xD2""f\x92\x9B@\x0E\xA4""4w\xA6\x93\xCD \x0E\x82\x9AzPM4\xD0""0\xCD&h\xCF""4\xC2y\xA6!I\xA6\x13JO4\xDC\xD0\x01\x9A\x0F\x14\xD2z\xD3KP1Kb\x98X\xD2\x13M'\x8A\x00Ri\x84\xD0M0\x9A`)5\x1B""6=(g\x02\xAA\\\xDD\xC7n\xA5\x9D\x86})\x01\xEA\xC9m\x1E\x89h\b(fa\xF9U\x1B{\xB8n&d\xBBr\x0B\xF0\x0Ex\xA6J\xD2\xDD\xA3M+\x90\x9D\x81\x15\x8B+\xE1\xC8\xCFJ\xE8H\xB9\xCC\x9BW\xF0\xDC\xD6\xCCn\xADNP\xF3\x95\xACxoY\x1B\xCB\x98mj\xE8\xF4\xFD""f[`\x12O\xDE""Dx*\xDC\xD5\xCB\xCD\n\xC3]\x84\xCB""b\xCB\x1C\xF8\xC9""CM\xEA""c\xB1\x81\x1C\xE3\x82\xA6\xAE\xA3\xB1Pq\x8A\xAD\x1E\x8Ds\xA7\xC9\xB2\xE1H#\xD6\xAE(\xC0\xE9Y\xB2\x84\xE4\x9E\x95kN\xD5.\xF4\xBB\xB5\x9A\xD6VF\x1E\x9D\xFD\x8D@x\x14\xF8\xA3\xCBn#\x8A\x13\x11\xE9\x16>&\xD3u\xDB""Ai\xAC""D\xB1\xC8\x7F\x88\xFD\xD2}A\xEDX\x9A\xF7\x85\xA7\xD3\xB7\\[\xE6kS\xC8""e\xE4\x81\xEF\\\xE0n+{H\xF1M\xE6\x9B\x18\x82_\xF4\x8B""c\xC7\x97'8\x1E\xC6\x8F@\xF5""9\xC7\\\xF6\xAB\x9AN\x8F""6\xA7t\xA9\x1A\xF1\x9EMo\xDD\xE9zv\xB2\xE9""6\x97\"\xC7$\x87\xE6\x84\x9E\x95\xDB\xE8\xBA,\x1A""E\xA2\xC6\x80\x19""1\xF3""7\xBD\x0F@Z\x92\xE9\xBAM\xBE\x9B""d\xB6\xE8\x80\xF1\xF3""1\x1DMS\xD4\xB4\b\xAET\xB4""c\r\xED[t\x8C\tR\x01\xC1=\xEAJh\xF2}v\xD2[)DE\x8Es\xD2\xA1\xB1\x1EO8\xF9\x8Dt^%\xB5\x93\xED\n%\x84\x16\xCF\x12\x03\xD6\xA0\x1E\x1F\x9B\xEC\xAB""2\f\x9Cg\x14\xE5kY\x19\xC6\xFDJ\xC2\\\x8E\xB4,\xDB\x0F 2\x9E\xA2\xAB\xC8\x92@\xDB\\\x11L\r\x9A\x82\x84\xBA\x9Dm\xD8M\x02\x10\xE0\xE7\xDA\xB6\xB4\xBDj+\xB2\xBB[\xC8\xB9\x1E\xFC\x1A\xC4t\x0E\xA4\x1A\xC8\x9E""7\xB6\x97r\x92=\ri\x16\x9E\x8C\x86\xAD\xB1\xEBv:\xC0v\x10\xDD""a$\xEC\xDD\x8Dkg#\"\xBC\xB3G\xD7\xB2\xA2+\xD5/\x18\xFE!\xD4W[g\xAC%\xAE\xD0\xB3\t\xAD\xDB\xA0?yhj\xC5""F}\xCE\x9A\x8A\x8A\x0B\x88\xAE""c\x12""D\xE1\x94\xFAT\xB5&\x81""E\x14P\x01""E\x14P\x04RU9{\xD5\xC9*\x9C\x86\xA4\xC8\xA1:\xE4\x1A\xCC\xB8L\xE6\xB5\xA5\xE7""5Be\xCEi\x01\x89q\x1FZ\xCC\x9E*\xDC\xB8N\xB5\x9B""2u\xA9)\x18S\xC3\xE9Uw\x94""85\xAF<u\x9D<]hN\xC5\rY=\xE9\xDB\xCDS$\xA1\xE6\x9E\xB2""f\xA9;\x93""b\xD6\xFA""7\xD4\x1B\xE9\xDB\xE9\x81)jM\xD5\x1E\xEA""7P\x03\x89\xA3""4\xC2\xD4\xD2i\x00\xF2x\xA6\xE6\x9B\x9AL\xD0\x02\x93II\x9A""Bi\x80\xA4\xD3\t\xA0\x9AnsH\x05""4\xDA)\xA4\xD0""1sHM4\x9E""1M\xCD\x00)4\xDA)3L\x00\xD2\x1A\t\xEDM&\x95\xC0""3HM%!4\x80(\xCD\x19\xA4""4\f\t\xA4\xA0\xD3I\x14\xC4)4\xC2h'4\xD2i\f\\\xE2\x9B\x9Ei\xA4\xD2\x13\xC5""0\x17'4\x84\xD3sHM\x00.i\xA4\xD3w\x1Ail\xD0\x03\x8BS\t\xA4&\x9AZ\x8E\xA0)4\xC2i\xA5\xAA""2\xF4\x00\xE6lTO(\x03$\xE2\xAB]^\xC7n\xA4\xB3s\xE9\\\xFD\xE6\xA5-\xC1 \x1D\xAB\xEDL\r+\xDD""ac\xCA""E\xCBz\xD6\x1C\xD7\x0F""3\x16v$\xD4Y\xE0\xD2\x01\x9Ai\x01\xED\xBA\x8E\xA0\x92""7\x97\x11\x01\x07\x03\x15\x94\xC3q\xDC\rVdu9\xCEjE\x93j\xFC\xC6\xB7\x93l\x85\xA6\xE2\xC8\xDB\x17\x9E\xB5sL\x9A\xE2\x16\x12\x06*{b\xAB[\xC0n$\xF3\x1B\xEE\x8E\x82\xB4\xD2\x12:T\xB7""d=\xCE\xA2\xD7T\xB5\xD4\xE3[k\xF8\xD4""1\xE0IL\xD5\xBC)yd\xBEthd\x84\xF2\x19y\xE2\xB9\xE4%Mu~\x1D\xF1""5\xED\xA0\xFB+Fn\xAD\xB1\x8F-\xB9*=\x8F\xF4\xA5{\x8A\xC7(!r\xF8\xC1\xE3\xADZ_\x94""b\xBB\x89\xF4\xAD?XW\x9E\xC8\xAC\x13\xF7\x88\xF5\xFC\xAB\x95\xBE\xB0\x96\xD6""B\xB2\xA1R=\xA9""4\t\x94\x1B\xDA\xA2""3s\xB4u\xA2g*v\x8E\xB5wF\xB6\x13]\t\xA4_\x95""9\xFA\x9A""C:\xEF\r\xDAG\xA4Y\x0B\xCB\x9C}\xA2o\xB8\xA7\xB0\xAE\xA6\xD3TY0\t\xAE\x1A\xEE[\x89""e\xF3\x18\x90\x07\x00z\nu\xB6\xA0\xF1""0\x04\x9AW\x13\xB9\xE9H\xEA\xE3*i\xD5\xC9\xD9k'\x8F\x9A\xB5\xFF\x00\xB5\x94\xC7\x9C\x8C\xD0""5>\xE6~\xBB\xFB\xFB\xD8""c\xEA\x03V\xF4\x10\xAA\xDB*\x15\x18\xC5s\xAB(\xB9\xD4\x15\x89\xCE\rt\xE9\xF7\x17\xE9@\xA3\xA9\x8D\xAA\xE8\x91\\\xA1""eP\x0F\xB5qw\xBAt\xD6\x8Er\xA7o\xADz\x7FZ\xA1{\xA7\xC7p\x84\x15\x06\x90\xDA\xB1\xE6[\x8D""7\xCB\x8AW_4\x12\xB9\xE7\x15\xD0j\x9A\x0B\xC4K\xC4""8\xF4\xAC\x07""FF*\xC0\x83H\x06\xDFi\xABn\xEB%\xBB\r\x8D\xD0T\xD6\xF2\xF9h\x01<\xD4""E\xB8\x19""9\xC5""D\xEF\xE9M\xC8I\x1BvZ\xC4\xF6S\x07\x89\xF8\xEE\xA7\xA1\xAE\xD3K\xD6\xAD\xF5$\x00\x10\x93wC\xFD+\xCB""D\xC4\x1E\xB5""b\x0B\xB7\x89\xC3\xA3\x95""a\xD0\x8A.=Q\xEB\xD4W)\xA2\xF8\xAD""d\xDB\x05\xE9\xC3t\x12z\xFDk\xAAWWP\xCA""A\x07\xA1\x14\xCALZ\r\x14\x84\xD0&\xC8\xA4\xE6\xA9\xC8*\xE3\x9C\xD5Y\x07Z\x92\nr\x0E\xB5NE\xCEj\xF4\xA0\xD5)A \xD2\x03:u\x19""5\x9F*V\x94\xAB\x8F\xADR\x95i\r\x19s'Z\xA3,u\xAB""2\xD5\x19R\x91""FL\xB1U'\f\x86\xB5\xE5L\xD5)\xA3\xCEil2\xB2\xCB\x91R\x07\x15]\xD3i\xC8\xA4W\xF5\xAAN\xE2h\xB5\xBB\x9A""2*\x00\xD4\xED\xD4\xC4K\xB8Q\x9A\x8Fu\x05\xA8\x18\xF2i\x0BS3Fh\x01\xDB\xB8\xE6\x9B\x9A""Bi\t\xA0""B\x93I\x9A""Bi\xA4\xF3\x9A\x00R\xDC\xD3I\xC5&M&h\x18\xA4\xE6\x92\x9AM7u\x00;<\xD2""f\x9B\x9AL\xF3@\x0B\x9A""Bi\t\xA4\xA2\xE0.i3HO4\xD2\xD4\fu!4\xD2z\xD2""f\x84!I\xFC\xE9\xA4\xD2\x13M-@\xC5&\x98h\xDDM'4\x80Ri\xB9\xA4&\x9AZ\x9A""B\x14\xB5""4\x9A""B\xD9\xA6\x16\xA0\x07\x16\xA6\x13H[\xDE\x98[\xAD\x17\x01KS\x19\xB1Mg\xC5R\xBB\xD4\"\xB7RY\x86\xEFJ\x06Z\x92@\xAAY\x88\x02\xB1\xEFu\x85L\xA4<\x9FZ\xCD\xBB\xD4\xA5\xB9""8\xCE\x17\xD2\xA8\x9EM4\xBB\x80\xF9""fy\x9C\xB3\xB1&\xA3\xA2\x8A\xA0\x0E\xD4\xB4\x94\xBC\xD0\x07\xB6\xDF\xF8v{Pe\xB6o>\xDC\xF2\b\xE7\x15\x93\x05\x9B\xDC\xDCm\np\x0F""5\xAB\xA3\xEB""7V\x92\xF9i\x99\x13\xF8\x90\xF2+\xB1\xD3?\xB2\xB5\t\xB7\xA4""b\x19\xCFT<s[\xAD\f\xCE""A\xAD\x1E\xD9@)\x81RD\xD8\xE9]\xDD\xE6\x8E\x18\x1D\xE8""9\xEF\\\xCD\xFE\x8E\xF6\xECJ\f\x8A\x86\x98)\x19\xC2?9\xF6\xA8\xF9\x8FLW\xA0xgA\x16V\xA2iW\xF7\x8F\xCF=\xAB""7\xC2:\ty\x05\xDD\xC2p>\xE8""5\xDEyc\x00\x01\x81I n\xC6\x15\xEE\x94\x8E\xFEt,b\x99z:\xF1Y\x17:\x80#\xEC\xDA\x9D\xB8\x93\xB7\x9C:\xD7""d\xD1g\xB5""bj\xB6\xD0\xB4M\xFB\xA0O\xA9\xAA&\xE7\x0F\xAAh\x86""9\x16[vY!~\x87<\x8A\xDC\xD1""4\x83\x15\xB8w^)\xBA""f\x8E""d\xB9\xDE\xD9\t\x9E\x99\xAE\xB1!T@\x80p+9n_C\x16k \xD9\xF9k.\xE7O\xC6H\x15\xD6\xB4#\x15R{`\xD9\xE2\xA4\x93\x8F\xCC\x96\xED\xDF\x15""8\xD4\x1Bn3Z7vc\x07\x8A\xC0\xBA\x85\xA2""cA[\x9A\xD6\x17\xB8\xB8\x07""5\xDC\xD9^$\xB0\xAEO5\xE5\x96\xD2\x94\x90\x1E\x95\xD2Y\xEA""e\x14|\xD4 \xD8\xEE\x83\xABt4\xEA\xE7l\xF5@\xCC\x06\xEA\xDE\x8AQ\"\x02\r1\xA9_q%\x81%R\b\xAEsU\xD0\x12PY\x17\x07\xD4WQH\xCA\x18`\xD2\xB0""5\xD8\xF2{\xEB)m\\\x86S\x8FZ\xCE""bk\xD4\xB5\x1D*;\x84""9Pk\x88\xD5""4)-\xD8\xB4""c+R\b\xC0""cM\x0ET\xF5\xA7\xBA""2\x92\b\xC1\xA8M!\x96Rz\xE8\xF4_\x13\xCD""b\xC2)Ix=\x0FQ\\\x8EH4\xF4\x9A\x9D\xC5""c\xD9\xAC\xF5\b/`Yap\xC0\xFAT\xCC\xD9\xAF&\xD3\xB5k\x8D>P\xF0\xB9\x03\xBA\x9E\x86\xBB\xBD+\xC4""0j(\x14\x90\x92\xE3\x95""4\xC4\xD1\xB4\xCF\x8A\x85\x8D!j\x8D\x9A\x81\f\x93\xA1\xAA\x92\xF1\x9A\xB1#qUd9\x06\x90\x14&<\x9A\xA9 \xCD[\x94sUd\xE2\x90\xCAR\xAFZ\xA7*\xD5\xD9""9\xE6\xAA\xCBI\x8D\x14%Z\xA9\"\xD6\x84\x82\xAAH\xB4\x8A""3\xA5N\xA2\xA9\xC8\x84\x1E+JE\xAA\xB2%H\xCA\x8A\xF8""4\xE0\xD9\xA4\x91*-\xC4\x1C\x1A\xB8\xC8V,\xEE\xF5\xA3uA\xBE\x9D\x9A\xA1\x12\xEE\xCD\x1B\x8DG\x9A""3@\x0F\xCD!jfh&\x90Xvy\xA4&\x9B\xBA\x93u\x17\x0B\x0E\xCD""4\xB5""4\xB5\x1Dh\x01""3M'\x14\xC9&\x00\x90*\x03#\x1A`Z\xC8\xF5\x14\x9B\xAA\xA1-I\xBC\x83\xD7\x9A\x00\xB7\x9C\xD2\x16\xA8VPx4\xFC\xD2\x01\xC4\xD3sI\x9Ail\n\x00q4\x85\xA9\x99\xA4&\x98\nX\xD3sHM!8\xA4""1sMf\xA6\x96\xA6\x93\x9A`8\x9E)\xB9\xA6\x93\xCD""4\xB0\xA0""B\xB3""b\x9A[\xD2\x9AMF\xCE=hz\fylT\x12\xCC\x10\x12\xC4\x00=j\xA5\xE6\xA3\x15\xB2\x9C\x9C\xB7`+\x9E\xBB\xD4%\xBAnN\x17\xD0P\x90\xACi_\xEB\x18\xCA""C\xCF\xBD""b<\x8F#\x16""c\x92i\x94U$0\xA2\x8A""1L\x05\xEFK\x8C\xD0\x07zp\x19\xA0\x06\x81N\xC6i\xC1i\xD8\x00z\x9A\x00\xFA#\xC3\x1E\x1D\x1CI\"\xF1\xD4\x93\xDE\xBAK\xDD\x02\xDE\xE4\x06\x88y2\xAFG^+B\xD5\x12\bV8\xC0\xC0\xABI\x96\xEA""1[\xBDL\x1C\x8Ez\rV\xF7I\"\rF\x1F>\x01\xC0\x90zV\xA1\xB7\xB4\xD4""c\x13\xDA""0t<\x95\xCF\"\xA5\xBE""0\xC7n\xE6p\nc\xA1\xAE\bj2\xD9\xDF""4\xB6.bL\xE7h<\x1AW\r\xCFI\xB5""d\x8D\x02(\xC0\x1D\xAA\xF2>Erz_\x89\xADo\xE4H\xAE\xB1o)\xE0""1\xE8Mtg\xCC\x84\x02\xC3*z0\xE8ih\r2\xC5\xC4\xE2\x18\x8B`\x9F\xA5""c]\xCA\x97""d\"\x8E;\x8A\xBD%\xDF\xCA""FsP\xDA\xC2\xAC\xC6LRz D\x96\x96\xCB\fc\x03\x93V\x82P8\xA5\xDD\xC7\xBD""fX\xD6^*\xBB\xA5X&\xA3j\x00\xCC\xB9\x8Cm&\xB0n\xA0\x0EO\x15\xD2\xDC)n*\x84\x96\xA3\x93R\xC1\x1C\xBC\x96""eI T&F\x88\xE2\xBA)m\xAB""2\xEE\xCF \xE2\x82\xAE""Eg~VA\x93]\x86\x9B\xA9\x82\xA0\x13^|bx\xE4\xEFZ\x96wo\x1E""2h\xB8\xACz:]\xA3\x0EML$V\xE8k\x8A\x8BT \x0F\x9A\xAE""E\xAB\xF2>jb\xB9\xD5\x1C\x1A\xA3uh\x92\xA9\xC8\xA6\xD9\xDF\t\xD3\xAD[-\x9A\x18\x1CN\xAF\xA0\x06\xDC\xF1\x8C\x1A\xE4nm^\xDD\xCA\xBA\x91^\xB9""4K <W=\xAAh\xE9:\x92\x14""f\xA5\xA2\x93<\xE9\x85""F}\x8Dj\xDF\xE9\x92[9\xF9IZ\xCCu\xC1\xA9(E\x90\x8A\xB3\r\xCB\xC4\xE1\xD1\x8A\xB0\xE8""ES#4\xCD\xE5M\x17\x11\xDFh\xFE'\x12\x05\x86\xEC\x80\xDD\x03\xFA\xD7J%\x0E\xA0\xA9\x04{W\x91$\xDC\xF5\xAD\xED'\xC4R\xD9\x91\x1C\xA4\xBC_\xA8\xA7q4wN\xD5^CQ\xC1{\r\xDC""BH\x9C""049\xA0\x92\tj\xAC\x83\"\xACHx\xAA\xEEi\x8C\xA7 \xAA\xB2\n\xBD \xAAr\nLeG\x15U\xC5\\q\xCDW\x90R\x19I\xC5V\x91j\xEB\n\x82""E\xE2\xA6\xC3(:\xF5\x15U\xD2\xB4\x19sU\xA4ZLe\"\xC5O=)\xC1\xA9\xD2'^*\x03\x95>\xD4\xD4\xBB\x85\x89\xF7R\xEE\xA8\x15\xF3\xD6\x9D\xBE\xAC""D\x9B\xA8-Q\xEE\xA3u\x02\x1F\x9AL\xD3""7Rf\x80\x1F\x9A\x8AYp0)\xC5\xB8\xAA\xD2\xB7&\x84\x00>c\x9AvF*5<R\xE6\x86\x03\xB3\x8A""c\x01\x8A""3HM4\x03\x0B`\xD4\xE8\xF9Z\xA8\xC7\x9A\x9A?\xB9""C\x02R\xD4\xDC\xD2""f\x90\xB0\xA5""a\x8E\xCD""74\x99\xA6\x96\xE6\x80\x14\xB5""0\x9A""Bi\xBB\xB8\xA6\x02\xE6\x9A[\x9Ail\xD3""7P\x03\xCBqL-L/\x83\xC9\xAC\xFB\xCDJ+py\xDC\xDE\x82\x8B\x81rY\x95\x14\x96 \n\xC3\xBD\xD6s\x94\x83\xF3\xAC\xEB\xAB\xF9nX\xE5\x88_J\xAB\xD6\x9D\x80t\x92<\x8D\xB9\x89$\xD3h\xA5\xC5P\tF3KN\x0B@\r\xC5;\x14\xB8\xA5\xC5\x00 \x14\xE1\xD2\x80)q@\tJ)q\xCD(Z\x00\xFA\xDA\x19""F\x06*\xD2\xCA\x00\xC9""5\xCDZ\xDFt\xC9\xC5Z\x92\xF0\xB4{T\xE7""5\xB1\xCE""3Y\xB9k\xA0""bS\xF2\x0E\xB5\xCCMj\xC8r+x\xF3Q\xBC!\xC7J\x97\xA9KC\x9Coq\xCDk\xE9~$\xBB\xD3@\x8D\xC9\x9E\x03\xFC\fs\x8F\xA5Gse\x9E@\xA6i\x9Aq\x9E\xEFs\x8F\xDD\xA1\xFC\xCD-\x8BZ\x9D\x95\x95\xC1\xBF\xDB \x8C\xC6\xAD\xCE\xDC\xF4\xAD\x94\x01\x14\x01T,\xD1""a\x8C`b\xAE\x07\x18\xA9n\xE4\x93\xEE\xC5""7vj=\xF9\xEFK\xBB""4\x80~i\x84\xD2""f\x82h\x02'\xA8\x19jv9\xA8\xD9M!\x95$\x8B<\xD5Y\xA1\xC8\xE4V\x89\x15\x1B #\x9A@`\xCB""f\t\xE9U^\xD8\xAFJ\xE8\x1E.j\xB4\xB0""dt\xA0g9,\xAD\x13`\x9A`\xBD""a\xDE\xAF_Y\xE4\x13\x8A\xC3\x91\n6\r+\x8D\x1Dn\x87\xA8\x96p\xA4\xD7_\x1C\x9B\x90\x1C\xD7\x9Ai\x12\x15\xB8\x1C\xD7}e.\xE8\x85""4\xC4\xD1\xA1\xBA\xA2\x91""C\n7R\x16\xA0""Fe\xF6\x9E\x93!\xCA\x8A\xE3\xB5=\x19\xA2""b\xD1\x8F\xC2\xBD\x05\xB0G5J\xE6\xD9""e\x07\"\x93""CL\xF2\xC9#db\x18T.+\xB3\xD4\xF4P\xD9*0k\x97\xB9\xB4x\x18\x86\x06\xA2\xC5\xA6P\xE4S\x96\\R\xB2\xD4L\xBC\xD2\x03R\xC7S\x9A\xCA]\xF1""9\xC7u\xECk\xAE\xB0\xD6\"\xBELd,\x9D\xD4\xD7\x9E\x07*y\xA9\xE2\xB8h\xD8""2\xB1\f;\x8Ai\x89\xA3\xD1\xDC\xE6\xAB\xBD""b\xE9\xBA\xF8\x90\b\xAEH\r\xD0""7\xADl\xB1\f\x01\x07 \xD5\\V!z\xAD \xC9""5i\xEA\xBC\x82\x80*8\xAA\xB2\x0Ej\xE3\x8E\xB9\xAA\xB2-!\x95\\T\x12\n\xB2\xD5\x0B\x8E)1\x95\x18Uv^M[u\xA8Yr\r!\x94\xDDj\xBB\xAF\x15q\xC6sU\xDDjX\x14\xD8\x15<R\x07\xCDL\xE2\xAB\xBA\xE3\x91\xD6\x84\xEC\x16$\xCD\x19\xA8""C\x1Av\xEA\xB4\xC4K\x9AL\xD3""7R\x16\x04S\x01\xC4\xD4""2\x03\xCD;4\x9Dh\x02\x15m\xA6\xA4\xDC\x0Fzk'qQ\x1C\x8E\xD5@K\xBA\x98\xCC*<\xBE:Q\xB0\x9E\xA6\x80\x15r\xC6\xA6\x07\x03\x14\xC0\x02\x8E(\xCD\x00<\xB6\x07\x14\xD2\xC6\x9AM4\xB7Z\x00qn\xF4\xD6=\xE9\x85\xA9\xA4\xD0\x03\xF7qL-M-L-\xCD \x1E[\x02\xAB\xCB:\xC4\xA5\x9D\x80\x15Z\xF3R\x8A\xD9H\xC8""f\xF4\x15\xCE]\xDE\xCBt\xE4\xB3|\xBE\x94\xD2\xB8\x17\xEFu\x86|\xA4<\x0FZ\xC8ggb\xCCrM6\x96\xAC\x03\x14\nZQ\xD6\x80\x13\xBD""8\x01H:\xD3\xE8\x01""6\xD2\x8AZ\\P\x02R\xE2\x94sN\xDB@\r\x02\x97\x1E\x94\xFC`R\x81@\r\x02\x9D\x8ApRjP\x98\x14\x01\xF4""D+i\xA8\x8F""2\xD6M\x8F\xDD\rZ\xF2\x1E\x10\x03\xA9\x1FZ\xE4\x15""d\x85\xC3#\x15""a\xD0\x8A\xDF\xB1\xF1#\xF9""B\xDA\xF4nN\xCD\xE9Zheb\xFE\xDA""6\x9FJ\xBB\r\xA2\xDD[\xF9\xD6\x92\xAC\xA3\xA9Q\xD6\xAAJLa\xB7)R;\x1A\x1A\x11Zl\xB6#Q\xF3\x1A\xD3\xB1\xB6X\xD1x\xE9T-\x86\xE9\x03""c\xE6""5\xB1\x19\n\x00\xA8""e\x17U\xEAM\xC2\xAA\x86\xC5""8=!\x16\x83S\xB7Um\xDE\xF4\xA2""C\x8A\x00\xB1\xBA\x97vj\x00\xF4\xED\xD4\x80v\r-45.\xEA\x06""5\x97""4\xC2\xA4u\xA9\xB2)\x18\x82(\x02\xB1QQ:U\x82""9\xA6\x11\x9A\x04""f\\\xC3\xB8\x1E+\x02\xFA\xD3\x92@\xAE\xAAT\xC8\xAC\xCB\x98\x01\x07\x8A\x92\x93""0\xAC\x14\xA4\xC3\"\xBB]:O\x90\n\xE6R\r\xB2""d\n\xDC\xB1|\x01@\xDB""6\xF7Ry\xB8\xEBP\x87\xE2\x91\x98\x11L\x92""b\xE0\xF4\xA6\x13P\xEE\xC5\x1B\xA9\x00\x92\xA2\xBEk\x1A\xFF\x00MIT\xFC\xA2\xB6\x0BTO\x83@\xCE\x06\xFBLxX\x95\x19\x15\x94\xE8""A\xC1\x15\xE8\x97""6\xAB <W7\xA8i=Y\x05KE&sL\xBCTD\x95\xAB\x93""B\xF1""1\f*\xBB(5\x05\b\x92\xD6\xC6\x9D\xADIm\x84\x90\x97\x8F\xF5\x15\x84T\x8E\x94,\x98\xEB""B`\xCF@\x8A\xEA+\x98\xC3\xC6\xC0\x83H\xFD+\x8C\xB5\xBE\x96\xD5\xC3#q\xDCz\xD7Ig\xA9\xC5x\x98\xCE\xD7\xEE\rU\xC9\xB1;\x8A\xAF \xABO\xD2\xABIL\n\x8E*\x16\x15""a\xFA\xD4\x0E(\x02\xBB\x0Ej\x19:T\xED\xD6\xA0z\x91\x95\x88\xA8\\U\xA2\xBC{\xD4\x0E)1\x95XsP\xBA\xF5\xAB.8\xA8XR`Uu\xC5""E\xB8\xAFQV]sP:f\x84\xEC""0\xC8\xA3""5\x16J\xF1K\xBA\xAD""1X~\xE1\x9A\x0Bq\xC5""F[4\x84\xD3\x10\xF2\xD4\xDAnh\xCD\x00)\xA3""4\xC2\xC3\x14\x85\xA9\x80\xE2x\xA6\x96\xA6\x13\xCFZBi\x80\xE2\xD4\xD2\xD4\xD2\xC2\x9A\xCD\xCD\x00<\x9E*2\xD4\xC6|u5\x9By\xAAG\x00!N[\xD2\x90\x17\xE5\x9D\"R\xCC@\x15\x85{\xAC""3\x92\x90\xF0=k>\xE6\xEE[\x86\xCB\xB1\xC7\xA5W\xABH\x07""3\xB3\xB1,I&\x9B""E-0\nZ\x05""8\x0E(\x01\x07Z\\b\x9C\x05-\x00""4\nv)qF(\x00\xC5(\xA7\x81N\t@\x86\xA8\xE3\x8Ap\x14\xE0\x9E\xD4\xF0\x99\xA0""c\x02\x9Az\xA7sSE\x13\xC9 H\xD0\xBB\x9E\x02\xA8\xAE\xAFH\xF0\\\xF7;e\xBE>\\}|\xB1\xD7\xF1\xA0M\x9C\xCD\xA5\x94\xF7\x92\x88\xED\xA1i\x1C\xFA\x0E\x05vzG\x82Q\n\xCD\xA8""6\xF6\xEB\xE5\x8E\x82\xBA\xBB-2\xD6\xC2!\x1D\xBCJ\xA0z\x0E\xB5p-\x17\x11j{ \xD9\xC0\xAC\xC9m\x99\x0FN+\xADxCv\xAA\x93\xD9\x82\x0E""Eib.s\xD6\xB7\xF7Zt\xA2[Y\x99\x18v\x07\x8A\xDE\xB7\xD7%\xD6rg\xB7@\xE3\x82\xEB\xDE\xB1\xEE\xEC\xB7L#\x8C\x8EO85\xBD\xA6Z,1\x05\x03\x00T\xB6\xD0\xF4""e\x88!1\x9D\xC7\xA9\xAB\x8B\xD2\x98""E.jEr`\xE6\x9D\xBCT\x1B\xA8\xDDH\x0B\x02Ni\xDB\xBD\xEA\xB0jpj\x06X\x0Ei\xE1\xBD\xEA\xBA\xB1=i\xF9\xA0""E\x80\xD4\xBB\xAA\xB8lR\x87""4\x01>\xEEiwT!\xC6)K\xD0\x03\xCF&\x92\x99\xBC\xD0\x1B""4\x86#\xADS\x9A<\xD5\xD2""F*\x17\x19\x14\x80\xCD\xF2\xBEj\xB5\x07\xCAi\x19y\xA7 \xC5\x03/+\xF1K\xBCT \xF1""Fh\x11!jijf\xEA""B\xD8\x14\x00\xF2i\x8C\xD8\xA6\xEF\xA6\x93\x9A\x00\x18\xD5ic\x0E""9\x15)4\xC6""94\f\xC4\xBE\xD3\x96@N\xDA\xE7.\xAC\x9E\x16<q]\xCC\x80\x11Y\xD7V\xAA\xEAx\xA9h\xA4\xCE%\xD6\xA2""e\x07\x9A\xDB\xBD\xD3\x8A\x92\xCA+&D*pEE\x8A+\x82T\xD4\x91L\xC8\xC1\x94\xE0\x8A""F\\\xD4$\x15\xE9H\x0E\x96\xC7Y\x0E\x04s\x9C\x1E\xCDZ\f\xC1\x86""A\xC8\xAE-^\xB4l\xB5""7\x80\x84|\xB2\x7F*\xA4\xC5""cu\xC5@\xE2\xA5I\x92x\xC3!\x04\x1Ak\n\xAB\x88\xAA\xE2\xA1""aV\\T\f)\x01\x03\n\x81\xD6\xAC\xB0""5\x0B\xD2\x19Q\x85""D\xC0\xD5\x96Z\x85\xC7""5,ef\x15\x03\xF5\xAB.8\xA8\x1CR\x19]\x81\xA8\x8F\x15""d\x8A\x85\xC5\x17\xD4\x06\x06\x14""f\x98\xC3\x07\"\x9B\xBB\x9A\xD1;\x88\x90\x9Aijf\xEEi3T\x03\x89\xA6\x96\xA6\x96\xF7\xA6\x17\xA1\by8\xA6\x96\xA6\x96\xE3\xAD""F\xCE(\x02""B\xC3=j\t\xAER%,\xCD\x80*\x85\xE6\xA6\x90\f/\xCC\xD5\x87=\xD4\x97\r\x97o\xC2\x9D\xAE\x05\xEB\xDDY\xA5\xCA""E\xC2\xFA\xD6Yb\xC7$\xE4\xD2QT\x90\x05\x18\xA5\xC5\x18""4\xC0)i@\x06\x94P\x01\xD7\x03\x14\xECP\x05=W4\x00\x80sN\xDBR$D\x9C\x01\x93ZQ\xE8\xD7\r\x10\x96m\xB0\xC7\xFD\xE9""8\xA0\f\xAD\x86\xAC[\xD9\xCB;b4'\xDF\xB5[cajx\xDD;\x8E\xE7\x85\xA8%\xBF\xB9\xB8\x1EZ\x11\x1C~\x88""1H\t\xE5\xB6\xB5\xB3\x8C\x89\xA5\xF3&\xEC\x91\xF6\xFA\xD5""E\x1B\x86v\xE3\xDA\x96""8v\xF2y'\xA9""5\xB3\xA5\xE8W\xBA\x9B\x0F\"\"\xB1\xE7\x99\x18`S\x15\xCC\xA5Nzd\xFAWA\xA4\xF8R\xF7Q*\xEE\xBE""D\x07\xBB\x0EMv:G\x84\xAC\xF4\xFD\xB2H<\xE9\xC7\xF1""7o\xA5t+\x1E\xD1\x80\x00\x14\xAE##K\xF0\xFD\x8E\x94\x83\xCA\x88\x17\xEE\xED\xC9""5\xAC\x17\x1CT\x9Biq@\f\x0BK\xB6\x9D\x8A\\P#\xA5\x0BPK\x85S\xBB\x9C\xF6\xA9\x9D\xF6\x02}+2I\xFC\xD9\x0F\xA0\xAD\x99\x99\x1CV\x88n\x0B\x8E\xFE\xB5\xAC\x98""E\x00U8\x97h\xCDM\xE6VM\xDD\x96O\xBA\x90\x9A\x87\x7F\xA1\xA5\xDFHD\xB9\xE6\x8D\xD5\x1E\xEA""7P2`\xD4\xE0\xD5_u(|R\x02\xC8jpz\xAE\x1A\x9E\x1A\x98\x13\xEE\xE2\x8C\x91P\x06\xA7o\xA0""D\xDB\xE9\xC1\xAA\x00\xE0\xD0_\xDE\x90\xCB\x14g\x15T\xDC\x15\xECiVB\xE6\x80,\xE7\x8Ai\"\xA3\xC9\x1D\xE9\t\xA4\x00\xDDi\x83\x8A""B\xD9\xA6\xE7\x9A\x06N\xAD\x81N\xDDP+S\xB7P!\xFB\xB3I\x9E)\xB9\xCD!4\fvi\xA4\xFB\xD3wc\x8Ai\xA0\x05'\x9Ai#\x14\xD2i\xA4\xD0\x02\x13Q74\xF2j64\x80\xAF""4*\xCB\xD2\xB1\xAF""4\xF0\xC0\x90""9\xAD\xE6\xA8$P\xC2\x93\xD4\xA4\xCE""6{w\x88\x9C\x8E*\xB3-uw6\xAA\xE0\xF1X\x97""6\x05""2TT4RfK&zSC\x15""85a\xD0\xAFQQ2\xE6\xA4""d\xF6\xF7r[\xB8""dn;\x8A\xDB\xB6\xBD\x8E\xE5p8n\xE2\xB9\x92\n\xD3\x92""fB\n\x92\b\xAAN\xC2\xB1\xD4""0\xA8\x1CsU-5\x11&\x12S\x83\xEBW[\x04g\xADP\x88H\xC8\xA8$\x15""d\x8A\x89\xD7\x8A@Tj\x85\x86jw\x1C\xF3P\xB0\xC5!\x90""0\xA8\x18sVXT\f1I\xA0 aP\xB0\xFC\xAA\xC3\x0Fj\x85\x86""E!\x95\xD8""f\xA2""aS\x91Q0\xA3\xA8\x10\x96\xC5!n(qQ3\x11\xC5Z\x90\x87\x16\xA8\xCB\xF3Lw\xC7$\xD6m\xDE\xA6\xB1""d)\xCB{U\xA0/Mr\x91)flV%\xE6\xA8\xF2\xE5""c\xE0z\xD5)\xAE$\x9D\xB2\xCC~\x95\x15RB\x14\xB1'$\xE4\xD2QK\x8A\xA0\x12\x94\n\\R\xFB\xD0\x02\n\\f\x94\x0Fjp\x18\xA0\x00\nP)@\xA7\xAA\x8C\xD0\x02*g\xAD_\xB0\xD3\xE5\xBC\x93lc\n>\xF3\x1E\x82\x9Bmh\xD3""e\xD8\xEC\x89y,i\xD7:\x96\xD8\xFE\xCFm\x95\x88u\xC7\xF1P\x06\xDF\xF6\x8E\x9D\xA1![(\x92\xE6\xEF\x18i\xE4\x19U\xFA\n\xE7\xEE\xF5+\x9B\xE9K\xCB+H\xC4\xFE\x02\xAA\xE1\xDF\xA9\xC0\xABV\x96\x92\\J\xB0\xDB\xC4\xD2""9\xEC\xA2\x84\x84""E\x1C$\x9D\xCDZv\x1A]\xCD\xFC\x81-\xA1g>\xB8\xE0WY\xA2\xF8\x1D\x98,\xBA\x8Bq\xD7\xCA_\xEAk\xB6\xB4\xB0\xB7\xB3\x8CG\x04J\x8A;\x01""E\xC5s\x97\xD2<\x15\x05\xBE\xD9o\x8F\x9B']\x9F\xC2+\xAD\x8A\xDE""8P$j\x15G@\x05M\x8A\\R\x10\xD0\xB8\xA5\xC5;m\x1Bh\x01\xB8\xA5\xC5;\x1E\xF4""b\x80\x1B\x8A""1O\x02\x92\x80""4[P\x86\xE2""2\"\x90g\xB8=j\xAC|\xBEqYz]\x97\x92\x0B\xB3\x12X\xF1\x9E\xC2\xB5\xD7\x81\x81U&+X\x9D""d\xC7Jyl\xD5l\xD4\x81\xFDj@\x90""6)\xC1\xAA""0\xC0\xD2\xE6\x90\xC9\xB7\n\\\xD4""9\xA7\x06\xA0""D\x80\xD3\xB1\xEFQ\x83J\x1A\x80\x1F\xBB\x06\x9E\x1A\xA3\xCD&h\x02|\xD1\x9A\x87q\xC5(j\x00\x97""4f\x99\xBA\x82y\xA0""d\xA0\x8E\xF4\xE0Tt\xA8""7b\x97} &\xDFI\xBA\xA3\xDDI\xBA\x81\x0F&\x92\x9B\xBA\x8C\xD0""1\xD4SwRy\x9E\xB4\x00\xFD\xD8\xA3uFOzBM $\xDC)7Ty\xA4'\x14\xC0s74\xC2qF\xE1L'\xDE\x90\x03""54\x9A\t\xE6\x9AM\x03\x11\x8F\x15\x11\xA7\x93Q\x9A\x04""F\xC3""5ZXC\x03\xC5ZcQ\xB1\xE2\x81\x98\x97V!\x81 sY2\xC0\xD1\x92\b\xAE\xA9\xD4\x1A\xA3=\xBA\xB8<T4U\xCEm\x85""D\xC9\xE9ZW\x16\x8C\x84\x90""8\xAAL\b\xE2\xA1\x8C\x80""1\x1FZ\xBFk\xA8""4XW\xE5j\x93(n\rFr\xB4\\gJ\x92,\xAB\xB9H\"\x9A\xFD+\n\x0B\xB7\x85\xB2\xA7\xF0\xADXo\x12u\xC1""8oJ\xA4\xC4\x0E\xB9""5\x03\xAF\x15""a\xC5""B\xE3""4\xD8\x8A\xEC*\x17\x15`\xD4N*FVaQ0\xC5N\xC3\x9A\x85\x85\x00WaP\xB0\xEBV\x1CT,)Xeg\xA8$5a\xEA\xB4\xA3\x1D)\x81\x8B\xA9\xDC:eT\xE2\xB1I$\xE6\xB4\xB5""2\f\x84Vmm\x1D\x84\x14\n)qV \xC5(\x14\xA0""f\x97\x18\xA0\x00\n\x06?\nu.(\x01""1N\x02\x93\x14\xFD\xBCP\x00\x00\xF4\xAB\xD6\xB1\xDAG\x17\x9Fu/\x03\xA4k\xD4\xD5""08\xE6\x94\x85\x03-\x8A\x00\x92\xEE\xF2[\xD2\x11G\x95\x02\xFD\xD4\x15\bU\x8C""d\xD3|\xD2\xFC""F3\xEFNHpw9\xDCh\x10\x81\x9ES\xF2\r\xA3\xD4\xD7""c\xE0""B\xB6\xFA\xA3""Fy2.rk\x97""E\xE7""5\xB5\xA0Lm\xB5[y3\x81\xBB\x1F\x9D\x02g\xAF\xAA\xF1R\x05\xA6""BCF\xA4zT\xC0R\x10\x81izS\xB1K\x8A\x00h\x14""b\x9D""E\x00""7\x14\xB8\xA5\xA3\x14\x00\x94\x11N\xC5%\x00$K\xB4\x0E\xD5""05\n\xB0""5*\xF4\xA4\x03\xC5<\n\x8A\x94""5\x00I\xC5(\xCDG\xBA\x97""4\x00\xFD\xD4\xE0\xD5\x1E}iH\xC8\xE2\x80%\rK\x9A\x84\x12:\xD3\xC3P\x04\x99\xA5\r\x8A\x8C\x1A\\\xD0\x04\x81\xA9w\n\x8A\x8A\x00\x97u.j\x1C\xFB\xD1\xBA\x80&\xCD.j \xF4n\xA4\x04\xB9\xA3""5\x11j7R\x02\\\xD2""f\xA3\xDD""F}\xE8\x02L\xE6\x8C\xD4{\xA8\xCE\x05\x00II\x9A""f\xEFZ7zP1K\xD2\x16\xCD""0\xD2\x13\x8A\x04""8\x9Ei3L\xDD\x9A""Bh\x1A\x1CM0\xB5!4\x84\xD0\x00i\x84\xD0N)\x84\xD0\x00MF\xC6\x9CMF\xC6\x81\x8Dj\x89\xAA""CQ\x9A\x00\xAF,A\x87J\xCC\xB9\xB3\xEA""Ek\x9A\x89\xD4\x1E\xD5-\f\xE7$\x88\xA1\xE4T,\xB5\xBB=\xBA\xB5""fOl\xC8I\x03\x8A\x86\x86g\xB2\x91\xC8\xA4We9\x07\x06\xA6""e\xEC""EB\xE9\xE9R3B\x0B\xFD\xC0,\x9D}j\xD1 \x8C\x8A\xC1\xCE\x0E*\xD5\xBD\xE1\x8C\xED""c\x95\xAAO\xB8\xAC^aQ8\xA9""C+\xAET\xE6\x98\xFD)\x81X\x8C\x9E\x95\x1B\x8A\x9C\x8A\x89\xA9\x01Q\xC5""B\xD5""e\xC5@\xC2\x98\x15\xD8\n\xAB(\xE2\xAD\xB0\xE6\xABJ84\x80\xE5\xF5>e5B\xB4""5?\xF5\xF5""C\x15\xBCv\x10\x80qN\x02\x8CS\x80\x18\xAA\x00\x02\x97\x14\xB4\xA2\x80\x12\x9D\x8A""1N\x03\x8A\x00LS\x85""5\x9DW\x1D\xCF\xA5""4+\xC8r~Q\xE9@\x0E""2\x0E\x8A""7\x1A""A\x1B""9\xCC\x87>\xD5\"\xA8^\x83\x14\xE0(\x10*\xE3\x00\fS\xC2\xE6\x90\x03R\x01@\n\xA3\x9A\xB9nJ\xB8#\x82\b\"\xAB(\xE6\xAC\xC3\xD6\x9A\x13=\x8BH\x98\\i\xD0\xC8\x0FU\x15\xA2\x05s>\x0E\xB9\x13i*\x99\xE6""3\xB6\xBA\x81I\x92\x83\x14\xB8\xA5\x02\x94\n@7\x14""b\x9F\x8A""1\x8A`3\x1CR\xF7\xA7""c4\x98\xA0\x04\xC5!\x14\xECQ\x8A\x00\x80qOV\xA6""f\x93uH\xC9\xC3""dR\x12""A\xA8\xC1\x18\xA7""f\x90\x0F\r\x8E\xA2\x9C\x18\x11\x91Qf\x94\x1CP\x04\x9B\xB1N\rQ\xEE\x14\xB9\xC0\xE2\x80%\xCEO4\xBF\x8D""C\xB8\xD3\x83P\x04\xA0\xD1\x91L\x06\x8C\x91@\x12g\xDE\x8C\x9A\x8F""4f\x80$\xCD.j,\xD1\xBA\x80%\xCD&y\xA8\xF3@4\x01.h\xCD""FM\x1B\xA9\f\x93""4\xB9\xA8\xB7R\xEE\xA0\t3Fj=\xF4\x9B\xE8\xB8X\x97""4\x9B\xAA=\xD4\x9B\xA8\x11&i\xA4\xD3sHM\x00.y\xA4&\x90\x9Aij\x06.i\t\xA4\xEBM&\x81\nO4\xDC\xD0M4\x9E(\x18\xD6""4\xD2h&\x9AO\x14\x86!\xA8\xCFJq\xA6\x1A\x00i\xE9L=i\xE6\xA3j\x00""cUi#\f*\xC3TF\x90\xCC\xCB\x8Bn\xB8\xAA\x0E\x85O5\xBC\xEB\x9A\xA7=\xB8""aR\xE2;\x98\xCE\xB9\xA8NT\xD5\xE9""b(H\xAA\xCC\xA0\xF0j,1!\xB8h\x9B\x83\xC7\xA5hG2\xCC\xBF)\xE7\xD2\xB2\x18\x15""4\xA9#!\xC8""84\xD3\x03Y\x85""F\xF5\x1C""7BA\x86\xE0\xD4\xAD\xD2\x98\x8A\xF2\x0E*\xBB\x8A\xB4\xDC\x8A\xAE\xE3\x8A`Vq\xDE\xAA\xCB\xD0\xD5\xC7\x1CUYG\xCAh\x03\x95\xD4\x87\xEF\xCDQ\xC7j\xD0\xD4\xBF\xD7\xD5\x1CV\xD1\xD8""B\x01N\x02\x8AZ\xA0\nP(\x14\x9B\x8F\xF0\xD0\x03\x89\x03\xA9\xE2\x9B\xB9\x9F\x80""0=iBs\x93\xC9\xA7\x81\xCD\x00""5c\x00\xD4\x80P)@\xA0\x00\nz\xAD\x00S\xC5\x02\x00""9\xA7\nAR\x01\x91L\x05""AV\x13\x00\x8A\x85""EL\x94!\x1D\xB7\x81\xAE""6\xCF<\x04\xF5\xC3\n\xEF\xD4W\x95xf\xE3\xEC\xFA\xCC\x07\xB3\xE5Mz\xACg*(d!\xE0R\xE2\x8CR\xD2\x18\x98\xA3\x14\xB8\xA2\x80\x13\x14""b\x9D\x8A""1@\r\xC5\x14\xA4Q\xD6\x80)f\x80i\x99\xA3u@\xC9\x06)\xC0\xD4""A\xA9\xC1\xA8\x19%(4\xC0\xD9\x14\xB9\x14\x00\xE1K\x9Anis@\x87n\xA7\x0E\x94\xCC\xD1\x9A@I\x9A]\xD5\x1Eh\xCD""0$\xCD\x19\xA6g\x8A""3H\x07\xE6\x8C\xD3sFh\xB8\x0E\xCD.j2sI\x93L\tsEG\xBA\x8D\xD4\x86I\x9A@\xD4\xCD\xD4n\xA0\t3\xC5%3u&sH\t\x01\xA3""4\xCC\xD2\x13@\x0E'\x9AL\xD3sE\x00?u!9\x14\xCC\xD0M\x00)4f\x99\x9AM\xD4\xC0V4\xD2i\t\xA4&\x90\xC2\x9AO\x14\xB9\xA6\xB1\xA0\x06\x9A""e8\xF4\xA6\xD0\x02\x1A\x8D\xA9\xE6\x98h\x18\xC6\xE9Q\x1A\x94\xD4m\xD6\x81\f5\x13\x0Fj\x95\xAA&4\x80\xAB""4!\x81\xE2\xB3g\x87i\xC8\xAD\x87\xE9T\xE4L\x92\rKW(\xCA""a\x9A\xAE\xCAGJ\xD0\x9A\fr*\xAB\xAF\xB5""E\xAC""4W\x0F\x83Vb\xBA""8\xDA\xFC\xFB\xD5wL\xF2:\xD4""D\xE3\x83""E\xC0\xD5\xC8#\"\xA2u\xEBU\"\xBA""1\xF0y\x15lH\xB2(*j\x93\x11\x03\xAE*\xAC\xAB\x95""5zA\xC5V\x91x\xA6\x80\xE4u0|\xFE\x95""B\xB6uh\xB0\xFB\xAB \x9CV\xF1\xD8""Bb\x83\xC7NM(\x19\xA5\x03\xB5""0\x1B\xB0\xF5""c\xF8S\xC0\xA0u\xE6\x9C\x05\x00\x18\xCD:\x81K\x8Eh\x00\x02\x94\nP2)\xC0P \x03\x8Ax\x14\x98\xA7\x01L\x01""E=A\xC5&9\xA9\x14""b\x98\x85Q\xCFZ\x9A""0sQ\xADL\x9Dh\x02\xE5\xA4\xBEL\xF1\xCA\x0E""605\xEB\xF6\x12\x89\xAD""cps\x95\xAF\x1C""A\xC6+\xD4<+rn4xI9*0h{\x12\xF7""7\x80\xA7""b\x85\x14\xECT\x80\xDCR\xE2\x9D\x8E)\x05\x02\x1B\x8A""0i\xF8\xE6\x8CP\x03""0h\xC5?\x14\x11@\xCC\x9Am&y\xA7g\xD6\xA0\xA1\x07\xD6\x9D\x9AL\n1H\x07\x06""4\xE0sL\x14\xB9\xA6\x04\x9B\xB1J\x1B\x8A\x8F""4\xB9\xA4\x04\x84\xE2\x82\xD4\xC0\xD4u\xA0\x07\xEF\xA0""52\x92\x80""D\xD9\xA5\xCD""B\x1C\x8Ax`h\x01\xF9\xA4\xDDM\xA4\xDD\xEB@\x0F\xCD!4\xDC\xD2\xE6\x80\x1D\x9AM\xD4\x9F\xCA\x83\xD2\x80\x1D\x9C\xD2""f\x9B\x9A""7P\x03\xF3@4\xCD\xD4\x13\x8A@IHi\x81\xA8\xCD\x00;4g\x8A""f\xEA\r\x00;4\xD2qI\x9AJ\x06)<Rf\x92\x90\xD0!sHO\x14\x8DM9\xA0""c\xB3\xC5""4\x9A""Bx\xA4\xCD\x00\x19\xA4\xA2\x92\x80\x10\xD3\x1A\x95\x8E\r4\x9A\x00i\xA6\x1ARi\x87\xA5 \x1A\xD5\x11\xA9\x18\xE2\xA2""4\f\x8D\x8DVo\xBDV\x1A\xA1""a\xF3R\x02""7PF*\x9C\xD0g\xA5h\x11\xC5""D\xC3\"\x90\xCC\x89\x17mA\"\x82+Rh\x83\x0E\x95""FH\xCA\xD4\xB4""2\x8B\x02\xB4$\x8D\x19\x04\x1A\x99\xD6\xAB\xBA\x91\xD3\xA5\b\x0B\xCBp\xB2\xAE\x0F\rC\xAF\x15\x9B\xBF\x07\x83S\xC7w\xC6\xD7\xFC\xEA\x84gk1\x8F \xB6y\x06\xB9\xC2;\xD7""E\xAC\xBE""b \x1E\rs\xDD\xEBh\xEC!@\xA5\xC5\n)\xD8\xAA\x01\x05-\x14\xE0(\x00\x00\xD3\x80\xA3\x1CS\x94w\xA0\x05\x03\x8A\x05.)@\xA0""B\x81\xC7""4\xE1\xC5&)\xF8\xA6\x00\x07z\x91z\nn8\xA7\xAE""1\x8A\x04""8\n\x9A""1\xC5""D*e\x1E\x94\xC4L\x95\xDCx\x1E\xEB+5\xB9?u\xB2?\x1A\xE2Pt\x15\xD0xR\x7F#Y\x0B\xDA""E\xC7\xE5LOc\xD3V\x9E*4$\xE2\xA5\x15\x02\x17\x14\x98\xA5\xA2\x81\x89\x8A""1N\xC6iq@\x86\xD1K\x8A""1@\x1C\xECW\x10\xCC""3\x14\xAA\xFF\x00""CRf\xBC\xAE\xDBV\x9A\x12""0\xE4~5\xBFg\xE2i\x97\x1B\xA4\xDC=\x1A\x93K\xA1\xAF+Gm\xBA\x9C\b5\x89m\xAF\xDA\xCD\x80\xE7""c{\xF4\xADX\xE6I\x00""d`G\xA8""5;\b\xB1\x90h\xA8\x81\xA7\x06\xC5\x02\x1FK\x93M\xDD""F\xEA""C\x1D\x9A\\\xD3r(\xCD\x02\x1D\x9AZfis@\x0E\xA4\xA3""4R\x18\xB9\xA5\xEBL-\xE9@jb\x1F\x9CQ\x9AL\xD2""f\x90\x0E\x04\xD2\xE6\x99\x9AZc\x17<QI\x9A)\x00\xB4""f\x92\x90\xD0\x02\xD1\x9A""fh\x07\x14\x00\xFC\xD1M\xCD\x19\xA0\x07SsI\x9Am\x00;<\xD2g\x9AJL\xD0!\xC7\xAD""4\x9A\r\x06\x81\x88zSI\xC5)\xA4""4\x00\x03""E7\xA5!4\x00""1\xA8\xCF""4\xA4\xF3HM!\x8C""4\xDARi\xA6\x81\fc\xCD""Fi\xC4\x9A""a\xA0""dmQ\xE2\xA4n\r2\x90\fn)\x86\x9E\xD4\xC3@\x10\xB0\xA8$\x8F\"\xAC""50\x80i\f\xCC\x96,sU\\V\xAC\x8A\x0Ej\x9C\xB1\x0EH\xA5`3dL\x1C\x8A\xA7,\x81""A&\xB4""eR3X\xBA\xA1\xDB\x1F\x07\x93M+\xB0+\\\xDCy\xAB\x8C\xE6\xA9w\xA6\xAFZ\x7F\xA5n\x95\x90\x98\xA2\x9DH)i\x80S\xA9\x00\xA7\x81\xED@\x81""E8R\f\x8Ap\xA6\x02\xF5\xA7\x00=i1\xC5=G\x14\x00\x01\x81O\xA4\x02\x9E\x00\xA0""B/<S\xC0\xE6\x90.;\xD3\xC5""0\x14)52\xAF""4\xC5\x06\xA6\x8E\x81t%\x8F\xA8\xAB\xD6""3\x1B}B\xDEQ\xFC.3U\x14\ny\xE0""du\x14\xC9g\xB1\xDB""6\xF8\x95\xBB\x11V\x05""ex~\xE4]i0\xC8\x0Er\xA2\xB5\xD4sP\b1\xC5&*B)\x00\xA6\x02\x01\xC5.1J:R\xE2\x80\x18z\xD0\x00\xA7\xE2\x80""1@\x1F""7,\xBD\x8DJ\xB2\x11\xF7Z\xA8\xE7\x06\x9E\x1B\x06\xB1gI\xAB\x15\xDB\xA9\xFB\xD5\xA3i\xACOn\xF9IY\x7F\x1A\xE7\xD6L\xD4\x81\xFEji\x83G\xA1\xD8x\x9C""2\x81p\xB9\xFF\x00ik~\xDE\xEE\x0B\xA4\r\x14\x8A\xC3\xD8\xD7\x94""ArP\xF5\xAD;]A\xE3`\xE9!V\xF5\x06\x9D\x88h\xF4\xC0is\\\x9D\x97\x8A\x19HK\xA5\xDC\xBF\xDF^\xB5\xD1[^Cs\x18xdW_cKbZ-n\xA5\x06\x99\xBA\x96\x80\x1F\x9E\xF4""f\x99\x9CR\xD0\x03\xB3\xC7Z7qL\xA3""4\x00\xFC\xE6\x8C\xD3(\xCD\x02\x1F\x92)s\x9A\x8F""4\xA0\xD2\x18\xFD\xD8\xA5\rQ\xD2\xE6\x80\x1EO\x14\x9B\xA9\xB9\xA2\x8B\x80\xED\xDE\xF4\x16\xA6\xE6\x93""4\x00\xE3""Fi\xB9\xA2\x80\x1F\x9E))\xB9\xA5\xCD\x00.i3I\x9Ei(\x01I\xA2\x92\x8A\x00""3Fh\xA4'\x14\x00\xB4\xD3""Fi\xA4\xD0\x02\x13HiI\xA6\x9A\x00""Bi\xA4\xE0R\x93\xC5""0\xD2\x18\x84\xD2\x13""A\xA6\xB1\xA0""CI\xA6\x1ARi\xA7\xA5!\x91\xB7Zm9\x8F""4\xCE\xF4\xC0kTf\xA5""cQ\x1A@Ds\x9A""a\xA9\x0FJa\xA0\b\rD\xC0T\xCC*#\xD0\xD0\x05I\xA3\x04W=\xAB\xC0@\fk\xA7q\xC5""ckJ>\xCB\x9Fz\xA8\xEE\f\xE5\x80\xF9\xCD""8P\x01\xDCi@\xAD@QE.(\xC7""4\x00\xE5\xE4S\x8D""4q\xD2\x9F@\x85\x1D)A\xA4\x02\x9C\xAB\xCD""0\x1C\x07\xAD<SE<P!E8u\x14\x80zS\x85\x00\xC5\x1CS\x80\xC9\xA4\xEBO^\x0E)\x81\"\xF4\xA9\x13\xBD""F\x9FJ\x98sLD\xA9Rc\x8A""b\x01S\xA8\xE2\x82|\x8E\xE3\xC0\xF7""A\xB4\xF6\x84\x9F\xF5lEvI^i\xE0\xDB\x8F'T\x96\x02p\x1C\x06\x15\xE9""1\x9C~5/pD\xB8\xA4\xA7/4\x1CP!(\xA2\x94\xD3\x01\xA0R\xD3\x82\x9AL\n@|\xCB\x8EqF9\xA7\xD2t\xE3\x15\x86\xA7`\xD0\b9\x14\xE0\xE4\x1Ei\b\xC7""4\x1C\x1A\x00\x95""d \xF0jh\xEE\b\xEBU\x05.i\xA6\xC4\xD2""5c\xBA\xC8\xEBW-\xAF\xE5\x82@\xF0\xCA\xC8\xC3\xD0\xD6\x00l\x1E*U\x98\x8E\xF5\\\xC4\xF2\x9E\x83\xA7x\xA9\\\x88\xEFT!\xED \xE9\xF8\xD7K\f\xF1\xCD\x18x\xDDYOB\x0Ek\xC7\xD6\xE4\xF4&\xB4\xB4\xFDj\xE6\xC2@a\x93\x0B\xDDO \xD2\xF4\x17-\xCFR\x06\x97""5\x83\xA5x\x8E\xDA\xFC\x04r\"\x9B\xD0\x9E\x0F\xD2\xB6\x83R!\xA2Z)\x81\xA9""A\xA6\x02\xE6\x8A@h\xA4\x02\xD2\xD2""f\x96\x80\n)3FE\x00-\x19\xA4\xF7\xA2\x80\x16\x8AL\xD1\x9Eh\x01h\xCD\x14\x87\xDA\x80\x17""4Rf\x93""4\x00\xEAL\xD2""f\x92\x80\x17""4f\x9B\x9CPM\x00;4\x99\xA6\xE6\x8C\xD0\x00M&h&\x98M \x1D\x9A""Bi\xB9\xA4'4\x00\x13M\xCD)\xE6\x9Ah\x18\x86\x9Ax\x14\xA4\xE6\x9A\xDD(\x01\x86\x9Ai\xD4\xDA\x04""Fi\xA2\x9C\xD4\xD1Hb5FjF\xA8\xCD\x00""Fi\x84""b\xA4j\x8C\xD0\x04OP\xB7z\x9D\x87\x15\x0B\n\x00\x85\x87\x15\x8B\xAD\x7F\xC7\xB7\xE3[m\xC0\xAC]df\xDB\xF1\xAA\x8E\xE0\xCE""dr\xD4\xB8\xA5\xC6\x1A\x94u\xAD""D \xA5\xC7\xA5\x00S\xC0\xA0\x04\x1Di\xD4\x9D\xE9i\x80\xA2\x9E\x07""9\xA6\x8E\xD4\xF0""9\xA0\x05\x02\x9C\x07""4\x82\x9E\r\x02\x1C\x05""8R\nx\x19\xA0\x00})\xE2\x91""F\x05""8S\x01\xDD\xBD\xEAX\xF1\xDE\xA3^i\xE8y\xA6\"u\x19\xA9\xD0qP'J\x99=sA%\xCD&\x7F\xB3kv\xD2g\x82v\x9F\xC6\xBD""f\x06\f\x8A{\x11^7!1\xEC\x90uV\r\xF9\x1A\xF5\x9D*q>\x9F\x14\x80\xE7*)05\x01\xE2\x8C\xE6\x98\xB4\xEA@8\xD1\xD6\x82\xA4\f\xD3\x95r:\xD3\x10})\xA4w\xA7\x01\x8A^1\xD2\x80>e\xCD-W\x12z\xD4\x8A\xFCu\xAE""c\xB2\xC4\x98\x07\xAD!\x19\xA4\x07""4n\xC5\x17\x1D\x84""9\x14\x80\xD2\xE7\"\x93\x02\x9D\xC5`\xC9\xCD)cM&\x8F\xE5@\x87\x07\xE6\xA5YqU\xFA\x1AL\xD3\x03""F9\x88\xE4\x1A\xE9\xB4\x7F\x15Km\x88\xAE\xC9\x96.\x80\xFF\x00\x10\xFF\x00\x1A\xE3\x15\xF9\xA9\xA3\x93=\xE9\x05\xB4=\x86\xDA\xEE\x1B\xB8\x84\xB0H\xAE\x87\xA1\x06\xA7\x06\xBC\xAFN\xD5\xAEt\xE9w\xC2\xFC\x1F\xBC\xA7\xA3W}\xA4\xEBv\xDA\xA4""Ca\xDB(\x1F""4d\xF2?\xC6\x99\x9B\x8D\x8D\x80\xD4\xEC\xD4 \xD3\x81\xA0\x91\xFC\xD1\x9AL\xD1\x9A\x00\\\xE6\x8C\xD2RP\x03\xB3\xCD.i\xB9\xE6\x8C\xD0\x02\xD1H\r)\xA4\x01\x9A""Bh\xCD%\x00.sF\xEAnh\xCD\x00.h\xCD%&h\x01\xD9\xA4\xCD%\x14\x00\xB9\xA4\xCD\x14\xD3@\nO\x14\xCC\xD0i(\x00&\x92\x8AJ\x00""3E\x14\x99\xA0\x02\x98\xC7""4\xE2p*2i\fCM\xA7SM\x00""Fi\xAB\xDE\x9C\xD4\x83\xA5\x00""5\xAA""3R\x9A""a\x14\x01\x19\x15\x11\x15)\xA6""0\xA0\b\x9A\xA2n\xB5+Tm\xC1\xA2\xC0""B\xE2\xB1\xB5\x91\xFE\x8B\xF8\xD6\xCBu\xAC\x8Dg\xFE=?\x1A\xA8\xEE\x07/\x8F\x9A\x9CG4\xB8\xF9\xA9q[\bozQGJZ\x00\x05\x1E\x94R\x81\x93@\x0E\x1Dx\xA7\xF4""4\xD0)\xD8\xA0\x05\x14\xF5\xA6\x81\x9Ap4\xC4H)\xC2\x99\x9Ap4\x01 \xA5\x14\xD5\x1D\xE9\xC0\xD0\x03\xC5H\xBCTk\x8C\xE6\xA5\xE8(\x11""2\xFD\xD1R\xA1\x18\xE0T)\xD2\xA5""B;\xD3%\xA2G\x1B\xA2#<\xD7}\xE0\xBB\xAF?FD'%2\xA7\xF0\xAE\x0B\x82\rt\xBE\x06\xB9\xF2\xEEn-\x89\xE3;\x80\xA1\xEC&z\nb\xA5\xDB\x81\x91P\xA7\xADJ\t\xC6;T\x8C]\xC4\xF1J\xB9\x1CSi\xC0\xE6\x98\x87\x02;\xF5\xA5<\x8CS;\xD3\x8D\x00|\xAE\x1B\x9Apb:T \xD3\x81?Z\xC0\xEB,,\x83\xBD?uU\x06\x9E\xAC""EM\x8A\xB9`\x1A]\xD5\bzp9\xA2\xC2\x1Ei\r\x19\xA4\xA6\x02w\xA5\xC6)\xA6\x8C\x91\xEF@\x0BOV\xC5""0\x10iE\x17\x15\x8B\x0B&=\xEA\xD4\x17\x0F\f\x89$NQ\xD4\xE4\x11\xDA\xB3\x83\x11R\xAB\xF1\xC5\x01\xB9\xE8\x9A\x1F\x89\x92\xF0,\x17""d$\xFD\x03vo\xFE\xBDt\xAA\xD9\xAF\x1CIH#\x07\x06\xBA\xED\x03\xC4\x85\n\xDA\xDE\xBF\xCB\xD1$'\xA7\xB1\xA1\x90\xE2v\xE0\xD3\xAA\xBAJ\b\x04\x1A\x94""7\x14\\\x91\xC6\x8E\xD4\x99\xA3""4\b\\\xD1IK@\x05\x00\xD1I@\nM\x19\xA3\xB5%\x00\x14\x86\x96\x8A\x00L\xD1\x9Ai84P\x03\xB3\x8A""3M\xCD&h\x01\xC4\xD2\x13I\x9AN\xD4\x00\x13\x9Anii)\x00QE%\x03\n\x0FJL\xE6\x83@\by\xEDL4\xF3\xC5""0\xD0\x02SM)\xEBM\xCF""4\bkRc\x8ASI\xDA\x81\x8D""4\xC3\xC5<\xD3[\x14\x01\x19\xA6""1\x14\xF3Lo\xAD\x00""Dj&\xA9[\xA7\x15\x19\xA0\b^\xB2""5\x81\x9B""F\xFA\xD6\xBB\xE6\xB2uq\xFE\x88~\xB5Q\xDC\x19\xCC\x0F\xBDN\xA4\xE8\xD4\xA7\xADl!\r\x14v\xA2\x98\n)h\x1D:R\x8A@(\xE0S\x81\xCD""7\xAD(\xA0\x07g\x14\xE1\xC7""4\xDAz\xF5\xC5\x00""8t\xA5\x14\x01KLL\x95i\xC3\x9A\x89Z\xA4\x07\x9A\x04<\x01\x8C\xD4\x8Ai\x80\xE7\x8Ap<\xE2\x9A\x02""e8\x15\"\x9A\xAE*`q\x8CP\"u5\xA1\xE1\xDB\x8F\xB2\xF8\x86\x13\x9C\t\x01SY\x80\xE4\xF5\xA0L`\xBB\x82""e\xFE\x07\x04\xFD(\x13=\xA6""3\x95\x06\xA5\x06\xA9\xE9\xF2\x89mc~\xB9\x02\xAE""f\xA4""D\x9B@\x19\xA6\xE6\x90\x9A(\x01GZ^\xF4\xD0i{\xD3\x03\xE5""0i\xDB\xBBT\x01\xF1O\rY4u&L\b\xA5\xE7""5\x1A\xB5;56\x18\xF0i\xE1\xC8\xA8\xC1\x06\x94{R\x19""0l\xD3\xBF\x9DW\xCE)\xE1\xF3\xCD\x16\x02Zn\b\xA4\xDF\x8E\xB4\xEE\x18P\x17\x1B\xD0\xF1N\r\x9Ai\xC8\xA6\xF7\xA0\tM(8\xA8\x95\xFDjL\xE4R\x02Ul\xF4\xA9\x96LU@j@\xD4\xC4u\xFA\x07\x88\x8D\xB9[[\xB6&\x13\xC2\xB9?s\xFF\x00\xAD]\xB2>T2\x9D\xCAy\x04W\x8F$\x98""5\xD5xo\xC4""F\xD9\xD6\xD2\xE9\xB3\x01\xE1\x18\x9F\xB9\xFF\x00\xD6\xA2\xC4\xC9\x1D\xE2\xBEi\xF9\xA8\x17\x04nS\x90})\xE0\xE2\x83""2L\xD2\xD4y\xA5\xCD\x03\x1F""E74\xA0\xD0!sE&i(\x01sFi\r&h\x01M6\x94\x9Ei;P\x00q\xDA\x9B""Fh\xA0""a\x9AJ)\t\xA4!h\xA4\xCD&M\x03\x17""4\x9D""E\x14\x94\x00t\xA4\xEBK\xD6\x8C\xD0\x03M6\x9CM74\bCIJi\xA6\x9D\xC0i\xEBM\xA7\x1Am\x00!\xA8\xC9""4\xF6\xA8\xCD!\x89Q\xB7""5!\xE2\xA3""c@\x11\x9A\x8D\x8DHMD\xD4\x01\x13\xD6V\xAE?\xD0\xDA\xB5^\xB2\xF5""f\xC5\x93\xD5Gp\xE8r\xE4\xF3""E!<\xD2\xD6\xC2\x17\x14\x94Q@\n)E6\x9C:P\x03\x80\x14\xE1\xC5\"\xD2\xD0\x02\x8Ax4\xD0""3N\xA6\"@x\xA2\x9A:S\xBAP\x01N\\\x93L\xA9\x01\xE2\x80&\x8C\xF1O\x04THiG&\x98\x8B\x0B\xC0\xE9@84\xD5\xE9""E\x02'\x06\x92""a\xBA#\x81MSR\x13\xF2\x1A\x00\xF4\xDF\n]\xFD\xAB""F\x81\xB3\xC8\\\x1A\xE8""Fq\xCDp?\x0F\xAE\xB3o5\xB9<\xA3\x9C\x0F""c]\xE0<R\xEAN\xC3\xF3K\xDE\x9A\b\xC5""8R\x00\xE9N\x14\x94\xAAGz\x00\xF9$58\x1A\x8A\x94\x12)\x1B\xDC\x9C=<=W\rO\rJ\xC3L\x9F<\xD3\xF7\x0FZ\x80=85KE&O\x9C\xD2\xE7\xD6\xA3\rN\r\x9E\xB5\r\x15q\xF9\xA0\x1CS3\x83N\x07""4l\x04\x83\xE6\xE6\x94\xAD""5\x18\x0E*L\x83J\xE3H\x8C\x8Eh\x07\x1FJR\r&8\xA6.\xA3\xB7zt\xA7\x03\xDE\xA2\x14\xE0i\x01(>\x95*>>\xB5\\74\xF0i\x88\xEE\xBC/\xE2\f\xEC\xB1\xBA\x7Fh\xD8\xFF\x00*\xEC+\xC6\xE2r\x84""08#\x9A\xF4?\f\xEB\xA3P\x80[\xCE\xDF\xE9\x11\x8E\xFF\x00\xC4=i\x13%\xD4\xE8\xB3J\r74Pf\xC7""f\x80y\xA6\xD1@\x0F'\x8A""3\xC5""74f\x80\x1D\x9AL\xD2QL\x05\xCF""4Sh\xA0\x07\x1Am.i)\f)\xA6\x9DM\xA0""AI\xDC\xD2\xF6\xA4\xA0""a\x9AL\xD2\x1AN\xF4\x00\xBD\xA8\xA4\xCFZJ\x00)(4\x86\x81\bO4\x84\xF1""A\xEBM\xA0\x00\x9Ai\xA5""4\xD2i\x80\x8Cj3On\x94\xC3Hb\x1E\x95\x11\xA9\t\xE2\xA3\xA0\x10\xC6\xE9Q\x9E\x94\xF6\xA8\xC9\xA0\b\xDB\xE9Yz\xB0\xCD\xA3\xD6\xA3\x1C\xD6v\xA6\xA0\xDAIT\xB7\x03\x93?z\x94\xD0xj\x0FZ\xD8""AE\x14v\xA0\x05\xA7S;S\x81\x14\x00\xEE\xD4\xAAOznx\xA5\x04\xD0\x03\xE9""A\xC5""7\xB5(4\xC0\x93\xAD""8\x1Ey\xA8\xC1\xE2\x9E\xBDh\x10\xB4\xE0i\x99\xE7\xA5(\xA6\f\x99\r<u\xA8T\xD4\x80\xE6\x90\x89\x96\x9C:\xD4j\xD5\"\x9A`<g4\xF0x\xEBL\xDC""8\xA5\f1\x9A\x04mx2\xEB\xEC\xFA\xFB""BN\x04\xAB\x9F\xCA\xBDP\x1C\x8A\xF1k\t\xFE\xCB\xADZL\x0E>p\x0F\xD0\xD7\xB2[\xBF\x99\n\xB6z\x8ALOrqJ\x0E:\xD3h\xEBH\x07\xEF\xCD.\xEA\x8CqK\x9A\x04|\x99""E8\xA6)\xB4\x8D\x82\x94\x13IE\x00<585EJ\x1A\x80'V5 l\xD5pi\xE1\x88\xA9h\xBE""b|\xE7\xBD(85\x1A\xB6i\xD5""6*\xE4\x81\xB9\xA9\x14\xD4\x19\xA7+\x1E\xE6\x93@\x99""d\x11\xDE\x9AV\x91\x1Ct\xA7\xF5\xFAT\x94""3\xA7JL\xD3\x98s\xC5""4\xD0&(9\xA7\x03\x8A""e;<S\x02P\xDE\xF5j\xD6\xEEK;\x84\x9E&*\xE8r\b\xAA@\xE0\xF3O\x06\x8B\n\xE7\xAC\xE9:\x9Cz\x9D\x8AN\x9Dz0\xF4""5\xA1\x9A\xF3\x0F\x0Fj\xED\xA5\xDF\r\xEC~\xCF!\xC3\x8FOz\xF4\xA8\xE4WEe9V\x19\x04R\xB1\x9B""D\xD9\xA2\x99\x9A\\\xF1H\x91\xD4\x99\xA6\x93""A&\x98\xC7\xE6\x8C\xD4y\xC5\x1B\xB9\xA0""D\x99\xA4\xCD""4\x1E)s@\xC5\xCF\x14""f\x92\x8A\x00\\\xD2\x13HM74\x00\xEC\xF1IFi\t\xA0\x00\xF5\xA6\xD2\xE7\x8AJ\x02\xE1""E\x14P\x02\x1A""a\xA7\x1A""e\x02\x10\x9AJ\t\xA3""4\fBi\x84\xD2\xB5""4\x9A\x04!9\xA6\x9AZBM\x03\x10\xD3\r8\xD3\x0FZ@1\x85""F\xD5!>\xB5\x1B\x11L\bZ\xA8\xEA\x1F\xF1\xE9'\xD2\xAF\xB5Q\xD4\x00""6r\x8F\xF6i\xAD\xC0\xE4[\xEF""f\x82i\x18\xF2h'\x9A\xDC@)i;R\xD0\x02\xE6\x81\xEFIK@\n:R\xE6\x90\x9AQ@\n\x0F\x14\xE1\xD2\x9B\xD2\x97""4\x00\xFE\xD4\xA0\x91L\xE9N\x06\x98\x87\x9C\xE3\x83N\x1D)\x85\xBAR\x86\xA0\x07\x8Fj~pj<\xD3\xB3@X\x94""5H\xAD\xC7Z\x80\x1A\x90\x1C\nb&-G~\xB4\xD0w\nS\xC0\xE2\x98\x86\xDC""1U\x0E\xBDT\x82?\n\xF5\xFD\x02\xE8]iPH\x0Er\x82\xBC\x81\xC6\xE8\xCDz\x0F\x80\xAE\xC4\xBA""8\x88\x9E""c%jZ\x13;QEF\x1A\x9C\x0Ei\x00\xE0h\xCD&i3@\x1F*\x06\xE2\x82\x03PV\x9B\x93H\xD4\n\x91M\xA9""A\xE2\x90\xA8#\x8A\x00\x8E\x8A\b\xA2\x80\nxl\xD3(\xA0\tCS\xC3\x1A\x84\x1ApjM\x14\x99""89\xA7\x03P\x06\xA9\x14\xF3R\xD1I\x92\xAB\xF6\xA9\x91\xFF\x00*\xAD\xD6\x9E\xA7\x15-!\x96\x88\xCF+Q\xD0\x8D\x8AwZC\x1B\x8A\x01\xC1\xA5\xC6M!\x18\xA0\x07\xF5\xA7)\xC7\x15\x1A\x9C\x1A\x93\xA9\xC8\xA0""D\x8Ak\xB7\xF0\x96\xB3\xE6\xC5\xF6\t\xDB\xE7O\xF5""d\xF7\x1E\x95\xC2\x82T\xD5\x8B{\x89-\xAE\x12x\x9B\x0E\x87\"\x9B\xD4G\xAF\x03""Fx\xAC\xFD+Q\x8FR\xB1\x8Et<\x91\x86\x1E\x86\xAF\xD4\x99\xD8\\\xD1\x9AL\xD1@\x0B@\x07""4Q\x9A\x04/Z)3K\x9FZ\x06(\xE3\xAD\x04\xD2t\xA4\xCEi\x00\x13IFi)\x88\\\xD0i3E\x03\n(\xA4\xCD\x00\x14\xBD\xA9\xB9\xE2\x8C\xE4P\x02\x13M4\x13\x8Am\x00!\xA0\xF1GzC@\x86\xB1\xA6\xE6\x95\x8EM7\xBD\x03\noz\r\x04\xD2\x01\xA4\x9A""a\xE6\x9Ci\xA6\x80#j\x8C\xD4\x8D\xD6\xA3""a\xD6\x80\"c\x9A\xA7}\xCD\xAC\x83\xDA\xAE\x1A\xA7y\x81o'\xFB\xB5H\x19\xC7\xC9\xF7\xB1""F(\x97\x87""84\x84\xF4\xAD\xC4--7>\x94\xEC\xF1Hb\xD1\x9AL\xD2\xD3\x10\xBD\xA8\x07\x9Am(\xEB@\x0E\xC9\xE2\x9D\x93L<\x1A\\\xD0\x03\xD4\xF1\x8Au0S\x85""1\x0E\xCE""E(\xE9M\xA5\x06\x80\x1E""1\x8Avi\x99\xA1Z\x80\xB9(\xFDi\xC0\x9C\xE2\xA3\x07\x9Ap?6h\x02""d<\xD3\xF9'\x15\bni\xF9\xA6\"]\xBC\x11]'\x80\xAE|\x9B\xFB\x9Br~\xF6\x1C\n\xE6\x14\xE6\xAF\xF8~\xE3\xEC\xBE\"\xB7n\x8A\xF9""C@\x9E\xC7\xB0\xA9\xC8\xA7\x03\x8A\x86""6\xDC\x80\x8A\x90t\xA9\x10\xEC\x9C\xF5\xA3""9\xA6\xE6\x94\x1A\x00\xF9""dd\xD2\x11\xEDO\xDBHA\xA4j3\x18\x14""f\x9D\x8A""B\xBE\x94\x00\xB8\f)\x85q@8\xA7\x03\x9E\xB4\x01\x1D\x14\xE6Zm\x00\x14\xB4\x94P\x03\x81\xA9\x15\xAA*Pq@\xD3,\xA9\xCD:\xA0V\xA9\x95\xB2*\x19""d\xAAM<\x1E*%4\xF0}*F?4\xEC\xF1\x83L\xA5\xA4""0\xC6)\xC0\xD2u\xA7t\xA0\x07\xD3\x96\x9A)\xC2\x81""3w\xC3z\xA9\xD3\xAF\xB6H\x7Fq/\r\xEC}k\xD1\x15\x83(#\xA1\xAF\"S\x9E\xF5\xDC\xF8[V\xFBM\xB7\xD9%o\xDE\xC4>R{\xAD'\xDC\x96\x8E\x9A\x8Anih Z(\xA2\x81\x05\x14RR\x01sIFh\xA6""0\xCD&i\r&x\xA0\x05&\x8C\xF1M\xCD\x19\xA0\x07""f\x93""4\x84\xD2P\x00M8\x1CS)\xD9\xC0\xA0\x06\x93\xCD!\xA5""4\x94\x00\x94\x9D\xA9i\t\xA4\x04m\xD6\x92\x94\xF5\xA6\x9A`!\xA4$\xD0i8\xE9H\x06\x93\x9Ai\xA5""4\xD2h\x01\xADQ\x9A{ri\x8D@\x11""5T\xBC\x19\xB7\x7F\xF7j\xD9\x18\xAA\xB7JL\x12\x7F\xBAj\x90""3\x8C\x97\xEF\x1A\t\xE0Q(\xF9\x8D""4\xF4\x15\xB8\x80\x1Ei\xD9\xA6\nvpx\xA0\x07q\x8Eh\xCE)\x01\xA0\xF6\xA0\x07w\xA0Rb\x94t\xA0\x05\xA5\xE2\x90Q@\x0F\x06\x9C""8\xE2\xA3\xA5\x07\x9A\x04?<\xD2\x83\x9A""fy\xCD/Z\x00\x90\xD0)\xA3\x9E""E\x19\xA6\x04\xA0\xD3\xB3Q\x83N\x07\x9A\x03\xA1 jx9\xA8\x81\x14\xF5lP\"Pp(\x12\xF9""70\xCC:\xA3\x83M\xDD\x9A\x8EQ\x98\xCF""4\xC2\xC7\xB5i\xD3\t\xAC\xE3px\"\xAE\x03\\\xE7\x84n\xFE\xD5\xA1\xC0\xC4\xE4\x85\xC1\xAE\x85MI(}\x19\xA4\xCD 9\xA0\x0F\x9BR\xC2W\xED\x8A\x99""4\xC5'\xE7z\xD0S\x86\xF6\xA1\x81\xED\\\xCE\xAB:\x94\x11\b\xD2\xED\x8C""D\f\xEE\xC7Z\xC2\x91""6HW\xB8""8\xAE\x8Dr9\xACK\xF5\xDBt\xDCu\xAA\xA5&\xDD\x98\xA7\x14\xB6)\x11\x9AoCR\xD3H\xCDlf\x00\xFA\xD3J\xD0""A\x14\xA0\xF1\xCD\x00""2\x8As.)\xB4\x00R\x8AJ(\x01\xC0\xD4\x8AqQ\x03O\x06\x81\xA2\xC2""0\xA7\x83\x83\xC5WSR\xA9\xCD""CE\xA6K\x9C\xD3\x81\xC7Z`\xA7\n\x91\xEA=H\xCD""8\xF5\xA8\xC7Z\x7FQ\x9E\xF4\x87\xD0x c\xD6\x9C\x0F""4\xC1\xD6\x9C""1\xEB@2D<\xD5\xBBK\xA9,\xEE""c\xB8\x88\xE1\x90\xE6\xA9\x03R\xAB\x03@\x99\xEAv\x17\xB1\xDFZG4g!\x87>\xC6\xAD""f\xB8/\r\xEA\xBF""b\xBB\xFB<\xAD\xFB\x99O\xE4k\xBBR\x0Fz\x93""6\xAC>\x8AL\xD1\x9A""b\x16\x8AL\xD1@\x05\x14PM\x00&y\xC5""4\x9A_\xC2\x93\x14\x00\x94v\xA5\"\x90\xF4\xC5\x00\x1Di(\xA4\xA0\x05\xA0\xD2R\x93@\x84\xA6\x9AZi\xE9Hb\x93M9\xE9""Fi\r\x00""4\xD2\x1ASHM\x00""0\x9C\xD3i\xC7\xAD""4\xD0\x03i\xA7\xD2\x9Ci\xA7\x14\x00\xC3L4\xF3L#\x8A`Fj\xBC\xFC\xC5 \xCF\xF0\x9A\x9C\x9A\x85\xFE""dq\xFE\xC9\xA6\x81\x9CL\xDF|\xFDi\xBD\x85:\x7F\xF5\x87\xEBM\xFE\x11[\x89\r\xA5\xEFIFx\xA4""1\xE0\xF1J\x0Fje\x14\by\xEB""A8\xA6\xF4\x14\xB9\xED@\x0E\xEDJ:S\x03S\x81\xE6\x98\x0E\xA3""4\x99\xA2\x90\x0F\xCF\x14\x02i\xB4""f\x98\x12\x03\xC7J]\xD5\x18""4\xB9\xA0""D\xA2\x8A""b\x92""E<P\x03\xB3Vm\xA1\x92\xEAu\x86%\xDC\xED\xC0\x15R\xAFiR\x98u;i=$\\\xFD""3M0c%\x8D\xE0\x95\xA2\x93\x87^\xD4\xD2""A\x06\xAE\xF8\xAE""3k\xAF\x83\xD1Z\xB3\xC1\xCF""4'pgu\xF0\xFA\xEB""6\xF3\xDB\x13\xCA?\x03\xD8\xD7v:W\x94\xF8*\xE3\xEC\xFA\xFB""FN\x04\xA9\xFA\x8A\xF5@x\x06\x93 u-74\xBD{\xD2\x0B\x1E\f\xD8\x0FR\x02\x18VpyV_\x9B\x91WVN\x05rIX\xEC""D\xA0`V\x1E\xAA\x00\xB8\xFA\xD6\xCF\x99\xCD""d\xEA\xA3,\xADN\x97\xC4*\x9B\x19\xB4\x94\xB4\x95\xD4""b!\x19\xA6\x91\x8Aq\x19\xA4\xEB@\x00""9\x18""4\xD2""1E;\xA8\xA0\x06QJF\r%\x00\x14\xE0i\xB4\xA0\xD0\x03\xC1\xA9\x15\xB9\xA8""A\xA7\x83I\x94\x99""eZ\x9C\x0F\xBD@\xB9\xCDH\x0F\xBD""E\x8A\xB9""7\xA7""4\xE5""5\x10""4\xF5""4\x8A%\x03""4`\x8AL\xE3\x9Aw\xDE\x19\xA4\x16\x14\x1Az\xB6*,\xD2\x86\x14\b\xB4\xA7\"\xBB\xAF\r\xEA\xBFm\xB5\xF2%9\x9A!\x8E\x7F\x88z\xD7\x03\x1Bg\xBD]\xB2\xBA\x92\xCA\xE9.b?2\x1EG\xA8\xF4\xA0M\x1E\xA0\x0F""4\xB9\xAAv7\x91\xDE\xDA\xC7<G\xE5""a\x9F\xA5Z\x15&b\xE6\x94g\xBD%\x02\x98\x0E\xE2\x90\xD2n\xA4""4\x80\x1A\x8E\xD4\x9F\x8D\x14\x00\xB4\x84\xD1\x9A)\x80\xDA)M%\x00\x02\x83J)\t\xA0\x06\x93L'\xDE\x9CM4\xD2\x01\t\xA4\xA5\xCD%\x00!\xA6\x93K\xCD""0\x9A\x00\t\xE6\x93""4f\x92\x80\x10\xF5\xA6\x13\xCD""8\xD3\x18\xF6\xC5\x00""4\xD3\x18\xF1O\xA8\xDA\x98\x11\xB7J\x85\xF8""F\xFA\x1A\x98\xD4M\xF7[\xE8i\xA0""8\x99\xCF\xEF\x1B\xEBL\xEDO\xB8\xFF\x00X\xDFSQ\x8E\x95\xB0\x82\x8A)3@\x0B\x9Eii(\xCD\x00-.i3\xC5\x03\xAD""0\x14`\xF5\xA5\x06\x9BKH\x07S\xAA""1\xD6\x9D\x9Cw\xA6\x03\x87J3M\x07\"\x97\"\x80\x1C)\xD4\xCC\xD2\xEE\xA0\x07\x8E)\xDB\xB1\xEFQ\xE7\xBD.h\x02L\xE6\x9F\x1B\x15u#\xA8<T@\xF4\x14\xE5<\xD0#\xA2\xF1\xE2\xEF""6\x97@cr)\xFD+\x02&\xCA\n\xEA<^\xF1\xDD\xF8KJ\x99\x06\x19`\n\xE7\xDCW\x1Fi&\xE8""FhA\xBA""F\xAE\x99""9\xB5\xD6,\xE7\xCE\x06\xF0\xA7\xF1\xAF""f\x81\xF7\xC2\xAC;\x8A\xF0\xC7'ha\xD5H\"\xBD\x8F""B\xB9\x17:U\xBC\xB9\xCEPP\xC9{\x9A\x94""f\x9B\x9AZB<\x06\xE4\x1D\xE3=jh\xD0\x94\x15\r\xCC\x81\xA5\xCDK\f\xA4(\xC0\xAEG\xF0\x9D\x89jL#\xCEk3UL*\x9A\xD2\xDEI\xAC\xBDU\xC9U=\xB3""E+\xF3\n{\x19t\x86\x9Cx\xA6\xD7Y\x88\x87\x83I\x8AS@<P\x03O4\x94\xEA""B=(\x01H\xC8\xA6S\x81\xC5\f=(\x01\xB4QE\x00(\xA7\x03L\xA5\x19\xA0""d\xC0\xF3R\x83\x9A\xAC\rJ\xADI\x94\x99:\xD3\xC0\xC5""D\xA7\"\xA5\x07\xB5""f\xCA\x1E\x0F\x18\xA7+\xE0\xE3\xB5""3\xDCR\xE3""4\x86\xC7\x1Cg#\xA1\xA4\x1Dh\x07\x8CP:\xF3""B\x13$RA\xC5Y\x8D\x87J\xAA\xA6\xA4""F\xE7\x19\xA0\x0E\x87""A\xD5N\x9Dt\"\x91\x8F\xD9\xE5<\xFF\x00\xB2}k\xBAW\xDC""2\x0E""A\xAF,R\x18m?\x85u\xBE\x1A\xD6<\xC5\x16""7\r\xFB\xC5\x1F#\x1F\xE2\x1E\x94\x89\x92:\x80}\xE9i\x82\x96\x91\x03\x89\xA6\x93\xC6('\x9AJ\x00SE%\x14\f(\xCD\x14\x84\xFAP!h\xA4\xCD\x14\x00\xB9\xA6\xB1\xA5\xC8\xCD""4\xD0\x02v\xA6\xD2\xD2\x1A\x00J(\xA4""9\xA0\x06\x93\x8Am)\xE3\xAD""7\xB5\x00%!\xA5\xA4\xA0\x06\x13M4\xA4\x9A""Bx\xA0\x06\x93Q\xB1\xA7\x1E)\x86\x98\rn\x95\x04\x9Fu\xBE\x95""3T/\x9D\x8D\x8E\xB8\xA6\x80\xE2\xEE""3\xE6\xBE}MD>\xE8\xA9ns\xE6\xBE}MB\x0F\xCA+v!h\xA3<\xD2R\x01ii;\xD1\x9A\x00Z)(\xED@\x0BK\xD4\xD3ih\x01\xD4\x99\xF5\xA4\xA5\xA6\x03\xB3\xC5\x00\xE6\x9B\x9CR\xD0\x03\xB3\xC5;\xAD""E\x8Ap<\xD0\x03\xF3\xC5-4b\x97""4\x02\x1D\xD6\x94\x1C\x1E)\xB9\xA5\x1Di\x88\xEA\xAF\xBF\x7F\xE0{|\x8C\xED\xDC\xBF\xA9\xAE""2\xC9\xF0\n\xD7\xA0Z\xDB-\xC7\xC3\x97\x98""0\xDD\x1D\xC3!_\xA8\xCF\xF5\xAF;\x8B\xF7w,\x0F\xAD.\xA0\xBE\x13Lr\xA4W\xA5x\x16\xEF\xCF\xD1V2y\x8C\x95\xAF""3V\xC8\x15\xD8x\x02\xEB""e\xD5\xC5\xB9<\x1C""0\xA6&zFh\x1Di\x01\xE2\x8C\xD4\x88\xF0\x19\xE3\x1B\xFA\x8A\x9A%\xF9""F*\x1B\x91\x93\xDE\xAD\xC1\xFE\xAC\x0E\xF8\xAEG\xB1\xD6\x80)5\x9D\xAA\x0F\xDD\x0F\xADi\x95""aYZ\x99;\x17>\xB4\xE9\xFC""B\x9E\xC6""fr\xB4\x0Ei\x07J\x07\xB5u\x18\x85&9\xA7R\x1A\x00""CI\xFC\xA9\xC4Rb\x80\x19\xD0\xD3\x87#\x9AR8\xA6\x83\x8A\x00""B))\xE4S(\x00\xA5\xCD%\x14\x00\xB5 <\xD4t\xA0\xD0""2ulT\xA1\xBB\xD5u5*\x9A\x86\x8A""E\x85""9\x14\xE0""A\xA8\x95\xA9\xE0\x83PX\xEE\xF4\xFE\x0Fjf3J:\xD0+\x0E\x1Di\xC0\xF7\xE9@\x1E\xB4\xA3\x14\xAE;\x12""F\xDD\xAAtr\xAC\xAE\x8CVD9\x04UA\xC1\xE2\xA6""F\xEF\xDE\x98\xAC\xCF@\xD0\xF5\x84\xD4\xED\x80r\x04\xE8""0\xEB\xEB\xEFZ\xD9\xAF""2\xB7\xB9\x96\xD2\xE1.`8\x91O\xE7\xED]\xEE\x99\xA9\xC3\xA9Z\xAC\xB1\x9F\x9B\xA3\xAFu5-X\x86\x8D\x02i\t\xA4\xCD!\xA0\x91\xE2\x8C\xE2\x99\x93K\x9A\x00vi\xBDO\x14\x13@4\x00\f\xD0Z\x9AM\x00\x9A@-.i\xA3\xAD/J`4\xD2g\x9AS\xD2\x92\x80\nCKM4\x00\xD3M\xEFJM6\x80\x12\x90\xF4\xA5\xA4#4\x01\x19>\x94\x94\xA7\xAD!\xA0\x06\x1EzS\r=\xA9\x98\xC5""0\x1Aj\x06""8\x04\x8A\x9C\xF4\xAA\xED\xF7M4\x07\x1Bu\xFE\xBD\xFE\xA6\xAB\xE7\x81V.\xF0'q\xEEj\xB0\xE0V\xC2\x17<\xD1\xD0\xF3I\xDE\x97\xDE\x81\x8B\x9A""8\xA6\x9A\\\xD0\x02\xE7\xAD\x19\xA6\xD2\xF6\xE2\x80\x1D\xDA\x80i\xB9\xE2\x8Fz\x04""8R\xD3""A8\xA5\xA0\x05\xA2\x93\xA5/j`/JPpi\xB4\xB9\xA4\x02\x83J\t\xE6\x93\xB7\xBD(\xE3\xF1\xA6\x02\x83O\x07\xBD""0\x9E""1J\x0F""a@\x8E\xD7\xC3\xED\xE6\xF8GQ\x87\xAE\xD7\r\x8F\xF3\xF4\xAE\n\xE9|\xAB\xD6\xFF\x00z\xBBo\x06\xBF\x99o\xA9Z\xFF\x00~\x1D\xC0}?\xFDu\xC8k\t\xB2\xED\x8F\xA9\xA5\xD4h|m\x91\xC7\x15\xBB\xE1[\x81m\xE2\x18y\xE1\xC6\xDA\xE7m\xDB(*\xED\xA4\xC6\xDE\xF6\xDE""a\xC1Y\x07\xF3\xAA\x13Z\x1E\xE0\x8D\x95\x06\x9C*\xB5\x9C\xBEm\xB2""89\xC8\x06\xACT\x90\x7F\xFF\xD9");
    QImage tempImage;
    tempImage.loadFromData(m_collectedData, "JPG");
    if (!tempImage.isNull())
    {
        qDebug() << __FUNCTION__ <<__LINE__<< m_imageRequestedFor;
        m_collectedData.clear();
        picDirPath = QDir::homePath();
        picDirPath.append("/PPImages");
        imageDir=picDirPath;
        if(!imageDir.exists())
        {
            QDir dir = QDir::home();
            dir.mkdir(picDirPath);
        }

        imagePath = picDirPath;
        imagePath.append(QString("/%1.jpg").arg(m_imageRequestedFor));
        QImageWriter writer;
        writer.setFormat("JPG");
        writer.setFileName(imagePath);
        writer.write(tempImage);
    }

    // search for next item to download
    bool isCuurentItemToDownload = false;
    for (int i=0; i<m_imageNameList.count(); ++i)
    {
        if (isCuurentItemToDownload)
        {
            imageFileName = m_imageNameList.at(i);
            isWaterMarkedRequired = false;
            setReceiveImageWithFileName(imageFileName);
            return;
        }
        if (m_receiveImageWithFileName == m_imageNameList.at(i))
            isCuurentItemToDownload = true;
    }
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::hostFound This slot is executed when an host is
 * found
 */
void ProxyPics::hostFound()
{
    qDebug() << __FUNCTION__ <<__LINE__<< "Host Found";
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::userAssignmentCount
 * @return
 */
bool ProxyPics::userAssignmentCount()
{
    return m_userAssignmentCount;
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setUserAssignmentCount
 * @param count
 */
void ProxyPics::setUserAssignmentCount(const int count)
{
    m_userAssignmentCount = count;
    emit userAssignmentCountChanged();
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::uiWidth
 * @return
 */
int ProxyPics::uiWidth() const
{
    return m_uiWidth;
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::uiHeight
 * @return
 */
int ProxyPics::uiHeight() const
{
    return m_uiHeight;
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setassignmentXMLFeed
 * @param assignmentXMLFeed
 */
void ProxyPics::setassignmentXMLFeed(QString assignmentXMLFeed)
{
    if (m_assignmentXMLFeed == assignmentXMLFeed)
        return;

    m_assignmentXMLFeed = assignmentXMLFeed;
    emit assignmentXMLFeedChanged(assignmentXMLFeed);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setAssignmentXMLFeed
 * @param assignmentXMLFeed
 */
void ProxyPics::setAssignmentXMLFeed(QString assignmentXMLFeed)
{
    if (m_assignmentXMLFeed == assignmentXMLFeed)
        return;

    m_assignmentXMLFeed = assignmentXMLFeed;
    emit assignmentXMLFeedChanged(assignmentXMLFeed);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setfirstName
 * @param firstName
 */
void ProxyPics::setfirstName(QString firstName)
{
    if (m_firstName == firstName)
        return;

    m_firstName = firstName;
    emit firstNameChanged(firstName);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setlastName
 * @param lastName
 */
void ProxyPics::setlastName(QString lastName)
{
    if (m_lastName == lastName)
        return;

    m_lastName = lastName;
    emit lastNameChanged(lastName);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setFirstName
 * @param firstName
 */
void ProxyPics::setFirstName(QString firstName)
{
    if (m_firstName == firstName)
        return;

    m_firstName = firstName;
    emit firstNameChanged(firstName);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setLastName
 * @param lastName
 */
void ProxyPics::setLastName(QString lastName)
{
    if (m_lastName == lastName)
        return;

    m_lastName = lastName;
    emit lastNameChanged(lastName);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setmapMode
 * @param mapMode
 */
void ProxyPics::setmapMode(const QString mapMode)
{
    if (m_mapMode == mapMode)
        return;

    m_mapMode = mapMode;
    emit mapModeChanged(mapMode);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setmapParameters
 * @param mapParameters
 */
void ProxyPics::setmapParameters(const QString mapParameters)
{
if (m_mapParameters == mapParameters)
return;

m_mapParameters = mapParameters;
emit mapParametersChanged(mapParameters);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setmapCenter
 * @param mapCenter
 */
void ProxyPics::setmapCenter(const QString mapCenter)
{
    if (m_mapCenter == mapCenter)
        return;

    m_mapCenter = mapCenter;
    emit mapCenterChanged(mapCenter);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setmapOrigin
 * @param mapOrigin
 */
void ProxyPics::setmapOrigin(const QString mapOrigin)
{
    if (m_mapOrigin == mapOrigin)
        return;

    m_mapOrigin = mapOrigin;
    emit mapOriginChanged(mapOrigin);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setmapZoom
 * @param mapZoom
 */
void ProxyPics::setmapZoom(const QString mapZoom)
{
if (m_mapZoom == mapZoom)
return;

m_mapZoom = mapZoom;
emit mapZoomChanged(mapZoom);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setmapType
 * @param mapType
 */
void ProxyPics::setmapType(const QString mapType)
{
    if (m_mapType == mapType)
        return;

    m_mapType = mapType;
    emit mapTypeChanged(mapType);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setCurrentAssignmentName
 * @param currentAssignmentName
 */
void ProxyPics::setCurrentAssignmentName(const QString currentAssignmentName)
{
    if (m_currentAssignmentName == currentAssignmentName)
        return;

    m_currentAssignmentName = currentAssignmentName;
    emit currentAssignmentNameChanged(currentAssignmentName);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setCurrentAssignmentAddress
 * @param currentAssignmentAddress
 */
void ProxyPics::setCurrentAssignmentAddress(const QString currentAssignmentAddress)
{
    if (m_currentAssignmentAddress == currentAssignmentAddress)
        return;

    m_currentAssignmentAddress = currentAssignmentAddress;
    emit currentAssignmentAddressChanged(currentAssignmentAddress);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setAssignmentTicket
 * @param assignmentTicket
 */
void ProxyPics::setAssignmentTicket(const QString assignmentTicket)
{
    if (m_assignmentTicket == assignmentTicket)
        return;

    m_assignmentTicket = assignmentTicket;
    emit assignmentTicketChanged(assignmentTicket);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setAssignmentNotes
 * @param assignmentNotes
 */
void ProxyPics::setAssignmentNotes(QString assignmentNotes)
{
    if (m_assignmentNotes == assignmentNotes)
        return;

    m_assignmentNotes = assignmentNotes;
    emit assignmentNotesChanged(assignmentNotes);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setUserPassword
 * @param userPassword
 */
void ProxyPics::setUserPassword(const QString userPassword)
{
    if (m_userPassword == userPassword)
        return;
    qDebug()<<__FUNCTION__<<__LINE__<<"user password "<<userPassword;
    m_userPassword = userPassword;
    emit userPasswordChanged(userPassword);
    if (userPassword=="")
        return;
    qDebug()<<__FUNCTION__<<__LINE__<<"Validate User -"<<UserId();
    //Allow to validate user only if it is login screen
    if (m_UserDetailSocket->isOpen() && (m_currentState == "Login")) {
       QString delimiter = QString("_#@#_");
       QString data = QString("validate_user");
       data.append(delimiter);
       data.append(UserId());
       data.append(delimiter);
       data.append(userPassword);
       m_UserDetailSocket->write((data).toStdString().c_str());
       qDebug()<<__FUNCTION__<<__LINE__<<data;
    } else {
       qDebug()<<__FUNCTION__<<__LINE__<<"User Detail Socket is not open";
    }
 }

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setDeviceLongitude
 * @param deviceLongitude
 */
void ProxyPics::setDeviceLongitude(const double deviceLongitude)
{
    if (m_deviceLongitude == deviceLongitude)
        return;

    m_deviceLongitude = deviceLongitude;
    emit deviceLongitudeChanged(deviceLongitude);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setAssignmentValue
 * @param assignmentValue
 */
void ProxyPics::setAssignmentValue(const QString assignmentValue)
{
    if (m_assignmentValue == assignmentValue)
        return;

    m_assignmentValue = assignmentValue;
    emit assignmentValueChanged(assignmentValue);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setAssignmentPubDate
 * @param assignmentPubDate
 */
void ProxyPics::setAssignmentPubDate(const QString assignmentPubDate)
{
    if (m_assignmentPubDate == assignmentPubDate)
        return;

    m_assignmentPubDate = assignmentPubDate;
    emit assignmentPubDateChanged(assignmentPubDate);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setUserId
 * @param UserId
 */
void ProxyPics::setUserId(const QString UserId)
{
    if (m_UserId == UserId)
        return;

    m_UserId = UserId;
    emit UserIdChanged(UserId);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setStreetAddress1
 * @param streetAddress1
 */
void ProxyPics::setStreetAddress1(const QString streetAddress1)
{
    if (m_streetAddress1 == streetAddress1)
        return;

    m_streetAddress1 = streetAddress1;
    emit streetAddress1Changed(streetAddress1);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setMapUrl
 * @param mapUrl
 */
void ProxyPics::setMapUrl(const QString mapUrl)
{
    if (m_mapUrl == mapUrl)
        return;

    m_mapUrl = mapUrl;
    emit mapUrlChanged(mapUrl);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setDeviceAltitude
 * @param deviceAltitude
 */
void ProxyPics::setDeviceAltitude(const double deviceAltitude)
{
    if (m_deviceAltitude == deviceAltitude)
        return;

    m_deviceAltitude = deviceAltitude;
    emit deviceAltitudeChanged(deviceAltitude);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setDeviceLatitude
 * @param deviceLatitude
 */
void ProxyPics::setDeviceLatitude(const double deviceLatitude)
{
if (m_deviceLatitude == deviceLatitude)
return;

m_deviceLatitude = deviceLatitude;
emit deviceLatitudeChanged(deviceLatitude);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setImageName
 * @param imageName
 */
void ProxyPics::setImageName(const QString imageName)
{
    if (m_imageName == imageName)
        return;

    m_imageName = imageName;
    emit imageNameChanged(imageName);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setstreetAddress1
 * @param streetAddress1
 */
void ProxyPics::setstreetAddress1(const QString streetAddress1)
{
    if (m_streetAddress1 == streetAddress1)
        return;

    m_streetAddress1 = streetAddress1;
    emit streetAddress1Changed(streetAddress1);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setStreetAddress2
 * @param streetAddress2
 */
void ProxyPics::setStreetAddress2(const QString streetAddress2)
{
    if (m_streetAddress2 == streetAddress2)
        return;

    m_streetAddress2 = streetAddress2;
    emit streetAddress2Changed(streetAddress2);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setCity
 * @param city
 */
void ProxyPics::setCity(const QString city)
{
    if (m_city == city)
        return;

    m_city = city;
    emit cityChanged(city);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setassignmentCreatorUserid
 * @param assignmentCreatorUserid
 */
void ProxyPics::setassignmentCreatorUserid(const QString assignmentCreatorUserid)
{
    if (m_assignmentCreatorUserid == assignmentCreatorUserid)
        return;

    m_assignmentCreatorUserid = assignmentCreatorUserid;
    emit assignmentCreatorUseridChanged(assignmentCreatorUserid);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setPhoneNumber
 * @param phoneNumber
 */
void ProxyPics::setPhoneNumber(const QString phoneNumber)
{
    if (m_phoneNumber == phoneNumber)
        return;

    m_phoneNumber = phoneNumber;
    emit phoneNumberChanged(phoneNumber);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setAssignmentCreatorUserid
 * @param assignmentCreatorUserid
 */
void ProxyPics::setAssignmentCreatorUserid(const QString assignmentCreatorUserid)
{
    if (m_assignmentCreatorUserid == assignmentCreatorUserid)
        return;

    m_assignmentCreatorUserid = assignmentCreatorUserid;
    emit assignmentCreatorUseridChanged(assignmentCreatorUserid);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setCurrentState
 * @param currentState
 */
void ProxyPics::setCurrentState(const QString currentState)
{
    if (m_currentState == currentState)
        return;
    updateStateMachine(currentState);
    m_currentState = currentState;
    emit currentStateChanged(currentState);

}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::setUserEmail
 * @param userEmail
 */
 void ProxyPics::setUserEmail(const QString userEmail)
{
    if (m_userEmail == userEmail)
        return;

    m_userEmail = userEmail;
    emit userEmailChanged(userEmail);
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @version 0.3.6
 * @brief ProxyPics::mapMode
 * @return
 */
 QString ProxyPics::mapMode() const
 {
     return m_mapMode;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::mapParameters
  * @return
  */
 QString ProxyPics::mapParameters() const
 {
     return m_mapParameters;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::mapCenter
  * @return
  */
 QString ProxyPics::mapCenter() const
 {
     return m_mapCenter;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::mapOrigin
  * @return
  */
 QString ProxyPics::mapOrigin() const
 {
     return m_mapOrigin;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::mapDestination
  * @return
  */
 QString ProxyPics::mapDestination() const
 {
     return m_mapDestination;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::mapZoom
  * @return
  */
 QString ProxyPics::mapZoom() const
 {
     return m_mapZoom;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::mapType
  * @return
  */
 QString ProxyPics::mapType() const
 {
     return m_mapType;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::value
  * @return
  */
 QString ProxyPics::value() const
 {
     return m_value;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::currentAssignmentName
  * @return
  */
 QString ProxyPics::currentAssignmentName() const
 {
     return m_currentAssignmentName;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::currentAssignmentAddress
  * @return
  */
 QString ProxyPics::currentAssignmentAddress() const
 {
     return m_currentAssignmentAddress;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::assignmentTicket
  * @return
  */
 QString ProxyPics::assignmentTicket() const
 {
     return m_assignmentTicket;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::assignmentNotes
  * @return
  */

 QString ProxyPics::assignmentNotes() const
 {
     return m_assignmentNotes;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::currentState
  * @return
  */
 QString ProxyPics::currentState() const
 {
     return m_currentState;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::userPassword
  * @return
  */
 QString ProxyPics::userPassword() const
 {
     return m_userPassword;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::userEmail
  * @return
  */
 QString ProxyPics::userEmail() const
 {
     return m_userEmail;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::assignmentCreatorUserid
  * @return
  */
 QString ProxyPics::assignmentCreatorUserid() const
 {
     return m_assignmentCreatorUserid;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::phoneNumber
  * @return
  */
 QString ProxyPics::phoneNumber() const
 {
     return m_phoneNumber;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::currentAssignmentId
  * @return
  */
 QString ProxyPics::currentAssignmentId() const
 {
     return m_currentAssignmentId;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::imageName
  * @return
  */
 QString ProxyPics::imageName() const
 {
     return m_imageName;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::newUserCreated
  * @return
  */
 bool ProxyPics::newUserCreated() const
 {
     return m_newUserCreated;
 }

 bool ProxyPics::newJobCreated() const
 {
     return m_newJobCreated;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::streetAddress1
  * @return
  */
 QString ProxyPics::streetAddress1() const
 {
     return m_streetAddress1;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::streetAddress2
  * @return
  */
 QString ProxyPics::streetAddress2() const
 {
     return m_streetAddress2;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::city
  * @return
  */
 QString ProxyPics::city() const
 {
     return m_city;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::UserId
  * @return
  */
 QString ProxyPics::UserId() const
 {
     return m_UserId;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::mapUrl
  * @return
  */
 QString ProxyPics::mapUrl() const
 {
     return m_mapUrl;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::deviceAltitude
  * @return
  */
 double ProxyPics::deviceAltitude() const
 {
     return m_deviceAltitude;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::deviceLatitude
  * @return
  */
 double ProxyPics::deviceLatitude() const
 {
     return m_deviceLatitude;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::deviceLongitude
  * @return
  */
 double ProxyPics::deviceLongitude() const
 {
     return m_deviceLongitude;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::assignmentValue
  * @return
  */
 QString ProxyPics::assignmentValue() const
 {
     return m_assignmentValue;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::assignmentPubDate
  * @return
  */
 QString ProxyPics::assignmentPubDate() const
 {
     return m_assignmentPubDate;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::assignmentXMLFeed
  * @return
  */
 QString ProxyPics::assignmentXMLFeed() const
 {
     return m_assignmentXMLFeed;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::firstName
  * @return
  */
 QString ProxyPics::firstName() const
 {
     return m_firstName;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::lastName
  * @return
  */
 QString ProxyPics::lastName() const
 {
     return m_lastName;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::postImagePath
  * @return
  */
 QString ProxyPics::postImagePath() const
 {
     return m_postImagePath;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::postImageName
  * @return
  */
 QString ProxyPics::postImageName() const
 {
     return m_postImageName;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::postImageTicket
  * @return
  */

 QString ProxyPics::postImageTicket() const
 {
     return m_postImageTicket;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::receiveImageId
  * @return
  */
 QString ProxyPics::receiveImageId() const
 {
     return m_receiveImageId;
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::receiveImageWithFileName
  * @return
  */

 QString ProxyPics::receiveImageWithFileName() const
 {
     return m_receiveImageWithFileName;
 }

 void ProxyPics::resetOnLogout()
 {
     state = "";
     setStreetAddress1("");
     setStreetAddress2("");
     setCity("");
 }

 /**
  * @author Louis Meadows <lou@positivelyfrontstreet.com>
  * @version 0.3.6
  * @brief ProxyPics::getDifferenceWithCurrentTime
  * This funtion take a dataTime formated string and manipulte to give the difference with current time
  * @param dateTime
  * @return QString
  */
 QString ProxyPics::getDifferenceWithCurrentTime(const QString &dateTime)
 {
     QDateTime datetime = QDateTime::fromString(dateTime);
     if (datetime.isValid())
     {
         QDateTime currentDT = QDateTime::currentDateTime();
         int daysTo = datetime.daysTo(currentDT);
         if ( daysTo > 0) {
             return QString("%1 Day(s)").arg(daysTo);
         } else {
             int secondsTo = datetime.secsTo(currentDT);
             // if its more than an hour
             if (secondsTo > 3600)
             {
                 int inHours = int(secondsTo/3600);
                 return QString("%1 Hour(s)").arg(inHours);
             } else if (secondsTo > 60){
                 int inMinutes = int(secondsTo/60);
                 return QString("%1 Minutes(s)").arg(inMinutes);
             } else {
                 return QString("Few Seconds");
             }
         }
     }

     return QString("Error in time convertion");
 }
