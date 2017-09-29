#ifndef LOADDATA_H
#define LOADDATA_H

#include "Pipeline/daqstage.h"
#include <fstream>

using namespace std;

namespace OSIP {
    template<class I>
    class LoadOCTPipeline : public DAQStage<I>
    {
    public:
        LoadOCTPipeline();

        void open(string FilenamePath, int N, vector<unsigned int> dim);

    protected:
        void preStage();

        void workStage();

        void postStage();

        vector<unsigned int> _dim;

        int _N;

        string _Filepath;
    };
}

#endif // LOADDATA_H
