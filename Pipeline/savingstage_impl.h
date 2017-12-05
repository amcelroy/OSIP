#ifndef SAVINGPIPELINE_IMPL_H
#define SAVINGPIPELINE_IMPL_H

#include "savingstage.h"

using namespace OSIP;

template<class I>
SavingStage<I>::SavingStage(){
//    //Set system endianess first;
//    //https://stackoverflow.com/questions/1001307/detecting-endianness-programmatically-in-a-c-program
//    if ( htonl(47) == 47 ) {
//      // Big endian
//        m_SystemEndianess = boost::endian::order::big;
//    } else {
//      // Little endian.
//        m_SystemEndianess = boost::endian::order::little;
//    }
}

template<class I>
bool SavingStage<I>::setSavePath(string FolderPath){
    boost::filesystem::path p(FolderPath);
    if(boost::filesystem::exists(p)){ //valid folder?
        if(boost::filesystem::is_directory(p)){ //valid directory?
            this->m_FolderPath = FolderPath;
            this->m_FolderPathSet = true;
            return true;
        }
    }

    m_FolderPathSet = false;
    return false;
}

template<class I>
void SavingStage<I>::toBinaryUInt16(unsigned short* data, unsigned long N){
    _saveData(data, sizeof(unsigned short)*N);
}

template<class I>
void SavingStage<I>::toBinaryFloat(float* data, unsigned long N){
    _saveData(data, sizeof(float)*N);
}

template<class I>
void SavingStage<I>::toBinaryDouble(double* data, unsigned long N){
    _saveData(data, sizeof(double)*N);
}

template<class I>
void SavingStage<I>::toBinaryInt16(short* data, unsigned long N){
    _saveData(data, sizeof(short)*N);
}

template<class I>
void SavingStage<I>::toBinaryUInt32(unsigned int* data, unsigned long N){
    _saveData(data, sizeof(unsigned int)*N);
}

template<class I>
void SavingStage<I>::toBinaryInt32(int* data, unsigned long N){
    _saveData(data, sizeof(int)*N);
}

template<class I>
void SavingStage<I>::toBinaryU8(unsigned char* data, unsigned long N){
    _saveData(data, sizeof(unsigned char)*N);
}

template<class I>
void SavingStage<I>::toBinaryChar(char* data, unsigned long N){
    _saveData(data, sizeof(char)*N);
}

template<class I>
void SavingStage<I>::toBinaryInt64(long* data, unsigned long N){
    _saveData(data, sizeof(long)*N);
}

template<class I>
void SavingStage<I>::toBinaryUInt64(unsigned long* data, unsigned long N){
    _saveData(data, sizeof(unsigned long)*N);
}

template<class I>
void SavingStage<I>::_saveData(void *data, unsigned long NBytes){
//    try{
//        //Save data
//        m_FileStream.write((char*)data, NBytes);
//    }catch(exception e){
//        this->sig_MessageLogged("Error saving file - ");
//    }
}

template<class I>
void SavingStage<I>::preStage(){

}

template<class I>
void SavingStage<I>::postStage(){

}

template<class I>
void SavingStage<I>::workStage(){
    if(this->m_FolderPathSet){
        while(!this->stopThread){
            if(this->pauseThread){
                this->pipelineSleep(10);
            }else{
                //dequeue data
                Payload<I> p = this->fetchPayload();

                if(!p.isValid()){
                    this->pipelineSleep(10);
                }else{
                    vector<string> labels = p.getDataNames();

                    auto start = chrono::high_resolution_clock::now();
                    for(unsigned long i = 0; i < labels.size(); i++){
                        if(m_StreamDictionary.count(labels.at(i)) == 0){
                            //not found, create stream and add to map
                            boost::filesystem::path folderPath(this->m_FolderPath);
                            boost::filesystem::path name(labels[i] + ".bin");
                            boost::filesystem::path fullpath = folderPath / name;

                        }else{
                            //found

                        }
                    }

                    auto stop = chrono::high_resolution_clock::now();

                    std::chrono::duration<double, std::micro> elapsed = stop - start;
                    this->sig_StageTimer(elapsed.count());
                }
            }
        }

        this->sig_SavingFinished(false, "Saving Completed");
    }else{
        string error = "Folder path not set for the Saving Stage";
        this->sig_MessageLogged(error);
        this->sig_SavingFinished(true, error);
    }



    postStage();
}


#endif // SAVINGPIPELINE_IMPL_H
