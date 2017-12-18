#ifndef OPENCLPIPELINE_IMPL_H
#define OPENCLPIPELINE_IMPL_H

#include <openclpipeline.h>

using namespace OSIP;

#define clError( err ) if(err) { this->sig_MessageLogged("OpenCLPipeline Error - " + to_string(err)); }

template<class I, class O>
OpenCLPipeline<I, O>::OpenCLPipeline(){


}

template<class I, class O>
bool OpenCLPipeline<I, O>::init(OPENCL::PLATFORM p, cl_device_type requestedDeviceType){
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

template<class I, class O>
bool OpenCLPipeline<I, O>::compileProgram(string programText, const char *options){
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
        delete buffer;
        return false;
    }else if(error != CL_SUCCESS){
        this->sig_MessageLogged("Error creating opencl program with error - " + to_string(error));
        return false;
    }

    return true;
}

template<class I, class O>
bool OpenCLPipeline<I, O>::compileProgram(const char *filename, const char* options){
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

template<class I, class O>
string OpenCLPipeline<I, O>::printOpenCLInformation(DEVICEINFO d){
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

#endif // OPENCLPIPELINE_IMPL_H
