#include "inlet.h"
#include "inlet_impl.h"

template class PIPELINESHARED_EXPORT Inlet<char>;
template class PIPELINESHARED_EXPORT Inlet<unsigned char>;

template class PIPELINESHARED_EXPORT Inlet<short>;
template class PIPELINESHARED_EXPORT Inlet<unsigned short>;

template class PIPELINESHARED_EXPORT Inlet<int>;
template class PIPELINESHARED_EXPORT Inlet<unsigned int>;

template class PIPELINESHARED_EXPORT Inlet<long>;
template class PIPELINESHARED_EXPORT Inlet<unsigned long>;

template class PIPELINESHARED_EXPORT Inlet<float>;
template class PIPELINESHARED_EXPORT Inlet<complex<float>>;

template class PIPELINESHARED_EXPORT Inlet<double>;
template class PIPELINESHARED_EXPORT Inlet<complex<double>>;
