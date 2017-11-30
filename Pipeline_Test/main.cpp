#include <QCoreApplication>
#include <pipeline.h>
#include <iostream>

using namespace std;
using namespace OSIP;

/**
 * @brief _Test_1_OpenCL_Test Creates an OpenCL device, compiles a program, and checks for errors.
 * @return
 */
bool _Test_1_OpenCL_Test(OpenCLPipeline<unsigned short, float>* p){
    cout << "Running OpenCL Test\r";
    try{
        bool passed = true;
        cout << "Checking for OpenCL Devices...\n";
        passed &= p->init(OPENCL::INTEL, CL_DEVICE_TYPE_CPU);

        if(passed){
            cout << "Passed: Found " + to_string(p->getOpenCLDevices().size()) + " OpenCL Devices\n";
            for(DEVICEINFO d : p->getOpenCLDevices()){
                string summary = p->printOpenCLInformation(d);
                cout.write(summary.c_str(), summary.size());
                cout.flush();
            }
        }else{
            cout << "Failed: No OpenCL devices found" << endl;
            return passed;
        }

        cout << "Compiling test OpenCL code...\n";
        passed &= p->compileProgram("../test.cl", NULL);
        if(passed){
            cout << "Passed: No compiler errors" << endl;
        }else{
            cout << "Failed: Compiler error, see below \r" << endl;
            cout << p->getOpenCLBuildLog();
            return passed;
        }

        return passed;
    }catch(...){

        return false;
    }
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    OpenCLPipeline<unsigned short, float> oclPipeline;

    _Test_1_OpenCL_Test(&oclPipeline);




    return a.exec();
}
