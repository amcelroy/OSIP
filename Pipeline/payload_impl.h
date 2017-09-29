#ifndef PAYLOAD_IMPL_H
#define PAYLOAD_IMPL_H

#include "payload.h"

using namespace OSIP;

template <class I>
Payload<I>::Payload(vector<unsigned int> dim, I *data)
{
    addData(dim, data);
}

#endif // PAYLOAD_IMPL_H
