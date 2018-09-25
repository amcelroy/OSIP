#include "nidaqmxgalvos.h"

using namespace OSIP;
using namespace Peripherals;

niDAQMXGalvos::niDAQMXGalvos(std::string device, unsigned long long SamplesPerChannel)
{
    m_Device = device;
    m_SamplesPerChannel = SamplesPerChannel;
}

niDAQMXGalvos::~niDAQMXGalvos(){
    DAQmxBaseClearTask(m_Handle);
}

void niDAQMXGalvos::setSampClkTiming(string channel){
    m_ExternalSamplClk = channel;
}

void niDAQMXGalvos::init(){
    DAQmxBaseCreateTask("Galvo", &m_Handle);
    DAQmxBaseCfgSampClkTiming(m_Handle,
                              (m_Device + "/" + m_ExternalSamplClk).c_str(),
                              200000.0,
                              DAQmx_Val_Falling,
                              DAQmx_Val_FiniteSamps,
                              m_SamplesPerChannel);

    DAQmxBaseCreateAOVoltageChan(m_Handle,
                                 (m_Device + "/" + "ao0:1").c_str(),
                                 "Analog Output channel 0 and 1",
                                 -10.0,
                                 10.0,
                                 DAQmx_Val_Volts,
                                 nullptr);


}

void niDAQMXGalvos::pause(){
    sig_Paused();
}

void niDAQMXGalvos::run(){
    sig_Run();
}

void niDAQMXGalvos::stop(){
    sig_Stop();
}

void niDAQMXGalvos::reset(){
    sig_Reset();
}
