#include "OCTBackend.h"

OCTBackend::OCTBackend()
{

}

void OCTBackend::loadOCT(string path){
    m_octcf.readOCTConfig(path + "parameters.oct_scan", &m_octc);

    m_OCTPipeline.getLoader()->configureOCTData(path + "data.bin", &m_octc);

    m_OCTPipeline.start(m_octc);
}

void OCTBackend::record(){
    //Show record save folder
    //Save DAQ files
    //Save galvo files
    //Save toggle lossless recording
}

