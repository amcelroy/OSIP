#include <QCoreApplication>
#include <pipeline.h>
#include <iostream>
#include "saving_test.h"

using namespace std;
using namespace OSIP;

/**
 * @brief _Test_1_OpenCL_Test Creates an OpenCL device, compiles a program, and checks for errors.
 * @return
 */
bool _Test_1_OpenCL_Test(OpenCLPipeline<unsigned short, float>* p){
//    cout << "Running OpenCL Test\r";
//    try{
//        bool passed = true;
//        cout << "Checking for OpenCL Devices...\n";
//        passed &= p->init(OPENCL::INTEL, CL_DEVICE_TYPE_CPU);

//        if(passed){
//            cout << "Passed: Found " + to_string(p->getOpenCLDevices().size()) + " OpenCL Devices\n";
//            for(DEVICEINFO d : p->getOpenCLDevices()){
//                string summary = p->printOpenCLInformation(d);
//                cout.write(summary.c_str(), summary.size());
//                cout.flush();
//            }
//        }else{
//            cout << "Failed: No OpenCL devices found" << endl;
//            return passed;
//        }

//        cout << "Compiling test OpenCL code...\n";
//        passed &= p->compileProgram("../test.cl", NULL);
//        if(passed){
//            cout << "Passed: No compiler errors" << endl;
//        }else{
//            cout << "Failed: Compiler error, see below \r" << endl;
//            cout << p->getOpenCLBuildLog();
//            return passed;
//        }

//        return passed;
//    }catch(...){

//        return false;
//    }
}

bool _Test_2_Saving_Test(string rootPath){
    SavingTest<int> i32_Save;
    i32_Save.run(rootPath);

    SavingTest<unsigned int> u32_Save;
    u32_Save.run(rootPath);

    SavingTest<unsigned short> u16_Save;
    u16_Save.run(rootPath);

    SavingTest<short> i16_Save;
    i16_Save.run(rootPath);

    SavingTest<unsigned char> u8_Save;
    u8_Save.run(rootPath);

    SavingTest<char> i8_Save;
    i8_Save.run(rootPath);

    SavingTest<float> f_Save;
    f_Save.run(rootPath);

    SavingTest<double> d_Save;
    d_Save.run(rootPath);

    return true;
}

int main(int argc, char *argv[])
{
    boost::filesystem::path p(argv[0]);
    string rootPath = p.branch_path().string();

    QCoreApplication a(argc, argv);

    OpenCLPipeline<unsigned short, float> oclPipeline;


    //_Test_1_OpenCL_Test(&oclPipeline);

    _Test_2_Saving_Test(rootPath);


    return a.exec();
}
