#include "octpipelinestage_cpu.h"

using namespace OSIP;

OCTPipelineStageCPU::OCTPipelineStageCPU()
{
    m_AScanSplits = 1;
}

void OCTPipelineStageCPU::configure(const OCTConfig& config){
    _Bias = config.Bias;
    _Gain = config.Gain;
    _PointsPerAScan = static_cast<unsigned int>(config.PointsPerAScan);
    _AScansPerBScan = static_cast<unsigned int>(config.AScansPerBScan - config.StartTrim - config.StopTrim);
    _NumberOfBScans = static_cast<unsigned int>(config.TotalBScans);
	_BScansPerTransfer = static_cast<unsigned int>(config.BScansPerTransfer);
	m_AScanToDisplay = _AScansPerBScan / 2;

    //omp_set_num_threads(2);
}

void OCTPipelineStageCPU::preStage(){

    if(fftwf_init_threads() == 0){
        this->log("Error initializing multi-threaded FFTW");
        fftwInit = false;
        return;
    }

    fftwf_plan_with_nthreads(4);

    if(_PointsPerAScan % m_AScanSplits != 0){
        log("Error, A-Scan Splits does not divide evenly");
        return;
    }else{
        _PointsPerAScan = _PointsPerAScan / m_AScanSplits;
    }

    _fft_out_size = _PointsPerAScan/2 + 1;

	fft_in.clear();
	fft_in.resize(_PointsPerAScan*_AScansPerBScan*m_AScanSplits*_BScansPerTransfer);
    fft_out = fftwf_alloc_complex(_fft_out_size*_AScansPerBScan*m_AScanSplits*_BScansPerTransfer);

	_window = nullptr;

    setEnfaceRange(1, _fft_out_size - 1);

    int rank = 1;
    int n[] = { static_cast<int>(_PointsPerAScan) };
    int howmany = static_cast<int>(_AScansPerBScan*m_AScanSplits*_BScansPerTransfer);
    int idist = static_cast<int>(_PointsPerAScan);
    int odist = static_cast<int>(_fft_out_size);
    int istride = 1;
    int ostride = 1;
    _fftplan = fftwf_plan_many_dft_r2c(rank,
                                       n,
                                       howmany,
                                       fft_in.data(),
                                       nullptr,
                                       istride,
                                       idist,
                                       fft_out,
                                       nullptr,
                                       ostride,
                                       odist,
                                       FFTW_ESTIMATE);

    m_enface_8bit.resize(static_cast<unsigned long>(_AScansPerBScan*_NumberOfBScans));
    m_bscan_8bit = vector<unsigned char>(_fft_out_size*_AScansPerBScan);

    fftwInit = true;

	PipelineStage::preStage();
}

