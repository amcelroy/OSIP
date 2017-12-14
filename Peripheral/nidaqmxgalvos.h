#ifndef NIDAQMXGALVOS_H
#define NIDAQMXGALVOS_H

#include "peripheral.h"
#include "nidaqmx.h"
#include "galvos.h"

using namespace std;

namespace OSIP{
    namespace Peripherals{
        class niDAQMXGalvos : public NIDAQmx, public Galvos
        {

        public:
            niDAQMXGalvos(string channel, unsigned long long SamplesPerChannel);

            void run() override;

            void pause() override;

            void reset() override;

            void stop() override;
        };
    }
}

#endif // NIDAQMXGALVOS_H
