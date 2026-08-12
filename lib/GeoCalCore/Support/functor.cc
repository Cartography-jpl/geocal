#include "functor.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<> template<class Archive>
void Functor<double, double>::serialize(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(DFunctor);
}

template<> template<class Archive>
void Functor<blitz::Array<double, 1>, blitz::Array<double, 1> >::serialize(Archive & UNUSED(ar), const unsigned int UNUSED(version))
{
  GEOCAL_GENERIC_BASE(VFunctor);
}

GEOCAL_IMPLEMENT(DFunctor);
GEOCAL_IMPLEMENT(VFunctor);
#endif
