/** @file imageprovider.h
 *  @brief Function prototypes for the image provider and image manager
 *  This contains the prototypes for the image provider and image manager
 *  @author
 */


#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QQuickImageProvider>

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider(ImageType type, Flags flags = 0);
    ~ImageProvider();
    QImage requestImage(const QString & id, QSize * size, const QSize & requestedSize);

};

class ImageManager : public QObject
{
    Q_OBJECT
public:
    ImageManager() {}
    Q_INVOKABLE static int getImageCount(const QString &id);
    Q_INVOKABLE static int getImageCountForCurrentUserCurrentAssignemnt(const QString &userId, const QString &assignmentId);
    Q_INVOKABLE static QString getImageName(const int index, const QString &userId);
    Q_INVOKABLE static QString getImageNameForCurrentUserCurrentAssignemnt(const int index, const QString &userId, QString assignmentId);
};

#endif // IMAGEPROVIDER_H
