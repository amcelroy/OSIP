#ifndef DISPLAYPIPELINESTAGE_H
#define DISPLAYPIPELINESTAGE_H

#include "pipelinestage.hpp"
#include <vector>
#include <boost/signals2.hpp>
//#include <omp.h>

using namespace std;

namespace OSIP {
    template<class I>
    class DisplayPipelineStage : public PipelineStage<I,I>
    {

    private:

    public:
        DisplayPipelineStage(){
            m_LUT = {66049,66049,66305,66561,66817,67073,67329,67585,67841,68097,68353,68609,68865,69121,69377,69633,69889,70145,70401,70657,70913,71169,71425,71681,71937,72193,72449,72705,72961,73217,73473,73729,73985,74241,74497,74753,75009,75265,75521,75777,76033,76289,76545,76801,77057,77313,77569,77825,78081,78337,78593,78849,79105,79361,79617,79873,80129,80385,80641,80897,81153,81409,81665,81921,147458,278276,409094,539912,670730,801548,932366,1063184,1194002,1324820,1455638,1586456,1717274,1848092,1978910,2109728,2240546,2371364,2502182,2633000,2763818,2894636,3025454,3156272,3287090,3417908,3548726,3679544,3810362,3941180,4071998,4202816,4268097,4398915,4529733,4660551,4791369,4922187,5053005,5183823,5314641,5445459,5576277,5707095,5837913,5968731,6099549,6230367,6361185,6492003,6622821,6753639,6884457,7015275,7146093,7276911,7407729,7538547,7669365,7800183,7931001,8061819,8192637,8323455,8454525,8586107,8717689,8849271,8980853,9112435,9244017,9375599,9507181,9638763,9770345,9901927,10033509,10165091,10296673,10428255,10559837,10691419,10823001,10954583,11086165,11217747,11349329,11480911,11612493,11744075,11875657,12007239,12138821,12270403,12401985,12533568,12599358,12730940,12862522,12994104,13125686,13257268,13388850,13520432,13652014,13783596,13915178,14046760,14178342,14309924,14441506,14573088,14704670,14836252,14967834,15099416,15230998,15362580,15494162,15625744,15757326,15888908,16020490,16152072,16283654,16415236,16546818,16678401,16678916,16679432,16679948,16680464,16680980,16681496,16682012,16682528,16683044,16683560,16684076,16684592,16685108,16685624,16686140,16686656,16687171,16687687,16688203,16688719,16689235,16689751,16690267,16690783,16691299,16691815,16692331,16692847,16693363,16693879,16694395,16694911,16695171,16695687,16696203,16696719,16697235,16697751,16698267,16698783,16699299,16699815,16700331,16700847,16701363,16701879,16702395,16702911,16703426,16703942,16704458,16704974,16705490,16706006,16706522,16707038,16707554,16708070,16708586,16709102,16709618,16710134,16710650,16710650};
            for(int i = 0; i < m_LUT.size(); i++){
                m_LUT[i] += 0xFF000000;
            }

        }

        /**
         * @brief setFramerate Sets the display rate in FPS. Values <= 0 will run at full speed
         * @param FramesPerSecond
         */
        void setFramerate(float FramesPerSecond){
            float tmp = FramesPerSecond;
            tmp = max(0.0f, FramesPerSecond);
            m_FramesPerSecond = tmp;
        }

        /**
         * @brief getFramerate Returns the framerate in FPS.
         * @return
         */
        float getFramerate() { return m_FramesPerSecond; }

        /**
         * @brief setMin Sets min value to scale the data to
         * @param min
         */
        void setMin(I min) { minValue = min; }

        /**
         * @brief setMax Sets the max value to scale the data to
         * @param max
         */
        void setMax(I max) { maxValue = max; }

        /**
         * @brief scaleTo8Bit Scales a value to 8 Bits (0 to 255)
         * @param array
         */
        void scaleTo8Bit(const vector<I>& array, vector<unsigned char> *output){
            I range = (maxValue - minValue);
            for(unsigned long i = 0; i < array.size(); i++){
                float tmp = 255*(array.at(i) - minValue) / range;
                tmp = fmaxf(0.0, tmp);
                tmp = fminf(tmp, 255.0);
                output->data()[i] = (unsigned char)tmp;
            }
        }

        /**
         * @brief scaleTo8Bit Scales a value to 8 Bits (0 to 255), then to RGBA using the look up tables
         * @param array
         */
        void scaleToRGBA(const I& array, unsigned int* output, unsigned long size){
            float range = (maxValue - minValue);

            #pragma omp parallel for
            for(unsigned long i = 0; i < size; i++){
                float tmp = 255.0f*(array[i] - minValue) / range;

                if(tmp < 0){
                    output[i] = m_LUT[0];
                    continue;
                }

                if(tmp > 255){
                    output[i] = m_LUT[255];
                    continue;
                }

                output[i] = m_LUT[(int)tmp];
            }
        }


        /**
         * @brief notifyTiming Add a subscriber for when a frame is ready
         * virtual function
         * @param subscriber
         */
        void subscribeFrameReady(const boost::signals2::signal<void()>::slot_type &subscriber) { sig_FrameReady.connect(subscriber); }

        /**
         * @brief unsubscribeFrameReady Disconnect all slots from the Current Frame event
         */
        void unsubscribeFrameReady(){
            sig_FrameReady.disconnect_all_slots();
        }

        void unsubscribeAll() override {
            PipelineStage<I, I>::unsubscribeAll();
            sig_FrameReady.disconnect_all_slots();
        }
    protected:
        /**
         * @brief minValue Minimum value to scale the data to
         */
        I minValue;

        /**
         * @brief maxValue Maximum to scale the data to
         */
        I maxValue;

        vector<unsigned int> m_LUT;

        /**
         * @brief m_FramesPerSecond Sets display rate in FPS, anything <= 0 disables FPS
         */
        float m_FramesPerSecond = 0;

        void work() { }

        void preStage() { }

        void postStage() { }



        boost::signals2::signal<void ()> sig_FrameReady;
    };
}

#endif // DISPLAYPIPELINESTAGE_H
