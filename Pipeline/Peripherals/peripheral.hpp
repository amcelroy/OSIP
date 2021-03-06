#ifndef PERIPHERAL_H
#define PERIPHERAL_H

#include "boost/signals2.hpp"

//#pragma once
//
//// Define EXPORTED for any platform
//#ifdef _WIN32
//# ifdef WIN_EXPORT
//#   define EXPORTED  __declspec( dllexport )
//# else
//#   define EXPORTED  __declspec( dllimport )
//# endif
//#else
//# define EXPORTED
//#endif

namespace OSIP {
    namespace Peripherals{
        class Peripheral
        {

        public:
            Peripheral(){ }

            virtual ~Peripheral() { }

            virtual void run(){ sig_Run(); }

            virtual void pause() { sig_Paused(); }

            virtual void reset() { sig_Reset(); }

            virtual void stop() { sig_Stop(); }

            virtual void init() { sig_Init(); }

            /**
             * @brief subscribePaused Sends signal when peripheral is paused
             * @param subscriber
             */
            void subscribePaused(const boost::signals2::signal<void ()>::slot_type &subscriber) { sig_Paused.connect(subscriber); }

            /**
             * @brief unsubscribePaused Disconnect all slots from Paused Event
             */
            void unsubscribePaused(){
                sig_Paused.disconnect_all_slots();
            }

            /**
             * @brief subscribeRun Sends signal when peripheral is run
             * @param subscriber
             */
            void subscribeRun(const boost::signals2::signal<void ()>::slot_type &subscriber) { sig_Run.connect(subscriber); }

            /**
             * @brief unsubscribePaused Disconnect all slots from Run Event
             */
            void unsubscribeRun(){
                sig_Run.disconnect_all_slots();
            }

            /**
             * @brief subscribePaused Sends signal when peripheral is reset
             * @param subscriber
             */
            void subscribeReset(const boost::signals2::signal<void ()>::slot_type &subscriber) { sig_Reset.connect(subscriber); }

            /**
             * @brief unsubscribePaused Disconnect all slots from Reset Event
             */
            void unsubscribeReset(){
                sig_Reset.disconnect_all_slots();
            }

            /**
             * @brief subscribePaused Sends signal when peripheral is stopped
             * @param subscriber
             */
            void subscribeStop(const boost::signals2::signal<void ()>::slot_type &subscriber) { sig_Stop.connect(subscriber); }

            /**
             * @brief unsubscribePaused Disconnect all slots from Stop Event
             */
            void unsubscribeStop(){
                sig_Stop.disconnect_all_slots();
            }

            /**
             * @brief subscribeInit Sends signal when peripheral is initialized
             * @param subscriber
             */
            void subscribeInit(const boost::signals2::signal<void ()>::slot_type &subscriber) { sig_Init.connect(subscriber); }

            /**
             * @brief unsubscribePaused Disconnect all slots from Init Event
             */
            void unsubscribeInit(){
                sig_Init.disconnect_all_slots();
            }

            /**
             * @brief unsubscribeAll Unsubscribe all slots
             */
            void unsubscribeAll(){
                sig_Paused.disconnect_all_slots();
                sig_Init.disconnect_all_slots();
                sig_Run.disconnect_all_slots();
                sig_Reset.disconnect_all_slots();
                sig_Stop.disconnect_all_slots();
            }

        protected:

            boost::signals2::signal<void ()> sig_Paused;

            boost::signals2::signal<void ()> sig_Run;

            boost::signals2::signal<void ()> sig_Reset;

            boost::signals2::signal<void ()> sig_Stop;

            boost::signals2::signal<void ()> sig_Init;
        };
    }
}

#endif // PERIPHERAL_H
