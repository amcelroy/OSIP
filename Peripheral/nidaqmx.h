#ifndef NIDAQMX_H
#define NIDAQMX_H

#include <NIDAQmxBase.h>
#include "peripheral.h"

namespace OSIP {
    class NIDAQmx : public Peripheral
    {
    public:
        NIDAQmx(std::string dev_channel);

        virtual void run();

        virtual void pause();

        virtual void reset();

        virtual void stop();

        virtual void init();

    private:

        TaskHandle m_Handle;

        std::string m_Channel;

    };
}

#endif // NIDAQMX_H
