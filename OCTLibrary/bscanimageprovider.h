#ifndef BSCANIMAGEPROVIDER_H
#define BSCANIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QMutex>
#include <memory>
#include "octlibrary_global.h"
#include <chrono>

using namespace std;

class OCTLIBRARYSHARED_EXPORT BScanImageProvider : public QQuickImageProvider
{
    QImage m_image;

    QMutex m_imageLock;

    unsigned int* m_ptrHolder = NULL;

public:
    BScanImageProvider()
        : QQuickImageProvider(QQmlImageProviderBase::Image)
    {

    }

    ~BScanImageProvider(){
        delete[] m_ptrHolder;
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize){
        QMutexLocker qm(&m_imageLock);
        return m_image;
    }

    void setPixels(const vector<unsigned int> &i, const vector<unsigned long long> &dims) {
        auto start = chrono::high_resolution_clock::now();

        if(m_ptrHolder == NULL){
            m_ptrHolder = new unsigned int[(int)dims.at(0)*(int)dims.at(1)];
        }

        QMutexLocker qm(&m_imageLock);
        //if(i != nullptr){
            //Insures the shared_ptr reference count is held here also
            memcpy(m_ptrHolder, i.data(), sizeof(unsigned int)*i.size());
            m_image = QImage((unsigned char*)m_ptrHolder, (int)dims.at(0), (int)dims.at(1), QImage::Format_RGB32);
            QPoint center = m_image.rect().center();
            QMatrix matrix;
            matrix.translate(center.x(), center.y());
            matrix.rotate(90);
            m_image = m_image.transformed(matrix);

            auto stop = chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::micro> elapsed = stop - start;
            int x = 0;
        //}

    }
};

#endif // BSCANIMAGEPROVIDER_H
