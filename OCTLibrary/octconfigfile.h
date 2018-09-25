#ifndef OCTSETTINGSFILE_H
#define OCTSETTINGSFILE_H

#include "boost/algorithm/string.hpp"
#include <fstream>

using namespace std;

typedef struct _OCTConfig {
    unsigned long PointsPerAScan;
    unsigned long AScansPerBScan;
    unsigned long TotalBScans;
    float Gain;
    float Bias;
    float Range;
    unsigned long Bits;
    unsigned long StopTrim;
    unsigned long StartTrim;
} OCTConfig;

class OCTConfigFile
{
public:
    OCTConfigFile() { }

    bool readOCTConfig(string path, OCTConfig *config);

    bool writeOCTConfig(string path, const OCTConfig& config) { return true; }
};

#endif // OCTSETTINGSFILE_H
