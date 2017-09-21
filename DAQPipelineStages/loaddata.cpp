#include "loaddata.h"
#include "loaddata_impl.h"
#include <complex>

template class DAQPIPELINESTAGESSHARED_EXPORT LoadData<char>;
template class DAQPIPELINESTAGESSHARED_EXPORT LoadData<unsigned char>;
template class DAQPIPELINESTAGESSHARED_EXPORT LoadData<short>;
template class DAQPIPELINESTAGESSHARED_EXPORT LoadData<unsigned short>;
template class DAQPIPELINESTAGESSHARED_EXPORT LoadData<int>;
template class DAQPIPELINESTAGESSHARED_EXPORT LoadData<unsigned int>;
template class DAQPIPELINESTAGESSHARED_EXPORT LoadData<float>;
template class DAQPIPELINESTAGESSHARED_EXPORT LoadData<double>;
template class DAQPIPELINESTAGESSHARED_EXPORT LoadData<complex<float>>;
template class DAQPIPELINESTAGESSHARED_EXPORT LoadData<complex<double>>;

