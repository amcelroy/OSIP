#ifndef OCTDISPLAYSTAGE_H
#define OCTDISPLAYSTAGE_H

#include "pipeline.hpp"
#include <QObject>
#include <QVector>
#include <boost/signals2.hpp>
#include "octlibrary_global.h"

namespace OSIP {

    class OCTLIBRARYSHARED_EXPORT OCTDisplayStage : public DisplayPipelineStage<float>
    {
    private:
        vector<unsigned char> m_bscan_8bit;

        vector<unsigned char> m_enface_8bit;

        mutex m_BScanAccessMutex;

        mutex m_EnFaceAccessMutex;

    public:
        OCTDisplayStage() { }

        vector<unsigned char>* getLastBScan8Bit(){
            lock_guard<mutex> lock(m_BScanAccessMutex);
            return &m_bscan_8bit;
        }

    protected:
        void work() override;

        void preStage() override;

        void postStage() override;

    };
}

#endif // OCTDISPLAYSTAGE_H
