#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "peripheral_global.h"
#include "boost/signals2.hpp"

namespace OSIP {
    class PERIPHERALSHARED_EXPORT Peripheral
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
        void subscribePaused(boost::signals2::signal<void ()>::slot_type subscriber) { sig_Paused.connect(subscriber); }

        /**
         * @brief subscribeRun Sends signal when peripheral is run
         * @param subscriber
         */
        void subscribeRun(boost::signals2::signal<void ()>::slot_type subscriber) { sig_Run.connect(subscriber); }

        /**
         * @brief subscribePaused Sends signal when peripheral is reset
         * @param subscriber
         */
        void subscribeReset(boost::signals2::signal<void ()>::slot_type subscriber) { sig_Reset.connect(subscriber); }

        /**
         * @brief subscribePaused Sends signal when peripheral is stopped
         * @param subscriber
         */
        void subscribeStop(boost::signals2::signal<void ()>::slot_type subscriber) { sig_Stop.connect(subscriber); }

        /**
         * @brief subscribeInit Sends signal when peripheral is initialized
         * @param subscriber
         */
        void subscribeInit(boost::signals2::signal<void ()>::slot_type subscriber) { sig_Init.connect(subscriber); }

    protected:

        boost::signals2::signal<void ()> sig_Paused;

        boost::signals2::signal<void ()> sig_Run;

        boost::signals2::signal<void ()> sig_Reset;

        boost::signals2::signal<void ()> sig_Stop;

        boost::signals2::signal<void ()> sig_Init;
    };
}

#endif // PERIPHERAL_H
