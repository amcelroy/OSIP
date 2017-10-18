#ifndef PIPELINE_H
#define PIPELINE_H

#include "pipeline_global.h"
#include "inlet.h"
#include <vector>
#include <complex>
#include <thread>
#include <string>
#include <chrono>
#include <thread>
#include "boost/signals2.hpp"
#include <assert.h>
#include <memory>

using namespace std;

namespace OSIP {
    template<class I, class O>
    class PipelineStage
    {
        boost::signals2::signal<void()> sig_StageFinished;
        boost::signals2::signal<void()> sig_StageStarted;
        boost::signals2::signal<void (float)> sig_StageTimer;
        boost::signals2::signal<void (string)> sig_MessageLogged;

    public:
        PipelineStage();

        shared_ptr<Inlet<I>> getInlet() { return _Inlet; }

        void connect(shared_ptr<Inlet<O>> inlet);

        void start();

        void stop() { stopThread = true; }

        void flushInlet();

        void flushOutlet();

        void pause() { pauseThread = true; }

        void pipelineSleep(int milli);

        void log(string msg);

        vector<string> getLog() { return _Log; }

        int emptyLog() {
            int itemsDeleted = _Log.size();
            _Log.clear();
            return itemsDeleted;
        }

        /**
         * @brief notifyFinished Add a subscriber to get notified when the PipelineStage has finished
         * @param subscriber
         */
        void subscribeFinished(const boost::signals2::signal<void()>::slot_type &subscriber) { sig_StageFinished.connect(subscriber); }

        /**
         * @brief notifyStarted Add a subscriber to get notified when the PipelineStage has started
         * @param subscriber
         */
        void subscribeStarted(const boost::signals2::signal<void()>::slot_type &subscriber) { sig_StageStarted.connect(subscriber); }

        /**
         * @brief notifyTiming Add a subscriber to get notified when the PipelineStage completes, usually in the stageThread
         * virtual function
         * @param subscriber
         */
        void subscribeTiming(const boost::signals2::signal<void(float)>::slot_type &subscriber) { sig_StageTimer.connect(subscriber); }

        virtual boost::signals2::signal<void ()>::slot_type slotDAQFinished();

        virtual boost::signals2::signal<void ()>::slot_type slotSavingFinished();

        virtual boost::signals2::signal<void ()>::slot_type slotProcessingFinished();
    protected:
        virtual void preStage();

        virtual void postStage();

        virtual void workStage();

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
         * @brief pauseThread Inidicates the thread should not process Inlet data, but not quit either
         */
        bool pauseThread = false;

        /**
         * @brief _Messages Place to store messages for logging and monitoring the PipelineStage
         */
        vector<string> _Log;

        void sendPayload(Payload<O> payload);

        Payload<I> fetchPayload();

        bool m_DAQFinished = false;

        bool m_ProcessingFinished = false;

        bool m_SavingFinished = false;

        std::thread _StageThread;
    };
}

#endif // PIPELINE_H
