#include <QCoreApplication>

#include "ProcessingPipelineStages/octpipelinestage_cpu.h"
#include "DAQPipelineStages/nodaqstage.h"
#include "DAQPipelineStages/loadoctpipeline.h"

using namespace OSIP;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    OCTPipelineStageCPU octProc(4/(powf(2, 12)), 2, 1376, 512);
    LoadOCTPipeline<unsigned short> loadOCT;
    loadOCT.open("/Users/amcelroy/Code/OSIP/test_data/data.bin", 512, vector<unsigned int>() = {512, 1376});


    //shared_ptr<Inlet<unsigned char>> t = fft.getInlet();

    loadOCT.connect(octProc.getInlet());

    loadOCT.start();
    octProc.start();

    return a.exec();
}
