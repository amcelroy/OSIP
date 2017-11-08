#include "enfaceimageprovider.h"

EnFaceImageProvider::EnFaceImageProvider()
    : QQuickImageProvider(QQmlImageProviderBase::Image)
{

}

QImage EnFaceImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize){
    QMutexLocker qm(&m_imageLock);
    return m_image;
}
