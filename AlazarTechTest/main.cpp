#include <QCoreApplication>
#include <alazartechdaqstage.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    AlazarTechDAQStage at;

    DAQParameters p;

    p.Channels = OSIP_CHANNELS::A;
    p.PointsPerTrigger = 2048;
    p.TriggersPerBuffer = 128;
    p.TotalBuffers = 128;
    p.ClockSource = OSIP_CLOCK_SOURCE::OSIP_INTERNAL_CLOCK;
    p.Trigger = OSIP_TRIGGER_SOURCE::OSIP_EXTERNAL_TRIGGER;
    p.VoltageGain = 4.0f/65535.0f;
    p.VoltageBias = 2.0f;

    at.init();
    at.updateDAQ(p);

    return a.exec();
}
