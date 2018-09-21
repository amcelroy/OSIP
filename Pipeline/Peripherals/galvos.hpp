#ifndef GALVOS_HPP
#define GALVOS_HPP

#include <Peripherals/peripheral.hpp>
#include <vector>

using namespace OSIP;
using namespace Peripherals;
using namespace std;

namespace OSIP{
    namespace Peripherals{
        class Galvos : public Peripheral
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
                                unsigned long StopTrim){
                unsigned long FastAxisPointsNoTrim = FastAxisPoints - StartTrim - StopTrim;
                unsigned long totalPoints = static_cast<unsigned long>(SlowAxisPoints*FastAxisPoints*SlowAxisRepitions);

                vector<float> v_SlowAxis(totalPoints);
                vector<float> v_FastAxis(totalPoints);

                float FA = static_cast<float>(FastAxisAmplitude);
                float FO = static_cast<float>(FastAxisOffset);

                float SA = static_cast<float>(SlowAxisAmplitude);
                float SO = static_cast<float>(SlowAxisOffset);

                float slope = static_cast<float>(FastAxisAmplitude / FastAxisPoints);

                for (unsigned long i = 0; i < SlowAxisPoints*SlowAxisRepitions; i += SlowAxisRepitions)
                {
                    for (unsigned long k = 0; k < SlowAxisRepitions; k++)
                    {
                        float ascan_offset = 0.0f;
                        for (unsigned long j = 0; j < FastAxisPoints; j++)
                        {
                            long kk = j - StartTrim;
                            if (kk < 0)
                            {
                                v_FastAxis[(i + k) * FastAxisPoints + j] = (kk) * slope * .9f + FO;
                            }
                            else if(0 <= kk && kk < static_cast<long>(FastAxisPoints))
                            {
                                v_FastAxis[(i + k) * FastAxisPoints + j] = (kk) * slope + FO;
                                ascan_offset = v_FastAxis[(i + k) * FastAxisPoints + j];
                            }
                            else
                            {
                                float tmp = slope * .9f;
                                v_FastAxis[(i + k) * FastAxisPoints + j] = v_FastAxis[(i + k) * FastAxisPoints + j - 1] + tmp;
                            }
                            v_SlowAxis[(i + k) * FastAxisPoints + j] = ((float)(i / SlowAxisRepitions) / SlowAxisPoints) * SA + SO;
                        }
                    }
                }
            }
        };
    }
}

#endif // GALVOS_HPP
