#include "octconfigfile.h"

bool OCTConfigFile::readOCTConfig(string path, OCTConfig *config){
    try{
        ifstream filein(path);

        stringstream ss;
        ss << filein.rdbuf();

        string filecontents(ss.str());

        vector<string> splitString;
        boost::split(splitString, filecontents, boost::is_any_of("\r\n"), boost::token_compress_on);

        for(string line : splitString){
            vector<string> splitLine;
            boost::split(splitLine, line, boost::is_any_of("="));
            if(splitLine.size() == 2){
                //Parse if needed
                boost::algorithm::trim(splitLine[0]);
                boost::algorithm::to_lower(splitLine[0]);

                if(splitLine[0] == "range"){
                    config->Range = stof(splitLine[1].c_str());
                }else if(splitLine[0] == "bits"){
                    config->Bits = stoi(splitLine[1].c_str());
                }else if(splitLine[0] == "b-scans"){
                    config->TotalBScans = stoi(splitLine[1].c_str());
                }else if(splitLine[0] == "stop trim"){
                    config->StopTrim = stoi(splitLine[1].c_str());
                }else if(splitLine[0] == "start trim"){
                    config->StartTrim = stoi(splitLine[1].c_str());
                }else if(splitLine[0] == "points per a-scan"){
                    config->PointsPerAScan = stoi(splitLine[1].c_str());
                }else if(splitLine[0] == "a-scans per b-scan"){
                    config->AScansPerBScan = stoi(splitLine[1].c_str());
                }
            }
        }

        config->Gain = config->Range / powf(2, config->Bits);
        config->Bias = config->Range / 2;


        return false;
    }catch(...){
        return true;
    }
}
