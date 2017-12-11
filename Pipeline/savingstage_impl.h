#ifndef SAVINGPIPELINE_IMPL_H
#define SAVINGPIPELINE_IMPL_H

#include "savingstage.h"

using namespace OSIP;

template<class I>
SavingStage<I>::SavingStage(){
//    //Set system endianess first;
//    //https://stackoverflow.com/questions/1001307/detecting-endianness-programmatically-in-a-c-program
//    if ( htonl(47) == 47 ) {
//      // Big endian
//        m_SystemEndianess = boost::endian::order::big;
//    } else {
//      // Little endian.
//        m_SystemEndianess = boost::endian::order::little;
//    }
}

template<class I>
bool SavingStage<I>::setSavePath(string FolderPath){
    boost::filesystem::path p(FolderPath);
    if(boost::filesystem::exists(p)){ //valid folder?
        if(boost::filesystem::is_directory(p)){ //valid directory?
            this->m_FolderPath = FolderPath;
            this->m_FolderPathSet = true;
            return true;
        }
    }

    m_FolderPathSet = false;
    return false;
}

template<class I>
void SavingStage<I>::toBinaryUInt16(unsigned short* data, unsigned long N){
    _saveData(data, sizeof(unsigned short)*N);
}

template<class I>
void SavingStage<I>::toBinaryFloat(float* data, unsigned long N){
    _saveData(data, sizeof(float)*N);
}

template<class I>
void SavingStage<I>::toBinaryDouble(double* data, unsigned long N){
    _saveData(data, sizeof(double)*N);
}

template<class I>
void SavingStage<I>::toBinaryInt16(short* data, unsigned long N){
    _saveData(data, sizeof(short)*N);
}

template<class I>
void SavingStage<I>::toBinaryUInt32(unsigned int* data, unsigned long N){
    _saveData(data, sizeof(unsigned int)*N);
}

template<class I>
void SavingStage<I>::toBinaryInt32(int* data, unsigned long N){
    _saveData(data, sizeof(int)*N);
}

template<class I>
void SavingStage<I>::toBinaryU8(unsigned char* data, unsigned long N){
    _saveData(data, sizeof(unsigned char)*N);
}

template<class I>
void SavingStage<I>::toBinaryChar(char* data, unsigned long N){
    _saveData(data, sizeof(char)*N);
}

template<class I>
void SavingStage<I>::toBinaryInt64(long* data, unsigned long N){
    _saveData(data, sizeof(long)*N);
}

template<class I>
void SavingStage<I>::toBinaryUInt64(unsigned long* data, unsigned long N){
    _saveData(data, sizeof(unsigned long)*N);
}

template<class I>
void SavingStage<I>::_saveData(void *data, unsigned long NBytes){
//    try{
//        //Save data
//        m_FileStream.write((char*)data, NBytes);
//    }catch(exception e){
//        this->sig_MessageLogged("Error saving file - ");
//    }
}

template<class I>
void SavingStage<I>::preStage(){

}

template<class I>
void SavingStage<I>::postStage(){

}

template<> H5::DataSet* SavingStage<unsigned char>::_createDataSet(const string &name, const H5::DataSpace &ds){
    return new H5::DataSet(m_H5File->createDataSet(name, H5::PredType::NATIVE_UCHAR, ds));
}

template<> H5::DataSet* SavingStage<char>::_createDataSet(const string &name, const H5::DataSpace &ds){
    return new H5::DataSet(m_H5File->createDataSet(name, H5::PredType::NATIVE_CHAR, ds));
}

template<> H5::DataSet* SavingStage<short>::_createDataSet(const string &name, const H5::DataSpace &ds){
    return new H5::DataSet(m_H5File->createDataSet(name, H5::PredType::NATIVE_INT16, ds));
}

template<> H5::DataSet* SavingStage<unsigned short>::_createDataSet(const string &name, const H5::DataSpace &ds){
    return new H5::DataSet(m_H5File->createDataSet(name, H5::PredType::NATIVE_UINT16, ds));
}

template<> H5::DataSet* SavingStage<int>::_createDataSet(const string &name, const H5::DataSpace &ds){
    return new H5::DataSet(m_H5File->createDataSet(name, H5::PredType::NATIVE_INT32, ds));
}

template<> H5::DataSet* SavingStage<unsigned int>::_createDataSet(const string &name, const H5::DataSpace &ds){
    return new H5::DataSet(m_H5File->createDataSet(name, H5::PredType::NATIVE_UINT32, ds));
}

template<> H5::DataSet* SavingStage<float>::_createDataSet(const string &name, const H5::DataSpace &ds){
    return new H5::DataSet(m_H5File->createDataSet(name, H5::PredType::NATIVE_FLOAT, ds));
}

