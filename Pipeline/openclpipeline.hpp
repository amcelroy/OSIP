#ifndef OPENCLPIPELINE_H
#define OPENCLPIPELINE_H

#include <fstream>
#include "pipelinestage.hpp"
#include "boost/signals2.hpp"

#ifdef __clang__
    #include <OpenCL/opencl.h>
#elif _WIN32
    #include <CL/opencl.h>
#elif _WIN64
    #include <CL/opencl.h>
#endif

#define clError( err ) if(err) { this->sig_MessageLogged("OpenCLPipeline Error - " + to_string(err)); }

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
        OpenCLPipeline(){

        }

        /**
         * @brief init Initializes the OpenCLPipeline with the requested Device and Platform
         * @param p
         * @param d
         * @return True if there are no errors, false if there are errors
         */
        bool init(OPENCL::PLATFORM p, cl_device_type requestedDeviceType){
            cl_uint numPlatforms = 0;
            clError(clGetPlatformIDs(0, NULL, &numPlatforms));

            vector<cl_platform_id> vecPlat(numPlatforms);
            clError(clGetPlatformIDs(numPlatforms, vecPlat.data(), NULL));

            map<cl_platform_id, string> _platforms;
            map<cl_platform_id, map<cl_device_id, DEVICEINFO>> _deviceMap;
            map<cl_device_id, DEVICEINFO> _deviceOfRequestedType;

            for(cl_platform_id p : vecPlat){
                size_t strSize = 0;
                clError(clGetPlatformInfo(p, CL_PLATFORM_VENDOR, 0, NULL, &strSize));
                vector<char> vendor(strSize);
                clError(clGetPlatformInfo(p, CL_PLATFORM_VENDOR, vendor.size(), vendor.data(), 0));
                _platforms[p] = string(vendor.begin(), vendor.end());

                map<cl_device_id, DEVICEINFO> _devices;

                cl_uint devCount = 0;
                clError(clGetDeviceIDs(p, CL_DEVICE_TYPE_ALL, 0, NULL, &devCount));
                vector<cl_device_id> devices(devCount);
                clError(clGetDeviceIDs(p, CL_DEVICE_TYPE_ALL, devCount, devices.data(), 0));

                for(cl_device_id d : devices){
                    DEVICEINFO di;
                    size_t retVal = 0;

                    di.PlatformID = p;
                    di.DeviceID = d;

                    clError(clGetDeviceInfo(d, CL_DEVICE_VENDOR, 0, NULL, &retVal));
                    vector<char> devVendor(retVal);
                    clError(clGetDeviceInfo(d, CL_DEVICE_VENDOR, retVal, devVendor.data(), 0));
                    di.Vendor = string(devVendor.begin(), devVendor.end());

                    cl_device_type dt = 0;
                    clError(clGetDeviceInfo(d, CL_DEVICE_TYPE, sizeof(cl_device_type), &di.Type, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_AVAILABLE, sizeof(cl_bool), &di.Available, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_ENDIAN_LITTLE, sizeof(cl_bool), &di.LittleEndian, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_GLOBAL_MEM_CACHE_SIZE, sizeof(cl_ulong), &di.GlobalMemCacheSize, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_GLOBAL_MEM_CACHE_TYPE, sizeof(cl_device_mem_cache_type), &di.GlobalMemCacheType, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_GLOBAL_MEM_SIZE, sizeof(cl_ulong), &di.GlobalMemSize, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_IMAGE_SUPPORT, sizeof(cl_bool), &di.ImageSupport, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_IMAGE2D_MAX_HEIGHT, sizeof(size_t), &di.Image2DMaxHeight, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_IMAGE2D_MAX_WIDTH, sizeof(size_t), &di.Image2DMaxWidth, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_IMAGE3D_MAX_DEPTH, sizeof(size_t), &di.Image3DMaxDepth, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_IMAGE3D_MAX_HEIGHT, sizeof(size_t), &di.Image3DMaxHeight, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_IMAGE3D_MAX_WIDTH, sizeof(size_t), &di.Image3DMaxWidth, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_LOCAL_MEM_SIZE, sizeof(cl_ulong), &di.LocalMemSize, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_MAX_CLOCK_FREQUENCY, sizeof(cl_uint), &di.MaxClockFrequency, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_MAX_COMPUTE_UNITS, sizeof(size_t), &di.MaxComputeUnits, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_MAX_CONSTANT_ARGS, sizeof(cl_uint), &di.MaxConstantArgs, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_MAX_CONSTANT_BUFFER_SIZE, sizeof(cl_ulong), &di.MaxConstantBufferSize, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_MAX_MEM_ALLOC_SIZE, sizeof(cl_ulong), &di.MaxMemAlloc, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_MAX_PARAMETER_SIZE, sizeof(size_t), &di.MaxParameterSize, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_MAX_READ_IMAGE_ARGS, sizeof(cl_uint), &di.MaxReadImageArgs, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_MAX_SAMPLERS, sizeof(cl_uint), &di.MaxSamplers, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_MAX_WORK_GROUP_SIZE, sizeof(size_t), &di.MaxWorkGroupSize, 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_MAX_WORK_ITEM_DIMENSIONS, sizeof(cl_uint), &di.MaxWorkItemDimensions, 0));

                    di.MaxWorkItemSizes.resize(di.MaxWorkItemDimensions);
                    clError(clGetDeviceInfo(d, CL_DEVICE_MAX_WORK_ITEM_SIZES, sizeof(size_t)*di.MaxWorkItemDimensions, di.MaxWorkItemSizes.data(), 0));

                    clError(clGetDeviceInfo(d, CL_DEVICE_PROFILING_TIMER_RESOLUTION, sizeof(size_t), &di.TimerResolutionNanoSeconds, 0));

                    _devices[d] = di;

                    m_OpenCLDevices.push_back(di);

                    if(di.Type == requestedDeviceType){
                        _deviceOfRequestedType[d] = di;
                    }
                }

                _deviceMap[p] = _devices;
            }

            bool deviceFound = false;
            for(auto i = _deviceOfRequestedType.begin(); i != _deviceOfRequestedType.end(); i++){
                DEVICEINFO di = (*i).second;
                switch(p){
                    case OPENCL::AMD:
                        if(di.Vendor.find("AMD") != string::npos){
                            m_SelectedDevice = di;
                            deviceFound = true;
                        }
                        break;

                    case OPENCL::INTEL:
                        if(di.Vendor.find("Intel") != string::npos){
                            m_SelectedDevice = di;
                            deviceFound = true;
                        }
                        break;

                    case OPENCL::NVIDIA:
                        if(di.Vendor.find("NVIDIA") != string::npos){
                            m_SelectedDevice = di;
                            deviceFound = true;
                        }
                        break;
                }
            }

            if(deviceFound == false){
                //no device found
                return deviceFound;
            }

            cl_int error = 0;
            m_Context = clCreateContextFromType(NULL, m_SelectedDevice.Type, NULL, NULL, &error);

            if(error != 0){
                this->sig_MessageLogged("Error creating OpenCL context for device " + m_SelectedDevice.Vendor);
                return false;
            }

            m_Queue = clCreateCommandQueue(m_Context, m_SelectedDevice.DeviceID, CL_QUEUE_PROFILING_ENABLE, &error);

            if(error != 0){
                this->sig_MessageLogged("Error creating OpenCL queue for device " + m_SelectedDevice.Vendor);
                return false;
            }

            return true;
        }

        /**
         * @brief compileProgram Compiles a text program. Emits a signal
         * if there are any compile errors and when the program is compiled.
         * @param programText Char array with the program
         * @param options Compile options, uses default options
         * if not used.
         */
        bool compileProgram(string programText, const char* options){
            cl_int error = 0;

            const char* programs[1] = { programText.c_str() };
            size_t programLengths[1] = { programText.size() };

            cl_program p = clCreateProgramWithSource(m_Context, 1, programs, programLengths, &error);

            if(error != 0){
                this->sig_MessageLogged("Error creating opencl program with error - " + to_string(error));
                return false;
            }

            cl_device_id idList[1] = { m_SelectedDevice.DeviceID };
            error = clBuildProgram(p, 1, &idList[0], options, NULL, NULL);

            size_t count = 0;
            if(error == CL_BUILD_PROGRAM_FAILURE){
                error = clGetProgramBuildInfo(p, m_SelectedDevice.DeviceID, CL_PROGRAM_BUILD_LOG, 0, NULL, &count);
                char* buffer = new char[count];
                error = clGetProgramBuildInfo(p, m_SelectedDevice.DeviceID, CL_PROGRAM_BUILD_LOG, count, buffer, 0);
                string a(buffer);
                m_OpenCLBuildLog = a;
                this->sig_MessageLogged(a);
                delete[] buffer;
                return false;
            }else if(error != CL_SUCCESS){
                this->sig_MessageLogged("Error creating opencl program with error - " + to_string(error));
                return false;
            }

            return true;
        }
        bool compileProgram(const char* filename, const char* options) {
            ifstream stream;

            stream.open(filename);

            string fileContents;

            if(stream.is_open()){
                std::string temp;
                while (std::getline(stream, temp)) {
                    fileContents += temp;
                }
            }

            return compileProgram(fileContents, options);
        }

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
        string printOpenCLInformation(DEVICEINFO d){
            string summary = "";

            summary += "Vendor: " + d.Vendor + "\n";
            summary += "OpenCL Device Type (CPU: 2, GPU: 4): " + to_string(d.Type) + "\n";
            summary += "Device Available: " + to_string(d.Available) + "\n";
            summary += "Little Endian: " + to_string(d.LittleEndian)  + "\n";
            summary += "Global Memory Chache Size: " + to_string(d.GlobalMemCacheSize)   + "\n";
            summary += "Image Support: " + to_string(d.ImageSupport)   + "\n";
            summary += "Image 2D Max Height: " + to_string(d.Image2DMaxHeight)  + "\n";
            summary += "Image 2D Max Width: " + to_string(d.Image2DMaxWidth)  + "\n";

            return summary;
        }

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
