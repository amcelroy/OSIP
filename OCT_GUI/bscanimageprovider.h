#ifndef BSCANIMAGEPROVIDER_H
#define BSCANIMAGEPROVIDER_H

#include <QQuickImageProvider>

class BScanImageProvider : public QQuickImageProvider
{
    QImage* m_image = nullptr;

    QMutex m_imageLock;

public:
    BScanImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    void setImage(QImage* i) {
        m_imageLock.lock();

        if(m_image){
            delete[] m_image->bits();
            m_image->~QImage();
        }

        m_image = i;
        m_imageLock.unlock();   
    }
};

#endif // BSCANIMAGEPROVIDER_H
