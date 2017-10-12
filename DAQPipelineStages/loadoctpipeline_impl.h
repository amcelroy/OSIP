#ifndef LOADDATA_IMPL_H
#define LOADDATA_IMPL_H

#include "daqpipelinestages_global.h"
#include "loadoctpipeline.h"

using namespace OSIP;

template<class I>
LoadOCTPipeline<I>::LoadOCTPipeline()
{

}

template<class I>
void LoadOCTPipeline<I>::configureOCTData(string path, OCTConfig *conf){
    _Filepath = path;
    _N = conf->TotalBScans;
    unsigned long dim1 = conf->PointsPerAScan;
    unsigned long dim2 = conf->AScansPerBScan - conf->StartTrim - conf->StopTrim;
    _dim = vector<unsigned long>();
    _dim.push_back(dim1);
    _dim.push_back(dim2);
}

template<class I>
void LoadOCTPipeline<I>::preStage()
{

}

template<class I>
void LoadOCTPipeline<I>::workStage()
{
    ifstream in;
    in.open(_Filepath, ios::in | ios::binary);
    if(in.fail()){
        this->log("Couldn't open file, halting pipeline stage");
        return;
    }

    unsigned long arraySize = 1;
    for(unsigned long ui : _dim){
        arraySize *= ui;
    }

    int I_size = sizeof(I) / sizeof(unsigned char);
    unsigned long bufferSize = arraySize*I_size;

    for(int i = 0; i < _N; i++){
        char* buffer = new char[bufferSize];
        I* recastData = new I[arraySize];

        try{
            //Loop through data and wrap it as payloads
            in.seekg(i*bufferSize, in.beg);
            in.read(buffer, bufferSize);
            //Recast data to make it easier to wrap

            for(int j = 0; j < bufferSize; j+=2){
                recastData[j/2] = (unsigned char)buffer[j] << 8 | (unsigned char)buffer[j + 1];
            }

            Payload<I> p(_dim, recastData);
            this->sendPayload(p);
            p.finished();
            delete[] buffer;
        }catch(...){
            this->log("Error reading from file " + _Filepath);
        }
    }
}

template<class I>
void LoadOCTPipeline<I>::postStage()
{

}

#endif // LOADDATA_IMPL_H
