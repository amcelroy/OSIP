#ifndef NODAQSTAGE_H
#define NODAQSTAGE_H

#include "octlibrary_global.h"
#include "Pipeline/daqstage_impl.h"

namespace OSIP {
    class OCTLIBRARYSHARED_EXPORT NoDAQStage : public OSIP::DAQStage<unsigned short>
    {
    public:
        NoDAQStage();

        static const unsigned long long WIDTH = 1024;

        static const unsigned long long HEIGHT = 1024;
    private:
        void preStage();

        void workStage();

        void postStage();

    };
}

#endif // NODAQSTAGE_H
