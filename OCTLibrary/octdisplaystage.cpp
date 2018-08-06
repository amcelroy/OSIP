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
                this->pipelineSleep(5);
            }else{
                auto start = chrono::high_resolution_clock::now();

                vector<vector<unsigned long long>> dims = p.getDimensions();
                vector<shared_ptr<vector<float>>> datas = p.getData();

                this->flushInlet();

                //dims: 0 - A per B, 1 - Total B, 2 - Current B
                auto enFace = p.findByDataName("EnFace_Slice");
                vector<unsigned long long> enFaceDims = get<1>(enFace);
                shared_ptr<vector<float>> enFaceData = get<0>(enFace);

                auto rawAScan = p.findByDataName("Raw_Ascan");
                auto intAScan = p.findByDataName("Intensity_Ascan");

                vector<qreal> q_rawAScan(get<1>(rawAScan)[0]); //new qreal vector of size rawAScan
                vector<qreal> q_intAScan(get<1>(intAScan)[0]); //new qreal vector of size rawAScan
                vector<float>* tmp_raw_ptr = get<0>(rawAScan).get();
                vector<float>* tmp_int_ptr = get<0>(intAScan).get();
                for(int i = 0; i < q_rawAScan.size(); i++){
                    q_rawAScan[i] = tmp_raw_ptr->at(i);
                }
                for(int i = 0; i < q_intAScan.size(); i++){
                    q_intAScan[i] = tmp_int_ptr->at(i);
                }
                tmp_raw_ptr = NULL;
                tmp_int_ptr = NULL;

                unsigned long arraySize = 1;
                for(unsigned long l : dims.at(0)){
                    arraySize *= l;
                }

                auto RGBA = vector<unsigned int>(arraySize);
                auto RGBA_enFace = vector<unsigned int>(enFaceDims[0]);

                auto tmp_ptr = datas.at(0).get();
                scaleToRGBA(tmp_ptr->data(), RGBA.data(), RGBA.size());
                tmp_ptr = NULL;

                auto en_face_tmp_ptr = enFaceData.get();
                scaleToRGBA(en_face_tmp_ptr->data(), RGBA_enFace.data(), RGBA_enFace.size());
                en_face_tmp_ptr = NULL;

//                thread color_thread(&OCTDisplayStage::scaleToRGBA, this, datas.at(0).get(), RGBA.get());
//                thread end_face_color_thread(&OCTDisplayStage::scaleToRGBA, this, enFaceData.get(), RGBA_enFace.get());

//                color_thread.join();
//                end_face_color_thread.join();
                m_bscanImageProvider->setPixels(RGBA, dims.at(0));
                m_AScanBackend->setRawAScan(q_rawAScan);
                m_AScanBackend->setIntensityAScan(q_intAScan);
                m_enFaceImageProvider->setRow(RGBA_enFace, enFaceDims);


                if(m_FramesPerSecond != 0){
                    std::this_thread::sleep_for(std::chrono::milliseconds((long)(1/m_FramesPerSecond)));
                }

                auto stop = chrono::high_resolution_clock::now();
                std::chrono::duration<double, std::micro> elapsed = stop - start;
                this->sig_StageTimer(elapsed.count());
                d_ThreadWorkTime = elapsed.count();

                p.finished();
            }
        }
    }
}

void OCTDisplayStage::postStage(){

}

