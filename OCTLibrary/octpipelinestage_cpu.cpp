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

    omp_set_num_threads(2);
}

void OCTPipelineStageCPU::preStage(){

    if(fftwf_init_threads() == 0){
        _Log.push_back("Error initializing multi-threaded FFTW");
        fftwInit = false;
        return;
    }

    fftwf_plan_with_nthreads(2);

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

void OCTPipelineStageCPU::work(){

    if(!fftwInit)
        return;

    int frame = 0;
    while(!stopThread){
        if(pauseThread){
            pipelineSleep(5);
        }else{
            //dequeue data
            Payload<unsigned short> p = fetchPayload();

            if(this->m_DAQFinished && _Inlet->getItemsInInlet() == 0){
                this->sig_ProcessingFinished();
            }

            if(!p.isValid()){
                pipelineSleep(5);
            }else{
                auto start = chrono::high_resolution_clock::now();

                //operate on p
                vector<unsigned long long> dim = p.getFirstDimension();
                int totalDim = 1;
                totalDim *= dim[0];
                totalDim *= dim[1];
                int currentFrame = dim[2];

                shared_ptr<vector<unsigned short>> tmp = p.getFirstData();

                auto dequeue_time = chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> dequeue_elapsed = dequeue_time - start;
                double time = dequeue_elapsed.count();

                if(fft_in == NULL){
                    fft_in = new vector<float>(_PointsPerAScan*_AScansPerBScan);
                    fft_out = new vector<fftwf_complex>(_fft_out_size*_AScansPerBScan);
                }

                //Apply hanning
                auto hann_start = chrono::high_resolution_clock::now();
                auto tmp_ptr = tmp->data();
#pragma omp parallel for
                for(int i = 0; i < totalDim; i++){
                    fft_in->data()[i] = tmp_ptr[i]*_Gain - _Bias;
                }
                tmp_ptr = NULL;
                auto hann_time = chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> hann_elapsed = hann_time - hann_start;
                time = hann_elapsed.count();

                frame += 1;

                //All done with the input data
                p.finished();

                //FFT
                auto fft_start = chrono::high_resolution_clock::now();
                fftwf_execute_dft_r2c(_fftplan, fft_in->data(), fft_out->data());
                auto fft_time = chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> fft_elapsed = fft_time - fft_start;
                time = fft_elapsed.count();

                auto alloc_start = chrono::high_resolution_clock::now();
                auto intensity = make_shared<vector<float>>(_fft_out_size*_AScansPerBScan);
                //auto phase = make_shared<vector<float>>(_fft_out_size*_AScansPerBScan);
                auto atten = make_shared<vector<float>>(_fft_out_size*_AScansPerBScan);
                auto enFace = make_shared<vector<float>>(_AScansPerBScan);
                auto alloc_time = chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> alloc_elapsed = alloc_time - alloc_start;
                time = alloc_elapsed.count();

                auto mag_start = chrono::high_resolution_clock::now();
                _computeBscan(fft_out->data(), intensity.get()->data(), atten.get()->data());
                auto mag_time = chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::milli> mag_elapsed = mag_time - mag_start;
                time = mag_elapsed.count();

                _computeEnFace(intensity->data(), enFace->data());

                auto rawStartIt = fft_in->begin() + _PointsPerAScan*m_AScanToDisplay;
                auto rawEndIt = fft_in->begin() + _PointsPerAScan*(m_AScanToDisplay + 1);
                auto _RawAScan = make_shared<vector<float>>(vector<float>(rawStartIt, rawEndIt));

                auto startIt = intensity->begin() + _fft_out_size*m_AScanToDisplay;
                auto endIt = intensity->begin() + _fft_out_size*(m_AScanToDisplay + 1);
                auto _IntAScan = make_shared<vector<float>>(vector<float>(startIt, endIt));

                //package
                Payload<float> p_out;
                vector<unsigned long long> dims;
                dims.push_back(_fft_out_size);
                dims.push_back(_AScansPerBScan);
                p_out.addData(dims, intensity, "Intensity");
                p_out.addData(dims, atten, "Attenuation");
                p_out.addData(vector<unsigned long long>{ (unsigned long long)_PointsPerAScan }, _RawAScan, "Raw_Ascan");
                p_out.addData(vector<unsigned long long>{ (unsigned long long)_fft_out_size }, _IntAScan, "Intensity_Ascan");
                p_out.addData(vector<unsigned long long>{ (unsigned long long)enFace->size(), (unsigned long long)_NumberOfBScans, (unsigned long)currentFrame }, enFace, "EnFace_Slice");

                //send
                sendPayload(p_out);

                //Release the payload on our end
                p_out.finished();

                auto stop = chrono::high_resolution_clock::now();

                std::chrono::duration<double, std::micro> elapsed = stop - start;
                d_ThreadWorkTime = elapsed.count();
                this->sig_StageTimer(elapsed.count());

                sig_FrameProcessed();
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

    delete[] fft_in;
    delete[] fft_out;
}

//void OCTPipelineStageCPU::_computeMagnitudeAndIntensity(fftwf_complex *f, float *mag, float *intensity){
//#pragma omp parallel for
//    for(int i = 0; i < _fft_out_size*_AScansPerBScan; i++){
//        mag[i] = f[i][0]*f[i][0] + f[i][1]*f[i][1];
//        intensity[i] = 10*log10f(mag[i]);
//    }
//}

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

//void OCTPipelineStageCPU::_computeIntensity(float *mag, float *intensity){
//    for(int i = 0; i < _fft_out_size*_AScansPerBScan; i++){
//        intensity[i] = 10*log10f(mag[i]);
//    }
//}

//void OCTPipelineStageCPU::_computePhase(fftwf_complex *f, float *phase){
//    for(int i = 0; i < _fft_out_size*_AScansPerBScan; i++){
//        phase[i] = atan2f(f[i][1], f[i][0]);
//    }
//}

void OCTPipelineStageCPU::_computeBscan(fftwf_complex *f, float *intensity, float *atten){
#pragma omp parallel for
    for(int i = 0; i < _AScansPerBScan; i++){
        float runningSum = 0;
        for(int j = _fft_out_size - 1; j >= 0; j--){
            unsigned long lin_coord = i*_fft_out_size + j;
            float mag = f[lin_coord][0]*f[lin_coord][0] + f[lin_coord][1]*f[lin_coord][1];
            intensity[lin_coord] = 10*log10f(mag);
            float tmp_mag = sqrtf(mag); //Didn't sqrt in the original mgnitude calc.
            runningSum += tmp_mag;
            atten[lin_coord] = tmp_mag / runningSum;
        }
    }
}

void OCTPipelineStageCPU::setEnfaceRange(int EnFace1, int EnFace2){
    _EnFaceLower = min(EnFace1, EnFace2);
    _EnFaceUpper = max(EnFace1, EnFace2);

    _EnFaceLower = max(0, _EnFaceLower);
    _EnFaceUpper = min(_fft_out_size, _EnFaceUpper);
}
