#include "simple_dem.h"
#include "geocal_serialize_support.h"
using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class G> template<class Archive>
void SimpleDemT<G>::serialize(Archive & ar, const unsigned int version)
{
  GEOCAL_GENERIC_BASE(Dem);
  GEOCAL_BASE(SimpleDem, Dem);
  ar & GEOCAL_NVP_(h);
}

BOOST_CLASS_EXPORT_IMPLEMENT(SimpleDemT<Geodetic>);
#endif
