/** @file imageprovider.h
 *  @brief This is implementation of image provider and image manager.
 *  Creates an image provider that will provide images of give type and
 *  based on there flags.Image manager handles name,counts based on user
 *  and assignments.
 *  @author
 */
#include "imageprovider.h"
#include <QDir>
#include <QDebug>

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @author S.Srinivas Rao
 * @version 0.3.6
 * @brief ImageProvider::ImageProvider : Creates an image provider that will provide images.
 * @param type : Defines type of image to be consructed
 * @param flags: Flags to be applied for creating image.
 */
ImageProvider::ImageProvider(ImageType type, Flags flags)
    : QQuickImageProvider(type, flags)
{
    qDebug() << __FUNCTION__ <<__LINE__<< type << flags;
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @author S.Srinivas Rao
 * @version 0.3.6
 * @brief ImageProvider::~ImageProvider : Destructor of class used to clear memory
 * and free resources.
 */
ImageProvider::~ImageProvider()
{

}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @author S.Srinivas Rao
 * @version 0.3.6
 * @brief ImageProvider::requestImage : This function retrives the image based on the attributes provided in the function
 * @param id : Id used for image.
 * @param size : size of the image.
 * @param requestedSize : Requested size of the image.
 * @return Returns constructed image from the given inputs
 */
QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << __FUNCTION__ <<__LINE__<< id << size;

    QString picDirPath = QDir::homePath();
    picDirPath.append("/PPImages");
    QString imagePath = picDirPath.append("/%1").arg(id);

    QImage image(imagePath);
    return image;
}

/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @author S.Srinivas Rao
 * @version 0.3.6
 * @brief ImageManager::getImageCount : This function is used to get image count for given id.
 * @param id : Id for which image count is to be calculated
 * @return : Number of images under given Id.
 */
int ImageManager::getImageCount(const QString &id)
{
    qDebug() << __FUNCTION__ <<__LINE__<<"user id:" <<id;
    if (id.isEmpty())
        return 0;

    QString simplifiedId = id;
    simplifiedId.remove(" ");
    // search for directory
    QString picDirPath = QDir::homePath();
    picDirPath.append("/PPImages");

    QDir imageDir(picDirPath);
    if(!imageDir.exists()) {
        qDebug()<<__FUNCTION__<<__LINE__<<"Image Directory does not yet exist";
        return 0;
    }

    int counter = 0;
    QFileInfoList files = imageDir.entryInfoList();

    foreach (QFileInfo file, files){
        qDebug()<<__FUNCTION__<<__LINE__<<"Image Directory does exist:"<<file.fileName();
        if (file.isDir())
            continue;
        else
        {
            if (file.fileName().contains(simplifiedId))
                counter++;
        }
    }

    qDebug() <<counter<< __FUNCTION__ <<__LINE__<< id;
    return counter;
}

//TODO - Can we change the name to getImageCountForUserAssignment - as this has arguments as userId and assignment Id
/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @author S.Srinivas Rao
 * @version 0.3.6
 * @brief ImageManager::getImageCountForCurrentUserCurrentAssignemnt : This function used to calculate number of images
 * based on userId and Assignment Id.
 * @param userId : User Id
 * @param assignmentId : Assignment Id
 * @return : Number of images for given userId and AssignmentId
 */
