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
    _NumberOfBScans = config.TotalBScans;
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
                auto start = chrono::high_resolution_clock::now();

                //operate on p
                vector<unsigned long> dim = p.getFirstDimension();
                int totalDim = 1;
                totalDim *= dim[0];
                totalDim *= dim[1];
                int currentFrame = dim[2];

                shared_ptr<vector<unsigned short>> tmp = p.getFirstData();

                float* fft_in = new float[_PointsPerAScan*_AScansPerBScan];
                fftwf_complex* fft_out = new fftwf_complex[_fft_out_size*_AScansPerBScan];

                //Apply hanning
                for(int i = 0; i < totalDim; i++){
                    fft_in[i] = tmp->data()[i]*_Gain - _Bias;
                }

                frame += 1;

                //All done with the input data
                p.finished();

                //FFT
                fftwf_execute_dft_r2c(_fftplan, fft_in, fft_out);

                auto intensity = make_shared<vector<float>>(_fft_out_size*_AScansPerBScan);
                auto phase = make_shared<vector<float>>(_fft_out_size*_AScansPerBScan);
                auto mag = make_shared<vector<float>>(_fft_out_size*_AScansPerBScan);
                auto atten = make_shared<vector<float>>(_fft_out_size*_AScansPerBScan);
                auto enFace = make_shared<vector<float>>(_AScansPerBScan);

                _computeMagnitude(fft_out, mag.get()->data());
                //_computeIntensity(mag.get()->data(), intensity.get()->data());
                //_computeAttenuationSimple(mag.get()->data(), atten.get()->data());

                //Compute Log10 and phase
                thread intensity_thread(&OCTPipelineStageCPU::_computeIntensity, this, mag.get()->data(), intensity.get()->data());
                thread attenuation_thread(&OCTPipelineStageCPU::_computeAttenuationSimple, this, mag.get()->data(), atten.get()->data());
                //thread phase_thread(&OCTPipelineStageCPU::_computePhase, this, fft_out, phase.get()->data());

                intensity_thread.join();
                attenuation_thread.join();

                _computeEnFace(intensity->data(), enFace->data());

                auto startIt = intensity->begin() + _fft_out_size*m_AScanToDisplay;
                auto endIt = intensity->begin() + _fft_out_size*(m_AScanToDisplay + 1);
                auto ascan = make_shared<vector<float>>(vector<float>(startIt, endIt));

                //package
                Payload<float> p_out;
                vector<unsigned long> dims;
                dims.push_back(_fft_out_size);
                dims.push_back(_AScansPerBScan);
                p_out.addData(dims, intensity, "Intensity");
                p_out.addData(dims, atten, "Attenuation");
                p_out.addData(vector<unsigned long>{ (unsigned long)_fft_out_size }, ascan, "Intensity_Ascan");
                p_out.addData(vector<unsigned long>{ (unsigned long)enFace->size(), (unsigned long)_NumberOfBScans, (unsigned long)currentFrame }, enFace, "EnFace_Slice");

                //send
                sendPayload(p_out);

                //Release the payload on our end
                p_out.finished();

                auto stop = chrono::high_resolution_clock::now();

                std::chrono::duration<double, std::micro> elapsed = stop - start;
                this->sig_StageTimer(elapsed.count());

                sig_FrameProcessed();

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

void OCTPipelineStageCPU::_computeMagnitude(fftwf_complex *f, float *mag){
    for(int i = 0; i < _fft_out_size*_AScansPerBScan; i++){
        mag[i] = sqrt(f[i][0]*f[i][0] + f[i][1]*f[i][1]);
    }
}

void OCTPipelineStageCPU::_computeEnFace(float *intensity, float *enface){
    float range = _EnFaceUpper - _EnFaceLower;
    for(int i = 0; i < _AScansPerBScan; i++){
        float avg = 0;
        for(int j = _EnFaceLower; j < _EnFaceUpper; j++){
            avg += intensity[i*_fft_out_size + j];
        }
        enface[i] = avg / range;
    }
}

void OCTPipelineStageCPU::_computeIntensity(float *mag, float *intensity){
    for(int i = 0; i < _fft_out_size*_AScansPerBScan; i++){
        intensity[i] = 10*log10f(mag[i]);
    }
}

void OCTPipelineStageCPU::_computePhase(fftwf_complex *f, float *phase){
    for(int i = 0; i < _fft_out_size*_AScansPerBScan; i++){
        phase[i] = atan2f(f[i][1], f[i][0]);
    }
}

void OCTPipelineStageCPU::_computeAttenuationSimple(float *mag, float *atten){
    for(int i = 0; i < _AScansPerBScan; i++){
        float runningSum = 0;
        for(int j = _fft_out_size - 1; j > -1; j--){
            unsigned long lin_coord = i*_fft_out_size + j;
            runningSum += mag[lin_coord];
            atten[lin_coord] = mag[lin_coord] / runningSum;
        }
    }
}

void OCTPipelineStageCPU::setEnfaceRange(int EnFace1, int EnFace2){
    _EnFaceLower = min(EnFace1, EnFace2);
    _EnFaceUpper = max(EnFace1, EnFace2);

    _EnFaceLower = max(0, _EnFaceLower);
    _EnFaceUpper = min(_fft_out_size, _EnFaceUpper);
}
