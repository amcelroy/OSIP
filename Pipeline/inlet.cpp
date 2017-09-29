#include "inlet.h"
#include "inlet_impl.h"

template class PIPELINESHARED_EXPORT OSIP::Inlet<char>;
template class PIPELINESHARED_EXPORT OSIP::Inlet<unsigned char>;

template class PIPELINESHARED_EXPORT OSIP::Inlet<short>;
template class PIPELINESHARED_EXPORT OSIP::Inlet<unsigned short>;

template class PIPELINESHARED_EXPORT OSIP::Inlet<int>;
template class PIPELINESHARED_EXPORT OSIP::Inlet<unsigned int>;

template class PIPELINESHARED_EXPORT OSIP::Inlet<long>;
template class PIPELINESHARED_EXPORT OSIP::Inlet<unsigned long>;

template class PIPELINESHARED_EXPORT OSIP::Inlet<float>;
template class PIPELINESHARED_EXPORT OSIP::Inlet<complex<float>>;

template class PIPELINESHARED_EXPORT OSIP::Inlet<double>;
template class PIPELINESHARED_EXPORT OSIP::Inlet<complex<double>>;
