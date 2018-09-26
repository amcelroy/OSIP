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

		        void stop(){

		        }

		        void reset(){

		        }

		        void acquire(){

		        }

		        bool init(){

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
