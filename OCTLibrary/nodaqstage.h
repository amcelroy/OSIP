#ifndef NODAQSTAGE_H
#define NODAQSTAGE_H

#include "pipeline.hpp"

namespace OSIP {
    class NoDAQStage : public OSIP::DAQStage<unsigned short>
    {
    public:
        NoDAQStage();

        static const unsigned long long WIDTH = 1024;

        static const unsigned long long HEIGHT = 1024;
    private:
        void preStage() { }

        void work() override {
            int i = 0;
            while(!stopThread){
                //make some data
                auto buff = make_shared<vector<unsigned short>>(WIDTH*HEIGHT);

                memset(buff.get(), i, WIDTH*HEIGHT);

                //send data
                Payload<unsigned short> p(vector<unsigned long long> {WIDTH, HEIGHT}, buff, "DAQ Data");
                sendPayload(p);

                p.finished();
                i++;

                pipelineSleep(10);
            }
        }

        void postStage() { }

    };
}

#endif // NODAQSTAGE_H
