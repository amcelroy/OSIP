#ifndef GALVOS_HPP
#define GALVOS_HPP

#include <Peripherals/peripheral.hpp>
#include <octconfigfile.h>
#include <vector>

using namespace OSIP;
using namespace Peripherals;
using namespace std;

namespace OSIP{
    namespace Peripherals{
        class Galvos : public Peripheral
        {
		
		public:
			struct GalvoParameters {
				double FastAxisAmplitude;
				double FastAxisOffset;
				double SlowAxisAmplitude;
				double SlowAxisOffset;

				GalvoParameters() :
					FastAxisAmplitude(5),
					FastAxisOffset(-2.5),
					SlowAxisAmplitude(5),
					SlowAxisOffset(-2.5)
				{}		
			};

        private:
            vector<double> m_FastAxisVoltages;

            vector<double> m_SlowAxisVoltages;

			GalvoParameters m_GalvoParameters;

        public:
			Galvos() { }

			~Galvos() { }

			GalvoParameters getGalvoParameters() { return m_GalvoParameters; }

			virtual void configure(const GalvoParameters &gp, const OCTConfig &o) = 0;

			vector<double> getFastAxisVoltage() { return m_FastAxisVoltages; }

			vector<double> getSlowAxisVoltage() { return m_SlowAxisVoltages; }

			void generateRaster(const GalvoParameters &gp, const OCTConfig &o) {
				m_GalvoParameters = gp;

				this->generateRaster(gp.SlowAxisAmplitude,
					gp.SlowAxisOffset,
					o.TotalBScans,
					gp.FastAxisAmplitude,
					gp.FastAxisOffset,
					o.AScansPerBScan,
					1,
					o.StartTrim,
					o.StopTrim);
			}

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

				m_SlowAxisVoltages.resize(totalPoints);
				m_FastAxisVoltages.resize(totalPoints);

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
								m_FastAxisVoltages[(i + k) * FastAxisPoints + j] = (kk) * slope * .9f + FO;
                            }
                            else if(0 <= kk && kk < static_cast<long>(FastAxisPoints))
                            {
								m_FastAxisVoltages[(i + k) * FastAxisPoints + j] = (kk) * slope + FO;
                                ascan_offset = m_FastAxisVoltages[(i + k) * FastAxisPoints + j];
                            }
                            else
                            {
                                float tmp = slope * .9f;
								m_FastAxisVoltages[(i + k) * FastAxisPoints + j] = m_FastAxisVoltages[(i + k) * FastAxisPoints + j - 1] + tmp;
                            }
							m_SlowAxisVoltages[(i + k) * FastAxisPoints + j] = ((float)(i / SlowAxisRepitions) / SlowAxisPoints) * SA + SO;
                        }
                    }
                }
            }
        };
    }
}

#endif // GALVOS_HPP
