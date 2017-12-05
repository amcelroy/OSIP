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
public:
    bool run(string path){
        Payload<I> payload;

        /**** 3D data, 64x64x64 ****/
        //create dimensions
        vector<unsigned long> dim = {64, 64, 64};
        unsigned long size = 1;
        for(unsigned long d : dim){
            size *= d;
        }

        vector<I> data(size);
        for(unsigned long i = 0; i < data.size(); i++){
            data[i] = i;
        }

        payload.addData(dim, shared_ptr<vector<I>>(&data), "64x64x64");
        /***************************/

        save.getInlet()->writeData(payload);
        save.subscribeSavingFinished(boost::bind(&SavingCompleteSignaler::savingComplete, &_SavingComplete, _1, _2));
        save.start();

        //Should return an error, since path is not set
        while(_SavingComplete.get() == false){
            this_thread::sleep_for(chrono::milliseconds(50));
        }

        auto test = typeid(I).name();
        string testName = "Test " + string(typeid(I).name()) + " 3D_64x64x64 - Set Path Should Fail";
        m_TestResults[testName.c_str()] = _SavingComplete.get();

        _SavingComplete.reset();

        testName = "Test " + string(typeid(I).name()) + " 3D_64x64x64 - Saving to disk";
        save.setSavePath(path);
        save.start();

        //Should return an error, since path is not set
        while(_SavingComplete.get() == false){
            this_thread::sleep_for(chrono::milliseconds(50));
        }
    }
};

#endif // SAVING_TEST_H
