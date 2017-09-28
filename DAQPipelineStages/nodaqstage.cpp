#include "nodaqstage.h"

NoDAQStage::NoDAQStage()
{

}

void NoDAQStage::preStage(){

}

void NoDAQStage::workStage(){

    int i = 0;
    while(!stopThread){
        //make some data
        unsigned short *buff = new unsigned short[WIDTH*HEIGHT];

        memset(buff, i, WIDTH*HEIGHT);

        //send data
        Payload<unsigned short> p(vector<unsigned int> {WIDTH, HEIGHT}, buff);
        sendPayload(p);

        p.finished();
        i++;

        pipelineSleep(10);
    }
}

void NoDAQStage:: postStage(){

}
