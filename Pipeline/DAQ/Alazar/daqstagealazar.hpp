/*
 * DaqStageAlazar.h
 *
 *  Created on: Sep 26, 2018
 *      Author: amcelroy
 */

#ifndef DAQSTAGEALAZAR_HPP_
#define DAQSTAGEALAZAR_HPP_

#include <daqstage.hpp>
#include <AlazarError.h>
#include <AlazarApi.h>
#include <AlazarCmd.h>

namespace OSIP{
	namespace DAQ{
		namespace Alazar{
			using namespace OSIP;
			using namespace DAQ;
			using namespace Alazar;

			class DaqStageAlazar : public DAQStage<unsigned short> {
			private:

				unsigned long _N;

				vector<unsigned short*> m_Buffers;

				unsigned long m_CurrentBuffer = 0;

				unsigned long m_BufferSizeBytes;

			public:
				DaqStageAlazar(){
					m_SystemCount = AlazarNumOfSystems();
					for(unsigned int i = 1; i <= m_SystemCount; i++){
						m_BoardCount = AlazarBoardsInSystemBySystemID(i);
						for(unsigned int j = 1; j <= m_BoardCount; j++){
							m_SelectedBoard = j;
						}
						m_SelectedSystem = i;
					}

					m_BoardHandle = AlazarGetBoardBySystemID(m_SelectedSystem, m_SelectedBoard);
					AlazarSetLED(m_BoardHandle, LED_ON);
				}

				~DaqStageAlazar() override {
					AlazarSetLED(m_BoardHandle, LED_OFF);
					AlazarClose(m_BoardHandle);
				}

				string typeName() override {
					return "DaqStageAlazar";
				}

		        void stop(){
					sig_DAQFinished();
		        	if(m_DAQFinished == false){
						freeBuffers();
		        	}
		        }

		        void reset(){

		        }

		        void acquire(){

		        }

		        bool init(){
					return true;
		        }

		        void createBuffers(int N, unsigned long size){
		        	m_Buffers.clear();
		        	m_BufferSizeBytes = size*sizeof(unsigned short);
					RETURN_CODE code;
		        	for(int i = 0; i < N; i++){
		        		m_Buffers.push_back((unsigned short*)AlazarAllocBufferU16(m_BoardHandle, size));
						//m_Buffers.push_back(new unsigned short[size]);
						code = AlazarPostAsyncBuffer(m_BoardHandle, m_Buffers[i], m_BufferSizeBytes);
						if (code != ApiSuccess) {
							this->log("Error during AlazarPostAsyncBuffer in ::daqstagealazar");
						}
		        	}

		        	m_CurrentBuffer = 0;
		        }

		        void freeBuffers(){
		        	for(unsigned short* b : m_Buffers){
		        		AlazarFreeBufferU16(m_BoardHandle, b);
		        	}
		        	m_Buffers.clear();
		        }

				RETURN_CODE readBufferRandom(vector<unsigned short> *out) {
					for (int i = 0; i < out->size(); i++) {
						out->data()[i] = rand();
					}

					return ApiSuccess;
				}

		        RETURN_CODE readBuffer(vector<unsigned short> *out){
		        	unsigned int tmp_index = m_CurrentBuffer % m_Buffers.size();

		        	RETURN_CODE code = AlazarWaitAsyncBufferComplete(m_BoardHandle, m_Buffers[tmp_index], 5000);

		        	if(code == ApiSuccess){
						if(out->size() != m_BufferSizeBytes/2){
							out->resize(m_BufferSizeBytes/2);
						}

						memcpy(out->data(), m_Buffers[tmp_index], m_BufferSizeBytes);

						code = AlazarPostAsyncBuffer(m_BoardHandle, m_Buffers[tmp_index], m_BufferSizeBytes);

						m_CurrentBuffer += 1;
		        	}else{
						this->log("Error posting AlazarPostAsyncBuffer in ::readBuffer with code: " + code);
		        		AlazarAbortAsyncRead(m_BoardHandle);
		        		AlazarAbortCapture(m_BoardHandle);
						AlazarClose(m_BoardHandle);
		        	}

		        	return code;
		        }

		        DAQParameters getDefaultDAQParameters() {
					return _DAQParameters;
				}

			protected:
				U32 m_SystemCount;
				U32 m_BoardCount;
				U32 m_SelectedBoard = 1;
				U32 m_SelectedSystem = 1;
				HANDLE m_BoardHandle;
				RETURN_CODE m_Error = RETURN_CODE::ApiSuccess;

		        void configureDAQ(const DAQParameters& daqp) override {

		        }

		        void work() override {

		        }
			};
		}
	}
}

#endif /* DAQSTAGEALAZAR_HPP_ */
