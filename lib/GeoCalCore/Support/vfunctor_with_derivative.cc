#include "vfunctor_with_derivative.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void VFunctorWithDerivative::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(VFunctor);
  GEOCAL_BASE(VFunctorWithDerivative, VFunctor);
}

GEOCAL_IMPLEMENT(VFunctorWithDerivative);
#endif
