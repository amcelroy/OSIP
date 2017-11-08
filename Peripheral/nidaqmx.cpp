#include "nidaqmx.h"

using namespace OSIP;

NIDAQmx::NIDAQmx(std::string device_channel)
{
    m_Channel = device_channel;
}

void NIDAQmx::init(){
    DAQmxBaseCreateTask("Galvo", &m_Handle);
    //DAQmxBaseCreateAOVoltageChan(m_Handle, m_Channel, "Galvo Voltage", -10, 10, DAQmx_Val_Voltage, NULL);
    //DAQmxBaseCfgSampClkTiming(m_Handle, "Dev1/PFI7", 500000, DAQmx_Val_Rising, DAQmx_Val_FiniteSamps, )
}

void NIDAQmx::run(){

}

void NIDAQmx::pause(){

}

void NIDAQmx::reset(){

}

void NIDAQmx::stop(){

}
