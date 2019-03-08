#ifndef OCTPIPELINE_H


#define OCTPIPELINE_H

#include <daqstage.hpp>
#include <loadoctpipeline.hpp>
#include "octpipelinestage_cpu.h"
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <Peripherals/galvos.hpp>
#include <Peripherals/laser.hpp>
#include <DAQ/Alazar/daqstagealazar9350.hpp>
#include "octconfigfile.h"
#include "octdisplaystage.h"

using namespace OSIP;
using namespace OSIP::DAQ::Alazar;

class OCTPipeline
{

private:
    shared_ptr<OSIP::DAQStage<unsigned short>> _Loader;

    shared_ptr<OSIP::OCTPipelineStageCPU> _Processor;

    shared_ptr<OSIP::OCTDisplayStage> _Display;

    shared_ptr<OSIP::Peripherals::Galvos> _Galvo;

    shared_ptr<OSIP::Peripherals::Laser> _Laser;

    bool m_LoadingFinished = false;

    bool m_ProcessingFinished = false;

    bool m_DAQRunning = false;

    OCTConfig m_OCTConfig;

	bool m_LoopDAQ = false;

public:
    OCTPipeline(){ }

    enum OCT_PIPELINE_STATES{ DAQ, PLAYBACK, RECORD };

    OSIP::DAQStage<unsigned short>* getLoader() { return _Loader.get(); }

    OSIP::OCTPipelineStageCPU* getProcessor() { return _Processor.get(); }

    OSIP::OCTDisplayStage* getDisplay() { return _Display.get(); };

    OSIP::Peripherals::Laser* getLaser() { return _Laser.get(); }

    OSIP::Peripherals::Galvos* getGalvos() { return _Galvo.get(); }

    void setConfig(const OCTConfig& o){
    	m_OCTConfig = o;
    }

    OCTConfig getConfig(){
    	return m_OCTConfig;
    }

    void changeMode(const OCT_PIPELINE_STATES &state){
    	try{
    		stop();
    	}catch(...){
    		std::cout << "Error stopping OCT Pipeline";
    	}

    	DAQParameters dp;

    	switch(state){
    	case OCT_PIPELINE_STATES::DAQ:{
    		_Loader = shared_ptr<DaqStageAlazar9350>(new DaqStageAlazar9350());
    		DaqStageAlazar9350* daqstage = dynamic_cast<DaqStageAlazar9350*>(_Loader.get());
    		dp = OCTConfigFile::packageDAQParameters(m_OCTConfig, daqstage);
    		daqstage->updateDAQ(dp);
			m_LoopDAQ = true;
    		break;
    	}


    	case OCT_PIPELINE_STATES::PLAYBACK:{
    		_Loader = shared_ptr<LoadOCTPipeline>(new LoadOCTPipeline());
    		LoadOCTPipeline* lop = dynamic_cast<LoadOCTPipeline*>(_Loader.get());
    		lop->setOCTConfig(m_OCTConfig);
    		break;
    	}

		case OCT_PIPELINE_STATES::RECORD: {
			//_Loader = shared_ptr<LoadOCTPipeline>(new LoadOCTPipeline());
			//LoadOCTPipeline* lop = dynamic_cast<LoadOCTPipeline*>(_Loader.get());
			//lop->setOCTConfig(m_OCTConfig);
			break;
		}

    	default:
    		break;
    	}

        _Processor = shared_ptr<OCTPipelineStageCPU>(new OCTPipelineStageCPU());
        _Display = shared_ptr<OCTDisplayStage>(new OCTDisplayStage());

        //Connect all the Inlets
        _Loader->connect(_Processor->getInlet());
        _Processor->connect(_Display->getInlet());

        //Connect the signals and slots

        //Signal that the loader is done
        _Loader->subscribeDAQFinished(std::bind(&OCTPipelineStageCPU::slotDAQFinished, _Processor));
        _Loader->subscribeDAQFinished(std::bind(&OCTPipeline::slotDAQFinished, this));
        _Loader->subscribeDAQStarted(std::bind(&OCTPipelineStageCPU::slotDAQStarted, _Processor));

        //Signal that current frame from the loader
        //_Loader->subscribeCurrentFrame(std::bind(&OCTPipeline::slotBScanChanged, this, std::placeholders::_1));

        //Signal that the processing is done
        _Processor->subscribeProcessingFinished(std::bind(&OCTPipeline::slotProcessingFinished, this));
    }

    void start(const OCTConfig& config){
		m_OCTConfig = config;
		_Processor->configure(config);
        _Display->configure(config);
        _Processor->start();
        _Display->start();
        startDAQ(config);
    }

    void stopDAQ(){
    	m_DAQRunning = false;
    	_Loader->stop();
    }

    void startDAQ(const OCTConfig& config){
		m_OCTConfig = config;
    	if(!m_DAQRunning){
			DaqStageAlazar9350* daqstage = dynamic_cast<DaqStageAlazar9350*>(_Loader.get());
			DAQParameters dp = OCTConfigFile::packageDAQParameters(m_OCTConfig, daqstage);
			daqstage->updateDAQ(dp);
			daqstage->start();
			m_DAQRunning = true;
    	}
    }

    void stop(){
    	if(_Processor != nullptr){
    		_Processor->stop();
    		_Processor.reset();
    	}

		//TODO: Fix error in StopDAQ
    	if(_Loader != nullptr){
    		stopDAQ();
            _Loader.reset();
    	}

    	if(_Display != nullptr){
    		_Display->stop();
    		_Display.reset();
    	}
    }

    void slotDAQFinished() { m_LoadingFinished = true; }

    void slotProcessingFinished(){
        m_ProcessingFinished  = true;
		if (m_LoopDAQ) {
			DaqStageAlazar9350* daqstage = dynamic_cast<DaqStageAlazar9350*>(_Loader.get());
			daqstage->start();
		}
    }
};

#endif // OCTPIPELINE_H
