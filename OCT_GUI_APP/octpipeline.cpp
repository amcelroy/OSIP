#include "octpipeline.h"

OCTPipeline::OCTPipeline()
{
    _Loader = shared_ptr<LoadOCTPipeline<unsigned short>>(new LoadOCTPipeline<unsigned short>());
    _Processor = shared_ptr<OCTPipelineStageCPU>(new OCTPipelineStageCPU());
    _Display = shared_ptr<OCTDisplayStage>(new OCTDisplayStage());

    //Connect all the Inlets
    _Loader->connect(_Processor->getInlet());
    _Processor->connect(_Display->getInlet());
}

void OCTPipeline::start(OCTConfig config){
    _Processor->configure(config);

    _Display->start();
    _Processor->start();
    _Loader->start();
}

