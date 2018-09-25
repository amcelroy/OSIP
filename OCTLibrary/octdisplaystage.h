#ifndef OCTDISPLAYSTAGE_H
#define OCTDISPLAYSTAGE_H

#include "pipeline.hpp"
#include <boost/signals2.hpp>
#include <octconfigfile.h>

namespace OSIP {

    class OCTDisplayStage : public DisplayPipelineStage<float>
    {
    private:
        vector<unsigned char> m_bscan_8bit;

        vector<unsigned char> m_enface_8bit;

        mutex m_BScanAccessMutex;

        mutex m_EnFaceAccessMutex;

    public:
        OCTDisplayStage() {

        }

        void configure(const OCTConfig& config){
            int width = config.AScansPerBScan - config.StartTrim - config.StopTrim;
            int height = config.TotalBScans;

            m_enface_8bit.resize(static_cast<unsigned long>(width*height));
        }

        vector<unsigned char> getLastBScan8Bit(){
            lock_guard<mutex> lock(m_BScanAccessMutex);
            return m_bscan_8bit;
        }

        vector <unsigned char> getLastEnFace8Bit(){
            lock_guard<mutex> lock(m_EnFaceAccessMutex);
            return m_enface_8bit;
        }
    protected:
        void work() override;

        void preStage() override;

        void postStage() override;

    };
}

#endif // OCTDISPLAYSTAGE_H
