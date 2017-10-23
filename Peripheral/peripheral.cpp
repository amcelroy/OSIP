#include "peripheral.h"

using namespace OSIP;

Peripheral::Peripheral()
{
}

void Peripheral::run(){
    sig_Run();
}

void Peripheral::pause(){
    sig_Paused();
}

void Peripheral::reset(){
    sig_Reset();
}

void Peripheral::stop(){
    sig_Stop();
}
