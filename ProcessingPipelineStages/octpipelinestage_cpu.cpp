#include "octpipelinestage_cpu.h"

OCTPipelineStageCPU::OCTPipelineStageCPU(float Gain, float Bias, int PointsPerAScan, int AScansPerBScan)
{
    _Gain = Gain;
    _Bias = Bias;
    _PointsPerAScan = PointsPerAScan;
    _AScansPerBScan = AScansPerBScan;
}

void OCTPipelineStageCPU::preStage(){

    if(fftwf_init_threads() == 0){
        _Log.push_back("Error initializing multi-threaded FFTW");
        fftwInit = false;
        return;
    }

    fftwf_plan_with_nthreads(2);

    _fft_out_size = _PointsPerAScan/2 + 1;

    fft_in = new float[_PointsPerAScan*_AScansPerBScan];
    fft_out = new fftwf_complex[_fft_out_size*_AScansPerBScan];

    int rank = 1;
    int n[] = {_PointsPerAScan};
    int howmany = _AScansPerBScan;
    int idist = 1;
    int odist = 1;
    int istride = _AScansPerBScan;
    int ostride = _fft_out_size;
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

    while(!stopThread){
        if(pauseThread){
            pipelineSleep(10);
        }else{
            //dequeue data
            Payload<unsigned short> p = fetchPayload();

            if(!p.isValid()){
                pipelineSleep(10);
            }else{
                //operate on p
                vector<unsigned int> dim = p.getFirstDimension();
                int totalDim = 1;
                for(unsigned int i : dim){
                    totalDim *= i;
                }

                unsigned short* in = p.getFirstData().get();

                //Apply hanning
                for(int i = 0; i < totalDim; i++){
                    fft_in[i] = in[i]*_Gain - _Bias;
                }

                //All done with the input data
                p.finished();

                //FFT
                fftwf_execute_dft_r2c(_fftplan, fft_in, fft_out);

                float* intensity = new float[_fft_out_size*_AScansPerBScan];
                float* phase = new float[_fft_out_size*_AScansPerBScan];

                //Compute Log10 and phase
                thread intensity_thread(&OCTPipelineStageCPU::_computeIntensity, this, fft_out, intensity);
                thread phase_thread(&OCTPipelineStageCPU::_computePhase, this, fft_out, phase);

                intensity_thread.join();
                phase_thread.join();

                //package
                Payload<float> p_out;
                p_out.addData(vector<unsigned int>(_fft_out_size, _AScansPerBScan), intensity);
                p_out.addData(vector<unsigned int>(_fft_out_size, _AScansPerBScan), phase);

                //send
                sendPayload(p_out);

                //Release the payload on our end
                p_out.finished();
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
        intensity[i] = 20*log10f(f[i][0]*f[i][1]);
    }
}

void OCTPipelineStageCPU::_computePhase(fftwf_complex *f, float *phase){
    for(int i = 0; i < _fft_out_size*_AScansPerBScan; i++){
        phase[i] = atan2f(f[i][1], f[i][0]);
    }
}
