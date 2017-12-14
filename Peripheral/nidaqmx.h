#ifndef NIDAQMX_H
#define NIDAQMX_H

#include <NIDAQmxBase.h>
#include "peripheral.h"

using namespace std;

namespace OSIP{
    namespace Peripherals{
        class NIDAQmx : public Peripheral
        {
        public:
            NIDAQmx(std::string dev_channel, unsigned long long SamplesPerChannel);

            void setSampClkTiming(string channel);

            virtual void run();

            virtual void pause();

            virtual void reset();

            virtual void stop();

            virtual void init();

            TaskHandle getTaskHandle() { return m_Handle; }

        protected:

            TaskHandle m_Handle;

            std::string m_Device;

            std::string m_ExternalSamplClk = "PFI7";

            unsigned long long m_SamplesPerChannel;
        };
    }
}

#endif // NIDAQMX_H
