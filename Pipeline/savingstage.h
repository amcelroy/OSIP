#ifndef SAVINGPIPELINE_H
#define SAVINGPIPELINE_H

#include "pipeline_global.h"
#include "pipelinestage.h"
#include "boost/signals2.hpp"
#include "boost/endian/arithmetic.hpp"
#include "boost/endian/conversion.hpp"
#include "boost/filesystem.hpp"
#include "hdf5.h"
#include <fstream>

using namespace std;

namespace OSIP {
    template<class I>
    class PIPELINESHARED_EXPORT SavingStage : public PipelineStage<I, I>
    {
        boost::signals2::signal<void (bool, string)> sig_SavingFinished;

        map<string, ofstream> m_StreamDictionary;

        string m_FolderPath;

        bool m_FolderPathSet = false;
    public:
        SavingStage();

        /**
         * @brief setSavePath Sets the folder path to save files in
         * @param path Folder path
         * @return True if the folder path both exists and is a folder, false otherwise
         */
        bool setSavePath(string FolderPath);

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
    protected:
        void preStage() override;

        void workStage() override;

        void postStage() override;

        void _saveData(void *data, unsigned long NBytes);
    };
}

#endif // SAVINGPIPELINE_H
