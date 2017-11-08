#include "octdisplaystage.h"

OCTDisplayStage::OCTDisplayStage()
{

}

void OCTDisplayStage::preStage(){

}

void OCTDisplayStage::workStage(){
    while(!this->stopThread){
        if(this->pauseThread){
            pipelineSleep(50);
        }else{
            Payload<float> p = this->fetchPayload();

            if(!p.isValid()){
                this->pipelineSleep(50);
            }else{
                vector<vector<unsigned long>> dims = p.getDimensions();
                vector<shared_ptr<vector<float>>> datas = p.getData();

                unsigned long arraySize = 1;
                for(unsigned long l : dims.at(0)){
                    arraySize *= l;
                }

                auto RGBA = make_shared<vector<unsigned int>>(arraySize);
                scaleToRGBA(datas.at(0).get(), dims.at(0), RGBA.get());

                m_bscanImageProvider->setPixels(RGBA, dims.at(0));

                //fetch Processed A-Scan
                auto ascanData = datas.at(2);
                auto ascanDim = dims.at(2);

                if(m_FramesPerSecond != 0){
                    std::this_thread::sleep_for(std::chrono::milliseconds((long)(1/m_FramesPerSecond)));
                }

                p.finished();
            }
        }
    }
}

void OCTDisplayStage::postStage(){

}