template<> H5::DataSet* SavingStage<double>::_createDataSet(const string &name, const H5::DataSpace &ds){
    return new H5::DataSet(m_H5File->createDataSet(name, H5::PredType::NATIVE_DOUBLE, ds));
}

template<> H5::DataSet* SavingStage<long>::_createDataSet(const string &name, const H5::DataSpace &ds){
    return new H5::DataSet(m_H5File->createDataSet(name, H5::PredType::NATIVE_INT64, ds));
}

template<> H5::DataSet* SavingStage<unsigned long>::_createDataSet(const string &name, const H5::DataSpace &ds){
    return new H5::DataSet(m_H5File->createDataSet(name, H5::PredType::NATIVE_UINT64, ds));
}

template<> void SavingStage<unsigned char>::_writeDataSet(H5::DataSet* ds, void *v){
    ds->write(v, H5::PredType::NATIVE_UINT8);
}

template<> void SavingStage<char>::_writeDataSet(H5::DataSet* ds, void *v){
    ds->write(v, H5::PredType::NATIVE_UINT8);
}

template<> void SavingStage<unsigned short>::_writeDataSet(H5::DataSet* ds, void *v){
    ds->write(v, H5::PredType::NATIVE_UINT16);
}

template<> void SavingStage<short>::_writeDataSet(H5::DataSet* ds, void *v){
    ds->write(v, H5::PredType::NATIVE_INT16);
}

template<> void SavingStage<unsigned int>::_writeDataSet(H5::DataSet* ds, void *v){
    ds->write(v, H5::PredType::NATIVE_UINT32);
}

template<> void SavingStage<int>::_writeDataSet(H5::DataSet* ds, void *v){
    ds->write(v, H5::PredType::NATIVE_INT32);
}

template<> void SavingStage<float>::_writeDataSet(H5::DataSet* ds, void *v){
    ds->write(v, H5::PredType::NATIVE_FLOAT);
}

template<> void SavingStage<double>::_writeDataSet(H5::DataSet* ds, void *v){
    ds->write(v, H5::PredType::NATIVE_DOUBLE);
}

template<> void SavingStage<long>::_writeDataSet(H5::DataSet* ds, void *v){
    ds->write(v, H5::PredType::NATIVE_INT64);
}

template<> void SavingStage<unsigned long>::_writeDataSet(H5::DataSet *ds, void *v){
    ds->write(v, H5::PredType::NATIVE_UINT64);
}

template<class I>
void SavingStage<I>::workStage(){
    if(this->m_FolderPathSet){
        //not found, create stream and add to map
        boost::filesystem::path folderPath(this->m_FolderPath);

        boost::filesystem::path name;
        if(m_Filename == ""){
            name = boost::filesystem::path("dataset.h5");
        }else{
            name = boost::filesystem::path(m_Filename);
        }

        boost::filesystem::path fullpath = folderPath / name;

        m_H5File = new H5::H5File(fullpath.string(), H5F_ACC_TRUNC);

        map<string, unsigned long long> frameCount;

        while(!this->stopThread){
            if(this->pauseThread){
                this->pipelineSleep(10);
            }else{
                //dequeue data
                Payload<I> p = this->fetchPayload();

                if(!p.isValid()){
                    this->pipelineSleep(10);

                    if(this->m_DAQFinished && this->_Inlet->getItemsInInlet() == 0){
                        this->stop();
                    }
                }else{
                    vector<string> labels = p.getDataNames();
                    vector<vector<unsigned long long>> dims = p.getDimensions();
                    vector<shared_ptr<vector<I>>> data = p.getData();

                    auto start = chrono::high_resolution_clock::now();
                    //For each label, either create a new dataspace or append to it
                    for(unsigned long i = 0; i < labels.size(); i++){
                        //Check if the HDF5 File already has a group of the label name
                        if(!m_H5File->exists(labels[i])){
                            frameCount[labels[i]] = 0;

                            H5::Group g = m_H5File->createGroup(labels[i]);
                            g.close();
                        }

                        H5::DataSet* dataset = _createDataSet("/" + labels[i] + "/" + to_string(frameCount[labels[i]]),
                                H5::DataSpace(dims[i].size(), dims[i].data()));
                        _writeDataSet(dataset, data[i]->data());
                        dataset->close();
                        frameCount[labels[i]] += 1;
                    }
                    auto stop = chrono::high_resolution_clock::now();

                    std::chrono::duration<double, std::micro> elapsed = stop - start;
                    this->sig_StageTimer(elapsed.count());
                }
            }
        }

        m_H5File->close();

        this->sig_SavingFinished(false, "Saving Completed");
    }else{
        string error = "Folder path not set for the Saving Stage";
        this->sig_MessageLogged(error);
        this->sig_SavingFinished(true, error);
    }

    postStage();
}


#endif // SAVINGPIPELINE_IMPL_H
