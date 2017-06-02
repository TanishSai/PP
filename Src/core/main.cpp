//
// Author: Louis Meadows
// Copyright Positively Front Street, Inc 2016

#include "main.h"
#include "imageprovider.h"
/**
 * @author Louis Meadows
 * @version 0.3.6
 * @brief main
 * @param argc
 * @param argv
 * @return
 *
 */

int main(int argc, char *argv[])
{

    QCoreApplication::setOrganizationName("Positively Front Street, Inc");
    QCoreApplication::setOrganizationDomain("proxypics.com");
    QCoreApplication::setApplicationName("ProxyPics");
    QCoreApplication::setApplicationVersion("0.3.6");
    QSettings settings;
    settings.setValue("HOSTNAME", "www.proxypics.com");
    qDebug()<<__LINE__<<"ProxyPics 0.3.6";

    // Controls on Android / iPhone much too small, switch on HighDPI
//    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

//    Qt::AA_EnableHighDpiScaling
//    Enables high-DPI scaling in Qt on supported platforms (see also High DPI Displays).
//    Supported platforms are X11, Windows and Android.
//    Enabling makes Qt scale the main (device independent) coordinate system
//    according to display scale factors provided by the operating system.
//    This corresponds to setting the QT_AUTO_SCREEN_SCALE_FACTOR environment variable to 1.
//    This value has been added in Qt 5.6. This attribute must be set before Q(Gui)Application is constructed.
//    Qt::AA_DisableHighDpiScaling
//    Disables high-DPI scaling in Qt, exposing window system coordinates.
//    Note that the window system may do its own scaling,
//    so this does not guarantee that QPaintDevice::devicePixelRatio() will be equal to 1.
//    In addition, scale factors set by QT_SCALE_FACTOR will not be affected.
//    This corresponds to setting the QT_AUTO_SCREEN_SCALE_FACTOR environment variable to 0.
//    This value has been added in Qt 5.6. This attribute must be set before Q(Gui)Application is constructed.

    QGuiApplication app(argc, argv);
    QUrl appPath(QString("%1").arg(app.applicationDirPath()));
    qInfo()<<"Gui Application Path:"<<appPath;

    proxyPicsIcon = QIcon(":/images/proxyPicsLogo.png");
    app.setWindowIcon(proxyPicsIcon);
    foreach (screen, QGuiApplication::screens())
            screen->setOrientationUpdateMask( Qt::PortraitOrientation );

    QPointer<ProxyPics> proxyPics = new ProxyPics();
    checkdata = "ProxyPics 0.3.6";
    proxyPics->CheckPoint(checkdata); // tell proxypics server UDP
    checkString = appPath.toString();
    checkdata = checkString.toLocal8Bit();
    proxyPics->CheckPoint(checkdata); // tell proxypics server PWD UDP

    engine = new QQmlApplicationEngine();
    qml = QUrl("qrc:/Src/gui/QML/main.qml");
    engine->clearComponentCache();
    engine->rootContext()->setContextProperty("proxyPics",proxyPics.data());

    ImageProvider *imageProvider = new ImageProvider(QQmlImageProviderBase::Image);
    engine->addImageProvider("imageprovider",imageProvider);
    ImageManager *imageManager = new ImageManager();
    engine->rootContext()->setContextProperty("imageManager", imageManager);

    const QString initialUrl = QStringLiteral("https://www.google.com/maps/?q=");
    engine->rootContext()->setContextProperty(QStringLiteral("initialUrl"),
                                ProxyPics::fromUserInput(initialUrl));

    const QString paypalUrl = QStringLiteral("http://www.proxyPics.com/paypal.html");
    engine->rootContext()->setContextProperty(QStringLiteral("http://www.proxyPics.com/paypal.html"),
                                              ProxyPics::fromUserInput(paypalUrl));
    engine->rootContext()->setBaseUrl(qml);
    engine->load(qml);

    firstRootItem = engine->rootObjects().first();

    return app.exec();
}

