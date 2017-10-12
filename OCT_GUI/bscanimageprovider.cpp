#include "bscanimageprovider.h"

BScanImageProvider::BScanImageProvider()
    : QQuickImageProvider(QQmlImageProviderBase::Image)
{

}

QImage BScanImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize){
    if(m_image){
        m_imageLock.lock();
        QImage copy = m_image->copy();
        QPoint center = copy.rect().center();
        QMatrix matrix;
        matrix.translate(center.x(), center.y());
        matrix.rotate(90);
        copy = copy.transformed(matrix);
        m_imageLock.unlock();
        return copy;
    }else{
        return QImage();
    }
}
