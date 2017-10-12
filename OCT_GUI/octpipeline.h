#ifndef OCTPIPELINE_H
#define OCTPIPELINE_H

#include "DAQPipelineStages/loadoctpipeline.h"
#include "ProcessingPipelineStages/octpipelinestage_cpu.h"
#include "octdisplaystage.h"

class OCTPipeline
{
private:
    shared_ptr<OSIP::LoadOCTPipeline<unsigned short>> _Loader;

    shared_ptr<OSIP::OCTPipelineStageCPU> _Processor;

    shared_ptr<OSIP::OCTDisplayStage> _Display;

public:
    OCTPipeline();

    OSIP::LoadOCTPipeline<unsigned short>* getLoader() { return _Loader.get(); }

    OSIP::OCTPipelineStageCPU* getProcessor() { return _Processor.get(); }

    OSIP::OCTDisplayStage* getDisplay() { return _Display.get(); }

    void start(OCTConfig config);


};

#endif // OCTPIPELINE_H
