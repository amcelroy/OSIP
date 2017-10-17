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

                minValue = -20;
                maxValue = 30;

                unsigned int* RGBA = new unsigned int[arraySize];
                scaleToRGBA(datas.at(0).get(), dims.at(0), RGBA);

                m_bscanImageProvider->setImage(new QImage((unsigned char*)RGBA, (int)dims.at(0).at(0), (int)dims.at(0).at(1), QImage::Format_RGB32));

                if(m_FramesPerSecond != 0){
                    std::this_thread::sleep_for(std::chrono::milliseconds((long)(1/m_FramesPerSecond)));
                }

                p.finished();

                if(this->m_ProcessingFinished && this->m_DAQFinished){
                    m_bscanImageProvider->setImage(NULL);
                    this->pause();
                }
            }
        }
    }
}

void OCTDisplayStage::postStage(){

}

