/*
 * DaqStageAlazar.h
 *
 *  Created on: Sep 26, 2018
 *      Author: amcelroy
 */

#ifndef DAQSTAGEALAZAR660_HPP_
#define DAQSTAGEALAZAR660_HPP_

#include "daqstagealazar.hpp"
#include <AlazarError.h>
#include <AlazarApi.h>
#include <AlazarCmd.h>
#include <payload.hpp>

namespace OSIP{
	namespace DAQ{
		namespace Alazar{
			using namespace OSIP;
			using namespace DAQ;
			using namespace Alazar;
			using namespace std;

			class DaqStageAlazar660 : public DaqStageAlazar {

			public:
				DaqStageAlazar660(){

				}

				~DaqStageAlazar660() override {

				}

		        void stop(){

		        }

		        void reset(){

		        }

		        void acquire(){

		        }

		        bool init(){

		        }


			protected:
		        void configureDAQ(const DAQParameters& daqp) override {
		        	float requestedVoltage = daqp.Voltage;
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
		        	switch(daqp.Coupling){
		        	case OSIP_COUPLING::OSIP_AC:
		        		coupling = AC_COUPLING;
		        		break;

		        	case OSIP_COUPLING::OSIP_DC:
		        	default:
		        		coupling = DC_COUPLING;
		        		break;
		        	}

		        	U32 impedance;
		        	switch(daqp.Impedance){
		        	case OSIP_50:
		        	default:
		        		impedance = IMPEDANCE_50_OHM;
		        		break;

		        	case OSIP_1M:
		        		impedance = IMPEDANCE_1M_OHM;
		        		break;
		        	}

		        	switch(daqp.ClockSource){
		        	case OSIP_EXTERNAL_CLOCK:
		        		m_Error = AlazarSetCaptureClock(m_BoardHandle, EXTERNAL_CLOCK_AC, SAMPLE_RATE_USER_DEF, CLOCK_EDGE_RISING, 0);
		        		break;

		        	case OSIP_INTERNAL_CLOCK:
		        	default:
		        		m_Error = AlazarSetCaptureClock(m_BoardHandle, INTERNAL_CLOCK, SAMPLE_RATE_125MSPS, CLOCK_EDGE_RISING, 0);
		        		break;

		        		log("Alazar set clock return code: " + to_string(m_Error));
		        	}

		        	U8 channel;
		        	switch(daqp.Channels){
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

	        		switch(daqp.Trigger){
	        		case OSIP_INTERNAL_TRIGGER:
	        			trigger = TRIG_CHAN_A;
	        			break;
	        		default:
	        		case OSIP_EXTERNAL_TRIGGER:
	        			trigger = TRIG_EXTERNAL;
	        			break;
	        		}

	        		switch(daqp.TriggerSlope){
	        		case OSIP_RISING:
	        			triggerSlope = TRIGGER_SLOPE_POSITIVE;
	        			break;
	        		default:
	        		case OSIP_FALLING:
	        			triggerSlope = TRIGGER_SLOPE_NEGATIVE;
	        			break;
	        		}

	        		float triggerLevel = (daqp.TriggerVoltage / requestedVoltage)*127 + 128;
	        		U8 triggerLevel_u8 = static_cast<U8>(triggerLevel);

	        		m_Error = AlazarSetTriggerOperation(m_BoardHandle,
	        				TRIG_ENGINE_OP_J,
							TRIG_ENGINE_J,
							trigger,
							triggerSlope,
							triggerLevel_u8,
							TRIG_ENGINE_K,
							TRIG_DISABLE,
							triggerSlope,
							128);
	        		log("Alazar Set Trigger Operation return code: " + to_string(m_Error));

	        		U32 timeout_ticks = (U32)(daqp.TriggerTimeoutSec / 10.e-6 + 0.5);
	        		m_Error = AlazarSetTriggerTimeOut(m_BoardHandle, timeout_ticks);
	        		log("Alazar Set Trigger Timeout return code: " + to_string(m_Error));

	        		U32 flags = ADMA_ALLOC_BUFFERS;
	        		U32 TrigPerAcq = 1;
	        		if(daqp.TotalBuffers > 1){
	        			TrigPerAcq = daqp.TotalBuffers*daqp.TriggersPerBuffer;
	        		}

	        		m_Error = AlazarBeforeAsyncRead(m_BoardHandle,
	        				channel,
							0,
							daqp.PointsPerTrigger,
							daqp.TriggersPerBuffer,
							TrigPerAcq,
							flags);
	        		log("Alazar Before Aysnc Read return code: " + to_string(m_Error));
		        }

		        void work() override {
		        	m_Error = AlazarStartCapture(m_BoardHandle);
		        	log("Alazar Start Capture return code: " + to_string(m_Error));

		        	unsigned long bufferSize = _DAQParameters.PointsPerTrigger*_DAQParameters.TriggersPerBuffer;

		        	if(m_Error == ApiSuccess){
		        		unsigned long _currentFrame = 0;
		        		while(m_Error == ApiSuccess && stopThread == false){
		        			if(pauseThread == true){
		        				this->pipelineSleep(50);
		        			}else{
								shared_ptr<vector<unsigned short>> buffer(new vector<unsigned short>(bufferSize));
								m_Error = AlazarWaitNextAsyncBufferComplete(m_BoardHandle,
										buffer->data(),
										bufferSize*sizeof(unsigned short),
										5000);

								Payload<unsigned short> p;
								vector<unsigned long long> dim;
								dim.push_back(_DAQParameters.PointsPerTrigger);
								dim.push_back(_DAQParameters.TriggersPerBuffer);
								dim.push_back(_currentFrame);

								p.addData(dim, buffer, "ATS660");

								_currentFrame += 1;
		        			}
		        		}
		        	}

		        	m_Error = AlazarAbortAsyncRead(m_BoardHandle);
		        	log("Alazar Abort Async return code: " + to_string(m_Error));
		        }
			};
		}
	}
}

#endif /* DAQSTAGEALAZAR_HPP_ */
