#include "auto_derivative.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION

template<class T> template<class Archive> 
void AutoDerivative<T>::serialize(Archive& ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(AutoDerivative<T>);
  ar & GEOCAL_NVP(val) & GEOCAL_NVP(grad);
}

GEOCAL_IMPLEMENT(AutoDerivative<double>);


#endif
