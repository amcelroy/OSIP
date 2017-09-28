#include <QCoreApplication>

#include "ProcessingPipelineStages/octpipelinestage_cpu.h"
#include "DAQPipelineStages/nodaqstage.h"
#include "DAQPipelineStages/loaddata.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    OCTPipelineStageCPU octProc(.1, 0, 1024, 1024);
    LoadData<unsigned short> loadOCT;


    //shared_ptr<Inlet<unsigned char>> t = fft.getInlet();

    loadOCT.connect(octProc.getInlet());

    loadOCT.start();
    octProc.start();

    return a.exec();
}
