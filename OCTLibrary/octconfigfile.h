#ifndef OCTSETTINGSFILE_H
#define OCTSETTINGSFILE_H

#include "loadoctpipeline.h"
#include "boost/algorithm/string.hpp"

using namespace OSIP;

class OCTConfigFile
{
public:
    OCTConfigFile();

    bool readOCTConfig(string path, OCTConfig *config);

    bool writeOCTConfig(string path, OCTConfig config);
};

#endif // OCTSETTINGSFILE_H
