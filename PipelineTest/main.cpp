#include <QCoreApplication>

#include "ProcessingPipelineStages/octpipelinestage_cpu.h"
#include "DAQPipelineStages/nodaqstage.h"
#include "DAQPipelineStages/loadoctpipeline.h"
#include "OCT_GUI/octconfigfile.h"

using namespace OSIP;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    OCTConfigFile octConfigFile;
    LoadOCTPipeline<unsigned short> loadOCT;
    OCTPipelineStageCPU octProc;
    OCTConfig octConfig;

    string path = "/Users/amcelroy/Code/OSIP/test_data/";

    octConfigFile.readOCTConfig(path + "parameters.oct_scan", &octConfig);

    octProc.configure(octConfig);
    loadOCT.configureOCTData(path + "data.bin", &octConfig);

    loadOCT.connect(octProc.getInlet());

    loadOCT.start();
    octProc.start();


    return a.exec();
}
