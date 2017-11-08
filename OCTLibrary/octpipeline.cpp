#include "octpipeline.h"

void OCTPipeline::init(){
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

void OCTPipeline::start(OCTConfig config){
    _Processor->configure(config);

    qml_BScanSlider->setProperty("enabled", QVariant(false));

    emit DAQChanged(QVariant(config.PointsPerAScan),
                    QVariant(config.AScansPerBScan),
                    QVariant(config.TotalBScans));

    _Display->start();
    _Processor->start();
    _Loader->start();
}

void OCTPipeline::slotProcessingFinished() {
    m_ProcessingFinished  = true;
    qml_BScanSlider->setProperty("enabled", QVariant(true));
}

void OCTPipeline::slotMinScaleChanged(QVariant min){
    _Display->setMin(min.toFloat());
}

void OCTPipeline::slotMaxScaleChanged(QVariant max){
    _Display->setMax(max.toFloat());
}

void OCTPipeline::slotBScanSliderChanged(QVariant frame){
    if(m_ProcessingFinished){
        _Loader->readFrame(frame.toInt());
    }
}

