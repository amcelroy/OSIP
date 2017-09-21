#include "daqstage.h"
#include "daqstage_impl.h"

template class PIPELINESHARED_EXPORT DAQStage<char>;
template class PIPELINESHARED_EXPORT DAQStage<unsigned char>;
template class PIPELINESHARED_EXPORT DAQStage<short>;
template class PIPELINESHARED_EXPORT DAQStage<unsigned short>;
template class PIPELINESHARED_EXPORT DAQStage<int>;
template class PIPELINESHARED_EXPORT DAQStage<unsigned int>;
template class PIPELINESHARED_EXPORT DAQStage<float>;
template class PIPELINESHARED_EXPORT DAQStage<double>;
template class PIPELINESHARED_EXPORT DAQStage<complex<float>>;
template class PIPELINESHARED_EXPORT DAQStage<complex<double>>;
