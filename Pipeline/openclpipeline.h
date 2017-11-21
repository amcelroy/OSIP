#ifndef OPENCLPIPELINE_H
#define OPENCLPIPELINE_H

#include "pipelinestage.h"
#include "boost/signals2.hpp"

#ifdef __clang__
    //#include <opencl-c.h>
#elif _WIN32
    #include <opencl.h>
#elif _WIN64
    #include <opencl.h>
#endif

using namespace std;

namespace OSIP {
    template<class I, class O>
    class OpenCLPipeline : public PipelineStage<I, O>
    {
    private:
        boost::signals2::signal<void ()> sig_OpenCLCompiled;
        boost::signals2::signal<void ()> sig_OpenCLMemoryWritten;
        boost::signals2::signal<void ()> sig_OpenCLMemoryRead;
        boost::signals2::signal<void ()> sig_OpenCLError;

    public:
        OpenCLPipeline();

        /**
         * @brief compileProgram Compiles a text program. Emits a signal
         * if there are any compile errors and when the program is compiled.
         * @param programText Char array with the program
         * @param options Compile options, uses default options
         * if not used.
         */
        void compileProgram(const char* programText, const char* options);

        /**
         * @brief allocateMemory Allocates unpinned memory on the device
         * @param x X Dimension
         * @param y Y Dimension
         * @param z Z Dimension
         */
        void allocateMemory(unsigned long x, unsigned long y, unsigned long z);
        void allocateMemory(unsigned long x, unsigned long y);
        void allocateMemory(unsigned long x);


    private:

        //Add pinned memory

        //Add allocate memory

        //Add

    };
}


#endif // OPENCLPIPELINE_H
