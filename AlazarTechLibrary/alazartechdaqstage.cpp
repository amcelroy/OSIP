#include "alazartechdaqstage.h"


AlazarTechDAQStage::AlazarTechDAQStage()
{
}

void AlazarTechDAQStage::configureDAQ(OSIP::DAQParameters daqp){

    U32 _clock = INTERNAL_CLOCK;
    U32 _clockRate = SAMPLE_RATE_125MSPS;

    if(daqp.ClockSource == OSIP_EXTERNAL_CLOCK){
        _clock = FAST_EXTERNAL_CLOCK;
        _clockRate = SAMPLE_RATE_USER_DEF;
    }

    U8 _channel = CHANNEL_A;

    switch(daqp.Channels){
    case OSIP_CHANNELS::B:
        _channel = CHANNEL_B;
        break;
    case OSIP_CHANNELS::C:
        _channel = CHANNEL_C;
        break;
    case  OSIP_CHANNELS::C:
        _channel = CHANNEL_C;
        break;
    case OSIP_CHANNELS::D:
        _channel = CHANNEL_D;
        break;
    case OSIP_CHANNELS::ALL:
        _channel = CHANNEL_ALL;
        break;
    }

    U32 _triggerSource = TRIG_CHAN_A;

    if(daqp.Trigger == OSIP_EXTERNAL_TRIGGER){
        _triggerSource = TRIG_EXTERNAL;
    }

    double _timeout = 50e-6;

    AlazarSetCaptureClock(h_Board, _clock, daqp.DAQClockRate, _clockRate, 0);
    AlazarInputControl(h_Board, _channel, AC_COUPLING, INPUT_RANGE_PM_800_MV, IMPEDANCE_50_OHM);
    AlazarSetTriggerOperation(h_Board,
                              TRIG_ENGINE_OP_J,
                              TRIG_ENGINE_J,
                              _triggerSource,
                              TRIGGER_SLOPE_POSITIVE,
                              160,
                              TRIG_ENGINE_K,
                              TRIG_DISABLE,
                              TRIGGER_SLOPE_POSITIVE,
                              128);
    AlazarSetTriggerTimeOut(h_Board, _timeout/10e-6);
    AlazarConfigureAuxIO(h_Board, AUX_OUT_TRIGGER, 0);
}

void AlazarTechDAQStage::work(){

}

void AlazarTechDAQStage::stop(){

}

void AlazarTechDAQStage::reset(){

}

void AlazarTechDAQStage::acquire(){

}

bool AlazarTechDAQStage::init(){
    U32 boardID = 1;
    U32 systemID = 1;
    U32 systemCount = AlazarNumOfSystems();

    if(systemCount < 1){
        log("No Alazart boards found, install drivers?");
    }

    for (U32 systemId = 1; systemId <= systemCount; systemId++)
    {
        U32 boardCount = AlazarBoardsInSystemBySystemID(systemId);
        for (U32 boardId = 1; boardId <= boardCount; boardId++)
        {
            char* buffer = new char[256];
            sprintf(buffer, "Found SystemID %u Board ID = %u \r\n", systemID, boardID);
            log(string(buffer));
            delete[] buffer;
        }
    }

    h_Board = AlazarGetBoardBySystemID(systemID, boardID);

    log("Alazar board selected, ready to configure");

    return true;
}
