#include "nidaqmxgalvos.h"

using namespace OSIP;
using namespace Peripherals;

niDAQMXGalvos::niDAQMXGalvos(string channel, unsigned long long SamplesPerChannel)
    : NIDAQmx(channel, SamplesPerChannel)
{

}

void niDAQMXGalvos::pause(){

}

void niDAQMXGalvos::run(){

}

void niDAQMXGalvos::stop(){

}

void niDAQMXGalvos::reset(){

}
