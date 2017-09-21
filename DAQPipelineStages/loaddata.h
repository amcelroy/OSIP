#ifndef LOADDATA_H
#define LOADDATA_H

#include "Pipeline/daqstage.h"
#include <fstream>

using namespace std;

template<class I>
class LoadData : public DAQStage<I>
{
public:
    LoadData();

    void open(string FilenamePath, int N, vector<unsigned int> dim);

protected:
    void preStage();

    void stageThread();

    void postStage();

    vector<unsigned int> _dim;

    int _N;

    string _Filepath;
};

#endif // LOADDATA_H
