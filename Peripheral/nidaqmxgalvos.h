#ifndef NIDAQMXGALVOS_H
#define NIDAQMXGALVOS_H

<<<<<<< Updated upstream
#include "peripheral.hpp"
#include <NIDAQmx.h>
=======
#include <peripheral.hpp>
#include <NIDAQmxBase.h>
>>>>>>> Stashed changes
#include "galvos.h"

using namespace std;

namespace OSIP{
    namespace Peripherals{
        class niDAQMXGalvos : public Peripheral, public Galvos
        {

        public:
            niDAQMXGalvos(string channel, unsigned long long SamplesPerChannel);

            void init() override;

            void run() override;

            void pause() override;

            void reset() override;

            void stop() override;

            TaskHandle getTaskHandle() { return m_Handle; }

            void setSampClkTiming(string channel);


        private:

            TaskHandle m_Handle;

            std::string m_Device;

            std::string m_ExternalSamplClk = "PFI7";

            unsigned long long m_SamplesPerChannel;
        };

    }
}

#endif // NIDAQMXGALVOS_H
