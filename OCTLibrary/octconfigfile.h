#ifndef OCTSETTINGSFILE_H
#define OCTSETTINGSFILE_H

#include <loadoctpipeline.hpp>
#include "boost/algorithm/string.hpp"
#include "octlibrary_global.h"

using namespace OSIP;

class OCTLIBRARYSHARED_EXPORT OCTConfigFile
{
public:
    OCTConfigFile() { }

    bool readOCTConfig(string path, OCTConfig *config);

    bool writeOCTConfig(string path, OCTConfig config) { return true; }
};

#endif // OCTSETTINGSFILE_H
