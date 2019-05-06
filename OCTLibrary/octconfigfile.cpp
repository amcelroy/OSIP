#include "octconfigfile.h"

bool OCTConfigFile::writeOCTConfig(string path, const OCTConfig& config, float FastAxisAmp, float FastAxisOff, float SlowAxisAmp, float SlowAxisOff) {
	string config_path = path + "parameters.oct_scan";

	ofstream fileout(config_path);

	stringstream ss;
	ss << "[SCAN PARAMETERS]" << endl;
	ss << "Range = " << to_string(config.Range) << endl;
	ss << "Bits = " << to_string(config.Bits) << endl;
	ss << "A - Scans Per Transfer = " << to_string(config.BScansPerTransfer) << endl; 
	ss << "B - Scans = " << to_string(config.TotalBScans) << endl;
	ss << "A - Scans per B - Scan = " << to_string(config.AScansPerBScan) << endl;
	ss << "Points Per A - Scan = " << to_string(config.PointsPerAScan) << endl;
	ss << "Start Trim = " << to_string(config.StartTrim) << endl;
	ss << "Stop Trim = " << to_string(config.StopTrim) << endl;
	ss << "B - Scan Repition = " << to_string(config.BScansRepitions) << endl; 
	ss << endl;
	ss << "[GALVO_SETTINGS]" << endl;
	ss << "Fast Axis Offset = " << to_string(FastAxisOff) << endl;
	ss << "Slow Axis Offset = " << to_string(SlowAxisOff) << endl;
	ss << "Fast Axis Amp = " << to_string(FastAxisAmp) << endl;
	ss << "Slow Axis Amp = " << to_string(SlowAxisAmp) << endl;

	fileout << ss.rdbuf();

	fileout.close();

	return true;
}

bool OCTConfigFile::readOCTConfig(string path, OCTConfig *config){
    try{
    	string config_path = path + "parameters.oct_scan";

        ifstream filein(config_path);

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
        config->PathConfig = config_path;
        config->PathData = path + "data.bin";

        return false;
    }catch(...){
        return true;
    }
}
