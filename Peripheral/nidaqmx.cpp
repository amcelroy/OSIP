#include "nidaqmx.h"

using namespace OSIP;
using namespace Peripherals;

NIDAQmx::NIDAQmx(std::string device, unsigned long long SamplesPerChannel)
{
    m_Device = device;
    m_SamplesPerChannel = SamplesPerChannel;
}

void NIDAQmx::setSampClkTiming(string channel){
    m_ExternalSamplClk = channel;
}

void NIDAQmx::init(){
    DAQmxBaseCreateTask("Galvo", &m_Handle);
    DAQmxBaseCfgSampClkTiming(m_Handle,
                              (m_Device + "/" + m_ExternalSamplClk).c_str(),
                              200000.0,
                              DAQmx_Val_Falling,
                              DAQmx_Val_FiniteSamps,
                              m_SamplesPerChannel);
}

void NIDAQmx::run(){

}

void NIDAQmx::pause(){

}

void NIDAQmx::reset(){

}

void NIDAQmx::stop(){

}
