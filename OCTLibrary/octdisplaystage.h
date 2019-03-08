#ifndef OCTDISPLAYSTAGE_H
#define OCTDISPLAYSTAGE_H

#include "pipeline.hpp"
#include <boost/signals2.hpp>
#include <octconfigfile.h>
#include <png.h>

namespace OSIP {

    class OCTDisplayStage : public DisplayPipelineStage<float>
    {
    private:
        vector<unsigned char> m_bscan_8bit;

        vector<unsigned char> m_enface_8bit;

		vector<unsigned char> m_bscan_png;

		vector<unsigned char> m_enface_png;

        mutex m_BScanAccessMutex;

        mutex m_EnFaceAccessMutex;

		png_structp m_bscan_ptr;

		png_structp m_enface_ptr;

		png_infop m_bscan_info_ptr;

		png_infop m_enface_info_ptr;

		bool _initialize_PNG = true;

		void _writePNG(png_structp png, png_infop info, int w, int h, vector<unsigned char>& scaledImage, vector<unsigned char> *pngBuffer);

    public:
        OCTDisplayStage() {

        }

        void configure(const OCTConfig& config){
            int width = config.AScansPerBScan - config.StartTrim - config.StopTrim;
            int height = config.TotalBScans;

			m_enface_png.resize(static_cast<unsigned long>(width*height));

            m_enface_8bit.resize(static_cast<unsigned long>(width*height));
        }

        vector<unsigned char> getLastBScan8Bit(){
            lock_guard<mutex> lock(m_BScanAccessMutex);
            return m_bscan_png;
        }

        vector <unsigned char> getLastEnFace8Bit(){
            lock_guard<mutex> lock(m_EnFaceAccessMutex);
            return m_enface_png;
        }
    protected:
        void work() override;

        void preStage() override;

        void postStage() override;

    };
}

#endif // OCTDISPLAYSTAGE_H
