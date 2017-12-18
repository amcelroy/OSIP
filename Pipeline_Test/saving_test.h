#ifndef SAVING_TEST_H
#define SAVING_TEST_H

#include <pipeline.h>
#include <boost/signals2.hpp>

using namespace OSIP;

class SavingCompleteSignaler{
    bool m_SavingComplete = false;

    bool m_Error = false;

public:
    void savingComplete(bool error, string message){
        m_SavingComplete = true;
        m_Error = error;
        cout << message << endl;
    }

    bool get(){ return m_SavingComplete; }

    void reset() { m_SavingComplete = false; }
};

template<class I>
class SavingTest{
    SavingStage<I> save;

    SavingCompleteSignaler _SavingComplete;

    map<string, bool> m_TestResults;

    vector<I>* createData(unsigned long long Rank, unsigned long long size){
        //create dimensions
        vector<unsigned long long> dim;

        for(int i = 0; i < Rank; i++){
            dim.push_back(size);
        }

        unsigned long long size1d = 1;
        for(unsigned long long d : dim){
            size1d *= d;
        }

        vector<I>* data = new vector<I>();
        for(unsigned long i = 0; i < size1d; i++){
            data->push_back(i);
        }

        return data;
    }
public:
    bool run(string path){
        Payload<I> payload;

        unsigned long long size = 512;
        int rank = 2;

        /**** 2D data, 512x512 ****/
        auto data_2D = createData(rank, size);
        vector<unsigned long long> v2d {size, size};
        payload.addData(v2d, shared_ptr<vector<I>>(data_2D), to_string(size) + "x" + to_string(size));
        save.getInlet()->writeData(payload);
        save.getInlet()->writeData(payload);
        /***************************/

        //Clear out the first test data
        payload.finished();

        size = 64;
        auto data_3D = createData(3, size);
        vector<unsigned long long> v3d {size, size, size};
        payload.addData(v3d, shared_ptr<vector<I>>(data_3D), to_string(size) + "x" + to_string(size));
        save.getInlet()->writeData(payload);
        save.getInlet()->writeData(payload);


        //Subscribe our local notifier about the save pipeline stage being completed
        save.subscribeSavingFinished(boost::bind(&SavingCompleteSignaler::savingComplete, &_SavingComplete, _1, _2));

        //Start the pipeline stage
        save.start();

        //Should return an error, since path is not set
        while(_SavingComplete.get() == false){
            this_thread::sleep_for(chrono::milliseconds(50));
        }

        //This test should fail
        string testName = "Test " + string(typeid(I).name()) + " Set Path Should Fail: ";
        m_TestResults[testName] = _SavingComplete.get();

        //Reset the notifer
        _SavingComplete.reset();

        testName = "Test " + string(typeid(I).name()) + " Saving to disk: ";
        save.setSavePath(path);
        save.setFilename("dataset_" + string(typeid(I).name()) + ".h5");
        save.start();
        save.slotDAQFinished();

        while(_SavingComplete.get() == false){
            this_thread::sleep_for(chrono::milliseconds(50));
        }

        //Check if file exists
        boost::filesystem::path tmp_path(path);
        if(boost::filesystem::exists(tmp_path)){
            m_TestResults[testName] = true;
        }

        //Perform reading and validating of the HDF5 Here
        //m_TestResults[testName.c_str()] &= test results

        for(auto j = m_TestResults.begin(); j != m_TestResults.end(); j++){
            cout << (*j).first << to_string((*j).second) << std::endl;
            cout.flush();
        }

        payload.finished();

        return true;
    }
};

#endif // SAVING_TEST_H
