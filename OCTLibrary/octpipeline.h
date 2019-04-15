#ifndef OCTPIPELINE_H


#define OCTPIPELINE_H

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <daqstage.hpp>
#include <loadoctpipeline.hpp>
#include "octpipelinestage_cpu.h"
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <Peripherals/galvos.hpp>
#include <Peripherals/laser.hpp>
#include <savingstage.hpp>
#include <DAQ/Alazar/daqstagealazar9350.hpp>
#include "octconfigfile.h"
#include "octdisplaystage.h"
#include "nidaqmxgalvos.h"


using namespace OSIP;
using namespace OSIP::DAQ::Alazar;
using namespace OSIP::Peripherals;
using namespace std;

class OCTPipeline
{


public:
    OCTPipeline(){ }

    enum OCT_PIPELINE_STATES{ DAQ, PLAYBACK, RECORD };

    OSIP::DAQStage<unsigned short>* getLoader() { return _Loader.get(); }

    OSIP::OCTPipelineStageCPU* getProcessor() { return _Processor.get(); }

    OSIP::OCTDisplayStage* getDisplay() { return _Display.get(); };

    OSIP::Peripherals::Laser* getLaser() { return _Laser.get(); }

    OSIP::Peripherals::niDAQMXGalvos* getGalvos() { return _Galvo.get(); }

	OSIP::SavingStage<unsigned short>* getSavingStage() { return _Saving.get(); }

	void setDataFolder(const std::string &folder) {

	}

    void setConfig(const OCTConfig& o){
    	m_OCTConfig = o;
    }

    OCTConfig getConfig(){
    	return m_OCTConfig;
    }

	void deconstructPipeline() {
		stop(); //Stop all processes
	}

    void changeMode(const OCT_PIPELINE_STATES &state){
		m_State = state;
		
		try{
			deconstructPipeline();
    	}catch(...){
    		std::cout << "Error stopping OCT Pipeline";
    	}

    	DAQParameters dp;

    	switch(m_State){
    	case OCT_PIPELINE_STATES::DAQ:{
			_Galvo = shared_ptr<niDAQMXGalvos>(new niDAQMXGalvos());
    		_Loader = shared_ptr<DaqStageAlazar9350>(new DaqStageAlazar9350());
    		DaqStageAlazar9350* daqstage = dynamic_cast<DaqStageAlazar9350*>(_Loader.get());
    		dp = OCTConfigFile::packageDAQParameters(m_OCTConfig, daqstage);
    		daqstage->updateDAQ(dp);
			m_LoopDAQ = true;
			_Galvo->create("/Dev1");
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

		_Saving = shared_ptr<SavingStage<unsigned short>>(new SavingStage<unsigned short>(H5T_STD_U16BE));
        _Processor = shared_ptr<OCTPipelineStageCPU>(new OCTPipelineStageCPU());
        _Display = shared_ptr<OCTDisplayStage>(new OCTDisplayStage());

        //Connect all the Inlets
        _Loader->connect(_Processor->getInlet());
        _Processor->connect(_Display->getInlet());
		_Loader->connect(_Saving->getInlet());

        //Connect the signals and slots

        //Signal that the loader is done
        _Loader->subscribeDAQFinished(std::bind(&OCTPipelineStageCPU::slotDAQFinished, _Processor));
        _Loader->subscribeDAQFinished(std::bind(&OCTPipeline::slotDAQFinished, this));
        _Loader->subscribeDAQStarted(std::bind(&OCTPipelineStageCPU::slotDAQStarted, _Processor));
		_Loader->subscribeDAQStarted(std::bind(&Galvos::run, _Galvo));
		_Loader->subscribeDAQFinished(std::bind(&Galvos::stop, _Galvo));
		_Loader->subscribeDAQFinished(std::bind(&SavingStage<unsigned short>::slotDAQFinished, _Saving));

        //Signal that current frame from the loader
        //_Loader->subscribeCurrentFrame(std::bind(&OCTPipeline::slotBScanChanged, this, std::placeholders::_1));

        //Signal that the processing is done
        _Processor->subscribeProcessingFinished(std::bind(&OCTPipeline::slotProcessingFinished, this));
    }

    void start(const OCTConfig& config, const Galvos::GalvoParameters &gp){
		m_GalvoParameters = gp;
		m_OCTConfig = config;
		_Galvo->configure(gp, config);
		_Processor->configure(config);
        _Display->configure(config);
        _Processor->start();
        _Display->start();
        startDAQ(config);
		m_LoopDAQ = true;
    }

    void stopDAQ(){
    	m_DAQRunning = false;
    	_Loader->stop();
		//_Loader->waitFinished();
    }

    void startDAQ(const OCTConfig& config){
		m_OCTConfig = config;
		DAQParameters dp;
		DaqStageAlazar9350* daqstage;
		LoadOCTPipeline* load;

    	if(!m_DAQRunning){
			switch (m_State) {
			case OCT_PIPELINE_STATES::DAQ:
				daqstage = dynamic_cast<DaqStageAlazar9350*>(_Loader.get());
				dp = OCTConfigFile::packageDAQParameters(m_OCTConfig, daqstage);
				daqstage->updateDAQ(dp);
				daqstage->start();
				m_DAQRunning = true;
				break;

			case OCT_PIPELINE_STATES::PLAYBACK:
				load = dynamic_cast<LoadOCTPipeline*>(_Loader.get());
				//DAQParameters dp = OCTConfigFile::packageDAQParameters(m_OCTConfig, daqstage);
				load->start();
				m_DAQRunning = true;
				break;
			}
    	}

		_Saving->start();
    }

    void stop(){
		m_LoopDAQ = false;

		//TODO: Fix error in StopDAQ
		if (_Loader != nullptr) {
			stopDAQ();
		}

    	if(_Processor != nullptr){
    		_Processor->stop();
			//_Processor->waitFinished();
    	}

    	if(_Display != nullptr){
    		_Display->stop();
			//_Display->waitFinished();
    	}

		if (_Saving != nullptr) {
			_Saving->stop();
		}
    }

    void slotDAQFinished() { m_LoadingFinished = true; }

    void slotProcessingFinished(){
        m_ProcessingFinished  = true;
		if (m_LoopDAQ && m_DAQRunning) {
			_Loader->start();
		}
    }

	private:
		shared_ptr<OSIP::DAQStage<unsigned short>> _Loader;

		shared_ptr<OSIP::OCTPipelineStageCPU> _Processor;

		shared_ptr<OSIP::OCTDisplayStage> _Display;

		shared_ptr<OSIP::Peripherals::niDAQMXGalvos> _Galvo;

		shared_ptr<OSIP::Peripherals::Laser> _Laser;

		shared_ptr<OSIP::SavingStage<unsigned short>> _Saving;

		bool m_LoadingFinished = false;

		bool m_ProcessingFinished = false;

		bool m_DAQRunning = false;

		OCTConfig m_OCTConfig;

		bool m_LoopDAQ = false;

		Galvos::GalvoParameters m_GalvoParameters;

		OCT_PIPELINE_STATES m_State;
};

#endif // OCTPIPELINE_H
