#include "nidaqmxgalvos.h"

using namespace OSIP;
using namespace Peripherals;

niDAQMXGalvos::niDAQMXGalvos() {

}

niDAQMXGalvos::~niDAQMXGalvos(){
    DAQmxClearTask(m_Handle);
}

void niDAQMXGalvos::setSampClkTiming(string channel){
    m_ExternalSamplClk = channel;
}

void niDAQMXGalvos::create(std::string device){
	m_Device = device;
    int32 error = DAQmxCreateTask("Galvo", &m_Handle);
	return;
}

void niDAQMXGalvos::configure(const GalvoParameters &gp, const OCTConfig &o) {
	//Stop DAQ if running
	int32 error = DAQmxStopTask(m_Handle);

	error = DAQmxCreateAOVoltageChan(m_Handle,
		(m_Device + "/" + "ao0:1").c_str(),
		"Analog Output channel 0 and 1",
		-10.0,
		10.0,
		DAQmx_Val_Volts,
		nullptr);

	error = DAQmxCfgSampClkTiming(m_Handle,
		(m_Device + "/" + m_ExternalSamplClk).c_str(),
		200000.0,
		DAQmx_Val_Falling,
		DAQmx_Val_FiniteSamps,
		o.AScansPerBScan*o.TotalBScans);
	
	//Generate new data
	this->generateRaster(gp, o);

	std::vector<double> concat = this->getFastAxisVoltage();
	std::vector<double> slowAxis = this->getSlowAxisVoltage();

	concat.insert(concat.end(), slowAxis.begin(), slowAxis.end());

	DAQmxWriteAnalogF64(m_Handle, slowAxis.size(), false, 10.0, DAQmx_Val_GroupByChannel, concat.data(), NULL, NULL);

}

void niDAQMXGalvos::init() {

}

void niDAQMXGalvos::pause(){
    sig_Paused();
}

void niDAQMXGalvos::run(){
	DAQmxStartTask(m_Handle);
    sig_Run();
}

void niDAQMXGalvos::stop(){
	DAQmxStopTask(m_Handle);
    sig_Stop();
}

void niDAQMXGalvos::reset(){
    sig_Reset();
}
