#ifndef PIPELINE_H
#define PIPELINE_H

#include "pipeline_global.h"
#include "inlet.h"
#include "outlet.h"
#include <vector>
#include <complex>
#include <thread>
#include <string>
#include <chrono>
#include <thread>

using namespace std;

template<class I, class O>
class PipelineStage
{

public:
    PipelineStage();

    shared_ptr<Inlet<I>> getInlet() { return _Inlet; }

    void connect(shared_ptr<Inlet<O>> inlet);

    void start();

    void stop() { stopThread = true; }

    void flushInlet();

    void flushOutlet();

    void pipelineSleep(int milli);

    void log(string msg) { _Log.push_back(msg); }

    vector<string> getLog() { return _Log; }

    int emptyLog() {
        int itemsDeleted = _Log.size();
        _Log.clear();
        return itemsDeleted;
    }
protected:
    virtual void preStage();

    virtual void postStage();

    virtual void stageThread();

    /**
     * @brief _Inlet Allocated during object instantiation
     */
    shared_ptr<Inlet<I>> _Inlet;

    /**
     * @brief _Outlets Vector of Outlets that
     */
    vector<shared_ptr<Inlet<O>>> _Outlets;

    /**
     * @brief stopThread Indicates that the thread should stop
     */
    bool stopThread = false;

    /**
     * @brief _Messages Place to store messages for logging and monitoring the PipelineStage
     */
    vector<string> _Log;

    void sendPayload(Payload<O> payload);

    Payload<I> fetchPayload();

    thread _StageThread;
};

#endif // PIPELINE_H
