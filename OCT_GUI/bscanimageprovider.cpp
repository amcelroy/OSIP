#include "bscanimageprovider.h"

BScanImageProvider::BScanImageProvider()
    : QQuickImageProvider(QQmlImageProviderBase::Image)
{

}

QImage BScanImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize){
    //if(m_image){
        //m_imageLock.lock();
        //m_imageLock.unlock();
    QMutexLocker qm(&m_imageLock);
    return m_image;
    //}else{
    //    return QImage();
    //}
}
