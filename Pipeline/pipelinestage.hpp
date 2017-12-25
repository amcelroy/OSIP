#ifndef PIPELINESTAGE_H
#define PIPELINESTAGE_H

#include "inlet.hpp"
#include <vector>
#include <complex>
#include <thread>
#include <string>
#include <chrono>
#include <thread>
#include "boost/signals2.hpp"
#include <assert.h>
#include <memory>
#include <chrono>

using namespace std;

namespace OSIP {
    template<class I, class O>
    class PipelineStage
    {
    protected:
        boost::signals2::signal<void()> sig_StageFinished;
        boost::signals2::signal<void()> sig_StageStarted;
        boost::signals2::signal<void (float)> sig_StageTimer;
        boost::signals2::signal<void (string)> sig_MessageLogged;

    public:
        PipelineStage(){
            _Inlet = shared_ptr<Inlet<I>>(new Inlet<I>);
        }

        shared_ptr<Inlet<I>> getInlet() { return _Inlet; }

        void connect(shared_ptr<Inlet<O>> inlet){
            _Outlets.push_back(inlet);
        }

        void start() {
            sig_StageFinished();

            stopThread = false;

            preStage();

            _StageThread = thread(&PipelineStage<I,O>::workStage, this);
            _StageThread.detach();
        }

        void stop() { stopThread = true; }

        void flushInlet() {
            //TODO
        }

        void flushOutlet() {
            //TODO
        }

        void pause() { pauseThread = true; }

        void pipelineSleep(int milli){
            std::this_thread::sleep_for(std::chrono::milliseconds(milli));
        }

        void log(string msg){
            _Log.push_back(msg);
            sig_MessageLogged(msg);
        }

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

        virtual boost::signals2::signal<void ()>::slot_type slotDAQFinished(){
            m_DAQFinished = true;
            return boost::signals2::signal<void ()>();
        }

        virtual boost::signals2::signal<void ()>::slot_type slotSavingFinished(){
            m_SavingFinished = true;
            return boost::signals2::signal<void ()>();
        }

        virtual boost::signals2::signal<void ()>::slot_type slotProcessingFinished(){
            m_ProcessingFinished = true;
            return boost::signals2::signal<void ()>();
        }

    protected:
        virtual void preStage(){
            sig_StageStarted();
        }

        virtual void postStage(){
            sig_StageFinished();
        }

        virtual void work(){

        }

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

        void sendPayload(Payload<O> payload){
            if(_Outlets.size() != 0){
                for(shared_ptr<Inlet<O>> o : _Outlets){
                    o.get()->writeData(payload);
                }
            }
        }

        Payload<I> fetchPayload(){
            return _Inlet.get()->readData();
        }

        bool m_DAQFinished = false;

        bool m_ProcessingFinished = false;

        bool m_SavingFinished = false;

        std::thread _StageThread;
    };
}

#endif // PIPELINESTAGE_H
