#ifndef DAQSTAGE_H
#define DAQSTAGE_H

#include "pipelinestage.hpp"
#include <boost/signals2.hpp>

namespace OSIP {
    template<class O>
    class DAQStage : public PipelineStage<O, O>
    {
    protected:
        /**
         * @brief m_Loop Signals whether the DAQ stage should run once or in a loop.
         *              mostly useful for "Live Mode" or loading from disk in a loop.
         */
        bool m_Loop = false;

        /**
         * @brief CurrentFrame Signal that emits the current frame
         */
        boost::signals2::signal<void (int)> sig_CurrentFrame;

        /**
         * @brief DAQFinished Signal that emits when the DAQ is finished
         */
        boost::signals2::signal<void ()> sig_DAQFinished;

    public:
        DAQStage();

        /**
         * @brief setLoop Sets DAQ to loop acquistion
         * @param TrueOrFalse
         */
        void setLoop(bool TrueOrFalse) { m_Loop = TrueOrFalse; }

        /**
         * @brief isLooping Returns if the DAQ is looping acquistion
         * @return
         */
        bool isLooping() { return m_Loop; }

        /**
         * @brief notifyTiming Add a subscriber to get notified when the DAQStage changes frame
         * virtual function
         * @param subscriber
         */
        void subscribeCurrentFrame(const boost::signals2::signal<void(int)>::slot_type &subscriber) { sig_CurrentFrame.connect(subscriber); }

        /**
         * @brief notifyTiming Add a subscriber to get notified when the DAQStage task is finished
         * virtual function
         * @param subscriber
         */
        void subscribeDAQFinished(const boost::signals2::signal<void()>::slot_type &subscriber) { sig_DAQFinished.connect(subscriber); }

    };
}
#endif // DAQSTAGE_H
