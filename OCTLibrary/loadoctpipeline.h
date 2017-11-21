#ifndef LOADDATA_H
#define LOADDATA_H

#include "Pipeline/daqstage.h"
#include <fstream>
#include <boost/signals2.hpp>

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
    private:
        bool m_BufferData = false;

        vector<Payload<I>> m_BufferedData;

        int m_StartAScan = -1;

        int m_StartBScan = -1;

        int m_StopAScan = -1;

        int m_StopBScan = -1;

    public:
        LoadOCTPipeline();

        void configureOCTData(string path, OCTConfig *conf);

        void setBufferData(bool YesNo) { m_BufferData = YesNo; }

        /**
         * @brief readFrame Reads a single frame and puts it on the outlet
         * @param frameNumber
         */
        void readFrame(int frameNumber);

        void reload();

        void setBounds(int startAScan, int stopAScan, int startBScan, int stopBScan) {
            m_StartAScan = startAScan;
            m_StartBScan = startBScan;
            m_StopBScan = stopBScan;
            m_StopAScan = stopAScan;
        }
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
