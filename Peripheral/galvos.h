#ifndef GALVOS_H
#define GALVOS_H

#include "peripheral.h"

namespace OSIP{
    namespace Peripherals{
        class Galvos : public Peripheral
        {
        public:
            Galvos();

            void run() override;

            void stop() override;

            void pause() override;

            void reset() override;
        };
    }
}

#endif // GALVOS_H