void OCTPipelineStageCPU::work(){
	try{
		if(!fftwInit)
			return;

		int q_empty_500ms = 0;

		this->flushInlet();  

		int frame = 0;
		while(!stopThread){
			if(pauseThread){
				pipelineSleep(5);
			}else{
				try{
				//dequeue data
				Payload<unsigned short> p = fetchPayload();

				if(this->m_DAQFinished && _Inlet->getItemsInInlet() == 0){
					this->sig_ProcessingFinished();
					this->m_DAQFinished = false;
				}

				if(_Inlet->getItemsInInlet() > 512){
					this->flushInlet();
				}

				if(!p.isValid()){
					pipelineSleep(5);
					if (q_empty_500ms < 100) {
						q_empty_500ms += 1;
					}
					else {
						q_empty_500ms = 0;
						if(this->m_DAQFinished)
							sig_ProcessingFinished();
					}
				}else{
						auto start = chrono::high_resolution_clock::now();
						vector<unsigned long long> dim = p.getFirstDimension();

						unsigned long points = dim[0];
						unsigned long ascans = dim[1];
						unsigned long bscansTransfered = dim[2];
						shared_ptr<vector<unsigned short>> tmp = p.getFirstData();

						//Safety check for Points per AScan and A per B
						if (_PointsPerAScan != dim[0] || _AScansPerBScan != dim[1] || fft_in.size() != tmp->size() || _BScansPerTransfer != dim[2]) {
							p.finished();
							this->flushInlet();
						}
						else {

							//operate on p						
							int totalDim = 1;
							totalDim *= dim[0];
							totalDim *= dim[1];
							totalDim *= dim[2];

							auto dequeue_time = chrono::high_resolution_clock::now();
							std::chrono::duration<double, std::milli> dequeue_elapsed = dequeue_time - start;
							double time = dequeue_elapsed.count();

							//Apply ha nning
							auto hann_start = chrono::high_resolution_clock::now();

							//TODO: Fix bug when daq restarts
							for (int i = 0; i < fft_in.size(); i++) {
								fft_in.data()[i] = tmp->data()[i] * _Gain - _Bias;
							}

							//Merge the a-scans
							if (m_AScanSplits > 1) {
								unsigned long counter = 0;
								for (unsigned long i = 0; i < _PointsPerAScan*_NumberOfBScans*m_AScanSplits; i += m_AScanSplits) {
									float tmp = accumulate(fft_in.begin() + i, fft_in.begin() + i + m_AScanSplits, 0.0f);
									fft_in[counter] = tmp;
									counter += 1;
								}
							}

							auto hann_time = chrono::high_resolution_clock::now();
							std::chrono::duration<double, std::milli> hann_elapsed = hann_time - hann_start;
							time = hann_elapsed.count();

							//FFT
							auto fft_start = chrono::high_resolution_clock::now();
							fftwf_execute_dft_r2c(_fftplan, fft_in.data(), fft_out);
							auto fft_time = chrono::high_resolution_clock::now();
							std::chrono::duration<double, std::milli> fft_elapsed = fft_time - fft_start;
							time = fft_elapsed.count();

							auto alloc_start = chrono::high_resolution_clock::now();
							unsigned long bscanSizeInFloat = _PointsPerAScan * _AScansPerBScan;
							unsigned long bscanSizeOutFloat = _fft_out_size * _AScansPerBScan;

							//TODO: Add code to split up the b-scans
							for (int i = 0; i < _BScansPerTransfer; i++) {
								m_CurrentFrame = static_cast<unsigned int>(dim[3] * _BScansPerTransfer + i);

								auto intensity = shared_ptr<vector<float>>(new vector<float>(_fft_out_size*_AScansPerBScan)); //Ok
								auto atten = shared_ptr<vector<float>>(new vector<float>(_fft_out_size*_AScansPerBScan));	//Ok
								auto enFace = shared_ptr<vector<float>>(new vector<float>(_AScansPerBScan));	//Ok
								auto alloc_time = chrono::high_resolution_clock::now();
								std::chrono::duration<double, std::milli> alloc_elapsed = alloc_time - alloc_start;
								time = alloc_elapsed.count();

								auto mag_start = chrono::high_resolution_clock::now();
								_computeBscan(fft_out + bscanSizeOutFloat * i, intensity->data(), atten->data()); //Not ok, need to way reference current bscan as function of i
								auto mag_time = chrono::high_resolution_clock::now();
								std::chrono::duration<double, std::milli> mag_elapsed = mag_time - mag_start;
								time = mag_elapsed.count();

								_computeEnFace(intensity->data(), enFace->data()); //Not ok, need to way reference current bscan as function of i

								m_RawAScanMutex.lock();
								auto rawStartIt = fft_in.begin() + bscanSizeInFloat * i + _PointsPerAScan * m_AScanToDisplay; //Not ok, need to way reference current bscan as function of i
								auto rawEndIt = fft_in.begin() + bscanSizeInFloat * i + _PointsPerAScan * (m_AScanToDisplay + 1); //Not ok, need to way reference current bscan as function of i
								m_RawAScan = vector<double>(rawStartIt, rawEndIt);
								m_RawAScanMutex.unlock();

								m_IntAScanMutex.lock();
								auto startIt = intensity->begin() + _fft_out_size * m_AScanToDisplay; //Not ok, need to way reference current bscan as function of i
								auto endIt = intensity->begin() + _fft_out_size * (m_AScanToDisplay + 1); //Not ok, need to way reference current bscan as function of i
								m_IntAScan = vector<double>(startIt, endIt);
								m_IntAScanMutex.unlock();

								//package
								Payload<float> p_out;
								vector<unsigned long long> dims;
								dims.push_back(_fft_out_size);
								dims.push_back(_AScansPerBScan);
								dims.push_back(m_CurrentFrame);
								p_out.addData(dims, intensity, "Intensity");
								p_out.addData(dims, atten, "Attenuation");
								p_out.addData(vector<unsigned long long>{ static_cast<unsigned long long>(enFace->size()),
									static_cast<unsigned long long>(_NumberOfBScans) },
									enFace, "EnFace_Slice");

								//send
								this->sendPayload(p_out);
							}

							auto stop = chrono::high_resolution_clock::now();
							std::chrono::duration<double, std::micro> elapsed = stop - start;
							d_ThreadWorkTime = elapsed.count();
							this->sig_StageTimer(static_cast<float>(elapsed.count()));


							try {
								sig_FrameProcessed();
							}
							catch (exception e) {
								log(string(e.what()));
							}
						}
					}
				}catch(exception e){
					log("Generic Error in OCT Pipeline CPU Stage");
				}
			}
		}
	}catch(exception e){
		log(string(e.what()));
	}

    this->postStage();
}

void OCTPipelineStageCPU::postStage(){
	fftwf_destroy_plan(_fftplan);

    fftwf_free(fft_out);

    fftwf_cleanup_threads();
    fftwInit = false;

	fftwf_cleanup();

	if(_window != nullptr)
		delete[] _window;

	PipelineStage::postStage();
}

void OCTPipelineStageCPU::_computeEnFace(float *intensity, float *enface){
    float range = _EnFaceUpper - _EnFaceLower;
    for(unsigned int i = 0; i < _AScansPerBScan; i++){
        float avg = 0;
        for(unsigned int j = _EnFaceLower; j < _EnFaceUpper; j++){
            avg += intensity[i*_fft_out_size + j];
        }
        enface[i] = avg / range;
    }
}

void OCTPipelineStageCPU::_computeBscan(fftwf_complex *f, float *intensity, float *atten){
//#pragma omp parallel for
    for(unsigned int i = 0; i < _AScansPerBScan; i++){
        float runningSum = 0;

        unsigned int j = _fft_out_size - 1;
        while(j != 0){
            unsigned long lin_coord = i*_fft_out_size + j;
            float mag = sqrtf(f[lin_coord][0]*f[lin_coord][0] + f[lin_coord][1]*f[lin_coord][1]);
            intensity[lin_coord] = 20*log10f(mag);
            runningSum += mag;
            atten[lin_coord] = mag / runningSum;
            j--;
        }
    }
}

void OCTPipelineStageCPU::setEnfaceRange(unsigned int EnFace1, unsigned int EnFace2){
    _EnFaceLower = min(EnFace1, EnFace2);
    _EnFaceUpper = max(EnFace1, EnFace2);
    _EnFaceUpper = min(_fft_out_size, _EnFaceUpper);
}
