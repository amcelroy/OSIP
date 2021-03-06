#ifndef SAVINGPIPELINE_H
#define SAVINGPIPELINE_H

#include "pipelinestage.hpp"
#include "boost/signals2.hpp"
#include "boost/endian/arithmetic.hpp"
#include "boost/endian/conversion.hpp"
#include "boost/filesystem.hpp"
//#include "hdf5.h"
//#include "H5Cpp.h"
//#include "zlib.h"
#include <fstream>
#include "octconfigfile.h"
#include <Peripherals/galvos.hpp>

#include <sys/types.h>
#include <sys/stat.h>

using namespace std;

namespace OSIP {
    template<class I>
    class SavingStage : public PipelineStage<I,I>
    {
        boost::signals2::signal<void (bool, string)> sig_SavingFinished;

        string m_FolderPath;

        string m_Filename = "";

        bool m_FolderPathSet = false;

        //H5::H5File* m_H5File;

        //H5::DataType m_H5Type;

		bool m_Save = false;
    public:
        SavingStage(){ //H5::DataType dt){
            //m_H5Type = dt;
        }

        /**
         * @brief setSavePath Sets the folder path to save files in
         * @param path Folder path
         * @return True if the folder path both exists and is a folder, false otherwise
         */
        bool setSavePath(string FolderPath){
            //boost::filesystem::path p(FolderPath);
            //if(boost::filesystem::exists(p)){ //valid folder?
            //    if(boost::filesystem::is_directory(p)){ //valid directory?
            //        this->m_FolderPath = FolderPath;
            //        this->m_FolderPathSet = true;
            //        return true;
            //    }
            //}

            //m_FolderPathSet = false;
            //return false;
        }

        /**
         * @brief setFilename Sets the filename of the HDF5 file. If not set, the default is dataset.hdf5
         * @param filename
         */
        void setFilename(const string &filename) { m_Filename = filename; }

        /**
         * @brief subscribeSavingFinished Subscribes to saving finished event
         * @param subcriber
         */
        void subscribeSavingFinished(const boost::signals2::signal<void (bool, string)>::slot_type &subscriber) { sig_SavingFinished.connect(subscriber); }

        /**
         * @brief unsubscribeSavingFinished Disconnect all slots from the Saving Finished event
         */
        void unsubscribeSavingFinished(){
            sig_SavingFinished.disconnect_all_slots();
        }

        void unsubscribeAll(){
            PipelineStage<I,I>::unsubscribeAll();
            sig_SavingFinished.disconnect_all_slots();
        }

		void save(const string &path, const OCTConfig &p, const Peripherals::Galvos::GalvoParameters& gp) {
			m_Save = true;

			OCTConfigFile o;

			this->m_FolderPath = path;
			
			m_Filename = path + "\\" + "data.bin";
			string param = path + "\\" + "parameters.oct_scan";

			o.writeOCTConfig(param, p, gp.FastAxisAmplitude, gp.FastAxisOffset, gp.SlowAxisAmplitude, gp.SlowAxisOffset);
	
			m_FolderPathSet = true;
		}
    protected:

		void preStage() override {
			stopThread = false;
			m_DAQFinished = false;
			PipelineStage::preStage();
		}

        void work() override{
            //if(this->m_FolderPathSet){
                //not found, create stream and add to map
                //std::filesystem::path folderPath(this->m_FolderPath);

                //string name;
                //if(m_Filename == ""){
                //    name = "data.bin";
                //}else{
                //    name = m_Filename;
                //}

                //string fullpath = this->m_FolderPath + "\\" + name;

                //m_H5File = new H5::H5File(fullpath, H5F_ACC_TRUNC);

				std::ofstream file;

                map<string, unsigned long long> frameCount;

                while(!this->stopThread){
                    if(this->pauseThread){
                        this->pipelineSleep(10);
                    }else{
                        //dequeue data
                        Payload<I> p = this->fetchPayload();

                        if(!p.isValid()){
                            this->pipelineSleep(10);

							string name = p.getFirstLabel();
                            if(this->m_DAQFinished && this->_Inlet->getItemsInInlet() == 0 && name.compare("final_frame") && m_Save){
								m_Save = false; //Saving completed
								file.flush();
								file.close();
                            }
                        }else{
							if (m_Save) {
								vector<string> labels = p.getDataNames();
								vector<vector<unsigned long long>> dims = p.getDimensions();
								shared_ptr<vector<I>> data = p.getFirstData();

								auto start = chrono::high_resolution_clock::now();
								//For each label, either create a new dataspace or append to it
								for (unsigned long i = 0; i < labels.size(); i++) {
									if (!file.is_open()) {
										file.open(m_Filename);
									}
									file.write(reinterpret_cast<const char*>(data->data()), data->size() * 2);
									
									////Check if the HDF5 File already has a group of the label name
									//if (!m_H5File->exists(labels[i])) {
									//	frameCount[labels[i]] = 0;

									//	H5::Group g = m_H5File->createGroup(labels[i]);
									//	g.close();
									//}

									//H5::DataSet* dataset = _createDataSet("/" + labels[i] + "/" + to_string(frameCount[labels[i]]),
									//	H5::DataSpace(dims[i].size(), dims[i].data()));
									//_writeDataSet(dataset, data[i]->data());
									//dataset->close();
									//frameCount[labels[i]] += 1;
								}
								auto stop = chrono::high_resolution_clock::now();

								std::chrono::duration<double, std::micro> elapsed = stop - start;
								this->sig_StageTimer(elapsed.count());
							}
							else {
								this->flushInlet();
							}

							p.finished();
                        }
                    }
                }

                //m_H5File->close();
				file.close();

                this->sig_SavingFinished(false, "Saving Completed");
            //}else{
            //    string error = "Folder path not set for the Saving Stage";
            //    this->sig_MessageLogged(error);
            //    this->sig_SavingFinished(true, error);
            //}

            this->postStage();

			PipelineStage::postStage();
        }

    private:
        //H5::DataSet* _createDataSet(const string &name, const H5::DataSpace &ds){
        //    return new H5::DataSet(m_H5File->createDataSet(name, m_H5Type, ds));
        //}

        //void _writeDataSet(H5::DataSet* ds, void *v){
        //    ds->write(v, m_H5Type);
        //}
    };
}

#endif // SAVINGPIPELINE_H
