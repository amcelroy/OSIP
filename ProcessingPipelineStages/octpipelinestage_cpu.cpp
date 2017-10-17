#include "octpipelinestage_cpu.h"

using namespace OSIP;

OCTPipelineStageCPU::OCTPipelineStageCPU()
{

}

void OCTPipelineStageCPU::configure(OCTConfig config){
    _Bias = config.Bias;
    _Gain = config.Gain;
    _PointsPerAScan = config.PointsPerAScan;
    _AScansPerBScan = config.AScansPerBScan - config.StartTrim - config.StopTrim;
}

void OCTPipelineStageCPU::preStage(){

    if(fftwf_init_threads() == 0){
        _Log.push_back("Error initializing multi-threaded FFTW");
        fftwInit = false;
        return;
    }

    //fftwf_plan_with_nthreads(2);

    _fft_out_size = _PointsPerAScan/2 + 1;

    float* fft_in = new float[_PointsPerAScan*_AScansPerBScan];
    fftwf_complex* fft_out = new fftwf_complex[_fft_out_size*_AScansPerBScan];

    int rank = 1;
    int n[] = {_PointsPerAScan};
    int howmany = _AScansPerBScan;
    int idist = _PointsPerAScan;
    int odist = _fft_out_size;
    int istride = 1;
    int ostride = 1;
    _fftplan = fftwf_plan_many_dft_r2c(rank,
                                       n,
                                       howmany,
                                       fft_in,
                                       NULL,
                                       istride,
                                       idist,
                                       fft_out,
                                       NULL,
                                       ostride,
                                       odist,
                                       FFTW_ESTIMATE);

     fftwInit = true;
}

void OCTPipelineStageCPU::workStage(){

    if(!fftwInit)
        return;

    int frame = 0;
    while(!stopThread){
        if(pauseThread){
            pipelineSleep(10);
        }else{
            //dequeue data
            Payload<unsigned short> p = fetchPayload();

            if(this->m_DAQFinished && _Inlet->getItemsInInlet() == 0){
                this->sig_ProcessingFinished();
            }

            if(!p.isValid()){
                pipelineSleep(10);
            }else{
                //operate on p
                vector<unsigned long> dim = p.getFirstDimension();
                int totalDim = 1;
                for(unsigned int i : dim){
                    totalDim *= i;
                }

                shared_ptr<vector<unsigned short>> tmp = p.getFirstData();
                int count = tmp.use_count();

                unsigned short* in = tmp.get()->data();
                float* fft_in = new float[_PointsPerAScan*_AScansPerBScan];
                fftwf_complex* fft_out = new fftwf_complex[_fft_out_size*_AScansPerBScan];

                //Apply hanning
                for(int i = 0; i < totalDim; i++){
                    fft_in[i] = in[i]*_Gain - _Bias;
                }

                frame += 1;

                //All done with the input data
                p.finished();

                //FFT
                fftwf_execute_dft_r2c(_fftplan, fft_in, fft_out);

                auto intensity = make_shared<vector<float>>(_fft_out_size*_AScansPerBScan);
                auto phase = make_shared<vector<float>>(_fft_out_size*_AScansPerBScan);

                //Compute Log10 and phase
                thread intensity_thread(&OCTPipelineStageCPU::_computeIntensity, this, fft_out, intensity.get()->data());
                thread phase_thread(&OCTPipelineStageCPU::_computePhase, this, fft_out, phase.get()->data());

                intensity_thread.join();
                phase_thread.join();

                //package
                Payload<float> p_out;
                vector<unsigned long> dims;
                dims.push_back(_fft_out_size);
                dims.push_back(_AScansPerBScan);
                p_out.addData(dims, intensity);

                //send
                sendPayload(p_out);

                //Release the payload on our end
                p_out.finished();

                delete[] fft_in;
                delete[] fft_out;
            }
        }
    }

    postStage();
}

void OCTPipelineStageCPU::postStage(){
    delete fft_in;
    fftwf_free(fft_out);

    fftwf_cleanup_threads();
    fftwInit = false;
}

void OCTPipelineStageCPU::_computeIntensity(fftwf_complex *f, float *intensity){
    for(int i = 0; i < _fft_out_size*_AScansPerBScan; i++){
        float mag = (f[i][0]*f[i][0] + f[i][1]*f[i][1]);
        intensity[i] = 10*log10f(mag);
    }
}

void OCTPipelineStageCPU::_computePhase(fftwf_complex *f, float *phase){
    for(int i = 0; i < _fft_out_size*_AScansPerBScan; i++){
        phase[i] = atan2f(f[i][1], f[i][0]);
    }
}
