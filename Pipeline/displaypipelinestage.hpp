#ifndef DISPLAYPIPELINESTAGE_H
#define DISPLAYPIPELINESTAGE_H

#include "pipelinestage.hpp"
#include <QObject>
#include <vector>

using namespace std;

namespace OSIP {
    template<class I>
    class DisplayPipelineStage : public PipelineStage<I,I>
    {

    public:
        DisplayPipelineStage(){
            m_redLUT = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 4, 7, 10, 13, 16, 20, 23, 27, 30, 33, 36, 39, 42, 45, 48, 52, 55, 58, 61, 65, 68, 71, 74, 78, 81, 84, 87, 90, 93, 96, 99, 103, 106, 110, 113, 117, 120, 123, 126, 130, 133, 136, 139, 142, 145, 148, 151, 155, 158, 161, 164, 168, 171, 174, 177, 181, 184, 187, 190, 193, 196, 200, 203, 207, 210, 213, 216, 220, 223, 226, 229, 233, 236, 239, 242, 245, 247, 250, 252, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
            m_greenLUT = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 6, 9, 12, 16, 19, 22, 25, 29, 32, 35, 38, 42, 45, 48, 51, 55, 58, 61, 64, 68, 71, 74, 77, 81, 84, 87, 90, 94, 97, 100, 103, 106, 109, 112, 115, 119, 122, 126, 129, 133, 136, 139, 142, 145, 148, 151, 154, 158, 161, 164, 167, 171, 174, 177, 180, 184, 187, 190, 193, 197, 200, 203, 206, 209, 212, 216, 219, 223, 226, 229, 232, 236, 239, 242, 245, 249, 250, 252, 253, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255 };
            m_blueLUT = { 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 3, 4, 6, 9, 12, 15, 19, 22, 25, 28, 32, 35, 38, 41, 45, 48, 51, 54, 58, 61, 64, 67, 71, 74, 77, 80, 84, 87, 90, 93, 97, 100, 103, 106, 110, 113, 116, 119, 122, 125, 128, 131, 135, 138, 142, 145, 149, 152, 155, 158, 161, 164, 167, 170, 174, 177, 180, 183, 187, 190, 193, 196, 200, 203, 206, 209, 213, 216, 219, 222, 225, 228, 232, 235, 239, 242, 245, 248, 252, 252, 253, 254, 255, 255, 255, 255, 255, 255, 255, 255 };
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
        void scaleTo8Bit(vector<I> *array, vector<unsigned long> dim, vector<unsigned char> *output){
            if(array == NULL){
                assert( array == NULL);
                return;
            }

            unsigned long arraySize = 1;
            for(unsigned long l : dim){
                arraySize *= l;
            }

            I range = (maxValue - minValue);
            for(unsigned long i = 0; i < arraySize; i++){
                double tmp = 255*(array->at(i) - minValue) / range;
                tmp = max(0.0, tmp);
                tmp = min(tmp, 255.0);
                output->data()[i] = (unsigned char)tmp;
            }
        }

        /**
         * @brief scaleTo8Bit Scales a value to 8 Bits (0 to 255), then to RGBA using the look up tables
         * @param array
         */
        void scaleToRGBA(vector<I>* array, vector<unsigned int>* output){
            if(array == NULL){
                assert( array == NULL);
                return;
            }

            I range = (maxValue - minValue);
            for(unsigned long i = 0; i < array->size(); i++){
                double tmp = 255*(array->at(i) - minValue) / range;
                tmp = max(0.0, tmp);
                tmp = min(tmp, 255.0);
                output->data()[i] = (unsigned int)(255 << 24 | m_redLUT.at(tmp) << 16 | m_greenLUT.at(tmp) << 8 | m_blueLUT.at(tmp));
            }
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

        /**
         * @brief m_FramesPerSecond Sets display rate in FPS, anything <= 0 disables FPS
         */
        float m_FramesPerSecond = 0;

        void work();

        void preStage();

        void postStage();

        vector<unsigned char> m_redLUT;

        vector<unsigned char> m_blueLUT;

        vector<unsigned char> m_greenLUT;
    };
}

#endif // DISPLAYPIPELINESTAGE_H
