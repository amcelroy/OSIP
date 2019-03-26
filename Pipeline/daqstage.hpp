#ifndef DAQSTAGE_H
#define DAQSTAGE_H

#include "pipelinestage.hpp"
#include <boost/signals2.hpp>
#include <string>


using namespace std;

namespace OSIP {
    enum OSIP_CLOCK_SOURCE { OSIP_INTERNAL_CLOCK, OSIP_EXTERNAL_CLOCK };

    enum OSIP_TRIGGER_SOURCE { OSIP_INTERNAL_TRIGGER, OSIP_EXTERNAL_TRIGGER };

    enum OSIP_IMPEDANCE { OSIP_50, OSIP_1M };

    enum OSIP_COUPLING { OSIP_AC, OSIP_DC };

    enum OSIP_CHANNELS { A, B, C, D, ALL };

    enum OSIP_SLOPE { OSIP_RISING, OSIP_FALLING };

    struct DAQParameters{
        OSIP_CLOCK_SOURCE ClockSource;
        float Voltage;
        unsigned long PointsPerTrigger;
        unsigned long TriggersPerBuffer;
        unsigned long TotalBuffers;
        unsigned long DAQClockRate;
        unsigned long Bits;
        OSIP_TRIGGER_SOURCE Trigger;
        OSIP_CHANNELS Channels;
        OSIP_IMPEDANCE Impedance;
        OSIP_COUPLING Coupling;
        OSIP_SLOPE TriggerSlope;
        float TriggerVoltage;
        double TriggerTimeoutSec;
		unsigned long BScansPerTransfer;

        DAQParameters() : Voltage(4.0f),
                            PointsPerTrigger(1024),
                            TriggersPerBuffer(256),
                            TotalBuffers(256),
                            DAQClockRate(100e6),
                            Bits(16),
                            Trigger(OSIP_INTERNAL_TRIGGER),
                            Channels(OSIP_CHANNELS::A),
                            TriggerTimeoutSec(1.0),
							Impedance(OSIP_50),
							Coupling(OSIP_DC),
							TriggerSlope(OSIP_RISING),
							TriggerVoltage(0.0f),
							BScansPerTransfer(4),
							ClockSource(OSIP_INTERNAL_CLOCK){ }
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
        boost::signals2::signal<void (unsigned long)> sig_CurrentFrame;

        /**
         * @brief DAQFinished Signal that emits when the DAQ is finished
         */
        boost::signals2::signal<void ()> sig_DAQFinished;

        /**
         * @brief DAQStarted Signal that emits when the DAQ is started
         */
        boost::signals2::signal<void ()> sig_DAQStarted;

        /**
         * @brief sig_DAQChanged Signal emitted when the DAQ values change
         */
        boost::signals2::signal<DAQParameters ()> sig_DAQChanged;

        /**
         * @brief _DAQParameters Place to set DAQ Parameters
         */
        DAQParameters _DAQParameters;

        virtual void configureDAQ(const DAQParameters& daqp) = 0;
    public:
        DAQStage() { }

        void updateDAQ(const DAQParameters& daqp){
            _DAQParameters = daqp;
            configureDAQ(_DAQParameters);
            sig_DAQChanged();
        }

        DAQParameters getDAQParameters(){
        	return _DAQParameters;
        }

        virtual string typeName() = 0;

        virtual void stop() = 0;

        virtual void reset() = 0;

        virtual void acquire() = 0;

        virtual bool init() = 0;

        virtual DAQParameters getDefaultDAQParameters() = 0;

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
        void subscribeCurrentFrame(const boost::signals2::signal<void(unsigned long)>::slot_type &subscriber) { sig_CurrentFrame.connect(subscriber); }

        /**
         * @brief unsubscribeCurrentFrame Disconnect all slots from the Current Frame event
         */
        void unsubscribeCurrentFrame(){
            sig_CurrentFrame.disconnect_all_slots();
        }

        /**
         * @brief notifyTiming Add a subscriber to get notified when the DAQStage task is finished
         * virtual function
         * @param subscriber
         */
        void subscribeDAQFinished(const boost::signals2::signal<void()>::slot_type &subscriber) { sig_DAQFinished.connect(subscriber); }

        /**
         * @brief unsubscribeDAQFinished Disconnect all slots from the DAQ finished event
         */
        void unsubscribeDAQFinished(){
            sig_DAQFinished.disconnect_all_slots();
        }

        /**
         * @brief subscribeDAQChanged Add subscriber when the DAQ is changed
         * @param subscriber
         */
        void subscribeDAQChanged(const boost::signals2::signal<DAQParameters()>::slot_type &subscriber) { sig_DAQChanged.connect(subscriber); }

        /**
         * @brief unsubscribeDAQChanged Disconnect all slots from the DAQ Changed event
         */
        void unsubscribeDAQChanged(){
            sig_DAQChanged.disconnect_all_slots();
        }

        /**
         * @brief subscribeDAQStarted Add subscriber when the DAQ is started
         * @param subscriber
         */
        void subscribeDAQStarted(const boost::signals2::signal<void()>::slot_type &subscriber) { sig_DAQStarted.connect(subscriber); }

        /**
         * @brief unsubscribeDAQStarted Disconnect all slots from DAQ Started event
         */
        void unsubscribeDAQStarted(){
            sig_DAQStarted.disconnect_all_slots();
        }

        void unsubscribeAll() override {
            PipelineStage<O, O>::unsubscribeAll();
            sig_CurrentFrame.disconnect_all_slots();
            sig_DAQChanged.disconnect_all_slots();
            sig_DAQFinished.disconnect_all_slots();
            sig_DAQStarted.disconnect_all_slots();
        }
    };
}
#endif // DAQSTAGE_H
