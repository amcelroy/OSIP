#ifndef OCTPIPELINE_H
#define OCTPIPELINE_H

#include <loadoctpipeline.hpp>
#include "octpipelinestage_cpu.h"
#include "octdisplaystage.h"
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <Peripherals/galvos.hpp>
#include <Peripherals/laser.hpp>
#include <DAQ/Alazar/daqstagealazar660.hpp>

using namespace OSIP;
using namespace OSIP::DAQ::Alazar;

class OCTPipeline
{

private:
    shared_ptr<OSIP::LoadOCTPipeline> _Loader;

    shared_ptr<OSIP::OCTPipelineStageCPU> _Processor;

    shared_ptr<OSIP::OCTDisplayStage> _Display;

    shared_ptr<OSIP::Peripherals::Galvos> _Galvo;

    shared_ptr<OSIP::Peripherals::Laser> _Laser;

    shared_ptr<DaqStageAlazar> _Alazar;

    bool m_LoadingFinished = false;

    bool m_ProcessingFinished = false;

public:
    OCTPipeline(){ }

    void init(){
        _Loader = shared_ptr<LoadOCTPipeline>(new LoadOCTPipeline());
        _Processor = shared_ptr<OCTPipelineStageCPU>(new OCTPipelineStageCPU());
        _Display = shared_ptr<OCTDisplayStage>(new OCTDisplayStage());
        _Alazar = shared_ptr<DaqStageAlazar>(new DaqStageAlazar660());

        //Connect all the Inlets
        _Loader->connect(_Processor->getInlet());
        _Processor->connect(_Display->getInlet());

        //Connect the signals and slots

        //Signal that the loader is done
        _Loader->subscribeDAQFinished(std::bind(&OCTDisplayStage::slotDAQFinished, _Display));
        _Loader->subscribeDAQFinished(std::bind(&OCTPipelineStageCPU::slotDAQFinished, _Processor));
        _Loader->subscribeDAQFinished(std::bind(&OCTPipeline::slotDAQFinished, this));
        _Loader->subscribeDAQStarted(std::bind(&OCTPipelineStageCPU::slotDAQStarted, _Processor));

        //Signal that current frame from the loader
        //_Loader->subscribeCurrentFrame(std::bind(&OCTPipeline::slotBScanChanged, this, std::placeholders::_1));

        //Signal that the processing is done
        _Processor->subscribeProcessingFinished(std::bind(&OCTDisplayStage::slotProcessingFinished, _Display));
        _Processor->subscribeProcessingFinished(std::bind(&OCTPipeline::slotProcessingFinished, this));
    }

    OSIP::LoadOCTPipeline* getLoader() { return _Loader.get(); }

    OSIP::OCTPipelineStageCPU* getProcessor() { return _Processor.get(); }

    OSIP::OCTDisplayStage* getDisplay() { return _Display.get(); }

    OSIP::Peripherals::Laser* getLaser() { return _Laser.get(); }

    OSIP::Peripherals::Galvos* getGalvos() { return _Galvo.get(); }

    void start(const OCTConfig& config){
        _Processor->configure(config);
        _Display->configure(config);

        _Display->start();
        _Processor->start();
        _Loader->start();
    }

    void stop(){
        _Display->stop();
        _Processor->stop();
        _Loader->stop();

        _Display.reset();
        _Loader.reset();
        _Processor.reset();
    }

    void slotDAQFinished() { m_LoadingFinished = true; }

    void slotProcessingFinished(){
        m_ProcessingFinished  = true;
    }
};

#endif // OCTPIPELINE_H
