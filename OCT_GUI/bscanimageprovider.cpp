#include "bscanimageprovider.h"

BScanImageProvider::BScanImageProvider()
    : QQuickImageProvider(QQmlImageProviderBase::Image)
{

}

QImage BScanImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize){
    QMutexLocker qm(&m_imageLock);
    return m_image;
}
