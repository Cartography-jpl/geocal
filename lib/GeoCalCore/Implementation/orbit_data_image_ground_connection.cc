#include "orbit_data_image_ground_connection.h"
#include "geocal_serialize_support.h"

using namespace GeoCal;
using namespace blitz;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION
template<class Archive>
void OrbitDataImageGroundConnection::serialize(Archive & ar, const unsigned int version)
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(ImageGroundConnection);
  ar & GEOCAL_NVP(orb) & GEOCAL_NVP(od) & GEOCAL_NVP(cam)
    & GEOCAL_NVP_(refraction)
    & GEOCAL_NVP(res) & GEOCAL_NVP(b) & GEOCAL_NVP(max_h);
  if(orb)
    orb->add_observer(*this);
}

GEOCAL_IMPLEMENT(OrbitDataImageGroundConnection);
#endif

