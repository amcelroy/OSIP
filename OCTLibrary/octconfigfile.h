#ifndef OCTSETTINGSFILE_H
#define OCTSETTINGSFILE_H

#include <loadoctpipeline.hpp>
#include "boost/algorithm/string.hpp"

using namespace OSIP;

class OCTConfigFile
{
public:
    OCTConfigFile() { }

    bool readOCTConfig(string path, OCTConfig *config);

    bool writeOCTConfig(string path, OCTConfig config) { return true; }
};

#endif // OCTSETTINGSFILE_H
