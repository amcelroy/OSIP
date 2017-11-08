#ifndef OCTPIPELINE_H
#define OCTPIPELINE_H

#include "loadoctpipeline.h"
#include "octpipelinestage_cpu.h"
#include "octdisplaystage.h"
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <QObject>

class OCTPipeline : public QObject
{
    Q_OBJECT

private:
    shared_ptr<OSIP::LoadOCTPipeline<unsigned short>> _Loader;

    shared_ptr<OSIP::OCTPipelineStageCPU> _Processor;

    shared_ptr<OSIP::OCTDisplayStage> _Display;

    bool m_LoadingFinished = false;

    bool m_ProcessingFinished = false;

    QObject *qml_BScanSlider;
public:
    OCTPipeline(QObject *parent = 0){ }

    void init();

    OSIP::LoadOCTPipeline<unsigned short>* getLoader() { return _Loader.get(); }

    OSIP::OCTPipelineStageCPU* getProcessor() { return _Processor.get(); }

    OSIP::OCTDisplayStage* getDisplay() { return _Display.get(); }

    void start(OCTConfig config);

    void slotBScanChanged(int CurrentFrame) { BScanUpdated(QVariant(CurrentFrame)); }

    void slotDAQFinished() { m_LoadingFinished = true; }

    void setBScanSlider(QObject *o) { qml_BScanSlider = o; }

    void slotProcessingFinished();

    void slotFrameProcessed() { emit FrameProcessed(); }

public slots:
    void slotMinScaleChanged(QVariant min);
    void slotMaxScaleChanged(QVariant max);
    void slotBScanSliderChanged(QVariant frame);
    void slotEnfaceChanged(QVariant one, QVariant two);

signals:
    void DAQChanged(QVariant PointsPerAScan, QVariant AScansPerBScan, QVariant NumberOfBScans);
    void BScanUpdated(QVariant CurrentBScan);
    void FrameProcessed();
    void EnFaceUpdated(QVariant CurrentEnFace);
};

#endif // OCTPIPELINE_H
