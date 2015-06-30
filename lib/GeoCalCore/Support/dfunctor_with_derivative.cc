#include "dfunctor_with_derivative.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void DFunctorWithDerivative::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(DFunctor);
  GEOCAL_BASE(DFunctorWithDerivative, DFunctor);
}

GEOCAL_IMPLEMENT(DFunctorWithDerivative);
#endif