int ImageManager::getImageCountForCurrentUserCurrentAssignemnt(const QString & userId, const QString &assignmentId)
{
    qDebug() << __FUNCTION__ <<__LINE__<<"assignment id"<<assignmentId<<"user id"<<userId;
    if (assignmentId.isEmpty())
        return 0;

    // search for directory
    QString picDirPath = QDir::homePath();
    picDirPath.append("/PPImages");

    QDir imageDir(picDirPath);    
    if(!imageDir.exists()) {
        qInfo()<<__FUNCTION__<<__LINE__<<"Image Directory does not yet exist";
        return 0;
    }

    int counter = 0;
    QFileInfoList files = imageDir.entryInfoList();
    foreach (QFileInfo file, files){
        qDebug()<<__FUNCTION__<<__LINE__<<"Image Directory does exist:"<<file.fileName();
        if (file.isDir())
            continue;
        else
        {
            // check for water marked pic or purchased pic
            QString imageWithwaterMarked = QString("%1_WM").arg(userId);
            imageWithwaterMarked.remove(" ");
            QString purchasedImage = userId;
            purchasedImage.remove(" ");
            QString fileName = file.baseName();
            qDebug() << imageWithwaterMarked << __FUNCTION__ <<__LINE__<< fileName;
            if ( (fileName.endsWith(imageWithwaterMarked) || fileName.endsWith(purchasedImage)) && fileName.contains(assignmentId) )
                counter++;
        }
    }

    qDebug() <<counter<< __FUNCTION__ <<__LINE__<< assignmentId<<"user id"<<userId;
    return counter;
}
/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @author S.Srinivas Rao
 * @version 0.3.6
 * @brief ImageManager::getImageName : Construct the image name from userId and index
 * @param index
 * @param userId : User Id
 * @return : Name of the image.
 */
QString ImageManager::getImageName(const int index, const QString &userId)
{
    qDebug() << __FUNCTION__ <<__LINE__<< index << userId;
    // search for directory
    QString picDirPath = QDir::homePath();
    picDirPath.append("/PPImages");
    QDir imageDir(picDirPath);
    if(!imageDir.exists()){
        qDebug()<<__FUNCTION__<<__LINE__<<"Image Directory does not yet exist";
        return 0;
        }
    int counter = -1;
    QFileInfoList files = imageDir.entryInfoList();
    foreach (QFileInfo file, files){
        qDebug()<<__FUNCTION__<<__LINE__<<"Image Directory does exist:"<<file.fileName();
        if (file.isDir())
            continue;
        else
        {
            if (file.fileName().contains(userId)){
                qDebug() << __FUNCTION__ <<__LINE__<< index << userId << file.fileName();
                counter++;
            }

            if(counter == index) {
                qDebug() << __FUNCTION__ <<__LINE__<< index << userId << file.fileName();
                return file.fileName();
            }
        }
    }

    return QString();
}

//TODO - Can we change the name to getImageCountForUserAssignment - as this has arguments as userId and assignment Id
//I belive this function arguments userId, assignmentId were later added.
/**
 * @author Louis Meadows <lou@positivelyfrontstreet.com>
 * @author S.Srinivas Rao
 * @version 0.3.6
 * @brief ImageManager::getImageNameForCurrentUserCurrentAssignemnt : This function is used to fetch the image name
 * using userid, assignmentid and index.
 * @param index
 * @param userId : User Id
 * @param assignmentId : Assignemnt Id
 * @return : Name of the image.
 */
QString ImageManager::getImageNameForCurrentUserCurrentAssignemnt(const int index,
                                                                  const QString &userId,
                                                                  QString assignmentId)
{
    qDebug() << __FUNCTION__ << index << userId;
    // search for directory
    assignmentId.remove(" ");
    QString picDirPath = QDir::homePath();
    picDirPath.append("/PPImages");

    QDir imageDir(picDirPath);
    if(!imageDir.exists()){
        qDebug()<<__FUNCTION__<<__LINE__<<"Image Directory does not yet exist";
        return 0;
    }

    int counter = -1;
    QFileInfoList files = imageDir.entryInfoList();
    foreach (QFileInfo file, files){
        if (file.isDir())
        {
            continue;
        }
        else
        {
            // check for water marked pic or purchased pic
            QString imageWithwaterMarked = QString("%1_WM").arg(userId);
            imageWithwaterMarked.remove(" ");
            QString purchasedImage = userId;
            purchasedImage.remove(" ");
            QString fileName = file.baseName();
            qDebug() <<__LINE__<< imageWithwaterMarked << __FUNCTION__ << fileName;
            if ( (fileName.contains(imageWithwaterMarked) || fileName.contains(purchasedImage)) &&
                 fileName.contains(assignmentId) )
            {
                counter++;
            }

            if(counter == index)
            {
                return file.fileName();
            }
        }
    }

    return QString();
}
