#ifndef SAVINGPIPELINE_IMPL_H
#define SAVINGPIPELINE_IMPL_H

#include "savingstage.h"

using namespace OSIP;

template<class I>
SavingStage<I>::SavingStage(){
    //Set system endianess first;
    //https://stackoverflow.com/questions/1001307/detecting-endianness-programmatically-in-a-c-program
    if ( htonl(47) == 47 ) {
      // Big endian
        m_SystemEndianess = boost::endian::order::big;
    } else {
      // Little endian.
        m_SystemEndianess = boost::endian::order::little;
    }
}

template<class I>
bool SavingStage<I>::createFile(string path){
    bool fileCreated = false;

    m_FileStream.open(path, std::ofstream::binary | std::ofstream::app);

    if(m_FileStream.is_open()){
        fileCreated = true;
        this->sig_MessageLogged("File created: " + path);
    }else{
        this->sig_MessageLogged("Failed to create file: " + path);
        return false;
    }
}

template<class I>
void SavingStage<I>::toBinaryUInt16(unsigned short* data, unsigned long N, boost::endian::order endian){
    if(m_SystemEndianess == endian){
        //save normally
        _saveData(data, sizeof(unsigned short)*N);
    }else{
        int elements = N / sizeof(unsigned short);
        for(int i = 0; i < elements; i++){
            boost::endian::endian_reverse_inplace(data[i]);
        }
        _saveData(data, sizeof(unsigned short)*N);
    }
}

template<class I>
void SavingStage<I>::toBinaryFloat(float* data, unsigned long N, boost::endian::order endian){
    if(m_SystemEndianess == endian){
        //save normally
        _saveData(data, sizeof(float)*N);
    }else{
        int elements = N / sizeof(float);
        for(int i = 0; i < elements; i++){
            boost::endian::endian_reverse_inplace(data[i]);
        }
        _saveData(data, sizeof(float)*N);
    }
}

template<class I>
void SavingStage<I>::toBinaryDouble(double* data, unsigned long N, boost::endian::order endian){
    if(m_SystemEndianess == endian){
        //save normally
        _saveData(data, sizeof(double)*N);
    }else{
        int elements = N / sizeof(double);
        for(int i = 0; i < elements; i++){
            boost::endian::endian_reverse_inplace(data[i]);
        }
        _saveData(data, sizeof(double)*N);
    }
}

template<class I>
void SavingStage<I>::toBinaryInt16(short* data, unsigned long N, boost::endian::order endian){
    if(m_SystemEndianess == endian){
        //save normally
        _saveData(data, sizeof(short)*N);
    }else{
        int elements = N / sizeof(short);
        for(int i = 0; i < elements; i++){
            boost::endian::endian_reverse_inplace(data[i]);
        }
        _saveData(data, sizeof(short)*N);
    }
}

template<class I>
void SavingStage<I>::toBinaryUInt32(unsigned int* data, unsigned long N, boost::endian::order endian){
    if(m_SystemEndianess == endian){
        //save normally
        _saveData(data, sizeof(unsigned int)*N);
    }else{
        int elements = N / sizeof(unsigned int);
        for(int i = 0; i < elements; i++){
            boost::endian::endian_reverse_inplace(data[i]);
        }
        _saveData(data, sizeof(unsigned int)*N);
    }
}

template<class I>
void SavingStage<I>::toBinaryInt32(int* data, unsigned long N, boost::endian::order endian){
    if(m_SystemEndianess == endian){
        //save normally
        _saveData(data, sizeof(int)*N);
    }else{
        int elements = N / sizeof(int);
        for(int i = 0; i < elements; i++){
            boost::endian::endian_reverse_inplace(data[i]);
        }
        _saveData(data, sizeof(int)*N);
    }
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
void SavingStage<I>::toBinaryInt64(long* data, unsigned long N, boost::endian::order endian){
    if(m_SystemEndianess == endian){
        //save normally
        _saveData(data, sizeof(long)*N);
    }else{
        int elements = N / sizeof(long);
        for(int i = 0; i < elements; i++){
            boost::endian::endian_reverse_inplace(data[i]);
        }
        _saveData(data, sizeof(long)*N);
    }
}

template<class I>
void SavingStage<I>::toBinaryUInt64(unsigned long* data, unsigned long N, boost::endian::order endian){
    if(m_SystemEndianess == endian){
        //save normally
        _saveData(data, sizeof(unsigned long)*N);
    }else{
        int elements = N / sizeof(unsigned long);
        for(int i = 0; i < elements; i++){
            boost::endian::endian_reverse_inplace(data[i]);
        }
        _saveData(data, sizeof(unsigned long)*N);
    }
}

template<class I>
void SavingStage<I>::_saveData(void *data, unsigned long NBytes){
    try{
        //Save data
        m_FileStream.write((char*)data, NBytes);
    }catch(exception e){
        this->sig_MessageLogged("Error saving file - ");
    }
}

template<class I>
void SavingStage<I>::preStage(){

}

template<class I>
void SavingStage<I>::postStage(){

}

template<class I>
void SavingStage<I>::workStage(){
    if(m_FileStream.is_open()){
//        while(!this->stopThread){
//            if(this->pauseThread){
//                this->pipelineSleep(10);
//            }else{
//                //dequeue data
//                Payload<int> p = this->fetchPayload();

//                if(!p.isValid()){
//                    this->pipelineSleep(10);
//                }else{
//                    auto start = chrono::high_resolution_clock::now();



//                    auto stop = chrono::high_resolution_clock::now();

//                    std::chrono::duration<double, std::micro> elapsed = stop - start;
//                    this->sig_StageTimer(elapsed.count());
//                }
//            }
//        }
    }

    this->sig_SavingFinished();

    postStage();
}


#endif // SAVINGPIPELINE_IMPL_H
