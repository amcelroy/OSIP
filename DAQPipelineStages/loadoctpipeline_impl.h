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
void LoadOCTPipeline<I>::open(string FilenamePath, int N, vector<unsigned int> dim)
{
    _Filepath = FilenamePath;
    _N = N;
    _dim = dim;
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

    long bufferSize = 1;
    for(unsigned int ui : _dim){
        bufferSize *= ui;
    }

    int I_size = sizeof(I);
    bufferSize *= I_size;

    for(int i = 0; i < _N; i++){
        char* buffer = new char[bufferSize];

        try{
            //Loop through data and wrap it as payloads
            in.read(buffer, bufferSize);
            //Recast data to make it easier to wrap
            I* recastData = (I*)buffer;

            Payload<I> p(_dim, recastData);
            this->sendPayload(p);
            p.finished();
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
