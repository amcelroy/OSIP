#ifndef OCTPIPELINE_H
#define OCTPIPELINE_H

#include <loadoctpipeline.hpp>
#include "octpipelinestage_cpu.h"
#include "octdisplaystage.h"
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <QObject>
#include "octlibrary_global.h"

using namespace OSIP;

class OCTLIBRARYSHARED_EXPORT OCTPipeline
{

private:
    shared_ptr<OSIP::LoadOCTPipeline> _Loader;

    shared_ptr<OSIP::OCTPipelineStageCPU> _Processor;

    shared_ptr<OSIP::OCTDisplayStage> _Display;

    bool m_LoadingFinished = false;

    bool m_ProcessingFinished = false;

public:
    OCTPipeline(){ }

    void init(){
        _Loader = shared_ptr<LoadOCTPipeline>(new LoadOCTPipeline());
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
        //_Loader->subscribeCurrentFrame(std::bind(&OCTPipeline::slotBScanChanged, this, std::placeholders::_1));

        //Signal that the processing is done
        _Processor->subscribeProcessingFinished(std::bind(&OCTDisplayStage::slotProcessingFinished, _Display));
        _Processor->subscribeProcessingFinished(std::bind(&OCTPipeline::slotProcessingFinished, this));
        //_Processor->subscribeFrameProcessed(std::bind(&OCTPipeline::slotFrameProcessed, this));
    }

    OSIP::LoadOCTPipeline* getLoader() { return _Loader.get(); }

    OSIP::OCTPipelineStageCPU* getProcessor() { return _Processor.get(); }

    OSIP::OCTDisplayStage* getDisplay() { return _Display.get(); }

    void start(OCTConfig config){
        _Processor->configure(config);
        _Display->start();
        _Processor->start();
        _Loader->start();
    }

    void slotDAQFinished() { m_LoadingFinished = true; }

    void slotProcessingFinished(){
        m_ProcessingFinished  = true;
    }
};

#endif // OCTPIPELINE_H
