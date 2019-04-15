#ifndef OCTSETTINGSFILE_H
#define OCTSETTINGSFILE_H

#include "boost/algorithm/string.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>
#include <daqstage.hpp>

using namespace std;
using namespace OSIP;

typedef struct _OCTConfig {
	string PathConfig;
	string PathData;
    unsigned long PointsPerAScan;
    unsigned long AScansPerBScan;
    unsigned long TotalBScans;
    float Gain;
    float Bias;
    float Range;
    unsigned long Bits;
    unsigned long StopTrim;
    unsigned long StartTrim;
	unsigned long BScansPerTransfer;
	unsigned long BScansRepitions = 1;
} OCTConfig;

class OCTConfigFile
{
public:
    OCTConfigFile() { }

    bool readOCTConfig(string path, OCTConfig *config);

	bool writeOCTConfig(string path, const OCTConfig& config);

    static DAQParameters packageDAQParameters(const OCTConfig& o, DAQStage<unsigned short> *d){
    	DAQParameters p = d->getDefaultDAQParameters();

    	p.TotalBuffers = o.TotalBScans;
    	p.PointsPerTrigger = o.PointsPerAScan;
    	p.TriggersPerBuffer = o.AScansPerBScan;
		p.BScansPerTransfer = o.BScansPerTransfer;

    	return p;
    }
};

#endif // OCTSETTINGSFILE_H
