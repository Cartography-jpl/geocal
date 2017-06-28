#include "spice_dem.h"
#include "geocal_serialize_support.h"
#include "spice_helper.h"
#include "planet_coordinate.h"

using namespace GeoCal;

#ifdef GEOCAL_HAVE_BOOST_SERIALIZATION

template<class Archive>
void SpiceDem::serialize(Archive & ar, const unsigned int version) 
{
  ar & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Dem)
    & GEOCAL_NVP_(naif_id)
    & GEOCAL_NVP_(time)
    & GEOCAL_NVP(body_name)
    & GEOCAL_NVP(fixed_frame_name)
    & GEOCAL_NVP_(kernel_list);
}

GEOCAL_IMPLEMENT(SpiceDem);
#endif

void SpiceDem::init()
{
  body_name = SpiceHelper::body_name(naif_id_);
  fixed_frame_name = SpiceHelper::fixed_frame_name(naif_id_);
}

boost::shared_ptr<GroundCoordinate> 
SpiceDem::surface_point(const GroundCoordinate& Gp) const
{
  // Blah blah, call latsrf
}
