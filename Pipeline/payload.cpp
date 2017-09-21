#include "payload.h"
#include "payload_impl.h"

template class PIPELINESHARED_EXPORT Payload<short>;
template class PIPELINESHARED_EXPORT Payload<unsigned short>;

template class PIPELINESHARED_EXPORT Payload<int>;
template class PIPELINESHARED_EXPORT Payload<unsigned int>;

template class PIPELINESHARED_EXPORT Payload<char>;
template class PIPELINESHARED_EXPORT Payload<unsigned char>;

template class PIPELINESHARED_EXPORT Payload<long>;
template class PIPELINESHARED_EXPORT Payload<unsigned long>;

template class PIPELINESHARED_EXPORT Payload<float>;
template class PIPELINESHARED_EXPORT Payload<complex<float>>;

template class PIPELINESHARED_EXPORT Payload<double>;
template class PIPELINESHARED_EXPORT Payload<complex<double>>;
