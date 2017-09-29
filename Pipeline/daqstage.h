#ifndef DAQSTAGE_H
#define DAQSTAGE_H

#include "pipeline_global.h"
#include "pipelinestage.h"

namespace OSIP {
    template<class O>
    class DAQStage : public PipelineStage<O, O>
    {
    public:
        DAQStage();
    };
}
#endif // DAQSTAGE_H
