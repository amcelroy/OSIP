#ifndef LOADDATA_H
#define LOADDATA_H

#include "daqstage.hpp"
#include <fstream>
#include <boost/signals2.hpp>
#include "octlibrary_global.h"
#include <chrono>

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

    class OCTLIBRARYSHARED_EXPORT LoadOCTPipeline : public DAQStage<unsigned short>
    {
    public:
        enum LOAD_STATE { INITIAL, RELOAD, SINGLE_FRAME, STOP, PAUSE };

    private:
        bool m_BufferData = false;

        vector<Payload<unsigned short>> m_BufferedData;

        int m_StartAScan = -1;

        int m_StartBScan = -1;

        int m_StopAScan = -1;

        int m_StopBScan = -1;

        int m_ReadFrame = 0;

        LOAD_STATE m_State = LOAD_STATE::INITIAL;

        vector<unsigned long long> _dim;

        int _N;

        string _Filepath;

        void _initial(){
            ifstream in;
            in.open(_Filepath, ios::in | ios::binary);
            if(in.fail()){
                this->log("Couldn't open file, halting pipeline stage");
                return;
            }

            unsigned long arraySize = 1;
            arraySize *= _dim[0];
            arraySize *= _dim[1];

            int I_size = sizeof(unsigned short) / sizeof(unsigned char);
            unsigned long long bufferSize = arraySize*I_size;

            for(int i = 0; i < _N; i++){
                auto start = chrono::high_resolution_clock::now();

                char* buffer = new char[bufferSize];
                auto recastData = make_shared<vector<unsigned short>>(arraySize);

                try{
                    //Loop through data and wrap it as payloads
                    in.seekg(i*bufferSize, in.beg);
                    in.read(buffer, bufferSize);
                    //Recast data to make it easier to wrap

                    for(unsigned long j = 0; j < bufferSize; j+=2){
                        recastData->data()[j/2] = (unsigned char)buffer[j] << 8 | (unsigned char)buffer[j + 1];
                    }

                    _dim[2] = i;
                    Payload<unsigned short> p(_dim, recastData, "Loaded BScan");
                    this->sendPayload(p);

                    if(m_BufferData){
                        m_BufferedData.push_back(p);
                    }

                    p.finished();

                    auto stop = chrono::high_resolution_clock::now();
                    std::chrono::duration<double, std::micro> elapsed = stop - start;
                    d_ThreadWorkTime = elapsed.count();
                    this->sig_StageTimer(elapsed.count());
                }catch(...){
                    this->log("Error reading from file " + _Filepath);
                }

                delete[] buffer;

                this->sig_CurrentFrame(i);
            }

            m_State = LOAD_STATE::SINGLE_FRAME;
        }

        /**
         * @brief readFrame Reads a single frame and puts it on the outlet
         * @param frameNumber
         */
        void _readFrame(){
            if(m_BufferData){
                if(m_BufferedData.size() == 0){
                    this->sig_MessageLogged("No data is buffered");
                }else{
                    if(m_ReadFrame < m_BufferedData.size() && m_ReadFrame >= 0){
                        this->sendPayload(m_BufferedData.at(m_ReadFrame));
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

            int I_size = sizeof(unsigned short) / sizeof(unsigned char);
            unsigned long bufferSize = arraySize*I_size;

            char* buffer = new char[bufferSize];
            auto recastData = make_shared<vector<unsigned short>>(arraySize);

            try{
                //Loop through data and wrap it as payloads
                in.seekg(m_ReadFrame*bufferSize, in.beg);
                in.read(buffer, bufferSize);
                //Recast data to make it easier to wrap

                for(unsigned long j = 0; j < bufferSize; j+=2){
                    recastData->data()[j/2] = (unsigned char)buffer[j] << 8 | (unsigned char)buffer[j + 1];
                }

                _dim[2] = m_ReadFrame;
                Payload<unsigned short> p(_dim, recastData, "Loaded BScan");
                this->sendPayload(p);
                p.finished();
            }catch(...){
                this->log("Error reading from file " + _Filepath);
            }

            delete[] buffer;

            this->sig_CurrentFrame(m_ReadFrame);
        }

        void _reload(){
            int start = 0;
            if(m_StartBScan != -1 && m_StartBScan < _N){
                start = m_StartBScan;
            }

            int stop = _N;
            if(m_StopBScan > start && m_StartBScan < _N){
                stop = m_StopBScan;
            }

            for(int i = start; i < stop; i++){
                m_ReadFrame = i;
                _readFrame();
            }
        }
    protected:
        void configureDAQ(DAQParameters daqp) override { }

    public:
        LoadOCTPipeline() { }

        void stop() override {

        }

        void acquire() override {

        }

        void reset() override {

        }

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

        void readFrame(int frameNumber){
            m_State = LOAD_STATE::SINGLE_FRAME;
            m_ReadFrame = frameNumber;
        }

        void reload(){
            m_State = LOAD_STATE::RELOAD;
        }

        int getNumberOfFrames(){
            return _N;
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
            while(m_State != LOAD_STATE::STOP){
                switch(m_State){
                case LOAD_STATE::INITIAL:
                    _initial();
                    m_State = LOAD_STATE::PAUSE;
                    break;

                case LOAD_STATE::SINGLE_FRAME:
                    _readFrame();
                    m_State = LOAD_STATE::PAUSE;
                    break;

                case LOAD_STATE::RELOAD:
                    _reload();
                    m_State = LOAD_STATE::PAUSE;
                    break;

                case LOAD_STATE::PAUSE:
                    this->pipelineSleep(5);
                    break;
                }
            }


            Payload<unsigned short> p(vector<unsigned long long>(0), nullptr, "Empty Frame");
            this->sendPayload(p);

            this->sig_DAQFinished();

//            if(this->m_Loop == true){
//                goto loop;
//            }
        }

        void postStage() override { }

        bool init() override { return true; }
    };
}

#endif // LOADDATA_H
