#ifndef INLET_IMPL_HPP
#define INLET_IMPL_HPP

#include "inlet.h"

using namespace OSIP;

template<class I>
Inlet<I>::Inlet()
{

}

template<class I>
void Inlet<I>::writeData(Payload<I> payloadIn)
{
    lock_guard<mutex> lock(_QueueLock);
    this->_InQueue.push(payloadIn);
}

template<class I>
Payload<I> Inlet<I>::readData()
{
    lock_guard<mutex> lock(_QueueLock);
    if(!_InQueue.empty()){
        Payload<I> dataOut = _InQueue.front();
        _InQueue.pop();
        return dataOut;
    }

    return Payload<I>();
}

#endif // INLET_IMPL_HPP
