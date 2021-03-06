#ifndef NIDAQMXGALVOS_H
#define NIDAQMXGALVOS_H

#include <NIDAQmx.h>
#include <Peripherals/galvos.hpp>

using namespace std;

namespace OSIP{
    namespace Peripherals{
        class niDAQMXGalvos : public Galvos
        {

        public:
			niDAQMXGalvos();

			~niDAQMXGalvos() override;

			void create(std::string device);

			void configure(const GalvoParameters &gp, const OCTConfig &o) override;

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
