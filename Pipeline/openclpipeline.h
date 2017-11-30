#ifndef OPENCLPIPELINE_H
#define OPENCLPIPELINE_H

#include <fstream>
#include "pipelinestage.h"
#include "boost/signals2.hpp"

#ifdef __clang__
    #include <OpenCL/opencl.h>
#elif _WIN32
    #include <opencl.h>
#elif _WIN64
    #include <opencl.h>
#endif

using namespace std;

namespace OSIP {
    struct OPENCL{
        enum PLATFORM { AMD, INTEL, NVIDIA };
    };

    struct DEVICEINFO{
        /***MISSING QUERIES***
        CL_DEVICE_ADDRESS_BITS
        CL_DEVICE_COMPILER_AVAILABLE
        CL_DEVICE_DOUBLE_FP_CONFIG
        CL_DEVICE_ERROR_CORRECTION_SUPPORT
        CL_DEVICE_EXECUTION_CAPABILITIES
        CL_DEVICE_EXTENSIONS
        CL_DEVICE_HALF_FP_CONFIG
        CL_DEVICE_MAX_WRITE_IMAGE_ARGS
        CL_DEVICE_MEM_BASE_ADDR_ALIGN
        CL_DEVICE_MIN_DATA_TYPE_ALIGN_SIZE
        CL_DEVICE_PLATFORM
        CL_DEVICE_PREFERRED_VECTOR_WIDTH_CHAR
        CL_DEVICE_PREFERRED_VECTOR_WIDTH_SHORT
        CL_DEVICE_PREFERRED_VECTOR_WIDTH_INT
        CL_DEVICE_PREFERRED_VECTOR_WIDTH_LONG
        CL_DEVICE_PREFERRED_VECTOR_WIDTH_FLOAT
        CL_DEVICE_PREFERRED_VECTOR_WIDTH_DOUBLE
        CL_DEVICE_PROFILE
        CL_DEVICE_QUEUE_PROPERTIES
        CL_DEVICE_SINGLE_FP_CONFIG
        CL_DEVICE_VENDOR_ID
        CL_DEVICE_VERSION
        CL_DRIVER_VERSION
        ***End***/
        cl_device_id DeviceID;
        cl_platform_id PlatformID;
        string Vendor;
        cl_device_type Type;
        cl_bool Available;
        cl_bool LittleEndian;
        cl_ulong GlobalMemCacheSize;
        cl_device_mem_cache_type GlobalMemCacheType;
        cl_ulong GlobalMemSize;
        cl_bool ImageSupport;
        size_t Image2DMaxHeight;
        size_t Image2DMaxWidth;
        size_t Image3DMaxDepth;
        size_t Image3DMaxHeight;
        size_t Image3DMaxWidth;
        cl_ulong LocalMemSize;
        cl_uint MaxClockFrequency;
        cl_uint MaxComputeUnits;
        cl_uint MaxConstantArgs;
        cl_ulong MaxConstantBufferSize;
        cl_ulong MaxMemAlloc;
        size_t MaxParameterSize;
        cl_uint MaxReadImageArgs;
        cl_uint MaxSamplers;
        size_t MaxWorkGroupSize;
        cl_uint MaxWorkItemDimensions;
        vector<size_t> MaxWorkItemSizes;
        size_t TimerResolutionNanoSeconds;
    };

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
         * @brief init Initializes the OpenCLPipeline with the requested Device and Platform
         * @param p
         * @param d
         * @return True if there are no errors, false if there are errors
         */
        bool init(OPENCL::PLATFORM p, cl_device_type requestedDeviceType);

        /**
         * @brief compileProgram Compiles a text program. Emits a signal
         * if there are any compile errors and when the program is compiled.
         * @param programText Char array with the program
         * @param options Compile options, uses default options
         * if not used.
         */
        bool compileProgram(string programText, const char* options);
        bool compileProgram(const char* filename, const char* options);

        /**
         * @brief allocateMemory Allocates unpinned memory on the device
         * @param x X Dimension
         * @param y Y Dimension
         * @param z Z Dimension
         */
        void allocateMemoryBytes(unsigned long x, unsigned long y, unsigned long z);
        void allocateMemoryBytes(unsigned long x, unsigned long y);
        void allocateMemoryBytes(unsigned long x);

        /**
         * @brief readMemory Convenience function to read memory of size dim
         * @param dim Vector of Dimensions
         * @return Shared_ptr of class O with data
         */
        shared_ptr<O> readMemory(vector<unsigned long> dim);

        /**
         * @brief writeMemory Convenience function to write data of size dim
         * @param dim Vector of dimensions
         * @param data Data to write to device, of size dim
         */
        void writeMemory(vector<unsigned long> dim, shared_ptr<I> data);

        /**
         * @brief findKernelByName Convenience class to find a kernel by name
         * @param name Kernel name
         * @param k Returned kernel, can be nullptr if kernel is not found
         */
        void findKernelByName(const char* name, cl_kernel* k);

        /**
         * @brief getOpenCLDevices Gets a vector of DEVICEINFO for all found devices
         * @return
         */
        vector<DEVICEINFO> getOpenCLDevices() { return m_OpenCLDevices; }

        /**
         * @brief printOpenCLInformation Creates a summary print out for a device of type DEVICEINFO
         * @param d
         * @return
         */
        string printOpenCLInformation(DEVICEINFO d);

        /**
         * @brief getOpenCLBuildLog Returns the OpenCL log from clGetProgramBuildInfo
         * @return
         */
        string getOpenCLBuildLog() { return m_OpenCLBuildLog; }

    private:

        //Add pinned memory

        //Add allocate memory

        /**
         * @brief m_Kernels List of kernels
         */
        vector<cl_kernel> m_Kernels;

        /**
         * @brief m_Programs List of programs
         */
        vector<cl_program> m_Programs;

        cl_command_queue m_Queue;

        cl_context m_Context;

        DEVICEINFO m_SelectedDevice;

        vector<DEVICEINFO> m_OpenCLDevices;

        bool m_Initialized = false;

        string m_OpenCLBuildLog = "No errors";
    };
}


#endif // OPENCLPIPELINE_H
