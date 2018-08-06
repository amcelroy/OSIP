#ifndef ENFACEIMAGEPROVIDER_H
#define ENFACEIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <memory>
#include <QMutex>
#include "octlibrary_global.h"

using namespace std;

class OCTLIBRARYSHARED_EXPORT EnFaceImageProvider : public QQuickImageProvider
{
    unsigned int* m_EnFace = NULL;

    QMutex m_imageLock;

    QImage m_image;
public:
    EnFaceImageProvider()
        : QQuickImageProvider(QQmlImageProviderBase::Image)
    {

    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize){
        QMutexLocker qm(&m_imageLock);
        return m_image;
    }

    ~EnFaceImageProvider(){
        delete[] m_EnFace;
    }

    /**
     * @brief setRow
     * @param i Current row to update in the EnFace, should be A-Scans per B-Scan long
     * @param dims Dims should be the following: 0 - A-Scans per B-Scan, 1 - Number Of BScans, 2 - Current B-Scan
     */
    void setRow(const vector<unsigned int> &i, vector<unsigned long long> dims){
        if(m_EnFace == NULL){
            m_EnFace = new unsigned int[dims[0]*dims[1]];
        }

        QMutexLocker qm(&m_imageLock);
        memcpy(m_EnFace + dims[0]*dims[2], i.data(), sizeof(unsigned int)*i.size());
        m_image = QImage((unsigned char*)m_EnFace, dims[0], dims[1], QImage::Format_RGB32);
    }
};

#endif // ENFACEIMAGEPROVIDER_H
