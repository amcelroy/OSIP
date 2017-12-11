#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "pipeline_global.h"
#include "boost/signals2.hpp"

namespace OSIP {
    class PIPELINESHARED_EXPORT Peripheral
    {

    public:
        Peripheral();

        virtual void run();

        virtual void pause();

        virtual void reset();

        virtual void stop();

        virtual void init();

        /**
         * @brief subscribePaused Sends signal when peripheral is paused
         * @param subscriber
         */
        void subscribePaused(const boost::signals2::signal<void ()>::slot_type &subscriber) { sig_Paused.connect(subscriber); }

        /**
         * @brief subscribeRun Sends signal when peripheral is run
         * @param subscriber
         */
        void subscribeRun(const boost::signals2::signal<void ()>::slot_type &subscriber) { sig_Run.connect(subscriber); }

        /**
         * @brief subscribePaused Sends signal when peripheral is reset
         * @param subscriber
         */
        void subscribeReset(const boost::signals2::signal<void ()>::slot_type &subscriber) { sig_Reset.connect(subscriber); }

        /**
         * @brief subscribePaused Sends signal when peripheral is stopped
         * @param subscriber
         */
        void subscribeStop(const boost::signals2::signal<void ()>::slot_type &subscriber) { sig_Stop.connect(subscriber); }

        /**
         * @brief subscribeInit Sends signal when peripheral is initialized
         * @param subscriber
         */
        void subscribeInit(const boost::signals2::signal<void ()>::slot_type &subscriber) { sig_Init.connect(subscriber); }

    protected:

        boost::signals2::signal<void ()> sig_Paused;

        boost::signals2::signal<void ()> sig_Run;

        boost::signals2::signal<void ()> sig_Reset;

        boost::signals2::signal<void ()> sig_Stop;

        boost::signals2::signal<void ()> sig_Init;
    };
}

#endif // PERIPHERAL_H
