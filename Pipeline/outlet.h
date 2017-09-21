#ifndef OUTLET_H
#define OUTLET_H

#include "pipeline_global.h"
#include <mutex>
#include <vector>
#include "payload.h"
#include <queue>
#include <complex>

template<class O>
class Outlet
{
private:
    /**
     * @brief _InQueue Input queue to write / read data from
     */
    queue<Payload<O>> _OutQueue;

    /**
     * @brief _QueueLock Locks the queue when reading or writing data
     */
    mutex _QueueLock;

public:
    Outlet();

    /**
     * @brief writeData Write data to the Inlet queue
     * @param data Data to be written
     * @return True if data is successfully written
     */
    void writeData(Payload<O> data);

    /**
     * @brief readData Read data from the Inlet queue
     * @param data Data to be read, allocated by the caller
     * @return True if the data is successfully read
     */
    bool readData(Payload<O> *data);
};

#endif // OUTLET_H
