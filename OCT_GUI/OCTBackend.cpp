#include "OCTBackend.h"

OCTBackend::OCTBackend()
{

}

void OCTBackend::loadOCT(string path){
    setMode(OCTBackend::MODE_LOADING);

    m_octcf.readOCTConfig(path + "parameters.oct_scan", &m_octc);

    m_OCTPipeline.getLoader()->configureOCTData(path + "data.bin", &m_octc);

    m_OCTPipeline.start(m_octc);

//    m_OCTPipeline.getLoader()->subscribeDAQFinished(boost::bind(&OCTBackend::onDaqFinished, this));

    m_OCTPipeline.getLoader()->subscribeDAQFinished(boost::bind(&OCTBackend::onProcessingFinished, this));
}

void OCTBackend::record(){
    //Show record save folder
    //Save DAQ files
    //Save galvo files
    //Save toggle lossless recording
}

void OCTBackend::reprocessEnFace(){
    setMode(OCTBackend::MODE_LOADING);
    //TODO: Add threading during reload
    m_OCTPipeline.getLoader()->reload();
}

void OCTBackend::onFullField(){
    if(m_Mode == OCTBackend::MODE_REVIEW){
        m_OCTPipeline.getLoader()->setBounds(-1, -1, -1, -1);
    }
}

QString OCTBackend::getBenchmarks(){
    string s_results = "";

    try{
        double LoaderTime = m_OCTPipeline.getLoader()->getThreadWorkTimeMicroSeconds();
        double ProcessTime = m_OCTPipeline.getProcessor()->getThreadWorkTimeMicroSeconds();
        double DisplayTime = m_OCTPipeline.getDisplay()->getThreadWorkTimeMicroSeconds();

        s_results = "Load Time: " + to_string(LoaderTime/1000) + "ms \r\n" +
                    "Process Time: " + to_string(ProcessTime/1000) + "ms \r\n" +
                    "Display Time: " + to_string(DisplayTime/1000) + "ms \r\n";
    }catch(...){

    }

    return QString(s_results.c_str());
}

void OCTBackend::enFaceSliderChanged(QVariant depth1, QVariant depth2){
    int low = min(depth1.toInt(), depth2.toInt());
    int high = max(depth1.toInt(), depth2.toInt());

    m_OCTPipeline.getProcessor()->setEnfaceRange(low, high);
}

void OCTBackend::enFaceSelectionBoundsChanged(QVariant xPixels,
                                              QVariant yPixels,
                                              QVariant selectionWidthPixels,
                                              QVariant selectionHeightPixels,
                                              QVariant canvasWidthPixels,
                                              QVariant canvasHeightPixels){
    //Convert pixels to percentage
    float xStart = xPixels.toFloat() / canvasWidthPixels.toFloat();
    float yStart = yPixels.toFloat() / canvasHeightPixels.toFloat();
    float width = selectionWidthPixels.toFloat() / canvasWidthPixels.toFloat();
    float height = selectionHeightPixels.toFloat() / canvasHeightPixels.toFloat();

    if(m_Mode == OCTBackend::MODE_REVIEW){
        int startAScan = xStart*m_octc.AScansPerBScan;
        int stopAScan = startAScan + width*m_octc.AScansPerBScan;
        int startBScan = yStart*m_octc.TotalBScans;
        int stopBScan = startBScan + height*m_octc.TotalBScans;
        m_OCTPipeline.getLoader()->setBounds(startAScan, stopAScan, startBScan, stopBScan);
    }
}

