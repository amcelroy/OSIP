#ifndef LOADDATA_H
#define LOADDATA_H

#include "Pipeline/daqstage.h"
#include <fstream>

using namespace std;

namespace OSIP {
    struct OCTConfig {
        int PointsPerAScan;
        int AScansPerBScan;
        int TotalBScans;
        float Gain;
        float Bias;
        float Range;
        int Bits;
        int StopTrim;
        int StartTrim;
    };

    template<class I>
    class LoadOCTPipeline : public DAQStage<I>
    {
    public:
        LoadOCTPipeline();

        void configureOCTData(string path, OCTConfig *conf);

    protected:
        void preStage();

        void workStage();

        void postStage();

        vector<unsigned long> _dim;

        int _N;

        string _Filepath;
    };
}

#endif // LOADDATA_H
