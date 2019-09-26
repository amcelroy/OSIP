/*
 * DaqStageAlazar.h
 *
 *  Created on: Sep 26, 2018
 *      Author: amcelroy
 */

#ifndef DAQSTAGEALAZAR9350_HPP_
#define DAQSTAGEALAZAR9350_HPP_

#include "daqstagealazar.hpp"
#include <AlazarError.h>
#include <AlazarApi.h>
#include <AlazarCmd.h>
#include <payload.hpp>
#include <iostream>

#define BUFFERS_LIVE 100

namespace OSIP{
	namespace DAQ{
		namespace Alazar{
			using namespace OSIP;
			using namespace DAQ;
			using namespace Alazar;
			using namespace std;

			class DaqStageAlazar9350 : public DaqStageAlazar {

			public:
				DaqStageAlazar9350(){
					_DAQParameters = getDefaultDAQParameters();
				}

				~DaqStageAlazar9350() override {

				}

		        void stop(){
		        	PipelineStage::stop();
		        }

		        void reset(){

		        }

		        void acquire(){

		        }

		        bool init(){
					return true;
		        }

				string typeName() override {
					return "DaqStageAlazar9350";
				}

		        DAQParameters getDefaultDAQParameters(){
		        	DAQParameters d;
		        	d.Bits = 12;
		        	d.Channels = A;
		        	d.Coupling = OSIP_AC;
		        	d.Impedance = OSIP_50;
		        	d.PointsPerTrigger = 1024;
		        	d.DAQClockRate = 400e6;
		        	d.TriggerVoltage = 1.0;
		        	d.TriggerTimeoutSec = 5;
		        	d.Voltage = 2.0f;
		        	d.Trigger = OSIP_EXTERNAL_TRIGGER;
		        	d.TriggerVoltage = 1.0f;
					d.BScansPerTransfer = 4;
		        	return d;
		        }


			protected:
		        void configureDAQ(const DAQParameters& daqp) override {
					_DAQParameters = daqp;

		        	float requestedVoltage = _DAQParameters.Voltage;
		        	U32 voltage;
		        	if(requestedVoltage <= .2f){
		        		voltage = INPUT_RANGE_PM_200_MV;
		        	}else if(requestedVoltage <= .4f){
		        		voltage = INPUT_RANGE_PM_200_MV;
		        	}else if(requestedVoltage <= .8f){
		        		voltage = INPUT_RANGE_PM_800_MV;
		        	}else if(requestedVoltage <= 2.0f){
		        		voltage = INPUT_RANGE_PM_2_V;
		        	}else if(requestedVoltage <= 4.0f){
		        		voltage = INPUT_RANGE_PM_4_V;
		        	}else if(requestedVoltage <= 8.0f){
		        		voltage = INPUT_RANGE_PM_8_V;
		        	}else if(requestedVoltage <= 16.0f){
		        		voltage = INPUT_RANGE_PM_16_V;
		        	}

		        	U32 coupling;
		        	switch(_DAQParameters.Coupling){
		        	case OSIP_COUPLING::OSIP_AC:
		        		coupling = AC_COUPLING;
		        		break;

		        	case OSIP_COUPLING::OSIP_DC:
		        	default:
		        		coupling = DC_COUPLING;
		        		break;
		        	}

		        	U32 impedance;
		        	switch(_DAQParameters.Impedance){
		        	case OSIP_50:
		        	default:
		        		impedance = IMPEDANCE_50_OHM;
		        		break;

		        	case OSIP_1M:
		        		impedance = IMPEDANCE_1M_OHM;
		        		break;
		        	}

		        	switch(_DAQParameters.ClockSource){
		        	case OSIP_EXTERNAL_CLOCK:
		        		m_Error = AlazarSetCaptureClock(m_BoardHandle, EXTERNAL_CLOCK_AC, SAMPLE_RATE_USER_DEF, CLOCK_EDGE_RISING, 0);
		        		break;

		        	case OSIP_INTERNAL_CLOCK:
		        	default:
		        		m_Error = AlazarSetCaptureClock(m_BoardHandle, INTERNAL_CLOCK, SAMPLE_RATE_500MSPS, CLOCK_EDGE_RISING, 0);
		        		break;

		        		log("Alazar set clock return code: " + to_string(m_Error));
		        	}

		        	U8 channel;
		        	switch(_DAQParameters.Channels){
		        	case OSIP_CHANNELS::A:
		        	default:
		        		channel = CHANNEL_A;
		        		break;

		        	case OSIP_CHANNELS::B:
		        		channel = CHANNEL_B;
		        		break;

		        	case OSIP_CHANNELS::ALL:
		        		channel = CHANNEL_A | CHANNEL_B;
		        		break;

		        	}

	        		m_Error = AlazarInputControl(m_BoardHandle,
	        				channel,
							coupling,
							voltage,
							impedance);
	        		log("Alazar set input control return code: " + to_string(m_Error));

	        		U32 trigger;
	        		U32 triggerEngine = TRIG_ENGINE_J;
	        		U32 triggerSlope;

	        		switch(_DAQParameters.Trigger){
	        		case OSIP_INTERNAL_TRIGGER:
	        			trigger = TRIG_CHAN_A;
	        			break;
	        		default:
	        		case OSIP_EXTERNAL_TRIGGER:
	        			trigger = TRIG_EXTERNAL;
	        			break;
	        		}

	        		switch(_DAQParameters.TriggerSlope){
	        		case OSIP_RISING:
	        			triggerSlope = TRIGGER_SLOPE_POSITIVE;
	        			break;
	        		default:
	        		case OSIP_FALLING:
	        			triggerSlope = TRIGGER_SLOPE_NEGATIVE;
	        			break;
	        		}

					if (trigger == TRIG_EXTERNAL) {
						m_Error = AlazarSetExternalTrigger(m_BoardHandle, DC_COUPLING, ETR_5V);
						log("AlazarSetExternalTrigger return code: " + to_string(m_Error));
					}

	        		float triggerLevel = (_DAQParameters.TriggerVoltage / requestedVoltage)*127 + 128;
	        		U8 triggerLevel_u8 = static_cast<U8>(triggerLevel);

	        		m_Error = AlazarSetTriggerOperation(m_BoardHandle,
	        				TRIG_ENGINE_OP_J,
							TRIG_ENGINE_J,
							trigger,
							triggerSlope,
							135,
							TRIG_ENGINE_K,
							TRIG_DISABLE,
							triggerSlope,
							128);
	        		log("Alazar Set Trigger Operation return code: " + to_string(m_Error));

	        		m_Error = AlazarSetRecordSize(m_BoardHandle, 0, _DAQParameters.PointsPerTrigger);
	        		log("Alazar set record size return code: " + to_string(m_Error));

	        		U32 timeout_ticks = (U32)(_DAQParameters.TriggerTimeoutSec / 10.e-6 + 0.5);
	        		m_Error = AlazarSetTriggerTimeOut(m_BoardHandle, timeout_ticks);
	        		log("Alazar Set Trigger Timeout return code: " + to_string(m_Error));

	        		U32 flags = ADMA_NPT | ADMA_EXTERNAL_STARTCAPTURE | ADMA_ALLOC_BUFFERS;
	        		U32 TrigPerAcq = 1;
					if (_DAQParameters.TotalBuffers == 1) {
						_DAQParameters.BScansPerTransfer = 1;
					}

					if (_DAQParameters.TotalBuffers == 1) {
						m_Error = AlazarBeforeAsyncRead(m_BoardHandle,
							channel,
							0,
							_DAQParameters.PointsPerTrigger,
							_DAQParameters.TriggersPerBuffer*_DAQParameters.BScansPerTransfer,
							0x7fffffff,
							flags);
						log("Alazar Before Aysnc Read return code: " + to_string(m_Error));
					}
					else {
						m_Error = AlazarBeforeAsyncRead(m_BoardHandle,
							channel,
							0,
							_DAQParameters.PointsPerTrigger,
							_DAQParameters.TriggersPerBuffer*_DAQParameters.BScansPerTransfer,
							_DAQParameters.TotalBuffers*_DAQParameters.TriggersPerBuffer,
							flags);
						log("Alazar Before Aysnc Read return code: " + to_string(m_Error));
					}
		        }

