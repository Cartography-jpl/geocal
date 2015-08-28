#include "array_ad.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;
#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION

template<class T, int D> template<class Archive> 
void ArrayAd<T, D>::serialize(Archive& ar, const unsigned int version)
{
  boost::serialization::void_cast_register<GeoCal::ArrayAd<T, D>, GenericObject>();
  ar & GEOCAL_NVP(val) & GEOCAL_NVP(jac) & GEOCAL_NVP(is_const);
}

GEOCAL_IMPLEMENT(ArrayAd_1d);
GEOCAL_IMPLEMENT(ArrayAd_2d);
GEOCAL_IMPLEMENT(ArrayAd_3d);
GEOCAL_IMPLEMENT(ArrayAd_4d);
#endif
