#ifndef BSCANIMAGEPROVIDER_H
#define BSCANIMAGEPROVIDER_H

#include <QQuickImageProvider>

using namespace std;

class BScanImageProvider : public QQuickImageProvider
{
    QImage m_image;

    QMutex m_imageLock;

    shared_ptr<vector<unsigned int>> m_ptrHolder;

public:
    BScanImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    void setPixels(shared_ptr<vector<unsigned int>> i, vector<unsigned long long> dims) {
        QMutexLocker qm(&m_imageLock);
        if(i != nullptr){
            //Insures the shared_ptr reference count is held here also
            m_ptrHolder = move(i);
            m_image = QImage((unsigned char*)m_ptrHolder->data(), (int)dims.at(0), (int)dims.at(1), QImage::Format_RGB32);
            QPoint center = m_image.rect().center();
            QMatrix matrix;
            matrix.translate(center.x(), center.y());
            matrix.rotate(90);
            m_image = m_image.transformed(matrix);
        }
    }
};

#endif // BSCANIMAGEPROVIDER_H
