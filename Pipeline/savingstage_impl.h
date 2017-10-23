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
void SavingStage<I>::postStage(){
    sig_SavingFinished();
}

template<class I>
void SavingStage<I>::toBinaryUInt16(string path, unsigned short* data, unsigned long N, boost::endian::order endian){

}

template<class I>
void SavingStage<I>::toBinaryFloat(string path, float* data, unsigned long N, boost::endian::order endian){

}

template<class I>
void SavingStage<I>::toBinaryDouble(string path, double* data, unsigned long N, boost::endian::order endian){

}

template<class I>
void SavingStage<I>::toBinaryInt16(string path, short* data, unsigned long N, boost::endian::order endian){

}

template<class I>
void SavingStage<I>::toBinaryUInt32(string path, unsigned int* data, unsigned long N, boost::endian::order endian){

}

template<class I>
void SavingStage<I>::toBinaryInt32(string path, int* data, unsigned long N, boost::endian::order endian){

}

template<class I>
void SavingStage<I>::toBinaryU8(string path, unsigned char* data, unsigned long N){

}

template<class I>
void SavingStage<I>::toBinaryChar(string path, char* data, unsigned long N){

}

template<class I>
void SavingStage<I>::_saveData(string path, void *data, unsigned long NBytes){
    try{
        //Open file
        std::ofstream ofs;
        ofs.open(path, std::ofstream::binary | std::ofstream::app);

        //Save data
        ofs.write(data, NBytes);

    }catch(exception e){
        this->sig_MessageLogged("Error saving file - ");
    }
}

#endif // SAVINGPIPELINE_IMPL_H
