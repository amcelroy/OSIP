#ifndef ALAZARTECHDAQSTAGE_H
#define ALAZARTECHDAQSTAGE_H

#include <daqstage.hpp>
#include <AlazarApi.h>

#ifdef _WIN32
#define __EXPORT __declspec(dllexport)
#endif

using namespace OSIP;

namespace OSIP{
    class __EXPORT AlazarTechDAQStage : public OSIP::DAQStage<unsigned short>
    {
    private:
        HANDLE h_Board;

    protected:
        void configureDAQ(OSIP::DAQParameters daqp) override;

    public:
        AlazarTechDAQStage();

        void work() override;

        void reset() override;

        void stop() override;

        void acquire() override;

        void init() override;
    };
}

#endif // ALAZARTECHDAQSTAGE_H
