#ifndef OUTLET_IMPL_H
#define OUTLET_IMPL_H

#include "outlet.h"

template<class O>
Outlet<O>::Outlet()
{

}

template<class O>
void Outlet<O>::writeData(Payload<O> dataIn)
{
    lock_guard<mutex> lock(_QueueLock);
    this->_OutQueue.push(dataIn);
}

template<class O>
bool Outlet<O>::readData(Payload<O> *dataOut)
{
    lock_guard<mutex> lock(_QueueLock);
    if(!_OutQueue.empty()){
        dataOut = &_OutQueue.front();
        _OutQueue.pop();
        return true;
    }else{
        return false;
    }
}

#endif // OUTLET_IMPL_H
