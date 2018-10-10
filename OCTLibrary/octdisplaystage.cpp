#include "octdisplaystage.h"

using namespace OSIP;

void OCTDisplayStage::preStage(){

}

void OCTDisplayStage::work(){
    while(!this->stopThread){
        if(this->pauseThread){
            pipelineSleep(5);
        }else{
            Payload<float> p = this->fetchPayload();

            if(!p.isValid()){
                this->pipelineSleep(33);
            }else{
                try{
                    auto start = chrono::high_resolution_clock::now();

                    vector<vector<unsigned long long>> dims = p.getDimensions();
                    vector<shared_ptr<vector<float>>> datas = p.getData();

                    this->flushInlet();

                    //dims: 0 - A per B, 1 - Total B, 2 - Current B
                    auto enFace = p.findByDataName("EnFace_Slice");
                    vector<unsigned long long> enFaceDims = get<1>(enFace);
                    shared_ptr<vector<float>> enFaceData = get<0>(enFace);

                    unsigned long long currentFrame = dims[0][2];

                    unsigned long arraySize = dims[0][0]*dims[0][1];

                    if(m_bscan_8bit.size() != arraySize){
                        m_bscan_8bit = vector<unsigned char>(arraySize);
                    }

                    m_BScanAccessMutex.lock();
                    scaleTo8Bit(*(datas.at(0).get()), &m_bscan_8bit);
                    m_BScanAccessMutex.unlock();

                    m_EnFaceAccessMutex.lock();
                    vector<unsigned char> tmp_slice(dims[0][1]);
                    scaleTo8Bit(*(enFaceData.get()), &tmp_slice);
                    memcpy(&m_enface_8bit.data()[currentFrame*dims[0][1]], tmp_slice.data(), dims[0][1]);
                    m_EnFaceAccessMutex.unlock();

                    if(m_FramesPerSecond != 0.0f){
                        std::this_thread::sleep_for(std::chrono::milliseconds(static_cast<long>(1/m_FramesPerSecond)));
                    }

                    auto stop = chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::micro> elapsed = stop - start;
                    this->sig_StageTimer(static_cast<float>(elapsed.count()));
                    d_ThreadWorkTime = elapsed.count();

                    p.finished();
                }catch(...){
                    log("Generic error in OCT Display Stage");
                }
            }
        }
    }
}

void OCTDisplayStage::postStage(){

}

