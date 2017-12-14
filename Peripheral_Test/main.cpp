#include <QCoreApplication>

#include "pipeline.h"
#include "nidaqmxgalvos.h"

using namespace OSIP;
using namespace Peripherals;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    niDAQMXGalvos ni("/Dev1", 512*512);
    ni.setSampClkTiming("PFI7");
    ni.init();


    return a.exec();
}
