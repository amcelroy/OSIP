#ifndef DISPLAYPIPELINESTAGE_H
#define DISPLAYPIPELINESTAGE_H

#include "displaypipelinestage_global.h"
#include "Pipeline/pipelinestage_impl.h"
#include <QObject>

namespace OSIP {

    template<class I>
    class DISPLAYPIPELINESTAGESHARED_EXPORT DisplayPipelineStage : public PipelineStage<I, I>
    {

    public:
        DisplayPipelineStage();

        /**
         * @brief setFramerate Sets the display rate in FPS. Values <= 0 will run at full speed
         * @param FramesPerSecond
         */
        void setFramerate(float FramesPerSecond);

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
        void scaleTo8Bit(I* array, vector<unsigned long> dim, unsigned char *output);

        /**
         * @brief scaleTo8Bit Scales a value to 8 Bits (0 to 255), then to RGBA using the look up tables
         * @param array
         */
        void scaleToRGBA(I* array, vector<unsigned long> dim, unsigned int *output);

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

        void workStage();

        void preStage();

        void postStage();

        vector<unsigned char> m_redLUT;

        vector<unsigned char> m_blueLUT;

        vector<unsigned char> m_greenLUT;
    };
}

#endif // DISPLAYPIPELINESTAGE_H
