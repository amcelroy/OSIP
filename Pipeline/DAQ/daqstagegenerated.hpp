#ifndef DAQSTAGEGENERATED_HPP_
#define DAQSTAGEGENERATED_HPP_

#include <daqstage.hpp>
#include <pipelinestage.hpp>

using namespace OSIP;

namespace OSIP {
	namespace DAQ {
		class DAQStageGenerate : public DAQStage<unsigned short> {

		protected:
			string typeName() override {
				return "GeneratedSignal";
			}

			void stop() override {
				PipelineStage::stop();
			}

			void reset() override {
				
			}

			bool init() override {
				return true;
			}

			void acquire() override {

			}

			void configureDAQ(const DAQParameters &d) {
				_DAQParameters = d;
			}

			DAQParameters getDefaultDAQParameters() {
				DAQParameters d;
				d.Bits = 16;
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
				d.BScansPerTransfer = 1;
				d.TotalBuffers = 256;
				d.TriggersPerBuffer = 256;
				return d;
			}

			void work() override {
				unsigned long long frameCounter = 0;

				while (stopThread == false) {
					pipelineSleep(10);
					
					shared_ptr<vector<unsigned short>> buffer(new vector<unsigned short>(_DAQParameters.PointsPerTrigger*_DAQParameters.TriggersPerBuffer*_DAQParameters.BScansPerTransfer));
					for (int i = 0; i < _DAQParameters.TriggersPerBuffer*_DAQParameters.BScansPerTransfer; i++) {
						for (int j = 0; j < _DAQParameters.PointsPerTrigger; j++) {
							buffer->data()[i*_DAQParameters.PointsPerTrigger + j] = j;
						}
					}

					Payload<unsigned short> p;
					vector<unsigned long long> dim;
					dim.push_back(_DAQParameters.PointsPerTrigger);
					dim.push_back(_DAQParameters.TriggersPerBuffer);
					dim.push_back(_DAQParameters.BScansPerTransfer);
					dim.push_back(frameCounter);

					if (frameCounter > _DAQParameters.TotalBuffers - 1) {
						//Stop thread
						stopThread = true;
						this->sig_DAQFinished();
						frameCounter = 0;
					}
					else {
						p.addData(dim, buffer, "Frame");
						this->sendPayload(p);
						frameCounter += _DAQParameters.BScansPerTransfer;
					}					
				}

				PipelineStage::postStage();
			}

		};
	}
}

#endif