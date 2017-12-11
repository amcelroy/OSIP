#ifndef SAVINGPIPELINE_H
#define SAVINGPIPELINE_H

#include "pipeline_global.h"
#include "pipelinestage.h"
#include "boost/signals2.hpp"
#include "boost/endian/arithmetic.hpp"
#include "boost/endian/conversion.hpp"
#include "boost/filesystem.hpp"
#include "hdf5.h"
#include "H5Cpp.h"
#include "zlib.h"
#include "szlib.h"
#include <fstream>

using namespace std;

namespace OSIP {
    template<class I>
    class PIPELINESHARED_EXPORT SavingStage : public PipelineStage<I, I>
    {
        boost::signals2::signal<void (bool, string)> sig_SavingFinished;

        string m_FolderPath;

        string m_Filename = "";

        bool m_FolderPathSet = false;

        H5::H5File* m_H5File;
    public:
        SavingStage();

        /**
         * @brief setSavePath Sets the folder path to save files in
         * @param path Folder path
         * @return True if the folder path both exists and is a folder, false otherwise
         */
        bool setSavePath(string FolderPath);

        /**
         * @brief setFilename Sets the filename of the HDF5 file. If not set, the default is dataset.hdf5
         * @param filename
         */
        void setFilename(const string &filename) { m_Filename = filename; }

        void toBinaryUInt16(unsigned short* data, unsigned long N);

        void toBinaryFloat(float* data, unsigned long N);

        void toBinaryDouble(double* data, unsigned long N);

        void toBinaryInt16(short* data, unsigned long N);

        void toBinaryUInt32(unsigned int* data, unsigned long N);

        void toBinaryInt32(int* data, unsigned long N);

        void toBinaryInt64(long* data, unsigned long N);

        void toBinaryUInt64(unsigned long* data, unsigned long N);

        void toBinaryU8(unsigned char* data, unsigned long N);

        void toBinaryChar(char* data, unsigned long N);

        /**
         * @brief subscribeSavingFinished Subscribes to saving finished event
         * @param subcriber
         */
        void subscribeSavingFinished(const boost::signals2::signal<void (bool, string)>::slot_type &subscriber) { sig_SavingFinished.connect(subscriber); }

        /**
         * @brief getHDF5 Returns the H5 datatype based on the template class
         * @return
         */
         H5::DataSet* _createDataSet(const string &name, const H5::DataSpace &ds);

         /**
          * @brief _writeDataSet Writes the H5 data based on the template class
          * @param b
          */
         void _writeDataSet(H5::DataSet *ds, void *b);
    protected:
        void preStage() override;

        void workStage() override;

        void postStage() override;

        void _saveData(void *data, unsigned long NBytes);
    };
}

#endif // SAVINGPIPELINE_H
