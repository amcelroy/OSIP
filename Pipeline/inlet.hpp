#ifndef INLET_H
#define INLET_H


#include <vector>
#include <queue>
#include "payload.hpp"
#include <mutex>
#include <complex>
#include "boost/signals2.hpp"

using namespace std;

namespace OSIP {
    template<class I>
    class Inlet
    {
    private:
        boost::signals2::signal<void(int)> sig_ElementsInQueue;

        /**
         * @brief _InQueue Input queue to write / read data from
         */
        queue<Payload<I>> _InQueue;

        /**
         * @brief _QueueLock Locks the queue when reading or writing data
         */
        mutex _QueueLock;
    public:
        Inlet(){ }

        int getItemsInInlet() { return _InQueue.size(); }

        /**
         * @brief writeData Write data to the Inlet queue
         * @param data Data to be written
         * @return True if data is successfully written
         */
        void writeData(Payload<I> data){
            lock_guard<mutex> lock(_QueueLock);
            this->_InQueue.push(data);
        }

        /**
         * @brief readData Read data from the Inlet queue
         * @param data Data to be read, allocated by the caller
         * @return True if the data is successfully read
         */
        Payload<I> readData(){
            lock_guard<mutex> lock(_QueueLock);
            if(!_InQueue.empty()){
                Payload<I> dataOut = _InQueue.front();
                _InQueue.pop();
                return dataOut;
            }

            return Payload<I>();
        }

        /**
         * @brief notifyQueueSize Adds a subscriber that gets notified about the size of elements in the Inlet queue
         * @param sub Subscriber
         */
        void subscribeQueueSize(const boost::signals2::signal<void(int)> &sub) { sig_ElementsInQueue.connect(sub); }
    };
}

#endif // INLET_H
