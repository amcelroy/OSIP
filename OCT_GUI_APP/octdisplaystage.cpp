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
                vector<shared_ptr<float>> datas = p.getData();

                unsigned long arraySize = 1;
                for(unsigned long l : dims.at(0)){
                    arraySize *= l;
                }

                minValue = -20;
                maxValue = 60;

                unsigned char* RGBA = new unsigned char[arraySize];
                scaleTo8Bit(datas.at(0).get(), dims.at(0), RGBA);
                shared_ptr<unsigned char> uc_ptr(RGBA);

                emit qt_Notifier.intensityBScanReady(uc_ptr, dims.at(0));

                std::this_thread::sleep_for(std::chrono::milliseconds(250));

                p.finished();
            }
        }
    }
}

void OCTDisplayStage::postStage(){

}
