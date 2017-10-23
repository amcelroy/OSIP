#ifndef SAVINGPIPELINE_H
#define SAVINGPIPELINE_H

#include "pipeline_global.h"
#include "pipelinestage.h"
#include "boost/signals2.hpp"
#include "boost/endian/arithmetic.hpp"
#include <fstream>

using namespace std;

namespace OSIP {
    template<class I>
    class PIPELINESHARED_EXPORT SavingStage : public PipelineStage<I, I>
    {
        boost::signals2::signal<void()> sig_SavingFinished;

        boost::endian::order m_SystemEndianess;
    public:
        SavingStage();

        void toBinaryUInt16(string path, unsigned short* data, unsigned long N, boost::endian::order endian = boost::endian::order::big);

        void toBinaryFloat(string path, float* data, unsigned long N, boost::endian::order endian = boost::endian::order::big);

        void toBinaryDouble(string path, double* data, unsigned long N, boost::endian::order endian = boost::endian::order::big);

        void toBinaryInt16(string path, short* data, unsigned long N, boost::endian::order endian = boost::endian::order::big);

        void toBinaryUInt32(string path, unsigned int* data, unsigned long N, boost::endian::order endian = boost::endian::order::big);

        void toBinaryInt32(string path, int* data, unsigned long N, boost::endian::order endian = boost::endian::order::big);

        void toBinaryU8(string path, unsigned char* data, unsigned long N);

        void toBinaryChar(string path, char* data, unsigned long N);

    protected:
        virtual void postStage();

        void _saveData(string path, void *data, unsigned long NBytes);
    };
}

#endif // SAVINGPIPELINE_H
