#ifndef ENFACEIMAGEPROVIDER_H
#define ENFACEIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <memory>

using namespace std;

class EnFaceImageProvider : public QQuickImageProvider
{
    unsigned int* m_EnFace = NULL;

    QMutex m_imageLock;

    QImage m_image;
public:
    EnFaceImageProvider();

    ~EnFaceImageProvider(){
        delete m_EnFace;
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    /**
     * @brief setRow
     * @param i Current row to update in the EnFace, should be A-Scans per B-Scan long
     * @param dims Dims should be the following: 0 - A-Scans per B-Scan, 1 - Number Of BScans, 2 - Current B-Scan
     */
    void setRow(shared_ptr<vector<unsigned int>> i, vector<unsigned long> dims){
        if(m_EnFace == NULL){
            unsigned long size = dims[0]*dims[1];
            m_EnFace = new unsigned int[size];
        }

        QMutexLocker qm(&m_imageLock);
        if(i != nullptr){
            //Insures the shared_ptr reference count is held here also
            memcpy(&(m_EnFace[(int)dims[2]*dims[0]]), i->data(), dims[0]*sizeof(unsigned int));
            m_image = QImage((unsigned char*)m_EnFace, dims[0], dims[1], QImage::Format_RGB32);

//            QPoint center = m_image.rect().center();
//            QMatrix matrix;
//            matrix.translate(center.x(), center.y());
//            matrix.rotate(90);
//            m_image = m_image.transformed(matrix);
        }
    }
};

#endif // ENFACEIMAGEPROVIDER_H