		        void work() override {
					unsigned long long frameCounter = 0;

		        	m_DAQFinished = false;

		        	unsigned long bufferSize = _DAQParameters.PointsPerTrigger*_DAQParameters.TriggersPerBuffer*_DAQParameters.BScansPerTransfer;

					this->configureDAQ(_DAQParameters);

					////TODO: Renable for acq
		   //     	if(_DAQParameters.TotalBuffers == 1){
		   //     		createBuffers(BUFFERS_LIVE, bufferSize);
		   //     	}else{
		   //     		createBuffers(_DAQParameters.TotalBuffers/_DAQParameters.BScansPerTransfer, bufferSize);
		   //     	}

					sig_DAQStarted();

					//TODO: Renable for acq!
		        	m_Error = AlazarStartCapture(m_BoardHandle);
		        	log("Alazar Start Capture return code: " + to_string(m_Error));

		        	if(m_Error == ApiSuccess){
		        		//while(m_Error == ApiSuccess && stopThread == false){
		        		while(stopThread == false){
		        			if(pauseThread == true){
		        				this->pipelineSleep(50);
		        			}else{
								shared_ptr<vector<unsigned short>> buffer(new vector<unsigned short>(bufferSize));

								//m_Error = readBufferRandom(buffer.get());
								m_Error = readBuffer(buffer.get());

								Payload<unsigned short> p;
								vector<unsigned long long> dim;
								dim.push_back(_DAQParameters.PointsPerTrigger);
								dim.push_back(_DAQParameters.TriggersPerBuffer);
								dim.push_back(_DAQParameters.BScansPerTransfer);
								dim.push_back(frameCounter);

								string label = "ATS9350";


								if(frameCounter > _DAQParameters.TotalBuffers - 1){
									//Stop thread
									stopThread = true;
									this->sig_DAQFinished();
									frameCounter = 0;
								}else{
									p.addData(dim, buffer, "Frame");
									this->sendPayload(p);
									frameCounter += _DAQParameters.BScansPerTransfer;
								}

								switch (m_Error) {
									case ApiTransferComplete:
										frameCounter = 0;
										stopThread = true;
										break;
									case ApiBufferOverflow:
										stopThread = true;
										break;
									case ApiSuccess:
										break;
									default:
										log("Alazar read buffer error: " + to_string(m_Error));
										stopThread = true;
										break;
								}
		        			}
		        		}
		        	}

					m_Error = AlazarAbortAsyncRead(m_BoardHandle);
					m_Error = AlazarAbortCapture(m_BoardHandle);

		        	m_DAQFinished = true;

	        		for(string s : this->_Log){
	        			cout << s << std::endl;
	        		}
	        		this->_Log.clear();

					//freeBuffers();

		        	DaqStageAlazar::stop();

					PipelineStage::postStage();
		        }
			};
		}
	}
}

#endif /* DAQSTAGEALAZAR_HPP_ */
