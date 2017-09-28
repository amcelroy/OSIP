#ifndef NODAQSTAGE_H
#define NODAQSTAGE_H

#include "daqpipelinestages_global.h"
#include "Pipeline/daqstage_impl.h"

class DAQPIPELINESTAGESSHARED_EXPORT NoDAQStage : public DAQStage<unsigned short>
{
public:
    NoDAQStage();

    static const int WIDTH = 1024;

    static const int HEIGHT = 1024;
private:
    void preStage();

    void workStage();

    void postStage();

};

#endif // NODAQSTAGE_H
