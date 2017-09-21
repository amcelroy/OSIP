#ifndef INLET_H
#define INLET_H

#include "pipeline_global.h"
#include <vector>
#include <queue>
#include "payload.h"
#include <mutex>
#include <complex>

using namespace std;

template<class I>
class Inlet
{
private:
    /**
     * @brief _InQueue Input queue to write / read data from
     */
    queue<Payload<I>> _InQueue;

    /**
     * @brief _QueueLock Locks the queue when reading or writing data
     */
    mutex _QueueLock;
public:
    Inlet();

    /**
     * @brief writeData Write data to the Inlet queue
     * @param data Data to be written
     * @return True if data is successfully written
     */
    void writeData(Payload<I> data);

    /**
     * @brief readData Read data from the Inlet queue
     * @param data Data to be read, allocated by the caller
     * @return True if the data is successfully read
     */
    Payload<I> readData();

};

#endif // INLET_H
