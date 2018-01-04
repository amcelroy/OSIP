#ifndef LOADDATA_H
#define LOADDATA_H

#include "daqstage.hpp"
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
        LoadOCTPipeline() { }

        void configureOCTData(string path, OCTConfig *conf){
            _Filepath = path;
            _N = conf->TotalBScans;
            unsigned long dim1 = conf->PointsPerAScan;
            unsigned long dim2 = conf->AScansPerBScan - conf->StartTrim - conf->StopTrim;
            _dim = vector<unsigned long long>();
            _dim.push_back(dim1); //Points in A-Scan
            _dim.push_back(dim2); //A-Scans per B-Scan
            _dim.push_back(0); //current frame
        }

        void setBufferData(bool YesNo) { m_BufferData = YesNo; }

        /**
         * @brief readFrame Reads a single frame and puts it on the outlet
         * @param frameNumber
         */
        void readFrame(int frameNumber){
            if(m_BufferData){
                if(m_BufferedData.size() == 0){
                    this->sig_MessageLogged("No data is buffered");
                }else{
                    if(frameNumber < m_BufferedData.size() && frameNumber > 0){
                        this->sendPayload(m_BufferedData.at(frameNumber));
                    }else{
                        this->sig_MessageLogged("Requested frame is not buffered");
                    }
                }

                return;
            }

            ifstream in;
            in.open(_Filepath, ios::in | ios::binary);
            if(in.fail()){
                this->log("Couldn't open file, halting pipeline stage");
                return;
            }

            unsigned long arraySize = 1;
            arraySize *= _dim[0];
            arraySize *= _dim[1];

            int I_size = sizeof(I) / sizeof(unsigned char);
            unsigned long bufferSize = arraySize*I_size;

            char* buffer = new char[bufferSize];
            auto recastData = make_shared<vector<I>>(arraySize);

            try{
                //Loop through data and wrap it as payloads
                in.seekg(frameNumber*bufferSize, in.beg);
                in.read(buffer, bufferSize);
                //Recast data to make it easier to wrap

                for(unsigned long j = 0; j < bufferSize; j+=2){
                    recastData->data()[j/2] = (unsigned char)buffer[j] << 8 | (unsigned char)buffer[j + 1];
                }

                _dim[2] = frameNumber;
                Payload<I> p(_dim, recastData, "Loaded BScan");
                this->sendPayload(p);
                p.finished();
            }catch(...){
                this->log("Error reading from file " + _Filepath);
            }

            delete[] buffer;

            this->sig_CurrentFrame(frameNumber);
        }

        void reload(){
            int start = 0;
            if(m_StartBScan != -1 && m_StartBScan < _N){
                start = m_StartBScan;
            }

            int stop = _N;
            if(m_StopBScan > start && m_StartBScan < _N){
                stop = m_StopBScan;
            }

            for(int i = start; i < stop; i++){
                readFrame(i);
            }
        }

        void setBounds(int startAScan, int stopAScan, int startBScan, int stopBScan) {
            m_StartAScan = startAScan;
            m_StartBScan = startBScan;
            m_StopBScan = stopBScan;
            m_StopAScan = stopAScan;
        }
    protected:
        void preStage() override{ }

        void work() override{
            ifstream in;
            in.open(_Filepath, ios::in | ios::binary);
            if(in.fail()){
                this->log("Couldn't open file, halting pipeline stage");
                return;
            }

            unsigned long arraySize = 1;
            arraySize *= _dim[0];
            arraySize *= _dim[1];

            int I_size = sizeof(I) / sizeof(unsigned char);
            unsigned long long bufferSize = arraySize*I_size;

        loop:
            for(int i = 0; i < _N; i++){
                char* buffer = new char[bufferSize];
                auto recastData = make_shared<vector<I>>(arraySize);

                try{
                    //Loop through data and wrap it as payloads
                    in.seekg(i*bufferSize, in.beg);
                    in.read(buffer, bufferSize);
                    //Recast data to make it easier to wrap

                    for(unsigned long j = 0; j < bufferSize; j+=2){
                        recastData->data()[j/2] = (unsigned char)buffer[j] << 8 | (unsigned char)buffer[j + 1];
                    }

                    _dim[2] = i;
                    Payload<I> p(_dim, recastData, "Loaded BScan");
                    this->sendPayload(p);

                    if(m_BufferData){
                        m_BufferedData.push_back(p);
                    }

                    p.finished();
                }catch(...){
                    this->log("Error reading from file " + _Filepath);
                }

                delete[] buffer;

                this->sig_CurrentFrame(i);
            }

            Payload<I> p(vector<unsigned long long>(0), nullptr, "Empty Frame");
            this->sendPayload(p);

            this->sig_DAQFinished();

            if(this->m_Loop == true){
                goto loop;
            }
        }

        void postStage() override { }

        vector<unsigned long long> _dim;

        int _N;

        string _Filepath;
    };
}

#endif // LOADDATA_H
