#ifndef SAVINGPIPELINE_H
#define SAVINGPIPELINE_H

#include "pipeline_global.h"
#include "pipelinestage.h"
#include "boost/signals2.hpp"

using namespace std;

namespace OSIP {
    template<class I>
    class PIPELINESHARED_EXPORT SavingStage : public PipelineStage<I, I>
    {
        boost::signals2::signal<void()> sig_SavingFinished;

    public:
        SavingStage();

    protected:
        virtual void postStage();
    };
}

#endif // SAVINGPIPELINE_H
