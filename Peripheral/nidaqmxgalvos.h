#ifndef NIDAQMXGALVOS_H
#define NIDAQMXGALVOS_H

#include "peripheral.h"
#include "nidaqmx.h"
#include "galvos.h"

using namespace std;

namespace OSIP{
    namespace Peripherals{
        class niDAQMXGalvos : public NIDAQmx
        {

        public:
            niDAQMXGalvos(const string &channel);

            void run() override;

            void init() override;

            void pause() override;

            void reset() override;

            void stop() override;
        };
    }
}

#endif // NIDAQMXGALVOS_H
