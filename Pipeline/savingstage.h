#ifndef SAVINGPIPELINE_H
#define SAVINGPIPELINE_H

#include "pipeline_global.h"
#include "pipelinestage.h"
#include "boost/signals2.hpp"
#include "boost/endian/arithmetic.hpp"
#include "boost/endian/conversion.hpp"
#include <fstream>

using namespace std;

namespace OSIP {
    template<class I>
    class PIPELINESHARED_EXPORT SavingStage : public PipelineStage<I, I>
    {
        boost::signals2::signal<void()> sig_SavingFinished;

        boost::endian::order m_SystemEndianess;

        ofstream m_FileStream;
    public:
        SavingStage();

        bool createFile(string path);

        void toBinaryUInt16(unsigned short* data, unsigned long N, boost::endian::order endian = boost::endian::order::big);

        void toBinaryFloat(float* data, unsigned long N, boost::endian::order endian = boost::endian::order::big);

        void toBinaryDouble(double* data, unsigned long N, boost::endian::order endian = boost::endian::order::big);

        void toBinaryInt16(short* data, unsigned long N, boost::endian::order endian = boost::endian::order::big);

        void toBinaryUInt32(unsigned int* data, unsigned long N, boost::endian::order endian = boost::endian::order::big);

        void toBinaryInt32(int* data, unsigned long N, boost::endian::order endian = boost::endian::order::big);

        void toBinaryInt64(long* data, unsigned long N, boost::endian::order endian = boost::endian::order::big);

        void toBinaryUInt64(unsigned long* data, unsigned long N, boost::endian::order endian = boost::endian::order::big);

        void toBinaryU8(unsigned char* data, unsigned long N);

        void toBinaryChar(char* data, unsigned long N);

        /**
         * @brief subscribeSavingFinished Subscribes to saving finished event
         * @param subcriber
         */
        void subscribeSavingFinished(boost::signals2::signal<void ()>::slot_type &subcriber);
    protected:
        void preStage();

        void workStage();

        void postStage();

        void _saveData(void *data, unsigned long NBytes);
    };
}

#endif // SAVINGPIPELINE_H
