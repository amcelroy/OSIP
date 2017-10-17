#ifndef OCTPIPELINESTAGE_H
#define OCTPIPELINESTAGE_H

#include "processingpipelinestages_global.h"
#include "Pipeline/pipelinestage.h"
#include "fftw3.h"
#include "windowmaker.h"
#include "boost/signals2.hpp"
#include "DAQPipelineStages/loadoctpipeline.h"
#include <boost/signals2.hpp>

namespace OSIP {
    class PROCESSINGPIPELINESTAGESSHARED_EXPORT OCTPipelineStageCPU : public PipelineStage<unsigned short, float>
    {

    public:
        OCTPipelineStageCPU();

        void preStage();

        void workStage();

        void postStage();

        void configure(OCTConfig config);

        /**
         * @brief notifyStarted Add a subscriber to get notified when the PipelineStage has started
         * @param subscriber
         */
        void subscribeProcessingFinished(const boost::signals2::signal<void()>::slot_type &subscriber) { sig_ProcessingFinished.connect(subscriber); }

    protected:

        boost::signals2::signal<void ()> sig_ProcessingFinished;

    private:        
        void _computeIntensity(fftwf_complex *f, float *intensity);

        void _computePhase(fftwf_complex *f, float *phase);

        WindowMaker _windowMaker;

        /**
         * @brief _fft_out_size Size of the output 1d-fft, usually _PointsPerAScan/2 + 1
         */
        int _fft_out_size;

        /**
         * @brief fft_in Array used to store pre-fft data
         */
        float* fft_in;

        /**
         * @brief fft_out Array to store post-fft data
         */
        fftwf_complex* fft_out;

        /**
         * @brief _window Array to store pre-fft window data
         */
        float* _window;

        /**
         * @brief _Gain Gain of the ADC, used to convert raw count to voltage
         */
        float _Gain;

        /**
         * @brief _Bias Bias of the ADC, used to convert raw count to voltage
         */
        float _Bias;

        /**
         * @brief _PointsPerAScan Number of points in an A-Scan
         */
        int _PointsPerAScan;

        /**
         * @brief _AScansPerBScan Number of A-Scans in a B-Scan
         */
        int _AScansPerBScan;

        /**
         * @brief _fftplan FFTW plan used to to compute FFT
         */
        fftwf_plan _fftplan;

        /**
         * @brief fftwInit Flag to indicate successfull initialization of the FFT library
         */
        bool fftwInit = false;
    };
}

#endif // OCTPIPELINESTAGE_H
