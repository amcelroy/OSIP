#ifndef MAINBACKEND_H
#define MAINBACKEND_H

#include <QObject>
#include "octpipeline.h"
#include "octconfigfile.h"

class OCTBackend : public QObject
{
    Q_OBJECT

    Q_PROPERTY(MODE Mode READ getMode WRITE setMode NOTIFY modeChanged)
public:
    OCTBackend();

    OCTPipeline* getOCTPipeline() { return &m_OCTPipeline; }

    enum MODE {
        MODE_REVIEW,
        MODE_RECORD,
        MODE_LIVE,
        MODE_LOADING,
    };
    Q_ENUM(MODE)

    MODE getMode() { return m_Mode; }

    void setMode(MODE m) {
        if(m_Mode != m){
            emit modeChanged();
        }
        m_Mode = m;
    }

    void onProcessingFinished(){
        m_Mode = MODE_REVIEW;
    }
private:
    OCTConfigFile m_octcf;
    OCTConfig m_octc;

    OCTPipeline m_OCTPipeline;

    MODE m_Mode = MODE_REVIEW;

signals:
    void modeChanged();

public slots:
    void loadOCT(string path);

    void record();

    void reprocessEnFace();

    void enFaceSelectionBoundsChanged(QVariant xPixels,
                                      QVariant yPixels,
                                      QVariant selectionWidthPixels,
                                      QVariant selectionHeightPixels,
                                      QVariant canvasWidthPixels,
                                      QVariant canvasHeightPixels);
};

#endif // MAINBACKEND_H