#include "nidaqmxgalvos.h"

using namespace OSIP;
using namespace Peripherals;

niDAQMXGalvos::niDAQMXGalvos(std::string device, unsigned long long SamplesPerChannel)
{
    m_Device = device;
    m_SamplesPerChannel = SamplesPerChannel;
}

void niDAQMXGalvos::setSampClkTiming(string channel){
    m_ExternalSamplClk = channel;
}

void niDAQMXGalvos::init(){
    DAQmxCreateTask("Galvo", &m_Handle);
    DAQmxCfgSampClkTiming(m_Handle,
                              (m_Device + "/" + m_ExternalSamplClk).c_str(),
                              200000.0,
                              DAQmx_Val_Falling,
                              DAQmx_Val_FiniteSamps,
                              m_SamplesPerChannel);
}

void niDAQMXGalvos::pause(){

}

void niDAQMXGalvos::run(){

}

void niDAQMXGalvos::stop(){

}

void niDAQMXGalvos::reset(){

}
