#ifndef GALVOS_H
#define GALVOS_H

<<<<<<< Updated upstream
#include "peripheral.hpp"
=======
#include "peripheral_global.h"
>>>>>>> Stashed changes

using namespace std;

namespace OSIP{
    namespace Peripherals{
        class Galvos
        {
        private:
            vector<double> m_FastAxisVoltages;

            vector<double> m_SlowAxisVoltages;

        public:
            Galvos();

            void generateRaster(double SlowAxisAmplitude,
                                double SlowAxisOffset,
                                int SlowAxisPoints,
                                double FastAxisAmplitude,
                                double FastAxisOffset,
                                int FastAxisPoints);
        };
    }
}

#endif // GALVOS_H
