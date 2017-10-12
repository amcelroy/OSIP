#include "mainbackend.h"

mainbackend::mainbackend()
{

}

void mainbackend::loadOCT(string path){
    m_octcf.readOCTConfig(path + "parameters.oct_scan", &m_octc);

    m_OCTPipeline.getLoader()->configureOCTData(path + "data.bin", &m_octc);

    m_OCTPipeline.start(m_octc);
}
