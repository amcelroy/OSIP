#include "nidaqmx.h"

using namespace OSIP;
using namespace Peripherals;

NIDAQmx::NIDAQmx(std::string device_channel)
{
    m_Channel = device_channel;
}

void NIDAQmx::init(){
    DAQmxBaseCreateTask("Galvo", &m_Handle);
}

void NIDAQmx::run(){

}

void NIDAQmx::pause(){

}

void NIDAQmx::reset(){

}

void NIDAQmx::stop(){

}
