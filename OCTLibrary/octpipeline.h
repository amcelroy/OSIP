#ifndef OCTPIPELINE_H
#define OCTPIPELINE_H

#include "loadoctpipeline.h"
#include "octpipelinestage_cpu.h"
#include "octdisplaystage.h"
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <QObject>

using namespace OSIP;

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

    void init(){
        _Loader = shared_ptr<LoadOCTPipeline<unsigned short>>(new LoadOCTPipeline<unsigned short>());
        _Processor = shared_ptr<OCTPipelineStageCPU>(new OCTPipelineStageCPU());
        _Display = shared_ptr<OCTDisplayStage>(new OCTDisplayStage());

        //Connect all the Inlets
        _Loader->connect(_Processor->getInlet());
        _Processor->connect(_Display->getInlet());

        //Connect the signals and slots

        //Signal that the loader is done
        _Loader->subscribeDAQFinished(std::bind(&OCTDisplayStage::slotDAQFinished, _Display));
        _Loader->subscribeDAQFinished(std::bind(&OCTPipelineStageCPU::slotDAQFinished, _Processor));
        _Loader->subscribeDAQFinished(std::bind(&OCTPipeline::slotDAQFinished, this));

        //Signal that current frame from the loader
        _Loader->subscribeCurrentFrame(std::bind(&OCTPipeline::slotBScanChanged, this, std::placeholders::_1));

        //Signal that the processing is done
        _Processor->subscribeProcessingFinished(std::bind(&OCTDisplayStage::slotProcessingFinished, _Display));
        _Processor->subscribeProcessingFinished(std::bind(&OCTPipeline::slotProcessingFinished, this));
        _Processor->subscribeFrameProcessed(std::bind(&OCTPipeline::slotFrameProcessed, this));
    }

    OSIP::LoadOCTPipeline<unsigned short>* getLoader() { return _Loader.get(); }

    OSIP::OCTPipelineStageCPU* getProcessor() { return _Processor.get(); }

    OSIP::OCTDisplayStage* getDisplay() { return _Display.get(); }

    void start(OCTConfig config){
        _Processor->configure(config);

        qml_BScanSlider->setProperty("enabled", QVariant(false));

        emit DAQChanged(QVariant(config.PointsPerAScan),
                        QVariant(config.AScansPerBScan),
                        QVariant(config.TotalBScans));

        _Display->start();
        _Processor->start();
        _Loader->start();
    }

    void slotBScanChanged(int CurrentFrame) { BScanUpdated(QVariant(CurrentFrame)); }

    void slotDAQFinished() { m_LoadingFinished = true; }

    void setBScanSlider(QObject *o) { qml_BScanSlider = o; }

    void slotProcessingFinished(){
        m_ProcessingFinished  = true;
        qml_BScanSlider->setProperty("enabled", QVariant(true));
    }

    void slotFrameProcessed() { emit FrameProcessed(); }

public slots:
    void slotMinScaleChanged(QVariant min){
        _Display->setMin(min.toFloat());
    }

    void slotMaxScaleChanged(QVariant max){
        _Display->setMax(max.toFloat());
    }

    void slotBScanSliderChanged(QVariant frame){
        if(m_ProcessingFinished){
            _Loader->readFrame(frame.toInt());
        }
    }

    void slotEnfaceChanged(QVariant one, QVariant two){
         _Processor->setEnfaceRange(one.toInt(), two.toInt());
    }

signals:
    void DAQChanged(QVariant PointsPerAScan, QVariant AScansPerBScan, QVariant NumberOfBScans);
    void BScanUpdated(QVariant CurrentBScan);
    void FrameProcessed();
    void EnFaceUpdated(QVariant CurrentEnFace);
};

#endif // OCTPIPELINE_H
