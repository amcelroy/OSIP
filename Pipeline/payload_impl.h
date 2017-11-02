#ifndef PAYLOAD_IMPL_H
#define PAYLOAD_IMPL_H

#include "payload.h"

using namespace OSIP;

template <class I>
Payload<I>::Payload(vector<unsigned long> dim, shared_ptr<vector<I>> data, string label)
{
    addData(dim, data, label);
}

#endif // PAYLOAD_IMPL_H
