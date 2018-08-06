#ifndef DAQSTAGE_H
#define DAQSTAGE_H

#include "pipelinestage.hpp"
#include <boost/signals2.hpp>

namespace OSIP {
    enum OSIP_CLOCK_SOURCE { OSIP_INTERNAL_CLOCK, OSIP_EXTERNAL_CLOCK };

    enum OSIP_TRIGGER_SOURCE { OSIP_INTERNAL_TRIGGER, OSIP_EXTERNAL_TRIGGER };

    enum OSIP_CHANNELS { A, B, C, D, ALL };

    struct DAQParameters{
        OSIP_CLOCK_SOURCE ClockSource;
        float VoltageGain;
        float VoltageBias;
        unsigned int PointsPerTrigger;
        unsigned int TriggersPerBuffer;
        unsigned int TotalBuffers;
        unsigned int DAQClockRate;
        unsigned int Bits;
        OSIP_TRIGGER_SOURCE Trigger;
        OSIP_CHANNELS Channels;
        double TriggerTimeoutMS;

        DAQParameters() : VoltageGain(4.0f/65535.0f),
                            VoltageBias(2.0f),
                            PointsPerTrigger(1024),
                            TriggersPerBuffer(256),
                            TotalBuffers(256),
                            DAQClockRate(100e6),
                            Bits(16),
                            Trigger(OSIP_INTERNAL_TRIGGER),
                            Channels(OSIP_CHANNELS::A),
                            TriggerTimeoutMS(1.0){ }
    };

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

        /**
         * @brief sig_DAQChanged Signal emitted when the DAQ values change
         */
        boost::signals2::signal<DAQParameters ()> sig_DAQChanged;

        /**
         * @brief _DAQParameters Place to set DAQ Parameters
         */
        DAQParameters _DAQParameters;

        virtual void configureDAQ(DAQParameters daqp) = 0;
    public:
        DAQStage() { }

        void updateDAQ(DAQParameters daqp){
            _DAQParameters = daqp;
            configureDAQ(_DAQParameters);
            sig_DAQChanged();
        }

        virtual void stop() = 0;

        virtual void reset() = 0;

        virtual void acquire() = 0;

        virtual bool init() = 0;

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

        /**
         * @brief subscribeDAQChanged Add subscriber when the DAQ is changed
         * @param subscriber
         */
        void subscribeDAQChanged(const boost::signals2::signal<DAQParameters()>::slot_type &subscriber) { sig_DAQChanged.connect(subscriber); }
    };
}
#endif // DAQSTAGE_H
