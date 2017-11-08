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

                //dims: 0 - A per B, 1 - Total B, 2 - Current B
                auto enFace = p.findByDataName("EnFace_Slice");
                vector<unsigned long> enFaceDims = get<1>(enFace);
                shared_ptr<vector<float>> enFaceData = get<0>(enFace);


                unsigned long arraySize = 1;
                for(unsigned long l : dims.at(0)){
                    arraySize *= l;
                }

                auto RGBA = make_shared<vector<unsigned int>>(arraySize);
                scaleToRGBA(datas.at(0).get(), RGBA.get());

                auto RGBA_enFace =  make_shared<vector<unsigned int>>(enFaceDims[0]);
                scaleToRGBA(enFaceData.get(), RGBA_enFace.get());

                m_bscanImageProvider->setPixels(RGBA, dims.at(0));
                m_enFaceImageProvider->setRow(RGBA_enFace, enFaceDims);

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

