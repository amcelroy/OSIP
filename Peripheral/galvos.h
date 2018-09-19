#ifndef GALVOS_H
#define GALVOS_H

#include "peripheral.hpp"

using namespace std;

namespace OSIP{
    namespace Peripherals{
        class EXPORTED Galvos : public Peripheral
        {
        private:
            vector<double> m_FastAxisVoltages;

            vector<double> m_SlowAxisVoltages;

        public:
            Galvos();

            ~Galvos() override;

            void generateRaster(double SlowAxisAmplitude,
                                double SlowAxisOffset,
                                unsigned long SlowAxisPoints,
                                double FastAxisAmplitude,
                                double FastAxisOffset,
                                unsigned long FastAxisPoints,
                                unsigned long SlowAxisRepitions,
                                unsigned long StartTrim,
                                unsigned long StopTrim);

        };
    }
}

#endif // GALVOS_H
